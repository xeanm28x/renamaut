#ifndef OPERACOES_HASH_H
#define OPERACOES_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "log.h"
#include "../gov_dev/gov_dev.h"

#define MAX_MODELO 50
#define TAM_CHAVE 17  /* incluindo \0 quando tem XXX-XXX-XXXX-XXXXXX */

typedef struct Registro {
    char renamaut[20];
    char fabricante[20];
    char modelo[50];
    char categoria[10];
    char aplicacao[10];
    int ano;
    char responsavel[20];
    int status;              /*0 para inativo e 1 para ativo*/
    char cidade[100];
    char estado[3];
    struct Registro* prox;
} Registro;

/*Tabela hash com encadeamento externo*/
typedef struct {
    int tamanho;
    Registro** tabela;
} HashTable;

/*Função hash djb2*/
int funcao_hash(const char* chave, int tamanho) {
    unsigned long hash = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        hash = (hash * 31 + chave[i]) % tamanho;
    }
    return hash;
}


HashTable* criar_tabela(int tamanho) {
    HashTable* ht = malloc(sizeof(HashTable));
    ht->tamanho = tamanho;
    ht->tabela = calloc(tamanho, sizeof(Registro*));
    return ht;
}

Registro* buscar(HashTable* ht, const char* chave) {
    int idx = funcao_hash(chave, ht->tamanho);
    Registro* atual = ht->tabela[idx];
    while (atual) {
        // LOG_DEBUG("Comparando com '%s'", atual->renamaut);
        if (strcmp(atual->renamaut, chave) == 0){
            // LOG_DEBUG("Chave '%s', Responsável '%s' ", atual->renamaut, atual->responsavel);
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void inserir(HashTable* ht, Registro reg) {
    char chave_normalizada[20];
    remove_mask(reg.renamaut, chave_normalizada);  // Remove a máscara (ex: hífens)
    // LOG_DEBUG("Inserindo registro: '%s'", chave_normalizada);
    if (buscar(ht, chave_normalizada) != NULL) {
        // LOG_DEBUG("Registro com chave '%s' já existe.", chave_normalizada);
        return; // Já existe
    }

    strcpy(reg.renamaut, chave_normalizada); // Armazena SEM máscara

    int idx = funcao_hash(reg.renamaut, ht->tamanho);
    Registro* novo = malloc(sizeof(Registro));
    *novo = reg;
    novo->prox = ht->tabela[idx];
    ht->tabela[idx] = novo;
}


int inativar(HashTable* ht, const char* chave) {
    Registro* reg = buscar(ht, chave);
    if (reg && reg->status == 1) {
        reg->status = 0;
        return 1;
    }
    return 0;
}

void liberar_tabela(HashTable* ht) {
    for (int i = 0; i < ht->tamanho; i++) {
        Registro* atual = ht->tabela[i];
        while (atual) {
            Registro* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(ht->tabela);
    free(ht);
}

int estimar_tamanho_tabela(int n) {
    int limite_max = 150001;
    int m = (int)(1.5 * n);
    if (m > limite_max) m = limite_max;
    while (m <= limite_max) {
        int primo = 1;
        for (int i = 2; i * i <= m; i++) {
            if (m % i == 0) {
                primo = 0;
                break;
            }
        }
        if (primo) return m;
        m++;
    }
    // Se não encontrar primo até o limite, retorna o próprio limite
    return limite_max;
}

#endif
