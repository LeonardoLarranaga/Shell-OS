#include "File Manager.h"

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <string.h>

// Lee el contenido de todo un archivo.
void readFile(char** result, char* filename, int* fd) {
    (*fd) = open(filename, O_RDONLY);    

    if ((*fd) == -1) return;

    struct stat st;
    fstat((*fd), &st);
    int size = st.st_size;
    (*result) = (char*) malloc(size + 1);

    read((*fd), *result, size);
    (*result)[size] = '\0';

    close((*fd));
}

// Crea un archivo en modo escritura. Si ya existe, se sobreescribe.
int createTypeFile(char* filename) {
    return open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);
}

// Escribe en un archivo.
int writeTypeFile(int fd, char* content) {
    return write(fd, content, strlen(content));
}

// Elimina un archivo.
int removeFile(char* filename) {
    return unlink(filename);
}