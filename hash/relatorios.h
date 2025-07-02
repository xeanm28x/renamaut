#ifndef RELATORIOS_H
#define RELATORIOS_H

#include "../gov_dev/gov_dev.h"
#include "operacoes_hash.h"
#include "log.h"
#include <stdio.h>
#include <string.h>

void imprimir(const Registro* reg) {
    char buffer[1024];
    char f_responsavel[15];


    if(strlen(reg->responsavel) < 14) {
        apply_mask_cpf(reg->responsavel, f_responsavel);
    } else {
        apply_mask_cnpj(reg->responsavel, f_responsavel);
    }

    snprintf(buffer, sizeof(buffer),
         "Número de Registro: %s\n"
         "Fabricante: %s\n"
         "Modelo: %s\n"
         "Categoria: %s\n"
         "Aplicacao: %s\n"
         "Ano de Fabricação: %d\n"
         "Responsável: %s\n"
         "Status: %s\n"
         "Cidade: %s\n"
         "Estado: %s\n",
         reg->renamaut,
         get_manufacturer_name_by_id(reg->fabricante),
         reg->modelo,
         get_category_name_by_code(reg->categoria),
         get_application_description_by_code(reg->aplicacao),
         reg->ano,
         f_responsavel,
         reg->status == 1 ? "Ativo" : "Inativo",
         reg->cidade,
         get_state_name_by_abbr(reg->estado)
    );
    printf("%s", buffer);
}

void relatorio_responsavel(HashTable* ht, const char* doc) {
    int encontrou_ativa = 0, encontrou_inativa = 0;

    printf("\nMáquinas ATIVAS:\n");
    for (int i = 0; i < ht->tamanho; i++) {
        Registro* r = ht->tabela[i];
        while (r) {
            LOG_DEBUG("Verificando registro: responsavel=%s, status=%s",
                      r->responsavel, r->status == 1 ? "Ativo" : "Inativo");

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

void relatorio_categoria(HashTable* ht, const char* categoria) {
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
