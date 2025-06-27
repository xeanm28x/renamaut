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

MaquinaAutonoma *buscar_numero_registro(const char *numero_registro, MaquinaAutonoma *lista)
{
    MaquinaAutonoma *p = lista;

    while (p != NULL)
    {
        if (strcmp(p->numero_registro, numero_registro) == 0)
            return p;
        p = p->proxima;
    }

    return NULL;
}

MaquinaAutonoma *buscar_responsavel(char *responsavel, MaquinaAutonoma *lista)
{
    MaquinaAutonoma *p = lista;

    while (p != NULL)
    {
        if (strcmp(p->responsavel, responsavel) == 0)
            return p;
        p = p->proxima;
    }

    return NULL;
}

void inserir(MaquinaAutonoma **lista, MaquinaAutonoma *novo)
{
    novo->proxima = NULL;

    if (*lista == NULL)
        *lista = novo;
    else
    {
        MaquinaAutonoma *aux = *lista;

        while (aux->proxima != NULL)
            aux = aux->proxima;

        aux->proxima = novo;
    }
}

void remover(MaquinaAutonoma **lista, MaquinaAutonoma *p)
{
    if (*lista == NULL || p == NULL)
        return;

    if (*lista == p)
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

    while (aux != NULL && aux->proxima != p)
        aux = aux->proxima;

    if (aux == NULL)
        return;

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

void inativar(MaquinaAutonoma **lista, MaquinaAutonoma *p)
{
    if (*lista != NULL)
    {
        MaquinaAutonoma *aux = *lista;

        while (aux != p)
            aux = aux->proxima;

        aux->status = 0;
    }
}

void imprimir(MaquinaAutonoma *celula)
{
    char mensagem[512];

    const char *numero_registro = celula->numero_registro;
    char f_numero_registro[17];
    const char *fabricante = celula->fabricante;
    const char *categoria = celula->categoria;
    const char *aplicacao = celula->aplicacao;
    const char *responsavel = celula->responsavel;
    const char *uf = celula->localizacao->uf;

    const char *nome_fabricante = get_manufacturer_name_by_id(fabricante);
    const char *desc_categoria = get_category_name_by_code(categoria);
    const char *desc_aplicacao = get_application_description_by_code(aplicacao);
    const char *desc_uf = get_state_name_by_abbr(uf);
    char f_responsavel[15];

    apply_mask_renamaut(numero_registro, f_numero_registro);

    if (strlen(responsavel) < 14)
        apply_mask_cpf(responsavel, f_responsavel);
    else
        apply_mask_cnpj(responsavel, f_responsavel);

    sprintf(
        mensagem,
        "\nNúmero de Registro: %s\n"
        "Fabricante: %s\n"
        "Modelo: %s\n"
        "Categoria: %s\n"
        "Aplicacao: %s\n"
        "Ano de Fabricação: %d\n"
        "Responsável: %s\n"
        "Status: %s\n"
        "Cidade: %s\n"
        "Estado: %s\n",
        f_numero_registro,
        nome_fabricante,
        celula->modelo,
        desc_categoria,
        desc_aplicacao,
        celula->ano_fabricacao,
        f_responsavel,
        celula->status == 0 ? "Inativa" : "Ativa",
        celula->localizacao->cidade,
        desc_uf);

    wait_enter(mensagem);
}

void imprimir_linha_relatorio(MaquinaAutonoma *celula)
{
    char mensagem[512];

    const char *fabricante = celula->fabricante;
    const char *categoria = celula->categoria;
    const char *aplicacao = celula->aplicacao;
    const char *uf = celula->localizacao->uf;

    const char *nome_fabricante = get_manufacturer_name_by_id(fabricante);
    const char *desc_categoria = get_category_name_by_code(categoria);
    const char *desc_aplicacao = get_application_description_by_code(aplicacao);
    const char *desc_uf = get_state_name_by_abbr(uf);

    sprintf(
        mensagem,
        "%s; %s; %s; %d; %s; %s - %s\n",
        celula->numero_registro,
        celula->responsavel,
        celula->modelo,
        celula->ano_fabricacao,
        celula->status == 0 ? "Inativa" : "Ativa",
        celula->localizacao->cidade,
        desc_uf);

    wait_enter(mensagem);
}

void liberar_lista(MaquinaAutonoma **lista)
{
    MaquinaAutonoma *aux = *lista;
    MaquinaAutonoma *atual;

    while (aux != NULL)
    {
        atual = aux;
        aux = aux->proxima;
        remover(lista, atual);
    }

    *lista = NULL;
}

#endif