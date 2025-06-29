#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../gov_dev/gov_dev.h"

#define TAM_TABELA 1000
#define ASPAS "\""
typedef struct Localizacao {
    char *cidade;
    char *uf;
} Localizacao;

typedef struct MaquinaAutonoma {
    char *numero_registro;
    char *fabricante;
    char *modelo;
    char *categoria;
    char *aplicacao;
    int ano_fabricacao;
    char *responsavel;
    int status;
    Localizacao *localizacao;
    struct MaquinaAutonoma *proxima;
} MaquinaAutonoma;

typedef struct {
    MaquinaAutonoma *tabela[TAM_TABELA];
} TabelaDispersao;

unsigned int hash(const char *chave) {
    unsigned long hash = 5381;
    int c;
    while ((c = *chave++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TAM_TABELA;
}

TabelaDispersao *criar_tabela() {
    TabelaDispersao *tabela = malloc(sizeof(TabelaDispersao));
    if (tabela != NULL) {
        for (int i = 0; i < TAM_TABELA; i++) {
            tabela->tabela[i] = NULL;
        }
    }
    return tabela;
}

void inserir(TabelaDispersao *tabela, MaquinaAutonoma *maquina) {
    if (!tabela || !maquina || !maquina->numero_registro) return;

    unsigned int indice = hash(maquina->numero_registro);
    maquina->proxima = tabela->tabela[indice];
    tabela->tabela[indice] = maquina;
}

MaquinaAutonoma *buscar_numero_registro(const char *numero, TabelaDispersao *tabela) {
    if (!tabela || !numero) return NULL;

    unsigned int indice = hash(numero);
    MaquinaAutonoma *atual = tabela->tabela[indice];

    while (atual != NULL) {
        if (strcmp(atual->numero_registro, numero) == 0) {
            return atual;
        }
        atual = atual->proxima;
    }

    return NULL;
}

MaquinaAutonoma *buscar_responsavel(const char *responsavel, TabelaDispersao *tabela) {
    if (!tabela || !responsavel) return NULL;

    MaquinaAutonoma *lista = NULL;

    for (int i = 0; i < TAM_TABELA; i++) {
        MaquinaAutonoma *atual = tabela->tabela[i];
        while (atual != NULL) {
            if (strcmp(atual->responsavel, responsavel) == 0) {
                MaquinaAutonoma *copia = malloc(sizeof(MaquinaAutonoma));
                *copia = *atual;
                copia->numero_registro = strdup(atual->numero_registro);
                copia->fabricante = strdup(atual->fabricante);
                copia->modelo = strdup(atual->modelo);
                copia->categoria = strdup(atual->categoria);
                copia->aplicacao = strdup(atual->aplicacao);
                copia->responsavel = strdup(atual->responsavel);
                copia->status = atual->status;
                copia->ano_fabricacao = atual->ano_fabricacao;

                copia->localizacao = malloc(sizeof(Localizacao));
                copia->localizacao->cidade = strdup(atual->localizacao->cidade);
                copia->localizacao->uf = strdup(atual->localizacao->uf);

                copia->proxima = lista;
                lista = copia;
            }
            atual = atual->proxima;
        }
    }

    return lista;
}

void inativar(TabelaDispersao *tabela, MaquinaAutonoma *maquina) {
    if (!tabela || !maquina) return;

    MaquinaAutonoma *encontrada = buscar_numero_registro(maquina->numero_registro, tabela);
    if (encontrada) {
        encontrada->status = 0;
    }
}

void imprimir(MaquinaAutonoma *maquina) {
    if (!maquina) return;

    printf("\nRegistro: %s\n", maquina->numero_registro);
    printf("Fabricante: %s\n", maquina->fabricante);
    printf("Modelo: %s\n", maquina->modelo);
    printf("Categoria: %s\n", maquina->categoria);
    printf("Aplicação: %s\n", maquina->aplicacao);
    printf("Ano de fabricação: %d\n", maquina->ano_fabricacao);
    printf("Responsável: %s\n", maquina->responsavel);
    printf("Status: %s\n", maquina->status ? "Ativa" : "Inativa");
    printf("Localização: %s - %s\n", maquina->localizacao->cidade, maquina->localizacao->uf);
}

void imprimir_linha_relatorio(MaquinaAutonoma *maquina) {
    if (!maquina) return;

    printf("%s | %s | %s | %s | %s | %d | %s | %s | %s\n",
           maquina->numero_registro,
           maquina->fabricante,
           maquina->modelo,
           maquina->categoria,
           maquina->aplicacao,
           maquina->ano_fabricacao,
           maquina->responsavel,
           maquina->localizacao->cidade,
           maquina->localizacao->uf);
}

void liberar_tabela(TabelaDispersao *tabela) {
    if (!tabela) return;

    for (int i = 0; i < TAM_TABELA; i++) {
        MaquinaAutonoma *atual = tabela->tabela[i];
        while (atual != NULL) {
            MaquinaAutonoma *temp = atual;
            atual = atual->proxima;

            free(temp->numero_registro);
            free(temp->fabricante);
            free(temp->modelo);
            free(temp->categoria);
            free(temp->aplicacao);
            free(temp->responsavel);
            free(temp->localizacao->cidade);
            free(temp->localizacao->uf);
            free(temp->localizacao);
            free(temp);
        }
    }

    free(tabela);
}

#endif