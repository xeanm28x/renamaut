#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include "operacoes_arvore_binaria.h"
#include <cjson/cJSON.h>

MaquinaAutonoma *ler(const char *filename) {
    FILE *file = fopen(filename, "r");

    if (!file) {
        file = fopen(filename, "w+");
    }

    MaquinaAutonoma *no = NULL;
    char linha[512];
}

#endif