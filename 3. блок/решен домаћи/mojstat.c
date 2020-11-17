#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
    // Program se poziva kao "./imeprograma imedatoteke", dakle treba da ima dva argumenta
    if(argc != 2) {
        printf("GRESKA: mojstat ocekuje 1 argument!");
        exit(1);
    }

    // Napomena: ne mora biti prosledjena datoteka u istom direktorijumu, vec to moze biti
    // putanja do neke datoteke.  Нpr. /etc/passwd
    char *putanja = argv[1]; // uzimamo ime datoteke kao putanju
    struct stat status_fajla; // struktura u okviru koje cemo da cuvamo podatke o datoteci
    int rc = stat(putanja, &status_fajla); // smestamo podatke u strukturu

    // Ukoliko povratna vrednost funkcije stat() nije 0, doslo je do greske
    if(rc != 0){
        printf("GRESKA: stat() se izvrsio neuspesno!");
        exit(1);
    }    

    // Stampamo podatke o datoteci, pogledajte prezentaciju ili na internetu da pogledate
    // sta sve sadrzi struktura stat
    printf("Velicina: %ld, broj alociranih blokova: %ld, broj referenci: %ld\n", 
        status_fajla.st_size, status_fajla.st_blocks, status_fajla.st_nlink);
    
    return 0;    
}