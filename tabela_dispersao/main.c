#include "../gov_dev/gov_dev.h"
#include "file_handler.h"
#include "operacoes_tabela_dispersao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char *arquivo_ermauf = "../base_dados/exemplo_100.ERMAUF";
    char *base_dados = "base_dados.txt";
    char numero_registro[19];
    TabelaDispersao *tabela = criar_tabela(); // corrigido: remover argumento

    // ler(base_dados, tabela);
    importar(arquivo_ermauf, tabela);
    MaquinaAutonoma *aux = NULL;

    int op;

    do {
        op = main_menu();

        switch (op) {
            case SEARCH_OPTION:
                printf("\nNúmero de registro: ");

                if (fgets(numero_registro, sizeof(numero_registro), stdin) != NULL) {
                    numero_registro[strcspn(numero_registro, "\n")] = '\0';
                    aux = buscar_numero_registro(numero_registro, tabela); // corrigido ordem

                    if (aux == NULL) wait_enter("\nMáquina Autônoma não encontrada.\n");
                    else {
                        wait_enter("\nMáquina encontrada!\n");
                        imprimir(aux);
                        wait_enter("\nPressione ENTER para continuar...");
                    }
                }
                break;

            case CHANGE_STATUS_OPTION:
                printf("\nNúmero de registro: ");

                if (fgets(numero_registro, sizeof(numero_registro), stdin) != NULL) {
                    numero_registro[strcspn(numero_registro, "\n")] = '\0';
                    aux = buscar_numero_registro(numero_registro, tabela); // corrigido ordem

                    if (aux == NULL) wait_enter("\nMáquina Autônoma não encontrada.\n");
                    else {
                        if (aux->status == 0) {
                            wait_enter("\nMáquina Autônoma já está inativa.\n");
                            break;
                        }

                        imprimir(aux);
                        int confirmacao = wait_confirmation("\nTem certeza que deseja inativar esta máquina? Essa operação é irreversível. [S\\N] ");

                        if (confirmacao == 1) {
                            inativar(tabela, aux);
                            wait_enter("\n\nMáquina inativada com sucesso!\n\nPressione ENTER para continuar...");
                        } else {
                            wait_enter("\n\nCancelando...\n");
                        }
                    }
                }
                break;

            case RESPONSABILITY_REPORT_OPTION: {
                char responsavel[15];
                printf("\nCPF ou CNPJ: ");

                if (fgets(responsavel, sizeof(responsavel), stdin) != NULL) {
                    responsavel[strcspn(responsavel, "\n")] = '\0';
                    if (!(validate_cpf(responsavel) || validate_cnpj(responsavel))) {
                        wait_enter("\nCPF ou CNPJ inválido.\n");
                        break;
                    }

                    // relatorio_por_responsavel(tabela, responsavel);
                }
                break;
            }

            case CATEGORY_REPORT_OPTION:
                //(não implementado aqui)
                break;

            case EXIT_SYSTEM:
                gravar(base_dados, tabela);
                liberar_tabela(tabela);
                break;

            default:
                wait_enter("Opção inválida! Por favor, tente novamente.");
                break;
        }
    } while (op != EXIT_SYSTEM);

    return 0;
}