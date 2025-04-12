#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../gov_dev/gov_dev.h"

typedef struct {
    char *cidade;
    char *uf;
} Localizacao;

typedef struct MaquinaAutonoma {
    char *renamaut;
    char *fabricante;
    char *modelo;
    char *categoria;
    char *aplicacao;
    int ano_fabricacao;
    char *responsavel;
    int status;
    Localizacao *localizacao;
    struct MaquinaAutonoma *esq;
    struct MaquinaAutonoma *dir;
} MaquinaAutonoma;

MaquinaAutonoma *buscar_renamaut(char *renamaut, MaquinaAutonoma *no) {
    MaquinaAutonoma *aux = no;

    if(no == NULL) {
        return NULL;
    }

    int cmp = strcmp(renamaut, no->renamaut);

    if(cmp == 0) {
        return no;
    } else if(cmp < 0) {
        return buscar_renamaut(renamaut, no->esq);
    } else {
        return buscar_renamaut(renamaut, no->dir);
    }
}

MaquinaAutonoma *buscar_responsavel(char *responsavel, MaquinaAutonoma *no) {
    MaquinaAutonoma *aux = no;

    if(no == NULL) {
        return NULL;
    }

    int cmp = strcmp(responsavel, no->responsavel);

    if(cmp == 0) {
        return no;
    } else if(cmp < 0) {
        return buscar_responsavel(responsavel, no->esq);
    } else {
        return buscar_responsavel(responsavel, no->dir);
    }
}

void inserir(MaquinaAutonoma **no, MaquinaAutonoma *novo) {
    if(*no == NULL) {
        novo->esq = NULL;
        novo->dir = NULL;
        *no = novo;
    } else {
        int cmp = strcmp(novo->renamaut, (*no)->renamaut);

        if(cmp < 0) inserir(&((*no)->esq), novo);
        else inserir(&((*no)->dir), novo);
    }
}

void remover(MaquinaAutonoma **no, char *renamaut) {
    if(*no != NULL) {
        int cmp = strcmp(renamaut, (*no)->renamaut);

        if(cmp == 0) {
            if((*no)->esq == NULL && (*no)->dir == NULL) {
                free(*no);
                *no = NULL;
            } else if((*no)->esq == NULL) {
                MaquinaAutonoma *aux = *no;
                *no = (*no)->dir;
                free(aux);
            } else if((*no)->dir == NULL) {
                MaquinaAutonoma *aux = *no;
                *no = (*no)->esq;
                free(aux);
            } else {
                //TODO.
            }
        } else if(cmp < 0) {
            remover(&(*no)->esq, renamaut);
        } else {
            remover(&(*no)->dir, renamaut);
        }
    }
}

#endif