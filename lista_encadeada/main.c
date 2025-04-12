#include "../gov_dev/gov_dev.h"
#include "file_handler.h"
#include "operacoes_lista_encadeada.h"
#include <stdio.h>
#include <stdlib.h>
// gcc main.c ../gov_dev/gov_dev.h ../gov_dev/gov_dev.o -o renamaut_exe -lcjson
// ./renamaut_exe 
int main(void) {
    char *arquivo_ermauf = "../base_dados/exemplo_100.ERMAUF";
    char *base_dados = "base_dados.txt";
    char numero_registro[19];
    MaquinaAutonoma *lista = ler(base_dados);
    importar(arquivo_ermauf, &lista);
    MaquinaAutonoma *aux = NULL;

    int op;

    do {
        op = main_menu();

        switch (op) {
            case SEARCH_OPTION:
                printf("\nNúmero de registro: ");

                if (fgets(numero_registro, sizeof(numero_registro), stdin) != NULL) {
                    numero_registro[strcspn(numero_registro, "\n")] = '\0';
            
                    aux = buscar_numero_registro(numero_registro, lista);
            
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

                    aux = buscar_numero_registro(numero_registro, lista);

                    if(aux == NULL) wait_enter("\nMáquina Autônoma não encontrada.\n");
                    else {
                        if(aux->status == 0){
                            wait_enter("\nMáquina Autônoma já está inativa.\n");
                            break;
                        }

                        imprimir(aux);

                        char mensagem[512];

                        sprintf(mensagem, "\nTem certeza que deseja inativar esta máquina? Essa operação é irreversível. [S\\N] ");

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
                char responsavel[15];
                MaquinaAutonoma *ativas = NULL;
                MaquinaAutonoma *inativas = NULL;

                printf("\nCPF ou CNPJ: ");

                //tirar mascara sempre.

                if(fgets(responsavel, sizeof(responsavel), stdin) != NULL) {
                    responsavel[strcspn(responsavel, "\n")] = '\0';
                    char *cpf_cnpj = responsavel;
                    int eh_valido = validate_cpf(cpf_cnpj) == 1 || validate_cnpj(cpf_cnpj) == 1;

                    if(eh_valido == 0) {
                        wait_enter("\nCPF ou CNPJ inválido.\n");
                        break;
                    }

                    aux = buscar_responsavel(cpf_cnpj, lista);

                    if(aux == NULL)  {
                        wait_enter("\nNão há nenhuma Máquina Autônoma pertencente a este responsável.\n");
                        break;
                    } else if (aux->status == 0){
                        inserir(&aux, inativas);
                    } else {
                        aux->proxima = NULL;
                        inserir(&aux, ativas);
                    } 

                    free(aux);

                    if(inativas == NULL) {
                        wait_enter("\nNenhum dado registrado.\n");
                    } else {
                        aux = inativas;
                        while(aux != NULL) {
                            imprimir_linha_relatorio(aux);
                            aux = aux->proxima;
                        }
                    }

                    free(aux);

                    if(ativas == NULL) {
                        wait_enter("\nNenhum dado registrado.\n");
                    } else {
                        aux = ativas;
                        while(aux != NULL) {
                            imprimir_linha_relatorio(aux);
                            aux = aux->proxima;
                        }
                    }
                }

                break;

            case CATEGORY_REPORT_OPTION:
                break;

            case EXIT_SYSTEM:
                aux = NULL;
                gravar(base_dados, lista);
                liberar_lista(&lista);
                break;

            default:
                wait_enter("Opção inválida! Por favor, tente novamente.");
                break;
        }
    } while(op != EXIT_SYSTEM);

    return 0;
}