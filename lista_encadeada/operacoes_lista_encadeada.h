#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../gov_dev/gov_dev.h"

typedef struct {
    char cidade[50];
    char uf[3];
} Localizacao;

typedef struct MaquinaAutonoma {

    char numero_registro[19];
    char fabricante[5];
    char modelo[51];
    char categoria[4];
    char aplicacao[4];
    int ano_fabricacao;
    char responsavel[15];
    int status;
    Localizacao localizacao;
    struct MaquinaAutonoma *proxima;

} MaquinaAutonoma;

MaquinaAutonoma *buscar(char *numero_registro, MaquinaAutonoma *lista) {
    MaquinaAutonoma *p = lista;

    while(p != NULL && strcmp(p->numero_registro, numero_registro) != 0) {
        p = p->proxima;
    }

    return p;
}

// void inserir(MaquinaAutonoma *celula, MaquinaAutonoma *lista) {

// }

void imprimir(MaquinaAutonoma *celula) {
    char mensagem[512];
    const char *nome_fabricante = get_manufacturer_name_by_acronym(celula->fabricante);

    sprintf(
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
        nome_fabricante,
        celula->modelo,
        celula->categoria,
        celula->aplicacao,
        celula->ano_fabricacao,
        celula->responsavel,
        celula->status,
        celula->localizacao.cidade,
        celula->localizacao.uf);

    wait_enter(mensagem);
}

#endif