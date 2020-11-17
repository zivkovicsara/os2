#include <stdio.h>     // fprintf, perror
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>    // getopt
#include <stdlib.h>    // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <dirent.h>    // opendir, readdir, closedir
#include <string.h>    // strlen, strncpy, strncmp, strncat
#include <stdbool.h>   // da bismo mogli da koristimo boolean promenjivu sa true i false
#include <time.h>      // strftime, localtime

#define VELICINA_STRINGA 1024

// Pomocna funkcija za stampanje podataka o nekoj datoteci, 
// poziva se ukoliko je program pozvan sa opcijom -l
void stampaj_datoteku(struct stat sb){
    printf("%5lo  ", (unsigned long) sb.st_mode);
    printf("%3ld  ", (long) sb.st_nlink);
    printf("%3ld  %3ld  ", (long) sb.st_uid, (long) sb.st_gid);
    printf("%4lld  ", (long long) sb.st_size);
    char stringZaVreme[VELICINA_STRINGA] = "";
    strftime(stringZaVreme, VELICINA_STRINGA, "%b %d %H:%M", localtime(&sb.st_mtime));
    printf("%s  ", stringZaVreme);
}

int main(int argc, char *argv[]){
    struct stat sb;
    int opt;
    char *putanja = ".";    // Podrazumevano se ls poziva nad tekucim direktorijumom, 
                            // osim ukoliko mu se ne prosledi argument
    bool opcija_l = false;
    DIR *dp;
    opterr = 0;    

    while ((opt = getopt(argc, argv, "l:")) != -1) {
        switch (opt) {
            case 'l':
                putanja = optarg;
                opcija_l = true;
                break;
            case '?':
                if (optopt == 'l')
                    opcija_l = true;
                break;
            default:
                break;
        }
    }    

    // Program moze da se pozove sa ili bez argumenata, ukoliko je argc > 1,
    // znamo da smo dobili datoteku kao ulazni argument
    if (!opcija_l && argc > 1)
        putanja = argv[1];
    
    // Ukoliko se poziv stat() izvrsi neuspesno, stampamo poruku i prekidamo program
    if (stat(putanja, &sb) == -1){
        /*  Za ispisivanje gresaka necemo koristiti printf, vec funkciju perror koja stampa
            podatke o greski na stderr izlaz. Kao parametar prima opis koji se stampa uz 
            opis greske. Isprovajte ovo, pozovite program nad datotekom koja ne postoji. */
        perror("stat"); 
        exit(EXIT_FAILURE); // EXIT_FAILURE je makro koji moze da se koristi kada se program neuspesno izvrsi
    }

    // Ukoliko se radi o direktorijumu
    if (S_ISDIR(sb.st_mode)) { 
    //S_ISDIR je makro koji proverava da li polje st_mode ima vrednost koja odgovara direktorijumu
    // Ukoliko ne moze da se otvori taj direktorijum (npr. ne postoji ili nemamo ovlascenja), prekidamo program
        if ((dp = opendir(putanja)) == NULL){
            perror("opendir"); // Ispisuje se poruka o gresci
            exit(EXIT_FAILURE);
        }
            
        struct dirent *d; // Struktura koja ce da nam cuva podatke o direktorijumu
        // Idemo kroz direktorijum i citamo datoteke sve dok ne stignemo do kraja (NULL)
        while ((d = readdir(dp)) != NULL) { 
            if (opcija_l) { // Ukoliko je odabran detaljan prikaz
                char putanja_datoteke[VELICINA_STRINGA] = "";
                strncpy(putanja_datoteke, putanja, strlen(putanja));
                strncat(putanja_datoteke, "/", 1);
                strncat(putanja_datoteke, d->d_name, strlen(d->d_name));
                if (stat(putanja_datoteke, &sb) == -1){ 
                    // Ukoliko ne moze da se pozove stat() nad tekucom datotekom
                    perror("stat"); 
                    exit(EXIT_FAILURE);                        
                }                    
                stampaj_datoteku(sb);
            }
            printf("%s\n", d->d_name);
        }
        // Nakon prolaska kroz direktorijum, zatvaramo ga jer nam vise nije potreban
        closedir(dp); 
    } else { 
        // Ukoliko je ls pozvan nad datotekom, onda se samo prikazuju podaci o toj datoteci
        if (opcija_l)
            stampaj_datoteku(sb);
        printf("%s\n", putanja);
    }

    exit(EXIT_SUCCESS); // Uspeno izvrsen program
                        // Ovde je moglo da stoji i exit(0)  
}