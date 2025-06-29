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

typedef struct EstadoGrupo
{
    char *uf;
    MaquinaAutonoma *maquinas;
    struct EstadoGrupo *proximo;
} EstadoGrupo;

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

MaquinaAutonoma *buscar_categoria(char *categoria, MaquinaAutonoma *lista)
{
    MaquinaAutonoma *p = lista;

    while (p != NULL)
    {
        if (strcmp(p->categoria, categoria) == 0)
            return p;
        p = p->proxima;
    }

    return NULL;
}

MaquinaAutonoma *copiar_maquina(MaquinaAutonoma *original)
{
    MaquinaAutonoma *novo = malloc(sizeof(MaquinaAutonoma));
    if (!novo)
        return NULL;

    novo->numero_registro = strdup(original->numero_registro);
    novo->fabricante = strdup(original->fabricante);
    novo->modelo = strdup(original->modelo);
    novo->categoria = strdup(original->categoria);
    novo->aplicacao = strdup(original->aplicacao);
    novo->ano_fabricacao = original->ano_fabricacao;
    novo->responsavel = strdup(original->responsavel);
    novo->status = original->status;

    if (original->localizacao)
    {
        novo->localizacao = malloc(sizeof(Localizacao));
        novo->localizacao->cidade = original->localizacao->cidade ? strdup(original->localizacao->cidade) : NULL;
        novo->localizacao->uf = original->localizacao->uf ? strdup(original->localizacao->uf) : NULL;
    }
    else
    {
        novo->localizacao = NULL;
    }

    novo->proxima = NULL;

    return novo;
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

void inserir_ordenado(MaquinaAutonoma **lista, MaquinaAutonoma *novo)
{
    if (*lista == NULL || strcmp(novo->numero_registro, (*lista)->numero_registro) < 0)
    {
        novo->proxima = *lista;
        *lista = novo;
    }
    else
    {
        MaquinaAutonoma *aux = *lista;

        while (aux->proxima != NULL && strcmp(novo->numero_registro, aux->proxima->numero_registro) > 0)
        {
            aux = aux->proxima;
        }

        novo->proxima = aux->proxima;
        aux->proxima = novo;
    }
}

void relatorio_responsavel(char *responsavel, MaquinaAutonoma *lista, MaquinaAutonoma **ativas, MaquinaAutonoma **inativas)
{
    *ativas = NULL;
    *inativas = NULL;
    MaquinaAutonoma *p = lista;

    while (p != NULL)
    {
        if (strcmp(responsavel, p->responsavel) == 0)
        {
            if (p->status == 1)
                inserir_ordenado(ativas, copiar_maquina(p));
            else
                inserir_ordenado(inativas, copiar_maquina(p));
        }
        p = p->proxima;
    }
}

void relatorio_categoria(const char *categoria, MaquinaAutonoma *lista, EstadoGrupo **grupo_estados)
{
    MaquinaAutonoma *p = lista;

    while (p != NULL)
    {
        if (strcmp(categoria, p->categoria) == 0 && p->status == 1 && p->localizacao && p->localizacao->uf)
        {
            EstadoGrupo *estado_atual = *grupo_estados;

            while (estado_atual != NULL && strcmp(p->localizacao->uf, estado_atual->uf) != 0)
            {
                estado_atual = estado_atual->proximo;
            }
            if (estado_atual == NULL)
            {
                estado_atual = malloc(sizeof(EstadoGrupo));
                estado_atual->uf = strdup(p->localizacao->uf);
                estado_atual->maquinas = NULL;
                estado_atual->proximo = *grupo_estados;
                *grupo_estados = estado_atual;
            }
            else
            {
                MaquinaAutonoma *nova_maquina = copiar_maquina(p);
                inserir_ordenado(&estado_atual->maquinas, nova_maquina);
            }
        }

        p = p->proxima;
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

        if (p->localizacao)
        {
            free(p->localizacao->cidade);
            free(p->localizacao->uf);
            free(p->localizacao);
        }

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
    const char *fabricante = get_manufacturer_name_by_id(celula->fabricante);
    const char *categoria = get_category_name_by_code(celula->categoria);
    const char *aplicacao = get_application_description_by_code(celula->aplicacao);
    const char *responsavel = celula->responsavel;
    const char *uf = (celula->localizacao && celula->localizacao->uf) ? get_state_name_by_abbr(celula->localizacao->uf) : "Não informado.";
    const char *cidade = (celula->localizacao && celula->localizacao->cidade) ? celula->localizacao->cidade : "Não informado.";
    char f_responsavel[15];

    apply_mask_renamaut(numero_registro, f_numero_registro);

    if (strlen(responsavel) < 14)
        apply_mask_cpf(responsavel, f_responsavel);
    else
        apply_mask_cnpj(responsavel, f_responsavel);

    sprintf(
        mensagem,
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
        f_numero_registro,
        fabricante,
        celula->modelo,
        categoria,
        aplicacao,
        celula->ano_fabricacao,
        f_responsavel,
        celula->status == 0 ? "Inativa" : "Ativa",
        celula->localizacao->cidade,
        uf);

    wait_enter(mensagem);
}

void imprimir_linha_relatorio_fabricante(MaquinaAutonoma *celula)
{
    char mensagem[512];

    const char *fabricante = celula->fabricante;
    const char *categoria = celula->categoria;
    const char *aplicacao = celula->aplicacao;
    const char *uf = (celula->localizacao && celula->localizacao->uf) ? get_state_name_by_abbr(celula->localizacao->uf) : "Não informado.";
    const char *cidade = (celula->localizacao && celula->localizacao->cidade) ? celula->localizacao->cidade : "Não informado.";

    sprintf(
        mensagem,
        "%s; %s; %s; %d; %s; %s - %s\n",
        celula->numero_registro,
        celula->responsavel,
        celula->modelo,
        celula->ano_fabricacao,
        celula->status == 0 ? "Inativa" : "Ativa",
        cidade,
        uf);

    wait_enter(mensagem);
}

void imprimir_linha_relatorio_categoria(EstadoGrupo *grupo_estados)
{
    EstadoGrupo *eg = grupo_estados;

    while (eg != NULL)
    {
        MaquinaAutonoma *p = eg->maquinas;

        while (p != NULL)
        {
            char mensagem[512];
            imprimir_linha_relatorio_fabricante(p);
            p = p->proxima;
        }

        eg = eg->proximo;
    }
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