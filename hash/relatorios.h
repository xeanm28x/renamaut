#ifndef RELATORIOS_H
#define RELATORIOS_H

#include "operacoes_hash.h"
#include <stdio.h>
#include <string.h>

static inline void imprimir(const Registro* reg) {
    printf("\n===== Registro RENAMAUT =====\n");
    printf("Número: %s\n", reg->renamaut);
    printf("Modelo: %s\n", reg->modelo);
    printf("Fabricante: %s\n", reg->fabricante);
    printf("Ano: %d\n", reg->ano);
    printf("Responsável: %s\n", reg->responsavel);
    printf("Categoria: %s\n", reg->categoria);
    printf("Aplicação: %s\n", reg->aplicacao);
    printf("Status: %s\n", reg->status);
    printf("Localização: %s - %s\n", reg->cidade, reg->estado);
    printf("=============================\n");
}

static inline void relatorio_responsavel(HashTable* ht, const char* doc) {
    printf("\nMáquinas ATIVAS:\n");
    for (int i = 0; i < ht->tamanho; i++) {
        Registro* r = ht->tabela[i];
        while (r) {
            if (strcmp(r->responsavel, doc) == 0 && strcmp(r->status, "ativo") == 0) {
                printf("%s;%s;%s;%d;%s;%s-%s\n", r->renamaut, r->responsavel, r->modelo, r->ano, r->status, r->cidade, r->estado);
            }
            r = r->prox;
        }
    }

    printf("\nMáquinas INATIVAS:\n");
    for (int i = 0; i < ht->tamanho; i++) {
        Registro* r = ht->tabela[i];
        while (r) {
            if (strcmp(r->responsavel, doc) == 0 && strcmp(r->status, "inativo") == 0) {
                printf("%s;%s;%s;%d;%s;%s-%s\n", r->renamaut, r->responsavel, r->modelo, r->ano, r->status, r->cidade, r->estado);
            }
            r = r->prox;
        }
    }
}

static inline void relatorio_categoria(HashTable* ht, const char* categoria) {
    for (char uf1 = 'A'; uf1 <= 'Z'; uf1++) {
        for (char uf2 = 'A'; uf2 <= 'Z'; uf2++) {
            int encontrou = 0;
            for (int i = 0; i < ht->tamanho; i++) {
                Registro* r = ht->tabela[i];
                while (r) {
                    if (strcmp(r->categoria, categoria) == 0 &&
                        r->estado[0] == uf1 && r->estado[1] == uf2 &&
                        strcmp(r->status, "ativo") == 0) {
                        if (!encontrou) {
                            printf("\nEstado: %c%c\n", uf1, uf2);
                            encontrou = 1;
                        }
                        printf("%s;%s;%s;%d;%s;%s-%s\n", r->renamaut, r->responsavel, r->modelo, r->ano, r->status, r->cidade, r->estado);
                    }
                    r = r->prox;
                }
            }
        }
    }
}

#endif
