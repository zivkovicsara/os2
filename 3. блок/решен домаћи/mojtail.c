#include <stdio.h>     // fprintf, perror
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>    // lseek, read
#include <stdlib.h>    // exit, atoi
#include <fcntl.h>     // open
#include <string.h>

// Pomocna funkcija za ispisivanje podataka o greskama, kao ulazni argument
// dobija string koji se ispisuje ispred podataka o greski do koje je doslo
void upravljaj_greskom(char *poruka) {
    perror(poruka); 
    exit(EXIT_FAILURE); 
}

int main(int argc, char *argv[]) {
    struct stat sb;
    int fd, pomeraj, linije;
    char * putanja = "";

    // Proveravamo da li je program pozvan na ispravan nacin, broj argumenata 
    // treba da bude tri, a drugi argument treba da bude u obliku -broj
    if (argc != 3 || strlen(argv[1]) <= 1 || argv[1][0] != '-') {
        fprintf(stderr, "Ispravan unos: %s -pomeraj imedatoteke\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    linije = atoi(argv[1]); // atoi = array to integer, pretvaramo broj u broj linija koje treba da obradimo
    linije *= -1; // Kako je dobijen negativan broj (jer se unosi kao -broj), mnozimo ga sa -1
    putanja = argv[2]; // Ime datoteke koju citamo

    if (stat(putanja, &sb) == -1)
        upravljaj_greskom("stat"); // Ukoliko nismo mogli da pozovemo stat() nad tom datotekom

    if ((fd = open(putanja, O_RDONLY)) == -1)
        upravljaj_greskom("open"); // Ukoliko nismo mogli da otvorimo tu datoteku

    if (lseek(fd, -1, SEEK_END) == -1)
        upravljaj_greskom("lseek"); // Ukoliko je doslo do greske tokom pozicioniranja na kraj datoteke

    char bafer[sb.st_size]; // Velicina bafera nam je jednaka velicini datoteke
    while (linije >= 0) { // Citamo liniju po liniju
        if (read(fd, bafer, 1) == -1) // Citamo bajt po bajt, zato smo stavili 1
            upravljaj_greskom("read"); // Ukoliko je doslo do greske tokom citanja
        if (bafer[0] == '\n')
            linije--; // Svaki put kada naidjemo na oznaku za novi red, umanjujemo broj linija
        pomeraj = lseek(fd, -2, SEEK_CUR);  // Pozicioniramo se na datu poziciju u datoteci
        if (pomeraj == -1)
            break;  // Moze da se desi da smo zadali veci broj linija nego sto ih ima ta datoteka,
                    // u tom slucaju pomeraj == -1 znaci da smo stigli do pocetka i da nema vise sta da se cita
    }

    if (read(fd, bafer, sb.st_size) == -1)
        upravljaj_greskom("read");

    printf("%s", bafer); // Stampamo bafer
    close(fd);

    exit(EXIT_SUCCESS);
}
