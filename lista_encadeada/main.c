#include "../gov_dev/gov_dev.h"
#include "file_handler.h"
#include "operacoes_lista_encadeada.h"
#include <stdio.h>
#include <stdlib.h>

void arquivos_menu(MaquinaAutonoma **lista){
    char mensagem[512];

    printf("\nPressione ENTER para continuar...\n");

    int confirmacao_importar_arquivo = wait_confirmation("\nDeseja importar novos arquivos de dados para o RENAMAUT? [S\\N] ");

    if(confirmacao_importar_arquivo == 1){
        char arquivo_ermauf[250];
        int confirmacao_importar_mais_arquivos = 0;

        do{
            printf("\n\nDiretório do arquivo: ");

            if(fgets(arquivo_ermauf, sizeof(arquivo_ermauf), stdin) != NULL) {
                arquivo_ermauf[strcspn(arquivo_ermauf, "\n")] = '\0';
                
                const char *p_arquivo_ermauf = arquivo_ermauf;

                int importacao = importar(p_arquivo_ermauf, lista);

                if(importacao == 0)
                    confirmacao_importar_mais_arquivos = wait_confirmation("\nTentar novamente? [S\\N] ");
                else {
                    printf("\nArquivo importado com sucesso!\n");
                    confirmacao_importar_mais_arquivos = wait_confirmation("\nDeseja importar mais arquivos? [S\\N] ");
                }
            } else {
                wait_enter("\nOcorreu um erro ao ler o diretório.\n");

                confirmacao_importar_mais_arquivos = wait_confirmation("\nTentar novamente? [S\\N] ");
            }
        } while(confirmacao_importar_mais_arquivos == 1);
    }
}

int main(void) {
    const char *base_dados = "base_dados.txt";
    char numero_registro[20];
    MaquinaAutonoma *lista = ler(base_dados);
    MaquinaAutonoma *aux = NULL;

    arquivos_menu(&lista);

    int op;

    do {
        op = main_menu();

        switch (op) {
            case SEARCH_OPTION:
                printf("\nNúmero de registro: ");

                if (fgets(numero_registro, sizeof(numero_registro), stdin) != NULL) {
                    numero_registro[strcspn(numero_registro, "\n")] = '\0';

                    if(strchr(numero_registro, '-') != NULL) {
                        const char *c_nr = numero_registro;
                        remove_mask(c_nr, numero_registro);
                    }
            
                    aux = buscar_numero_registro(numero_registro, lista);
            
                    if(aux == NULL) wait_enter("\nMáquina Autônoma não encontrada.\n");
                    else {
                        printf("\nMáquina encontrada!\n");
                        imprimir(aux);
                    }
                } else wait_enter("\nErro na leitura do número de registro. Entre em contato com o suporte.\n");
                break;

            case CHANGE_STATUS_OPTION:
                printf("\nNúmero de registro: ");

                if (fgets(numero_registro, sizeof(numero_registro), stdin) != NULL) {
                    numero_registro[strcspn(numero_registro, "\n")] = '\0';

                    if(strchr(numero_registro, '-') != NULL) {
                        const char *c_nr = numero_registro;
                        remove_mask(c_nr, numero_registro);
                    }

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
                            wait_enter("\n\nMáquina inativada com sucesso!\n");
                        }
                        
                        if(confirmacao == 0) wait_enter("\n\nCancelando...\n");
                    }
                }
                break;

            case RESPONSABILITY_REPORT_OPTION:
                // char responsavel[15];
                // MaquinaAutonoma *ativas = NULL;
                // MaquinaAutonoma *inativas = NULL;

                // printf("\nCPF ou CNPJ: ");

                // if(fgets(responsavel, sizeof(responsavel), stdin) != NULL) {
                //     responsavel[strcspn(responsavel, "\n")] = '\0';
                //     const char *cpf_cnpj = responsavel;
                //     int eh_valido = validate_cpf(cpf_cnpj) == 1 || validate_cnpj(cpf_cnpj) == 1;

                //     if(eh_valido == 0) {
                //         wait_enter("\nCPF ou CNPJ inválido.\n");
                //         break;
                //     }

                //     if(strchr(responsavel, '.') != NULL)
                //         remove_mask(cpf_cnpj, responsavel);

                //     aux = buscar_responsavel(responsavel, lista);

                //     if(aux == NULL)  {
                //         wait_enter("\nNão há nenhuma Máquina Autônoma pertencente a este responsável.\n");
                //         break;
                //     }
                    // } else if (aux->status == 0){
                    //     inserir(aux, &inativas);
                    // } else {
                    //     aux->proxima = NULL;
                    //     inserir(aux, &ativas);
                    // }

                    // free(aux);

                    // if(inativas == NULL) {
                    //     wait_enter("\nNenhum dado registrado.\n");
                    // } else {
                    //     aux = inativas;
                    //     while(aux != NULL) {
                    //         imprimir_linha_relatorio(aux);
                    //         aux = aux->proxima;
                    //     }
                    // }

                    // free(aux);

                    // if(ativas == NULL) {
                    //     wait_enter("\nNenhum dado registrado.\n");
                    // } else {
                    //     aux = ativas;
                    //     while(aux != NULL) {
                    //         imprimir_linha_relatorio(aux);
                    //         aux = aux->proxima;
                    //     }
                    // }
                // }
                break;

            case CATEGORY_REPORT_OPTION:
                break;

            case EXIT_SYSTEM:
                aux = NULL;
                gravar(base_dados, lista);
                liberar_lista(&lista);
                break;
        }
    } while(op != EXIT_SYSTEM);

    return 0;
}