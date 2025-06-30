#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "operacoes_hash.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Salva todos os registros da tabela de dispersão em um arquivo binário
static inline void salvar_tabela(HashTable* ht, const char* nome_arquivo)
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

// Carrega os registros de um arquivo binário para a tabela de dispersão
static inline void carregar_tabela(HashTable* ht, const char* nome_arquivo)
{
    printf("Carregando registros do arquivo: %s\n", nome_arquivo);
    if (!ht || !nome_arquivo) {
        fprintf(stderr, "Tabela ou nome do arquivo inválidos.\n");
        return;
    }
    FILE* f = fopen(nome_arquivo, "rb");
    if (!f) {
        // Pode não existir na primeira execução
        return;
    }

    Registro temp;
    while (fread(&temp, sizeof(Registro), 1, f) == 1) {
        printf("-> %s | %s | %s | %d | %s | %s-%s\n",
            temp.renamaut, temp.modelo, temp.fabricante, temp.ano,
            temp.status, temp.cidade, temp.estado);

        Registro novo;
        memcpy(&novo, &temp, sizeof(Registro));
        printf("Carregando registro: %s\n", novo.renamaut);
        novo.prox = NULL;
        inserir(ht, novo);
    }

    fclose(f);
}

static inline void importar_txt(HashTable* ht, const char* nome_arquivo) {
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
        if (token) strcpy(reg.status, strcmp(token, "1") == 0 ? "ativo" : "inativo");

        token = strtok(NULL, "|");
        if (token) strcpy(reg.cidade, token);

        token = strtok(NULL, "|\n");
        if (token) strcpy(reg.estado, token);

        inserir(ht, reg);
        LOG_DEBUG("Importando registro: renamaut=%s, fabricante=%s, modelo=%s, categoria=%s, aplicacao=%s, ano=%d, responsavel=%s, status=%s, cidade=%s, estado=%s",
              reg.renamaut, reg.fabricante, reg.modelo, reg.categoria, reg.aplicacao, reg.ano, reg.responsavel, reg.status, reg.cidade, reg.estado);
        printf("-> %s | %s | %s | %d | %s | %s-%s\n",
               reg.renamaut, reg.modelo, reg.fabricante, reg.ano,
               reg.status, reg.cidade, reg.estado);
    
    }

    fclose(f);
    printf("[importar_txt] Importação concluída.\n");
}


#endif // PERSISTENCIA_H
