#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include "operacoes_lista_encadeada.h"
#include <cjson/cJSON.h>

#define MAX_FILE_SIZE 2048000

MaquinaAutonoma *converter(const char *buffer) {
    cJSON *base_dados = cJSON_Parse(buffer);
    char mensagem_erro[50] = "Ocorreu um erro na importação dos dados.";
    MaquinaAutonoma *lista;

    if(base_dados != NULL) {

        cJSON *array_dados = cJSON_GetObjectItem(base_dados, "dados");

        if(array_dados != NULL) {
            int tamanho = cJSON_GetArraySize(array_dados);

            
            for (int i = 0; i < tamanho; i++) {}
            
            free(array_dados);
        } else {
            wait_enter(mensagem_erro);
        }

    } else {
        wait_enter(mensagem_erro);
    }
    free(base_dados);
}

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

    buffer[bytesRead] = '\0';

    converter(buffer);

    fclose(file);

    free(buffer);
}

#endif

// char *json_string = cJSON_Print(array_dados);

//             if (json_string != NULL) {
//                 wait_enter(json_string);
//                 free(json_string);
//             } else {
//                 wait_enter(mensagem_erro);
//             }