#include <stdio.h>     // fprintf, perror
#include <assert.h>    // getopt
#include <dirent.h>    // opendir, readdir, closedir


 int main(int argc, char *argv[]) {
    DIR *dp = opendir(".");  // otvaranje trenutnog direktorijuma
    assert(dp != NULL);
    struct dirent *d;
    while ((d = readdir(dp)) != NULL) { // čitanje direktorijuma
       printf("%d %c %s\n", (int) d->d_ino, d->d_type, d->d_name);
    }
    closedir(dp);   // zatvaranje trenutnog direktorijuma
    return 0;
 }
