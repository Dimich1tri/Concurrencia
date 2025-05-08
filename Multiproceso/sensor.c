#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#define FIFO_PATH "clima_fifo"

void enviarDato(const char *tipo, float valor)
{
    int fifo_fd = open(FIFO_PATH, O_WRONLY);
    if (fifo_fd >= 0)
    {
        char mensaje[128];
        snprintf(mensaje, sizeof(mensaje), "%s: %.2f\n", tipo, valor);
        write(fifo_fd, mensaje, strlen(mensaje));
        close(fifo_fd);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s <TipoSensor>\n", argv[0]);
        return 1;
    }

    const char *tipo = argv[1];
    srand(getpid());

    while (1)
    {
        float valor;
        int espera;

        if (strcmp(tipo, "Temperatura") == 0)
        {
            valor = (rand() % 4000) / 100.0 - 10; // -10 a 30
            espera = 5 + (rand() % 5 - 2);
        }
        else if (strcmp(tipo, "Humedad") == 0)
        {
            valor = rand() % 101;
            espera = 12 + (rand() % 5 - 2);
        }
        else if (strcmp(tipo, "Viento") == 0)
        {
            valor = (rand() % 500) / 10.0;
            espera = 5 + (rand() % 5 - 2);
        }
        else if (strcmp(tipo, "Precipitacion") == 0)
        {
            valor = (rand() % 200) / 10.0;
            espera = 10 + (rand() % 5 - 2);
        }
        else
        {
            fprintf(stderr, "Tipo de sensor inválido\n");
            return 1;
        }

        enviarDato(tipo, valor);
        sleep(espera > 0 ? espera : 1);
    }

    return 0;
}
