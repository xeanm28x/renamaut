#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../gov_dev/gov_dev.h"

typedef enum
{
    VERMELHO,
    PRETO
} Cor;

typedef struct
{
    char *cidade;
    char *uf;
} Localizacao;

typedef struct MaquinaAutonoma
{
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
    Cor cor;
} MaquinaAutonoma;

typedef struct EstadoGrupo
{
    char *uf;
    MaquinaAutonoma *maquinas;
    struct EstadoGrupo *esq;
    struct EstadoGrupo *dir;
} EstadoGrupo;

MaquinaAutonoma *buscar_numero_registro(const char *numero_registro, MaquinaAutonoma *raiz)
{
    MaquinaAutonoma *a = raiz;

    while (a != NULL)
    {
        int comparacao = strcmp(numero_registro, a->renamaut);

        if (comparacao == 0)
            return a;
        else if (comparacao < 0)
            a = a->esq;
        else
            a = a->dir;
    }

    return NULL;
}

MaquinaAutonoma *buscar_categoria(const char *categoria, MaquinaAutonoma *arvore)
{
    if (arvore == NULL)
        return NULL;

    if (strcmp(arvore->categoria, categoria) == 0)
        return arvore;

    MaquinaAutonoma *res = buscar_categoria(categoria, arvore->esq);

    if (res != NULL)
        return res;

    return buscar_categoria(categoria, arvore->dir);
}

MaquinaAutonoma *copiar_maquina(MaquinaAutonoma *original)
{
    MaquinaAutonoma *novo = malloc(sizeof(MaquinaAutonoma));

    if (!novo)
        return NULL;

    novo->renamaut = strdup(original->renamaut);
    novo->fabricante = strdup(original->fabricante);
    novo->modelo = strdup(original->modelo);
    novo->categoria = strdup(original->categoria);
    novo->aplicacao = strdup(original->aplicacao);
    novo->ano_fabricacao = original->ano_fabricacao;
    novo->responsavel = strdup(original->responsavel);
    novo->status = original->status;
    novo->cor = original->cor;

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

    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

MaquinaAutonoma *rotacao_esquerda(MaquinaAutonoma *raiz)
{
    MaquinaAutonoma *x = raiz->dir;
    raiz->dir = x->esq;
    x->esq = raiz;
    return x;
}

MaquinaAutonoma *rotacao_direita(MaquinaAutonoma *raiz)
{
    MaquinaAutonoma *x = raiz->esq;
    raiz->esq = x->dir;
    x->dir = raiz;
    return x;
}

void troca_cor(MaquinaAutonoma *raiz)
{
    raiz->cor = (raiz->cor == VERMELHO) ? PRETO : VERMELHO;
    if (raiz->esq)
        raiz->esq->cor = (raiz->esq->cor == VERMELHO) ? PRETO : VERMELHO;
    if (raiz->dir)
        raiz->dir->cor = (raiz->dir->cor == VERMELHO) ? PRETO : VERMELHO;
}

int eh_vermelho(MaquinaAutonoma *no)
{
    return no != NULL && no->cor == VERMELHO;
}

MaquinaAutonoma *inserir_rubro_negra(MaquinaAutonoma *raiz, MaquinaAutonoma *novo)
{
    if (raiz == NULL)
    {
        novo->cor = VERMELHO;
        novo->esq = novo->dir = NULL;
        return novo;
    }

    int cmp = strcmp(novo->renamaut, raiz->renamaut);
    if (cmp < 0)
        raiz->esq = inserir_rubro_negra(raiz->esq, novo);
    else if (cmp > 0)
        raiz->dir = inserir_rubro_negra(raiz->dir, novo);
    else
        ;

    if (eh_vermelho(raiz->dir) && !eh_vermelho(raiz->esq))
        raiz = rotacao_esquerda(raiz);
    if (eh_vermelho(raiz->esq) && eh_vermelho(raiz->esq->esq))
        raiz = rotacao_direita(raiz);
    if (eh_vermelho(raiz->esq) && eh_vermelho(raiz->dir))
        troca_cor(raiz);

    return raiz;
}

void inserir(MaquinaAutonoma **raiz, MaquinaAutonoma *novo)
{
    *raiz = inserir_rubro_negra(*raiz, novo);
    if (*raiz)
        (*raiz)->cor = PRETO;
}

void inserir_ordenado(MaquinaAutonoma **arvore, MaquinaAutonoma *novo)
{
    if (*arvore == NULL || strcmp(novo->renamaut, (*arvore)->renamaut) < 0)
    {
        novo->esq = NULL;
        novo->dir = *arvore;
        *arvore = novo;
    }
    else
    {
        MaquinaAutonoma *aux = *arvore;

        while (aux->dir != NULL && strcmp(novo->renamaut, aux->dir->renamaut) > 0)
        {
            aux = aux->dir;
        }

        novo->esq = aux->dir;
        aux->dir = novo;
    }
}

void inserir_estado(EstadoGrupo **raiz, const char *uf, MaquinaAutonoma *maquina)
{
    if (*raiz == NULL)
    {
        EstadoGrupo *novo = malloc(sizeof(EstadoGrupo));
        novo->uf = strdup(uf);
        novo->maquinas = NULL;
        novo->esq = NULL;
        novo->dir = NULL;
        inserir_ordenado(&novo->maquinas, copiar_maquina(maquina));
        *raiz = novo;
        return;
    }

    int comparacao = strcmp(uf, (*raiz)->uf);

    if (comparacao == 0)
    {
        inserir_ordenado(&(*raiz)->maquinas, copiar_maquina(maquina));
    }
    else if (comparacao < 0)
    {
        inserir_estado(&(*raiz)->esq, uf, maquina);
    }
    else
    {
        inserir_estado(&(*raiz)->dir, uf, maquina);
    }
}

void relatorio_responsavel(char *responsavel, MaquinaAutonoma *arvore, MaquinaAutonoma **ativas, MaquinaAutonoma **inativas)
{
    if (arvore == NULL)
        return;

    if (strcmp(arvore->responsavel, responsavel) == 0)
    {
        if (arvore->status == 1)
            inserir_ordenado(ativas, copiar_maquina(arvore));
        else
            inserir_ordenado(inativas, copiar_maquina(arvore));
    }

    relatorio_responsavel(responsavel, arvore->esq, ativas, inativas);
    relatorio_responsavel(responsavel, arvore->dir, ativas, inativas);
}

void relatorio_categoria(const char *categoria, MaquinaAutonoma *arvore, EstadoGrupo **grupo_estados)
{
    if (arvore == NULL)
        return;

    if (strcmp(arvore->categoria, categoria) == 0 && arvore->status == 1 && arvore->localizacao && arvore->localizacao->uf)
    {
        inserir_estado(grupo_estados, arvore->localizacao->uf, arvore);
    }

    relatorio_categoria(categoria, arvore->esq, grupo_estados);
    relatorio_categoria(categoria, arvore->dir, grupo_estados);
}

void inativar(MaquinaAutonoma **raiz, MaquinaAutonoma *a)
{
    MaquinaAutonoma *aux = buscar_numero_registro(a->renamaut, *raiz);

    if (aux != NULL)
    {
        aux->status = 0;
    }
}

void imprimir(MaquinaAutonoma *celula)
{
    char mensagem[512];

    const char *numero_registro = celula->renamaut;
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
        celula->renamaut,
        fabricante,
        celula->modelo,
        categoria,
        aplicacao,
        celula->ano_fabricacao,
        f_responsavel,
        celula->status == 0 ? "Inativa" : "Ativa",
        cidade,
        uf);

    wait_enter(mensagem);
}

