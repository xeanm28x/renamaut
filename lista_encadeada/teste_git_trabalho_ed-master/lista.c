#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON-master/cJSON.h"
#include "trabalho_ed.c"
#include "lista.h"

void adicionar_elemento_lista(MaquinaAutonoma **lst, MaquinaAutonoma *novo);
void remover_elemento_lista(MaquinaAutonoma **lst, char *numero_registro);