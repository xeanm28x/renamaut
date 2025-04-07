#include "../gov_dev/gov_dev.h"
#include "file_handler.h"
#include "operacoes_lista_encadeada.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    const char *filename = "../base_dados/exemplo_100.ERMAUF";
    char numero_registro[19];
    MaquinaAutonoma *lista = ler("base_dados.bin");
    importar(filename, &lista);
    MaquinaAutonoma *aux = NULL;

    int op;

    do {
        op = main_menu();

        switch (op) {
            case SEARCH_OPTION:
                printf("\nNúmero de registro: ");

                if (fgets(numero_registro, sizeof(numero_registro), stdin) != NULL) {
                    numero_registro[strcspn(numero_registro, "\n")] = '\0';
            
                    aux = buscar(numero_registro, lista);
            
                    if(aux == NULL) wait_enter("\nMáquina Autônoma não encontrada.\n");
                    else {
                        wait_enter("\nMáquina encontrada!\n");
                        imprimir(aux);
                        wait_enter("\nPressione ENTER para continuar...");
                    }
                } else wait_enter("\nErro na leitura do número de registro. Entre em contato com o suporte.\n");
                break;

            case CHANGE_STATUS_OPTION:
                printf("\nNúmero de registro: ");

                if (fgets(numero_registro, sizeof(numero_registro), stdin) != NULL) {
                    numero_registro[strcspn(numero_registro, "\n")] = '\0';

                    aux = buscar(numero_registro, lista);

                    if(aux == NULL) wait_enter("\nMáquina Autônoma não encontrada.\n");
                    else {
                        imprimir(aux);

                        char mensagem[512];

                        sprintf(mensagem, "\nTem certeza que deseja inativar esta máquina? [S\\N] ");

                        int confirmacao = wait_confirmation(mensagem);

                        if(confirmacao == 1) {
                            inativar(&lista, aux);
                            wait_enter("\n\nMáquina inativada com sucesso!\n\nPressione ENTER para continuar...");
                        }
                        
                        if(confirmacao == 0) wait_enter("\n\nCancelando...\n");
                    }
                }

                break;

            case RESPONSABILITY_REPORT_OPTION:
                break;

            case CATEGORY_REPORT_OPTION:
                break;

            case EXIT_SYSTEM:
                aux = NULL;
                gravar("base_dados.bin", lista);
                liberar_lista(&lista);
                break;

            default:
                wait_enter("Opção inválida! Por favor, tente novamente.");
                break;
        }
    } while(op != EXIT_SYSTEM);

    return 0;
}