#ifndef RELATORIOS_H
#define RELATORIOS_H

#include "../gov_dev/gov_dev.h"
#include "operacoes_hash.h"
#include "log.h"
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
    printf("Status: %s\n", reg->status == 1 ? "ativo" : "inativo");
    printf("Localização: %s - %s\n", reg->cidade, reg->estado);
    printf("=============================\n");
}

static inline void relatorio_responsavel(HashTable* ht, const char* doc) {
    int encontrou_ativa = 0, encontrou_inativa = 0;

    printf("\nMáquinas ATIVAS:\n");
    for (int i = 0; i < ht->tamanho; i++) {
        Registro* r = ht->tabela[i];
        while (r) {
            LOG_DEBUG("Verificando registro: responsavel=%s, status=%s",
                      r->responsavel, r->status == 1 ? "ativo" : "inativo");

            if (strcmp(r->responsavel, doc) == 0 && r->status == 1) {
                printf("%s;%s;%s;%d;%s;%s-%s\n",
                       r->renamaut, r->responsavel, r->modelo, r->ano,
                       "ativo", r->cidade, r->estado);
                encontrou_ativa = 1;
            }
            r = r->prox;
        }
    }
    LOG_DEBUG("===========");
    if (!encontrou_ativa)
        printf("Nenhuma máquina ativa encontrada.\n");
    LOG_DEBUG("===========");

    printf("\nMáquinas INATIVAS:\n");
    for (int i = 0; i < ht->tamanho; i++) {
        Registro* r = ht->tabela[i];
        while (r) {
            LOG_DEBUG("Verificando registro: responsavel=%s, status=%s",
                      r->responsavel, r->status == 1 ? "ativo" : "inativo");

            if (strcmp(r->responsavel, doc) == 0 && r->status == 0) {
                printf("%s;%s;%s;%d;%s;%s-%s\n",
                       r->renamaut, r->responsavel, r->modelo, r->ano,
                       "inativo", r->cidade, r->estado);
                encontrou_inativa = 1;
            }
            r = r->prox;
        }
    }
    if (!encontrou_inativa)
        printf("Nenhuma máquina inativa encontrada.\n");

    LOG_DEBUG("===========");
}

static inline void relatorio_categoria(HashTable* ht, const char* categoria) {
    char estados_visitados[27 * 27][3];
    int total_estados = 0;

    for (int i = 0; i < ht->tamanho; i++) {
        Registro* r = ht->tabela[i];
        while (r) {
            if (strcmp(r->categoria, categoria) == 0 && r->status == 1) {
                // Verifica se o estado já foi impresso
                int ja_impresso = 0;
                for (int j = 0; j < total_estados; j++) {
                    if (strncmp(estados_visitados[j], r->estado, 2) == 0) {
                        ja_impresso = 1;
                        break;
                    }
                }

                if (!ja_impresso) {
                    printf("\nEstado: %s\n", r->estado);

                    for (int k = 0; k < ht->tamanho; k++) {
                        Registro* r2 = ht->tabela[k];
                        while (r2) {
                            if (strcmp(r2->categoria, categoria) == 0 &&
                                r2->status == 1 &&
                                strncmp(r2->estado, r->estado, 2) == 0)
                            {
                                printf("%s;%s;%s;%d;%s;%s-%s\n",
                                       r2->renamaut, r2->responsavel, r2->modelo,
                                       r2->ano, "ativo", r2->cidade, r2->estado);
                            }
                            r2 = r2->prox;
                        }
                    }

                    strncpy(estados_visitados[total_estados], r->estado, 2);
                    estados_visitados[total_estados][2] = '\0';
                    total_estados++;
                }
            }
            r = r->prox;
        }
    }
}

#endif
