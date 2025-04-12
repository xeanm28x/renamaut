#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include "operacoes_lista_encadeada.h"
#include <cjson/cJSON.h>

int converter(const char *buffer, MaquinaAutonoma *lista) {
    cJSON *base_dados = cJSON_Parse(buffer);

    if(base_dados == NULL) {
        printf("\nOcorreu um erro na conversão JSON.\n");
        return 0;
    }

    cJSON *array_dados = cJSON_GetObjectItem(base_dados, "dados");

    if(array_dados == NULL) {
        cJSON_Delete(base_dados);
        printf("\nOcorreu um erro ao resgatar os dados após a conversão.\n");
        return 0;
    }

    int tamanho = cJSON_GetArraySize(array_dados);
    char mensagem[512];
    
    for (int i = 0; i < tamanho; i++) {
        cJSON *item = cJSON_GetArrayItem(array_dados, i);
        char *numero_registro = cJSON_GetObjectItem(item, "renamaut")->valuestring;
        MaquinaAutonoma *aux = buscar_numero_registro(numero_registro, lista);

        if(aux != NULL) continue;

        const char *p_renamaut = numero_registro;

        if(validate_renamaut(p_renamaut) == 0)
            continue;

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

    return 1;
}

int importar(const char *filename, MaquinaAutonoma *lista) {
    FILE *file = fopen(filename, "r");

    if (!file) {
        printf("\nErro ao abrir o arquivo.\n");
        return 0;
    }

    fseek(file, 0, SEEK_END);

    long file_size = ftell(file);

    rewind(file);

    char *buffer = (char *)malloc(file_size + 1);

    if (!buffer) {
        fclose(file);
        printf("\nErro ao alocar memória para o arquivo.\n");
        return 0;
    }

    size_t bytesRead = fread(buffer, 1, file_size, file);

    if (bytesRead != file_size) {
        printf("\nErro na leitura do arquivo.\n");
        free(buffer);
        fclose(file);
        return 0;
    }

    buffer[bytesRead] = '\0';

    int conversao = converter(buffer, lista);

    free(buffer);

    fclose(file);

    if(conversao == 0) return 0;

    return 1;
}

MaquinaAutonoma* ler(const char *filename) {
    FILE *file = fopen(filename, "r");

    if (!file) {
        file = fopen(filename, "w+");
    }

    MaquinaAutonoma *lista = NULL;

    char linha[512];
    char mensagem[512];

    while (fgets(linha, sizeof(linha), file)) {
        MaquinaAutonoma *novo = malloc(sizeof(MaquinaAutonoma));
        novo->localizacao = malloc(sizeof(Localizacao));

        linha[strcspn(linha, "\n")] = '\0';

        char *divisor = strtok(linha, "|");
        
        if (!divisor) continue;

        const char *renamaut = strdup(divisor);

        sprintf(mensagem, "\n%s\n", renamaut);
        wait_enter(mensagem);

        if(validate_renamaut(renamaut) == 0) {
            continue;
        }

        novo->numero_registro = strdup(divisor);

        divisor = strtok(NULL, "|");
        if (!divisor) continue;
        novo->fabricante = strdup(divisor);

        divisor = strtok(NULL, "|");
        if (!divisor) continue;
        novo->modelo = strdup(divisor);

        divisor = strtok(NULL, "|");
        if (!divisor) continue;
        novo->categoria = strdup(divisor);

        divisor = strtok(NULL, "|");
        if (!divisor) continue;
        novo->aplicacao = strdup(divisor);

        divisor = strtok(NULL, "|");
        if (!divisor) continue;
        novo->ano_fabricacao = atoi(divisor);

        divisor = strtok(NULL, "|");
        if (!divisor) continue;
        novo->responsavel = strdup(divisor);

        divisor = strtok(NULL, "|");
        if (!divisor) continue;
        novo->status = atoi(divisor);

        divisor = strtok(NULL, "|");
        if (!divisor) continue;
        novo->localizacao->cidade = strdup(divisor);

        divisor = strtok(NULL, "|");
        if (!divisor) continue;
        novo->localizacao->uf = strdup(divisor);
        
        novo->proxima = lista;
        lista = novo;
    }

    fclose(file);

    return lista;
}

void gravar(const char *filename, MaquinaAutonoma *lista) {
    FILE *file = fopen(filename, "w");

    if (!file) {
        printf("\nErro ao abrir o arquivo.\n");
        return;
    }

    MaquinaAutonoma *aux = lista;

    while (aux != NULL) {
        fprintf(file, "%s|%s|%s|%s|%s|%d|%s|%d|%s|%s\n",
            aux->numero_registro,
            aux->fabricante,
            aux->modelo,
            aux->categoria,
            aux->aplicacao,
            aux->ano_fabricacao,
            aux->responsavel,
            aux->status,
            aux->localizacao->cidade,
            aux->localizacao->uf);
        
        aux = aux->proxima;
    }

    fclose(file);
}

#endif