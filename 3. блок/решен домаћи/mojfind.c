#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

#define MAKS_DUZINA_PUTANJE 1024

// Funkcija za rekurzivnu pretragu direktorijuma
void pretrazi(const char* putanja_direktorijuma) {
  // Otvaramo direktorijum koji je prosledjen kao ulazni argument
  DIR *direktorijum = opendir(putanja_direktorijuma); 
  struct dirent *d;

  if (direktorijum == NULL) { 
      // Ukoliko dodje do greske tokom otvaranja
      fprintf(stderr, "Greska: opendir() neuspesno pozvan nad %s\n", putanja_direktorijuma);
      exit(1);
  }    
 
  // Prolazimo redom kroz tekuci direktorijum
  while ((d = readdir(direktorijum)) != NULL) {
    // Cuvamo putanju do tekuce datoteke/direktorijuma 
    char putanja[MAKS_DUZINA_PUTANJE]; 
    char *imefajla;
    imefajla = d->d_name; // Uzimamo ime datoteke/direktorijuma
    // Ne zelimo da ispisujemo tekuci (.) i roditeljski direktorijum (..)
    if (strncmp(imefajla, ".", 2) == 0 || strncmp(imefajla, "..", 3) == 0)
      continue; 

    switch(d->d_type) {
        // Ukoliko smo naisli na direktorijum
        case DT_DIR: 
            // Kopiramo putanju, dodajemo kosu crtu "/", dodajemo ime datoteke
            strncpy(putanja, putanja_direktorijuma, MAKS_DUZINA_PUTANJE);
            strncat(putanja, "/", MAKS_DUZINA_PUTANJE);
            strncat(putanja, imefajla, MAKS_DUZINA_PUTANJE);

            printf("%s\n", putanja); // Stampamo rezulatat
            // Rekurzivno pozivamo funkciju pretrazi nad tim direktorijumom
            pretrazi(putanja); 
            break;
        // Ukoliko smo naisli na bilo koji drugi tip datoteke - samo je odstampamo
        default: 
            printf("%s/%s\n", putanja_direktorijuma, imefajla);
            break;
    }
  }
  if (closedir(direktorijum) == -1) {
      fprintf(stderr, "Greska: closedir() neuspesno pozvan nad %s\n", putanja_direktorijuma);
      exit(1);
  }
}

int main(int argc, char *argv[]){
  char *trenutni_direktorijum; 
  if(argc == 1){
    // Ako je program pozvan bez dodatnih argumenata, krecemo od tekuceg direktorijuma
    trenutni_direktorijum = ".";
  } else if (argc == 2) { // Ukoliko je argc = 2, onda uzimamo argument sa ulaza
    trenutni_direktorijum = argv[1];
  } else { // Inace ispisujemo poruku o ispravnom pozivu programa
    fprintf(stderr, "Ispravan poziv: mojfind [imedirektorijuma]\n");
    exit(1);
  }

  pretrazi(trenutni_direktorijum);
  return 0;
}