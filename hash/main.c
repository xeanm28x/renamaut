#include "../gov_dev/gov_dev.h"
#include "operacoes_hash.h"
#include "file_handler.h"
#include "persistencia.h"
#include "relatorios.h"
#include "log.h"
#include "clock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define TAM_TABELA 37

int main(void)
{
    fclose(fopen("debug.log", "w"));             // limpa debug
    fclose(fopen("relatorio_tempo.log", "w"));   // limpa log de tempo

    const char *arquivo_txt = "../base_dados/base_renamaut.txt";
    const char *arquivo_json = "../base_dados/exemplo_5000.ERMAUF";
    char numero_registro[20];
    int op;

    int n_elementos = contar_registros_json(arquivo_json);
    LOG_DEBUG("Número de registros no JSON: %d", n_elementos);
    int tamanho_tabela = estimar_tamanho_tabela(n_elementos);
    LOG_DEBUG("Tamanho estimado da tabela: %d", tamanho_tabela);

    HashTable *tabela = criar_tabela(tamanho_tabela);

    clock_t ini_json = clock();
    importar_json(tabela, arquivo_json);
    clock_t fim_json = clock();
    medir_tempo(ini_json, fim_json, "Importação JSON");

    do {
        op = main_menu();

        switch (op) {

        case SEARCH_OPTION:
            printf("\nNúmero de registro: ");
            fgets(numero_registro, sizeof(numero_registro), stdin);
            numero_registro[strcspn(numero_registro, "\n")] = '\0';

            LOG_DEBUG("Buscando registro: '%s'", numero_registro);

            const char *nr1 = numero_registro;
            if (strchr(numero_registro, '-') != NULL)
                remove_mask(nr1, numero_registro);

            {
                clock_t ini = clock();

                if (validate_renamaut(numero_registro) == 0) {
                    clock_t fim = clock();
                    medir_tempo(ini, fim, "Validação RENAMAUT");
                    wait_enter("\nNúmero de registro inválido.");
                    break;
                }

                Registro *reg = buscar(tabela, nr1);
                clock_t fim = clock();

                if (reg == NULL) {
                    medir_tempo(ini, fim, "Busca RENAMAUT (sem sucesso)");
                    wait_enter("\nMáquina Autônoma não encontrada.");
                } else {
                    medir_tempo(ini, fim, "Busca RENAMAUT (sucesso)");
                    LOG_DEBUG("Chamando imprimir para '%s'", reg->renamaut);

                    imprimir(reg);
                    wait_enter("\nPressione Enter para continuar...");
                }
            }
            break;

        case CHANGE_STATUS_OPTION:
            printf("\nNúmero de registro: ");
            fgets(numero_registro, sizeof(numero_registro), stdin);
            numero_registro[strcspn(numero_registro, "\n")] = '\0';

            const char *nr2 = numero_registro;
            if (strchr(numero_registro, '-') != NULL)
                remove_mask(nr2, numero_registro);

            if (validate_renamaut(numero_registro) == 0) {
                wait_enter("\nNúmero de registro inválido.");
                break;
            }

            {
                clock_t ini = clock();
                Registro *reg = buscar(tabela, nr2);

                if (reg == NULL) {
                    clock_t fim = clock();
                    medir_tempo(ini, fim, "Inativação - Registro não encontrado");
                    wait_enter("\nMáquina Autônoma não encontrada.");
                } else if (reg->status == 0) {
                    clock_t fim = clock();
                    medir_tempo(ini, fim, "Inativação - Já inativa");
                    wait_enter("\nA Máquina informada já está inativa.");
                } else {
                    char mensagem[256];
                    sprintf(mensagem, "\nTem certeza que deseja inativar esta máquina? Essa operação é irreversível. [S/N] ");
                    if (wait_confirmation(mensagem)) {
                        reg->status = 0;
                        clock_t fim = clock();
                        medir_tempo(ini, fim, "Inativação realizada");
                        wait_enter("\nMáquina inativada com sucesso!");
                    } else {
                        clock_t fim = clock();
                        medir_tempo(ini, fim, "Inativação cancelada");
                        wait_enter("\nCancelando...");
                    }
                }
            }
            break;

        case RESPONSABILITY_REPORT_OPTION:
            {
                char responsavel[30];
                printf("\nCPF ou CNPJ: ");
                fgets(responsavel, sizeof(responsavel), stdin);
                responsavel[strcspn(responsavel, "\n")] = '\0';
                const char *doc = responsavel;

                if (strchr(responsavel, '.') != NULL || strchr(responsavel, '-') != NULL)
                    remove_mask(doc, responsavel);

                if (validate_cpf(responsavel) == 0 && validate_cnpj(responsavel) == 0) {
                    wait_enter("\nCPF ou CNPJ inválido.");
                    break;
                }

                clock_t ini = clock();
                relatorio_responsavel(tabela, responsavel);
                clock_t fim = clock();
                medir_tempo(ini, fim, "Relatório por Responsável");

                wait_enter("\nPressione Enter para continuar...");
            }
            break;

        case CATEGORY_REPORT_OPTION:
            {
                char cat[10];
                printf("\nCódigo da Categoria: ");
                fgets(cat, sizeof(cat), stdin);
                cat[strcspn(cat, "\n")] = '\0';

                clock_t ini = clock();
                relatorio_categoria(tabela, cat);
                clock_t fim = clock();
                medir_tempo(ini, fim, "Relatório por Categoria");

                wait_enter("\nPressione Enter para continuar...");
            }
            break;

        case EXIT_SYSTEM:
            // salvar_tabela(tabela, arquivo_txt);
            exportar_para_txt(tabela, "../base_dados/base_renamaut_hash.txt");
            liberar_tabela(tabela);
            break;
        }

    } while (op != EXIT_SYSTEM);

    return 0;
}