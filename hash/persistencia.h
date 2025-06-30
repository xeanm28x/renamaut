#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "operacoes_hash.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* salva os dados todos registros em um arquivo*/
void salvar_tabela(HashTable* ht, const char* nome_arquivo)
{
    FILE* f = fopen(nome_arquivo, "wb");
    if (!f) {
        perror("Erro ao abrir arquivo para escrita");
        return;
    }

    for (int i = 0; i < ht->tamanho; i++) {
        Registro* atual = ht->tabela[i];
        while (atual) {
            fwrite(atual, sizeof(Registro), 1, f);
            atual = atual->prox;
        }
    }

    fclose(f);
}

/*feito a carga de registros de um arquivo binario para a tabela*/
void carregar_tabela(HashTable* ht, const char* nome_arquivo)
{
    printf("Carregando registros do arquivo: %s\n", nome_arquivo);
    if (!ht || !nome_arquivo) {
        fprintf(stderr, "Tabela ou nome do arquivo inválidos.\n");
        return;
    }
    FILE* f = fopen(nome_arquivo, "rb");
    if (!f) {
        /*nao existe na primeira execucao*/
        return;
    }

    Registro temp;
    while (fread(&temp, sizeof(Registro), 1, f) == 1) {
        printf("-> %s | %s | %s | %d | %d | %s-%s\n",
            temp.renamaut, temp.modelo, temp.fabricante, temp.ano,
            temp.status, temp.cidade, temp.estado);

        Registro novo;
        memcpy(&novo, &temp, sizeof(Registro));
        LOG_DEBUG("Carregando registro: %s", novo.renamaut);
        novo.prox = NULL;
        inserir(ht, novo);
    }

    fclose(f);
}

void importar_txt(HashTable* ht, const char* nome_arquivo) {
    FILE* f = fopen(nome_arquivo, "r");
    if (!f) {
        perror("[importar_txt] Erro ao abrir arquivo");
        return;
    }

    printf("[importar_txt] Carregando registros do arquivo TXT...\n");

    char linha[512];
    while (fgets(linha, sizeof(linha), f)) {
        Registro reg;
        memset(&reg, 0, sizeof(Registro));

        char *token = strtok(linha, "|");
        if (token) strcpy(reg.renamaut, token);

        token = strtok(NULL, "|");
        if (token) strcpy(reg.fabricante, token);

        token = strtok(NULL, "|");
        if (token) strcpy(reg.modelo, token);

        token = strtok(NULL, "|");
        if (token) strcpy(reg.categoria, token);

        token = strtok(NULL, "|");
        if (token) strcpy(reg.aplicacao, token);

        token = strtok(NULL, "|");
        if (token) reg.ano = atoi(token);

        token = strtok(NULL, "|");
        if (token) strcpy(reg.responsavel, token);

        token = strtok(NULL, "|");
        if (token) reg.status = atoi(token);  // ← Agora é int

        token = strtok(NULL, "|");
        if (token) strcpy(reg.cidade, token);

        token = strtok(NULL, "|\n");
        if (token) strcpy(reg.estado, token);

        inserir(ht, reg);

        LOG_DEBUG("Importando registro: renamaut=%s, fabricante=%s, modelo=%s, categoria=%s, aplicacao=%s, ano=%d, responsavel=%s, status=%d, cidade=%s, estado=%s",
            reg.renamaut, reg.fabricante, reg.modelo, reg.categoria, reg.aplicacao, reg.ano, reg.responsavel, reg.status, reg.cidade, reg.estado);

        printf("-> %s | %s | %s | %d | %d | %s-%s\n",
            reg.renamaut, reg.modelo, reg.fabricante, reg.ano,
            reg.status, reg.cidade, reg.estado);
    }

    fclose(f);
    printf("[importar_txt] Importação concluída.\n");
}

void exportar_para_txt(HashTable* ht, const char* nome_arquivo) {
    if (!ht || !nome_arquivo) {
        LOG_DEBUG("[exportar_para_txt] Tabela ou nome de arquivo inválidos.");
        return;
    }

    FILE* f = fopen(nome_arquivo, "w");
    if (!f) {
        LOG_DEBUG("[exportar_para_txt] Erro ao abrir arquivo '%s' para escrita.", nome_arquivo);
        return;
    }

    for (int i = 0; i < ht->tamanho; i++) {
        Registro* r = ht->tabela[i];
        while (r) {
            int status_num = r->status;
            fprintf(f, "%s|%s|%s|%s|%s|%d|%s|%d|%s|%s\n",
                    r->renamaut, r->fabricante, r->modelo,
                    r->categoria, r->aplicacao, r->ano,
                    r->responsavel, status_num, r->cidade, r->estado);

            r = r->prox;
        }
    }

    fclose(f);
    LOG_DEBUG("[exportar_para_txt] Exportação concluída para o arquivo '%s'", nome_arquivo);
}

#endif
