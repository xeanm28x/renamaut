#include "../gov_dev/gov_dev.h"
#include "file_handler.h"
#include "operacoes_arvore_binaria.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    fclose(fopen("../tempos_execucao/arvore_binaria.log", "w"));

    const char *base_renamaut = "base_dados/base_renamaut_ab.txt";

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_json>\n", argv[0]);
        exit(1); 
    }

    const char *base_dados = argv[1];
    FILE *fp = fopen(base_dados, "r");
    if (!fp) {
        fprintf(stderr, "Erro: arquivo '%s' não encontrado.\n", base_dados);
        exit(1);
    }
    fclose(fp);

    char numero_registro[20];
    MaquinaAutonoma *arvore = NULL;
    MaquinaAutonoma *aux = NULL;
    int op;

    importar_renamaut(base_renamaut, &arvore);
    importar_base(base_dados, &arvore);

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

            char registro_limpo[20];

            strcpy(registro_limpo, numero_registro);

            if (strchr(registro_limpo, '-') != NULL)
            {
                remove_mask(registro_limpo, numero_registro);
            }

            if (validate_renamaut(registro_limpo) == 0)
            {
                wait_enter("\nNúmero de registro inválido.");
                break;
            }

            aux = buscar_numero_registro(registro_limpo, arvore);

            if (aux == NULL)
            {
                wait_enter("\nMáquina Autônoma não encontrada.");
            }
            else
            {
                imprimir(aux);
            }

            break;

        case CHANGE_STATUS_OPTION:

            printf("\nNúmero de registro: ");

            fgets(numero_registro, sizeof(numero_registro), stdin);

            numero_registro[strcspn(numero_registro, "\n")] = '\0';

            if (!strchr(numero_registro, '\n') && !feof(stdin))
            {
                int c;
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
            }

            const char *nr2 = numero_registro;

            if (strchr(numero_registro, '-') != NULL)
            {
                remove_mask(nr2, numero_registro);
            }

            if (validate_renamaut(numero_registro) == 0)
            {
                wait_enter("\nNúmero de registro inválido.");
                break;
            }

            aux = buscar_numero_registro(nr2, arvore);

            if (aux == NULL)
                wait_enter("\nMáquina Autônoma não encontrada.");
            else
            {
                if (aux->status == 1)
                {
                    char mensagem[512];

                    sprintf(mensagem, "\nTem certeza que deseja inativar esta máquina? Essa operação é irreversível. [S\\N] ");

                    int confirmacao = wait_confirmation(mensagem);

                    if (confirmacao == 1)
                    {
                        inativar(&arvore, aux);
                        wait_enter("\n\nMáquina inativada com sucesso!");
                    }

                    if (confirmacao == 0)
                        wait_enter("\n\nCancelando...");
                }
                else
                {
                    wait_enter("\nA Máquina informada já está inativa.");
                }
            }

            break;

        case RESPONSABILITY_REPORT_OPTION:
            char responsavel[20];
            MaquinaAutonoma *ativas = NULL;
            MaquinaAutonoma *inativas = NULL;

            printf("\nCPF ou CNPJ: ");

            fgets(responsavel, sizeof(responsavel), stdin);

            responsavel[strcspn(responsavel, "\n")] = '\0';

            if (strchr(responsavel, '.') != NULL)
                remove_mask(responsavel, responsavel);

            if ((strlen(responsavel) == 11 && validate_cpf(responsavel) == 0) || (strlen(responsavel) == 14 && validate_cnpj(responsavel) == 0) || strlen(responsavel) < 11 || strlen(responsavel) > 14)
            {
                wait_enter("\nNúmero de CPF/CNPJ inválido.");
                break;
            }

            relatorio_responsavel(responsavel, arvore, &ativas, &inativas);

            if (inativas == NULL)
            {
                wait_enter("\nNão há Máquinas Autônomas inativas para este responsável.");
            }
            else
            {
                printf("\n");

                aux = inativas;

                imprimir_linha_relatorio_fabricante(aux);
            }

            printf("\n");

            if (ativas == NULL)
            {
                wait_enter("\nNão há Máquinas Autônomas ativas para este responsável.");
            }
            else
            {
                printf("\n");

                aux = ativas;

                imprimir_linha_relatorio_fabricante(aux);
            }

            break;

        case CATEGORY_REPORT_OPTION:
            printf("\nCategoria: ");

            char categoria[30];

            fgets(categoria, sizeof(categoria), stdin);

            categoria[strcspn(categoria, "\n")] = '\0';

            if (!strchr(categoria, '\n') && !feof(stdin))
            {
                int c;
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
            }

            if (strlen(categoria) == 0)
            {
                wait_enter("\nCategoria inválida.");
                break;
            }

            const char *cod_categoria = NULL;

            if (strlen(categoria) == 6)
            {
                aux = buscar_categoria(categoria, arvore);

                if (aux != NULL)
                {
                    cod_categoria = categoria;
                }
            }
            else
            {
                cod_categoria = get_category_code_by_name(categoria);
            }

            if (cod_categoria == NULL)
            {
                wait_enter("\nCategoria não encontrada.");
                break;
            }

            EstadoGrupo *grupo_estados = NULL;

            relatorio_categoria(cod_categoria, arvore, &grupo_estados);

            if (grupo_estados == NULL)
            {
                wait_enter("\nNenhuma máquina ativa encontrada para essa categoria.");
            }
            else
            {
                printf("\n");
                imprimir_linha_relatorio_categoria(grupo_estados);
            }

            break;

        case EXIT_SYSTEM:
            aux = NULL;
            gravar(base_renamaut, arvore);
            liberar_arvore(&arvore);
            break;
        }
    } while (op != EXIT_SYSTEM);

    return 0;
}