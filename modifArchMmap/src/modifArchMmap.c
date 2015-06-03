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

//#define pathArchivoMapeado "/home/utnso/archivoBasura.txt"
#define pathArchivoMapeado "/home/utnso/archivoBasura.dat"

char *data;
int block_size=4*1024;

void cargarBloque(int nroBloque, char* info, int offset_byte){
	 int pos_a_escribir = nroBloque*block_size + offset_byte;
	   memcpy(data+pos_a_escribir, info, strlen(info));
	    data[pos_a_escribir+strlen(info)]='\0';

}

void mostrarBloque(int nroBloque){
	 printf("info bloque: %s\n", &(data[nroBloque*block_size ]));

}


int main(int argc, char *argv[])
{
    int fd, offset_byte=0;
    int offset_block;
    char cadena_a_agregar[]= "lendro rodriguez 21\nfranco aiello 22";
    struct stat sbuf;

    if (argc != 2) {
        fprintf(stderr, "usage: mmapdemo offset\n");
        exit(1);
    }

    if ((fd = open(pathArchivoMapeado, O_RDWR)) == -1) {
        perror("open()");
        exit(1);
    }

    if(fstat(fd, &sbuf) == -1) {
    	perror("fstat()");
    }

    offset_block = atoi(argv[1]);
    if (offset_byte < 0 || offset_byte > block_size-1) {
        fprintf(stderr, "mmapdemo: offset must be in the range 0-%d\n",(int)block_size-1);
        exit(1);
    }

    data = mmap((caddr_t)0, sbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (data == (caddr_t)(-1)) {
        perror("mmap");
        exit(1);
    }

//  printf("byte at Block %d, offset %d is '%s'\n",offset_block, offset_byte, data);

    cargarBloque(offset_block,cadena_a_agregar,offset_byte);

    mostrarBloque(offset_block);

    close(fd);
    return 0;
}
