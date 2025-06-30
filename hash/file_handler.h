#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operacoes_hash.h"

void importar_json(HashTable *ht, const char *arquivo_json) {
    FILE *f = fopen(arquivo_json, "r");
    if (!f) {
        perror("Erro ao abrir JSON");
        return;
    }

    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    rewind(f);

    char *conteudo = malloc(tamanho + 1);
    fread(conteudo, 1, tamanho, f);
    conteudo[tamanho] = '\0';
    fclose(f);

    cJSON *root = cJSON_Parse(conteudo);
    free(conteudo);

    if (!root) {
        fprintf(stderr, "Erro ao fazer parse do JSON.\n");
        return;
    }

    cJSON *dados = cJSON_GetObjectItem(root, "dados");
    if (!cJSON_IsArray(dados)) {
        fprintf(stderr, "Campo 'dados' inválido no JSON.\n");
        cJSON_Delete(root);
        return;
    }

    FILE *out = fopen("../base_dados/base_renamaut_hash.txt", "w");
    if (!out) {
        perror("Erro ao criar base_renamaut_hash.txt");
        cJSON_Delete(root);
        return;
    }

    cJSON *item = NULL;
    cJSON_ArrayForEach(item, dados) {
        Registro r = {0};

        strcpy(r.renamaut, cJSON_GetObjectItem(item, "renamaut")->valuestring);
        strcpy(r.fabricante, cJSON_GetObjectItem(item, "fab")->valuestring);
        strcpy(r.modelo, cJSON_GetObjectItem(item, "mod")->valuestring);
        strcpy(r.categoria, cJSON_GetObjectItem(item, "cat")->valuestring);
        strcpy(r.aplicacao, cJSON_GetObjectItem(item, "apl")->valuestring);

        cJSON *anoItem = cJSON_GetObjectItem(item, "ano");
        r.ano = (cJSON_IsNumber(anoItem)) ? anoItem->valueint : 0;

        strcpy(r.responsavel, cJSON_GetObjectItem(item, "resp")->valuestring);

        cJSON *statusItem = cJSON_GetObjectItem(item, "status");
        r.status = (cJSON_IsNumber(statusItem)) ? statusItem->valueint : 1; /*Como padrao e o ativo*/

        cJSON *loc_base = cJSON_GetObjectItem(item, "loc_base");
        if (loc_base) {
            strcpy(r.cidade, cJSON_GetObjectItem(loc_base, "cidade")->valuestring);
            strcpy(r.estado, cJSON_GetObjectItem(loc_base, "uf")->valuestring);
        }

        r.prox = NULL;
        inserir(ht, r);

        fprintf(out, "%s|%s|%s|%s|%s|%d|%s|%d|%s|%s\n",
                r.renamaut, r.fabricante, r.modelo,
                r.categoria, r.aplicacao, r.ano,
                r.responsavel, r.status, r.cidade, r.estado);
    }

    fclose(out);
    cJSON_Delete(root);
}


int contar_registros_json(const char *arquivo_json) {
    FILE *f = fopen(arquivo_json, "r");
    if (!f) {
        perror("Erro ao abrir JSON");
        return 0;
    }

    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    rewind(f);

    char *conteudo = malloc(tamanho + 1);
    fread(conteudo, 1, tamanho, f);
    conteudo[tamanho] = '\0';
    fclose(f);

    cJSON *root = cJSON_Parse(conteudo);
    free(conteudo);

    if (!root) {
        free(conteudo);
        return 0;
    }

    cJSON *dados = cJSON_GetObjectItem(root, "dados");
    int qtd = cJSON_GetArraySize(dados);

    cJSON_Delete(root);
    return qtd;
}
