#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "../gov_dev/gov_dev.h"
#include <stdio.h>
#include <stdlib.h>
#include "operacoes_lista_encadeada.h"
#include <cjson/cJSON.h>

void converter(const char *buffer, MaquinaAutonoma **lista)
{
    cJSON *base_dados = cJSON_Parse(buffer);

    if (base_dados == NULL)
    {
        // Erro ao converter o JSON.
        printf("\n[ERRO] E400.05: Ocorreu um erro interno.\n");
        return;
    }

    cJSON *array_dados = cJSON_GetObjectItem(base_dados, "dados");

    if (array_dados == NULL)
    {
        // Erro ao resgatar os dados do JSON.
        cJSON_Delete(base_dados);
        printf("\n[ERRO] E400.06: Ocorreu um erro interno.\n");
        return;
    }

    int tamanho = cJSON_GetArraySize(array_dados);
    char mensagem[512];

    for (int i = 0; i < tamanho; i++)
    {
        cJSON *item = cJSON_GetArrayItem(array_dados, i);
        char *numero_registro = cJSON_GetObjectItem(item, "renamaut")->valuestring;
        MaquinaAutonoma *aux = buscar_numero_registro(numero_registro, *lista);

        if (aux != NULL)
            continue;

        const char *p_renamaut = numero_registro;

        if (validate_renamaut(p_renamaut) == 0)
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

        inserir(lista, novo);
    }

    cJSON_Delete(base_dados);
}

void importar_base(const char *filename, MaquinaAutonoma **lista)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        // Erro ao abrir o arquivo RENAMAUT.
        printf("\n[ERRO] E400.02: Ocorreu um erro interno.\n");
        return;
    }

    fseek(file, 0, SEEK_END);

    long file_size = ftell(file);

    rewind(file);

    char *buffer = (char *)malloc(file_size + 1);

    if (!buffer)
    {
        fclose(file);
        // Sem memória suficiente.
        printf("\n[ERRO] E400.03: Ocorreu um erro interno.\n");
        return;
    }

    size_t bytesRead = fread(buffer, 1, file_size, file);

    if (bytesRead != file_size)
    {
        // Os dados não foram lidos corretamente.
        printf("\n[ERRO] E400.04: Ocorreu um erro interno.\n");
        free(buffer);
        fclose(file);
        return;
    }

    buffer[bytesRead] = '\0';

    converter(buffer, lista);
    free(buffer);
    fclose(file);
}

void liberar(MaquinaAutonoma *m)
{
    if (!m)
        return;
    free(m->numero_registro);
    free(m->fabricante);
    free(m->modelo);
    free(m->categoria);
    free(m->aplicacao);
    free(m->responsavel);
    if (m->localizacao)
    {
        free(m->localizacao->cidade);
        free(m->localizacao->uf);
        free(m->localizacao);
    }
    free(m);
}

void importar_renamaut(const char *filename, MaquinaAutonoma **lista)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        file = fopen(filename, "w+");

        if (!file)
        {
            // Erro ao abrir o arquivo de persistência.
            printf("\n[ERRO] E400.01: Ocorreu um erro interno.\n");
            return;
        }
    }

    char linha[512];
    char mensagem[512];

    while (fgets(linha, sizeof(linha), file))
    {
        MaquinaAutonoma *novo = malloc(sizeof(MaquinaAutonoma));

        if (!novo)
        {
            fclose(file);
            return;
        }

        novo->localizacao = malloc(sizeof(Localizacao));

        if (!novo->localizacao)
        {
            free(novo);
            fclose(file);
            return;
        }

        linha[strcspn(linha, "\n")] = '\0';

        char *divisor = strtok(linha, "|");

        if (!divisor)
            continue;

        novo->numero_registro = strdup(divisor);

        if (validate_renamaut(novo->numero_registro) == 0)
        {
            free(novo->numero_registro);
            free(novo->localizacao);
            free(novo);
            continue;
        }

        divisor = strtok(NULL, "|");

        if (!divisor)
        {
            liberar(novo);
            continue;
        }

        novo->fabricante = strdup(divisor);

        divisor = strtok(NULL, "|");

        if (!divisor)
        {
            liberar(novo);
            continue;
        }

        novo->modelo = strdup(divisor);

        divisor = strtok(NULL, "|");

        if (!divisor)
        {
            liberar(novo);
            continue;
        }

        novo->categoria = strdup(divisor);

        divisor = strtok(NULL, "|");

        if (!divisor)
        {
            liberar(novo);
            continue;
        }

        novo->aplicacao = strdup(divisor);

        divisor = strtok(NULL, "|");

        if (!divisor)
        {
            liberar(novo);
            continue;
        }

        novo->ano_fabricacao = atoi(divisor);

        divisor = strtok(NULL, "|");

        if (!divisor)
        {
            liberar(novo);
            continue;
        }

        novo->responsavel = strdup(divisor);

        divisor = strtok(NULL, "|");

        if (!divisor)
        {
            liberar(novo);
            continue;
        }

        novo->status = atoi(divisor);

        divisor = strtok(NULL, "|");

        if (!divisor)
        {
            liberar(novo);
            continue;
        }

        novo->localizacao->cidade = strdup(divisor);

        divisor = strtok(NULL, "|");

        if (!divisor)
        {
            liberar(novo);
            continue;
        }

        novo->localizacao->uf = strdup(divisor);
        novo->proxima = *lista;
        *lista = novo;
    }

    fclose(file);
}

void gravar(const char *filename, MaquinaAutonoma *lista)
{
    FILE *file = fopen(filename, "w");

    if (!file)
        return;

    MaquinaAutonoma *aux = lista;

    while (aux != NULL)
    {
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