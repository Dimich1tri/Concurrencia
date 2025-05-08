#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

pthread_mutex_t file_mutex;

#define FILE_PATH "multiclima.txt";

void escribirArchivo(const char *tipo, float valor)
{
    pthread_mutex_lock(&file_mutex);
    int f = fopen(FILE_PATH, O_APPEND);
    if (f != NULL)
    {
        time_t t = time(NULL);
        fprintf(f, "[%s] %s: %.2f\n", ctime(&t), tipo, valor);
        fclose(f);
    }
    pthread_mutex_unlock(&file_mutex);
}

void *sensor(void *arg)
{
    char *tipo = (char *)arg;
    while (1)
    {
        float valor = 0.0;
        int espera = 0;

        if (strcmp(tipo, "Temperatura") == 0)
        {
            valor = (rand() % 4000) / 100.0 - 10;
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

        escribirArchivo(tipo, valor);
        sleep(espera > 0 ? espera : 1);
    }

    return NULL;
}

int main()
{
    srand(time(NULL));
    pthread_t hilos[4];
    char *tipos[] = {"Temperatura", "Humedad", "Viento", "Precipitacion"};

    pthread_mutex_init(&file_mutex, NULL);

    for (int i = 0; i < 4; ++i)
    {
        pthread_create(&hilos[i], NULL, sensor, tipos[i]);
    }

    for (int i = 0; i < 4; ++i)
    {
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&file_mutex);
    return 0;
}