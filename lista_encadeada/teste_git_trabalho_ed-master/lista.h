#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MaquinaAutonoma {
    char numero_registro[17];
    char fabricante[5];
    char modelo[51];
    char categoria[4];
    char aplicacao[4];
    int ano_fabricacao;
    char responsavel[15];
    int status;
    char localizacao[60];
    struct MaquinaAutonoma *proximo;  // Necessário para encadeamento
} MaquinaAutonoma;


void adicionar_elemento_lista(MaquinaAutonoma **lst, MaquinaAutonoma *novo) {
    novo->proximo = *lst;  // O novo elemento aponta para o primeiro da lista
    *lst = novo;  // Atualiza a cabeça da lista

    printf("✅ Máquina %s adicionada à lista.\n", novo->numero_registro);
}

void remover_elemento_lista(MaquinaAutonoma **lst, char *numero_registro) {
    MaquinaAutonoma *atual = *lst, *anterior = NULL;

    // Percorre a lista procurando o elemento
    while (atual != NULL && strcmp(atual->numero_registro, numero_registro) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    // Se não encontrou o elemento
    if (!atual) {
        printf("⚠️ Erro: Máquina com número %s não encontrada.\n", numero_registro);
        return;
    }

    // Se for o primeiro elemento da lista
    if (!anterior) {
        *lst = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual);  // Libera a memória do nó removido
    printf("🗑️ Máquina %s removida da lista.\n", numero_registro);
}