void imprimir_linha_relatorio_fabricante(MaquinaAutonoma *arvore)
{
    if (arvore == NULL)
        return;

    imprimir_linha_relatorio_fabricante(arvore->esq);

    char mensagem[512];
    const char *fabricante = arvore->fabricante;
    const char *categoria = arvore->categoria;
    const char *aplicacao = arvore->aplicacao;
    const char *uf = (arvore->localizacao && arvore->localizacao->uf) ? get_state_name_by_abbr(arvore->localizacao->uf) : "Não informado.";
    const char *cidade = (arvore->localizacao && arvore->localizacao->cidade) ? arvore->localizacao->cidade : "Não informado.";

    sprintf(
        mensagem,
        "%s; %s; %s; %d; %s; %s - %s; %s\n", // Adiciona cor (opcional)
        arvore->renamaut,
        arvore->responsavel,
        arvore->modelo,
        arvore->ano_fabricacao,
        arvore->status == 0 ? "Inativa" : "Ativa",
        cidade,
        uf,
        arvore->cor == VERMELHO ? "Vermelho" : "Preto" // Mostra cor do nó
    );

    wait_enter(mensagem);

    imprimir_linha_relatorio_fabricante(arvore->dir);
}

void imprimir_linha_relatorio_categoria(EstadoGrupo *grupo_estados)
{
    if (grupo_estados == NULL)
        return;

    imprimir_linha_relatorio_categoria(grupo_estados->esq);

    imprimir_linha_relatorio_fabricante(grupo_estados->maquinas);

    imprimir_linha_relatorio_categoria(grupo_estados->dir);
}

void liberar_arvore(MaquinaAutonoma **arvore)
{
    if (*arvore == NULL)
        return;

    liberar_arvore(&((*arvore)->esq));
    liberar_arvore(&((*arvore)->dir));

    free((*arvore)->renamaut);
    free((*arvore)->fabricante);
    free((*arvore)->modelo);
    free((*arvore)->categoria);
    free((*arvore)->aplicacao);
    free((*arvore)->responsavel);

    if ((*arvore)->localizacao)
    {
        free((*arvore)->localizacao->cidade);
        free((*arvore)->localizacao->uf);
        free((*arvore)->localizacao);
    }

    free(*arvore);
    *arvore = NULL;
}

#endif