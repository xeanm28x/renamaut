#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_FILE_SIZE 2048000

void import(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        printf("Erro ao abrir o arquivo.\n");

        return;
    }

    char *buffer = (char *)malloc(MAX_FILE_SIZE);

    if (!buffer)
    {
        fclose(file);

        printf("Erro ao alocar memória para o arquivo.\n");

        return;
    }

    int bytesRead = fread(buffer, 1, MAX_FILE_SIZE - 1, file);

    printf("bytesRead: %d \n", bytesRead);

    // buffer[bytesRead] = '\0';

    fclose(file);

    // printf("Conteúdo do arquivo:\n%s\n", buffer);

    free(buffer);
}

#endif