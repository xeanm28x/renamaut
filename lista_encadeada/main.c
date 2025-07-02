#include "../gov_dev/gov_dev.h"
#include "../tempos_execucao/tempo.h"
#include "file_handler.h"
#include "operacoes_lista_encadeada.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    fclose(fopen("../tempos_execucao/lista_encadeada.log", "w"));

    const char *base_renamaut = "base_dados/base_renamaut_le.txt";
    char base_dados[256];

    if (argc > 1)
    {
        strncpy(base_dados, argv[1], sizeof(base_dados));
        base_dados[sizeof(base_dados) - 1] = '\0';
    }
    else
    {
        strcpy(base_dados, "base_dados/exemplo_1000.ERMAUF");
    }

    char numero_registro[20];

    MaquinaAutonoma *lista = NULL;
    MaquinaAutonoma *aux = NULL;
    int op;

    clock_t ini1 = clock();
    importar_renamaut(base_renamaut, &lista);
    clock_t fim1 = clock();
    medir_tempo(ini1, fim1, "Importar RENAMAUT", "../tempos_execucao/lista_encadeada.log");

    clock_t ini2 = clock();
    importar_base(base_dados, &lista);
    clock_t fim2 = clock();
    medir_tempo(ini2, fim2, "Importar Base ERMAUF", "../tempos_execucao/lista_encadeada.log");

    do {
        op = main_menu();

        switch (op) {
        case SEARCH_OPTION:
            printf("\nNúmero de registro: ");
            fgets(numero_registro, sizeof(numero_registro), stdin);
            numero_registro[strcspn(numero_registro, "\n")] = '\0';
            if (!strchr(numero_registro, '\n') && !feof(stdin)) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }

            const char *nr1 = numero_registro;
            if (strchr(numero_registro, '-') != NULL) {
                remove_mask(nr1, numero_registro);
            }

            if (validate_renamaut(numero_registro) == 0) {
                wait_enter("\nNúmero de registro inválido.");
                break;
            }

            clock_t ini = clock();
            aux = buscar_numero_registro(nr1, lista);
            clock_t fim = clock();
            medir_tempo(ini, fim, "Busca RENAMAUT", "../tempos_execucao/lista_encadeada.log");

            if (aux == NULL)
                wait_enter("\nMáquina Autônoma não encontrada.");
            else
                imprimir(aux);
            break;

        case CHANGE_STATUS_OPTION:
            printf("\nNúmero de registro: ");
            fgets(numero_registro, sizeof(numero_registro), stdin);
            numero_registro[strcspn(numero_registro, "\n")] = '\0';
            if (!strchr(numero_registro, '\n') && !feof(stdin)) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }

            const char *nr2 = numero_registro;
            if (strchr(numero_registro, '-') != NULL) {
                remove_mask(nr2, numero_registro);
            }

            if (validate_renamaut(numero_registro) == 0) {
                wait_enter("\nNúmero de registro inválido.");
                break;
            }

            clock_t ini_cs = clock();
            aux = buscar_numero_registro(nr2, lista);
            if (aux == NULL) {
                clock_t fim_cs = clock();
                medir_tempo(ini_cs, fim_cs, "Inativação - Registro não encontrado", "../tempos_execucao/lista_encadeada.log");
                wait_enter("\nMáquina Autônoma não encontrada.");
            } else if (aux->status == 0) {
                clock_t fim_cs = clock();
                medir_tempo(ini_cs, fim_cs, "Inativação - Já inativa", "../tempos_execucao/lista_encadeada.log");
                wait_enter("\nA Máquina informada já está inativa.");
            } else {
                char mensagem[512];
                sprintf(mensagem, "\nTem certeza que deseja inativar esta máquina? Essa operação é irreversível. [S\\N] ");
                int confirmacao = wait_confirmation(mensagem);
                clock_t fim_cs = clock();
                if (confirmacao == 1) {
                    inativar(&lista, aux);
                    medir_tempo(ini_cs, fim_cs, "Inativação realizada", "../tempos_execucao/lista_encadeada.log");
                    wait_enter("\n\nMáquina inativada com sucesso!");
                } else {
                    medir_tempo(ini_cs, fim_cs, "Inativação cancelada", "../tempos_execucao/lista_encadeada.log");
                    wait_enter("\n\nCancelando...");
                }
            }
            break;

        case RESPONSABILITY_REPORT_OPTION: {
            char responsavel[20];
            MaquinaAutonoma *ativas = NULL;
            MaquinaAutonoma *inativas = NULL;

            printf("\nCPF ou CNPJ: ");
            fgets(responsavel, sizeof(responsavel), stdin);
            responsavel[strcspn(responsavel, "\n")] = '\0';

            if (strchr(responsavel, '.') != NULL)
                remove_mask(responsavel, responsavel);

            if ((strlen(responsavel) == 11 && validate_cpf(responsavel) == 0) ||
                (strlen(responsavel) == 14 && validate_cnpj(responsavel) == 0) ||
                strlen(responsavel) < 11 || strlen(responsavel) > 14) {
                wait_enter("\nNúmero de CPF/CNPJ inválido.");
                break;
            }

            clock_t ini = clock();
            relatorio_responsavel(responsavel, lista, &ativas, &inativas);
            clock_t fim = clock();
            medir_tempo(ini, fim, "Relatório por Responsável", "../tempos_execucao/lista_encadeada.log");

            if (inativas == NULL) {
                wait_enter("\nNão há Máquinas Autônomas inativas para este responsável.");
            } else {
                printf("\n");
                aux = inativas;
                while (aux != NULL) {
                    imprimir_linha_relatorio_fabricante(aux);
                    aux = aux->proxima;
                }
            }

            printf("\n");

            if (ativas == NULL) {
                wait_enter("\nNão há Máquinas Autônomas ativas para este responsável.");
            } else {
                printf("\n\n");
                aux = ativas;
                while (aux != NULL) {
                    imprimir_linha_relatorio_fabricante(aux);
                    aux = aux->proxima;
                }
            }
            break;
        }

        case CATEGORY_REPORT_OPTION: {
            printf("\nCategoria: ");
            char categoria[30];
            fgets(categoria, sizeof(categoria), stdin);
            categoria[strcspn(categoria, "\n")] = '\0';

            if (!strchr(categoria, '\n') && !feof(stdin)) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }

            if (strlen(categoria) == 0) {
                wait_enter("\nCategoria inválida.");
                break;
            }

            const char *cod_categoria = NULL;
            if (strlen(categoria) == 6) {
                aux = buscar_categoria(categoria, lista);
                if (aux != NULL)
                    cod_categoria = categoria;
            } else {
                cod_categoria = get_category_code_by_name(categoria);
            }

            if (cod_categoria == NULL) {
                wait_enter("\nCategoria não encontrada.");
                break;
            }

            EstadoGrupo *grupo_estados = NULL;
            clock_t ini = clock();
            relatorio_categoria(cod_categoria, lista, &grupo_estados);
            clock_t fim = clock();
            medir_tempo(ini, fim, "Relatório por Categoria", "../tempos_execucao/lista_encadeada.log");

            if (grupo_estados == NULL) {
                wait_enter("\nNenhuma máquina ativa encontrada para essa categoria.");
            } else {
                printf("\n");
                imprimir_linha_relatorio_categoria(grupo_estados);
            }
            break;
        }

        case EXIT_SYSTEM:
            aux = NULL;
            gravar(base_renamaut, lista);
            liberar_lista(&lista);
            break;
        }
    } while (op != EXIT_SYSTEM);

    return 0;
}
