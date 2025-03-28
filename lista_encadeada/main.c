#include "../gov_dev/gov_dev.h"
#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const char *filename = "../base_dados/exemplo_20.ERMAUF";

    import(filename);

    int op = main_menu();

    switch (op)
    {
    case SEARCH_OPTION:
        printf("Consulta por número de registro\n");
        break;

    case CHANGE_STATUS_OPTION:
        printf("Inativação por número de registro\n");
        break;

    case RESPONSABILITY_REPORT_OPTION:
        printf("Relatório de responsabilidade\n");
        break;

    case CATEGORY_REPORT_OPTION:
        printf("Relatório por categoria\n");
        break;

    case EXIT_SYSTEM:
        return 0;
        break;

    default:
        printf("Opção inválida! Por favor, tente novamente.\n");
        break;
    }

    return 0;
}