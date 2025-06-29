#ifndef OPERACOES_HASH_H
#define OPERACOES_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MODELO 50
#define TAM_CHAVE 17  // incluindo '\0' para "XXX-XXX-XXXX-XXXXXX"

// Struct para registro da máquina
typedef struct Registro {
    char renamaut[20];
    char fabricante[20];
    char modelo[50];
    char categoria[10];
    char aplicacao[10];
    int ano;
    char responsavel[20];
    char status[10];  // "ativo" ou "inativo"
    char cidade[100];
    char estado[3];
    struct Registro* prox;
} Registro;

// Tabela hash com encadeamento externo
typedef struct {
    int tamanho;
    Registro** tabela;
} HashTable;

// Função hash
static inline int funcao_hash(const char* chave, int tamanho) {
    unsigned long hash = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        hash = (hash * 31 + chave[i]) % tamanho;
    }
    return hash;
}

// Inicializa tabela
static inline HashTable* criar_tabela(int tamanho) {
    HashTable* ht = malloc(sizeof(HashTable));
    ht->tamanho = tamanho;
    ht->tabela = calloc(tamanho, sizeof(Registro*));
    return ht;
}

// Insere na tabela
static inline void inserir(HashTable* ht, Registro reg) {
    int idx = funcao_hash(reg.renamaut, ht->tamanho);
    Registro* novo = malloc(sizeof(Registro));
    *novo = reg;
    novo->prox = ht->tabela[idx];
    ht->tabela[idx] = novo;
}

// Busca por RENAMAUT
static inline Registro* buscar(HashTable* ht, const char* chave) {
    int idx = funcao_hash(chave, ht->tamanho);
    Registro* atual = ht->tabela[idx];
    while (atual) {
        if (strcmp(atual->renamaut, chave) == 0)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}


// Inativa registro
static inline int inativar(HashTable* ht, const char* chave) {
    Registro* reg = buscar(ht, chave);
    if (reg && strcmp(reg->status, "ativo") == 0) {
        strcpy(reg->status, "inativo");
        return 1;
    }
    return 0;
}

// Libera tabela
static inline void liberar_tabela(HashTable* ht) {
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

// Estimar tamanho ideal da tabela
static inline int estimar_tamanho_tabela(int n) {
    int m = (int)(1.5 * n);
    while (1) {
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
}

#endif // OPERACOES_HASH_H
