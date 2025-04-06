#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include "operacoes_lista_encadeada.h"
#include <cjson/cJSON.h>

MaquinaAutonoma *converter(const char *buffer)
{
    cJSON *base_dados = cJSON_Parse(buffer);

    if(base_dados == NULL)
    {
        wait_enter("Ocorreu um erro na conversão JSON.");

        return NULL;
    }

    cJSON *array_dados = cJSON_GetObjectItem(base_dados, "dados");

    if(array_dados == NULL)
    {
        cJSON_Delete(base_dados);

        wait_enter("Ocorreu um erro ao resgatar os dados após a conversão.");

        return NULL;
    }

    int tamanho = cJSON_GetArraySize(array_dados);

    MaquinaAutonoma *lista = NULL;
    
    for (int i = 0; i < tamanho; i++)
    {
        cJSON *item = cJSON_GetArrayItem(array_dados, i);
        const char *numero_registro = cJSON_GetObjectItem(item, "renamaut")->valuestring;
        MaquinaAutonoma *aux = buscar(numero_registro, lista);

        if(aux != NULL) continue;

        MaquinaAutonoma *novo = malloc(sizeof(MaquinaAutonoma));

        novo->numero_registro = strdup(cJSON_GetObjectItem(item, "renamaut")->valuestring);
        novo->fabricante = strdup(cJSON_GetObjectItem(item, "fab")->valuestring);
        novo->modelo = strdup(cJSON_GetObjectItem(item, "mod")->valuestring);
        novo->categoria = strdup(cJSON_GetObjectItem(item, "cat")->valuestring);
        novo->aplicacao = strdup(cJSON_GetObjectItem(item, "apl")->valuestring);
        novo->ano_fabricacao = cJSON_GetObjectItem(item, "ano")->valueint;
        novo->responsavel = strdup(cJSON_GetObjectItem(item, "resp")->valuestring);
        novo->status = 1;
        
        cJSON *loc_base = cJSON_GetObjectItem(item, "loc_base");

        novo->localizacao = malloc(sizeof(Localizacao));
        novo->localizacao->cidade = strdup(cJSON_GetObjectItem(loc_base, "cidade")->valuestring);
        novo->localizacao->uf = strdup(cJSON_GetObjectItem(loc_base, "uf")->valuestring);
        novo->proxima = NULL;

        inserir(&lista, novo);
    }

    cJSON_Delete(base_dados);

    return lista;
}

MaquinaAutonoma *importar(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        printf("Erro ao abrir o arquivo.\n");

        return NULL;
    }

    fseek(file, 0, SEEK_END);

    long file_size = ftell(file);

    rewind(file);

    char *buffer = (char *)malloc(file_size + 1);

    if (!buffer)
    {
        fclose(file);

        printf("Erro ao alocar memória para o arquivo.\n");

        return NULL;
    }

    size_t bytesRead = fread(buffer, 1, file_size, file);

    if (bytesRead != file_size)
    {
        printf("Erro na leitura do arquivo.\n");

        free(buffer);

        fclose(file);

        return NULL;
    }

    buffer[bytesRead] = '\0';

    MaquinaAutonoma *lista = converter(buffer);

    free(buffer);

    fclose(file);

    return lista;
}

#endif