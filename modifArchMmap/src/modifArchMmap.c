#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/config.h>

#define pathArchivoMapeado "/home/utnso/archivoBasura.txt"

int main(int argc, char *argv[])
{
    int fd, offset_byte;
    int offset_block=0;
    int block_size=4*1024;
    char *data;
    char* agregar;
//    struct stat sbuf;

    if (argc != 2) {
        fprintf(stderr, "usage: mmapdemo offset\n");
        exit(1);
    }

    if ((fd = open(pathArchivoMapeado, O_RDWR)) == -1) {
        perror("open");
        exit(1);
    }

    offset_byte = atoi(argv[1]);
    if (offset_byte < 0 || offset_byte > block_size-1) {
        fprintf(stderr, "mmapdemo: offset must be in the range 0-%d\n",(int)block_size-1);
        exit(1);
    }

    data = mmap((caddr_t)0, block_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset_block*block_size);

    if (data == (caddr_t)(-1)) {
        perror("mmap");
        exit(1);
    }

    printf("byte at Block %d, offset %d is '%s'\n",offset_block, offset_byte, data);

    agregar= "lendro rodriguez 21\nfranco aiello 22 \0";
    //agregar[strlen(agregar)-1]='\0';
    strncpy(data, agregar,strlen(agregar));

    //char **parametrosSeparados;

    //parametrosSeparados = string_split(data, "\0");

    printf("info bloque: %s\n", &(data[0]));



    close(fd);
    return 0;
}
