#include "../gov_dev/gov_dev.h"
#include "file_handler.h"
#include "operacoes_lista_encadeada.h"
#include <stdio.h>
#include <stdlib.h>

void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}


int main(void)
{
    const char *filename = "../base_dados/exemplo_100.ERMAUF";
    char numero_registro[19];
    MaquinaAutonoma *lista = importar(filename);
    MaquinaAutonoma *aux = NULL;

    int op;

    do
    {
        op = main_menu();

        switch (op)
        {
            case SEARCH_OPTION:
                printf("Número de registro: ");

                limpar_buffer();

                fgets(numero_registro, sizeof(numero_registro), stdin);

                numero_registro[strcspn(numero_registro, "\n")] = '\0';

                aux = buscar(numero_registro, lista);

                if(aux == NULL) wait_enter("Máquina Autônoma não encontrada.");
                else imprimir(aux);

                break;

            case CHANGE_STATUS_OPTION:
                wait_enter("Inativação por número de registro\n");
                break;

            case RESPONSABILITY_REPORT_OPTION:
                wait_enter("Relatório de responsabilidade\n");
                break;

            case CATEGORY_REPORT_OPTION:
                wait_enter("Relatório por categoria\n");
                break;

            case EXIT_SYSTEM:
                aux = NULL;
                liberar_lista(&lista);
                break;

            default:
                wait_enter("Opção inválida! Por favor, tente novamente.\n");
                break;
        }
    } while(op != EXIT_SYSTEM);

    return 0;
}