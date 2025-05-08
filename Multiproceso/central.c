#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define FIFO_PATH "clima_fifo"
#define BUFFER_SIZE 128

int main()
{
    FILE *archivo = fopen(FIFO_PATH, "a");
    if (!archivo)
    {
        perror("No se pudo abrir el archivo clima.txt");
        exit(1);
    }

    int fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd < 0)
    {
        perror("No se pudo abrir el FIFO");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    while (1)
    {
        ssize_t bytes = read(fifo_fd, buffer, sizeof(buffer) - 1);
        if (bytes > 0)
        {
            buffer[bytes] = '\0';
            time_t t = time(NULL);
            fprintf(archivo, "[%s] %s", ctime(&t), buffer);
            fflush(archivo);
        }
    }

    close(fifo_fd);
    fclose(archivo);
    return 0;
}
