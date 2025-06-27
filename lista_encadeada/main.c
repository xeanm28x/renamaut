#include "../gov_dev/gov_dev.h"
#include "file_handler.h"
#include "operacoes_lista_encadeada.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const char *base_renamaut = "base_dados/base_renamaut.txt";
    char *base_dados = "base_dados/exemplo_20.ERMAUF";
    char numero_registro[20];
    MaquinaAutonoma *lista = NULL;
    MaquinaAutonoma *aux = NULL;
    int op;

    importar_renamaut(base_renamaut, &lista);
    importar_base(base_dados, &lista);

    do
    {
        op = main_menu();

        switch (op)
        {
        case SEARCH_OPTION:
            printf("\nNúmero de registro: ");

            fgets(numero_registro, sizeof(numero_registro), stdin);

            numero_registro[strcspn(numero_registro, "\n")] = '\0';

            if (!strchr(numero_registro, '\n') && !feof(stdin))
            {
                int c;
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
            }

            const char *c_nr = numero_registro;

            if (strchr(numero_registro, '-') != NULL)
            {
                remove_mask(c_nr, numero_registro);
            }

            if (validate_renamaut(numero_registro) == 0)
            {
                wait_enter("\nNúmero de registro inválido.\n");
                break;
            }

            aux = buscar_numero_registro(c_nr, lista);

            if (aux == NULL)
                wait_enter("\nMáquina Autônoma não encontrada.\n");
            else
            {
                imprimir(aux);
            }
            break;

        case CHANGE_STATUS_OPTION:
            // printf("\nNúmero de registro: ");

            // if (fgets(numero_registro, sizeof(numero_registro), stdin) != NULL) {
            //     numero_registro[strcspn(numero_registro, "\n")] = '\0';

            //     if(strchr(numero_registro, '-') != NULL) {
            //         const char *c_nr = numero_registro;
            //         remove_mask(c_nr, numero_registro);
            //     }

            //     aux = buscar_numero_registro(numero_registro, lista);

            //     if(aux == NULL) wait_enter("\nMáquina Autônoma não encontrada.\n");
            //     else {
            //         if(aux->status == 0){
            //             wait_enter("\nMáquina Autônoma já está inativa.\n");
            //             break;
            //         }

            //         imprimir(aux);

            //         char mensagem[512];

            //         sprintf(mensagem, "\nTem certeza que deseja inativar esta máquina? Essa operação é irreversível. [S\\N] ");

            //         int confirmacao = wait_confirmation(mensagem);

            //         if(confirmacao == 1) {
            //             inativar(&lista, aux);
            //             wait_enter("\n\nMáquina inativada com sucesso!\n");
            //         }

            //         if(confirmacao == 0) wait_enter("\n\nCancelando...\n");
            //     }
            // }
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
            gravar(base_renamaut, lista);
            liberar_lista(&lista);
            break;
        }
    } while (op != EXIT_SYSTEM);

    return 0;
}