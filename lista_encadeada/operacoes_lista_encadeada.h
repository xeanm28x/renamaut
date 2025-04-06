#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../gov_dev/gov_dev.h"

typedef struct
{
    char *cidade;
    char *uf;
} Localizacao;

typedef struct MaquinaAutonoma
{
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

MaquinaAutonoma *buscar(const char *numero_registro, MaquinaAutonoma *lista)
{
    MaquinaAutonoma *p = lista;

    while(p != NULL)
    {
        if(strcmp(p->numero_registro, numero_registro) == 0) return p;
        p = p->proxima;
    }

    return NULL;
}

void inserir(MaquinaAutonoma **lista, MaquinaAutonoma *novo)
{
    novo->proxima = NULL;

    if(*lista == NULL) *lista = novo;
    else
    {
        MaquinaAutonoma *aux = *lista;

        while(aux->proxima != NULL) aux = aux->proxima;

        aux->proxima = novo;
    }
}

void remover(MaquinaAutonoma **lista, MaquinaAutonoma *p)
{
    if(*lista == NULL || p == NULL) return;

    if(*lista == p)
    {
        *lista = p->proxima;

        free(p->numero_registro);
        free(p->fabricante);
        free(p->modelo);
        free(p->categoria);
        free(p->aplicacao);
        free(p->responsavel);
        free(p->localizacao->cidade);
        free(p->localizacao->uf);
        free(p->localizacao);
        free(p);

        return;
    }

    MaquinaAutonoma *aux = *lista;

    while(aux->proxima != p) aux = aux->proxima;

    aux->proxima = p->proxima;

    free(p->numero_registro);
    free(p->fabricante);
    free(p->modelo);
    free(p->categoria);
    free(p->aplicacao);
    free(p->responsavel);
    free(p->localizacao->cidade);
    free(p->localizacao->uf);
    free(p->localizacao);
    free(p);
}

void imprimir(MaquinaAutonoma *celula)
{
    char mensagem[512];

    sprintf(
        mensagem,
        "\nNúmero de Registro: %s\n"
        "Fabricante: %s\n"
        "Modelo: %s\n"
        "Categoria: %s\n"
        "Aplicacao: %s\n"
        "Ano de Fabricação: %d\n"
        "Responsável: %s\n"
        "Status: %d\n"
        "Cidade: %s\n"
        "Estado: %s\n",
        celula->numero_registro,
        celula->fabricante,
        celula->modelo,
        celula->categoria,
        celula->aplicacao,
        celula->ano_fabricacao,
        celula->responsavel,
        celula->status,
        celula->localizacao->cidade,
        celula->localizacao->uf);

    wait_enter(mensagem);
}

void liberar_lista(MaquinaAutonoma **lista)
{
    MaquinaAutonoma *aux = *lista;
    MaquinaAutonoma *atual;

    while(aux != NULL)
    {
        atual = aux;
        aux = aux->proxima;
        remover(lista, atual);
    }

    *lista = NULL;
}

#endif