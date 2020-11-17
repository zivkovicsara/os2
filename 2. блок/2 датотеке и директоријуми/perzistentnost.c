#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fd, pov_vred, brojac;

    fd = open("brojac.txt", O_CREAT | O_RDWR, 0666);
    assert(fd >= 0);
    
    pov_vred = read(fd, &brojac, sizeof(brojac));
    assert(pov_vred >= 0);
    
    if (pov_vred < sizeof(brojac))
        brojac = 0;
    
    brojac++;
    printf("brojac: %d\n", brojac);
    
    pov_vred = lseek(fd, 0, SEEK_SET);
    assert(pov_vred == 0);
    
    pov_vred = write(fd, &brojac, sizeof(brojac));
    assert(pov_vred == sizeof(brojac));
    
    // Sklonite komentar da bi program bezbedno zavrsio u slucaju greske:
    // fsync(fd);

    return 0;
}



