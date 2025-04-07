#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include "operacoes_lista_encadeada.h"
#include <cjson/cJSON.h>

void converter(const char *buffer, MaquinaAutonoma **lista) {
    cJSON *base_dados = cJSON_Parse(buffer);

    if(base_dados == NULL) {
        wait_enter("Ocorreu um erro na conversão JSON.");
        return;
    }

    cJSON *array_dados = cJSON_GetObjectItem(base_dados, "dados");

    if(array_dados == NULL) {
        cJSON_Delete(base_dados);
        wait_enter("Ocorreu um erro ao resgatar os dados após a conversão.");
        return;
    }

    int tamanho = cJSON_GetArraySize(array_dados);
    
    for (int i = 0; i < tamanho; i++) {
        cJSON *item = cJSON_GetArrayItem(array_dados, i);
        char *numero_registro = cJSON_GetObjectItem(item, "renamaut")->valuestring;
        MaquinaAutonoma *aux = buscar(numero_registro, *lista);

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

        inserir(lista, novo);
    }

    cJSON_Delete(base_dados);
}

void importar(const char *filename, MaquinaAutonoma **lista) {
    FILE *file = fopen(filename, "r");

    if (!file) {
        printf("\nErro ao abrir o arquivo.");
        return;
    }

    fseek(file, 0, SEEK_END);

    long file_size = ftell(file);

    rewind(file);

    char *buffer = (char *)malloc(file_size + 1);

    if (!buffer) {
        fclose(file);
        printf("\nErro ao alocar memória para o arquivo.");
        return;
    }

    size_t bytesRead = fread(buffer, 1, file_size, file);

    if (bytesRead != file_size) {
        printf("\nErro na leitura do arquivo.");
        free(buffer);
        fclose(file);
        return;
    }

    buffer[bytesRead] = '\0';

    converter(buffer, lista);

    free(buffer);

    fclose(file);
}

MaquinaAutonoma* ler(const char *filename) {
    FILE *file = fopen(filename, "rb");

    if (!file) {
        return NULL;
    }

    MaquinaAutonoma *lista = NULL;
    MaquinaAutonoma *novo = NULL;

    while (1) {
        novo = malloc(sizeof(MaquinaAutonoma));

        if (!novo) {
            printf("\nErro de alocação de memória.");
            break;
        }

        if (fread(novo, sizeof(MaquinaAutonoma), 1, file) != 1) {
            free(novo);
            break;
        }

        novo->proxima = lista;
        lista = novo;
    }

    fclose(file);

    return lista;
}

void gravar(const char *filename, MaquinaAutonoma *lista) {
    FILE *file = fopen(filename, "wb");

    if (!file) {
        printf("\nErro ao abrir o arquivo.");
        return;
    }

    MaquinaAutonoma *aux = lista;

    while (aux != NULL) {
        fwrite(aux, sizeof(MaquinaAutonoma), 1, file);
        aux = aux->proxima;
    }

    fclose(file);
}

#endif