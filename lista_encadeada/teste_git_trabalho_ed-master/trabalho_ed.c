#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON-master/cJSON.h"

typedef struct MaquinaAutonoma {
    char numero_registro[19];
    char fabricante[5];
    char modelo[51];
    char categoria[4];
    char aplicacao[4];
    int ano_fabricacao;
    char responsavel[15];
    int status;
    char localizacao[60];
    struct MaquinaAutonoma *proximo;  // Necessário para encadeamento
} MaquinaAutonoma;

void adicionar_elemento_lista(MaquinaAutonoma **lst, MaquinaAutonoma *novo) {
    if (*lst == NULL) {
        *lst = novo;
    } else {
        MaquinaAutonoma *p = *lst;
        while (p->proximo != NULL) {
            p = p->proximo;
        }
        p->proximo = novo; // Insere no final
    }
    printf("✅ Máquina %s adicionada à lista na posição correta de acordo com a ordem de entrada: %s\n", novo->numero_registro, novo->localizacao);
}

void adicionar_elemento_lista_relatorio(MaquinaAutonoma **lst, MaquinaAutonoma *novo) {
    MaquinaAutonoma *atual = *lst;
    MaquinaAutonoma *anterior = NULL;

    while (atual != NULL && strcmp(atual->numero_registro, novo->numero_registro) < 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (anterior == NULL) {
        novo->proximo = *lst;
        *lst = novo;
    } else {
        anterior->proximo = novo;
        novo->proximo = atual;
    }
}

void remover_elemento_lista(MaquinaAutonoma **lst, char *numero_registro) {
    MaquinaAutonoma *atual = *lst, *anterior = NULL;
    while (atual != NULL && strcmp(atual->numero_registro, numero_registro) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (!atual) return;
    if (!anterior) *lst = atual->proximo;
    else anterior->proximo = atual->proximo;
    free(atual);
}

char *ler_arquivo_json(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo JSON");
        return NULL;
    }

    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    rewind(arquivo);

    char *conteudo = (char *)malloc(tamanho + 1);
    if (!conteudo) {
        perror("Erro ao alocar memória");
        fclose(arquivo);
        return NULL;
    }

    fread(conteudo, 1, tamanho, arquivo);
    conteudo[tamanho] = '\0';
    fclose(arquivo);
    return conteudo;
}

void carregar_json(const char *nome_arquivo, MaquinaAutonoma **lst) {
    int continuar = 0;
    char *json_str = ler_arquivo_json(nome_arquivo);
    if (!json_str) return;

    cJSON *json = cJSON_Parse(json_str);
    if (!json) {
        printf("Erro ao processar JSON\n");
        free(json_str);
        return;
    }

    cJSON *dados = cJSON_GetObjectItem(json, "dados");
    if (!cJSON_IsArray(dados)) {
        printf("Formato de JSON inválido.\n");
        cJSON_Delete(json);
        free(json_str);
        return;
    }

    int qtd_registros = cJSON_GetArraySize(dados);
    printf("Foram encontrados %d registros.\n\nDeseja continuar?\n1 - SIM\n2 - NAO\n", qtd_registros);
    scanf("%d", &continuar);

    if (continuar != 1) {
        cJSON_Delete(json);
        free(json_str);
        return;
    }

    for (int i = 0; i < qtd_registros; i++) {
        cJSON *item = cJSON_GetArrayItem(dados, i);
        MaquinaAutonoma *nova_maquina = (MaquinaAutonoma *)malloc(sizeof(MaquinaAutonoma));
        if (!nova_maquina) {
            perror("Erro ao alocar memória para nova máquina");
            cJSON_Delete(json);
            free(json_str);
            return;
        }
        nova_maquina->proximo = NULL;

        strcpy(nova_maquina->numero_registro, cJSON_GetObjectItem(item, "renamaut")->valuestring);
        strcpy(nova_maquina->fabricante, cJSON_GetObjectItem(item, "fab")->valuestring);
        strcpy(nova_maquina->modelo, cJSON_GetObjectItem(item, "mod")->valuestring);
        strcpy(nova_maquina->categoria, cJSON_GetObjectItem(item, "cat")->valuestring);
        strcpy(nova_maquina->aplicacao, cJSON_GetObjectItem(item, "apl")->valuestring);
        nova_maquina->ano_fabricacao = cJSON_GetObjectItem(item, "ano")->valueint;
        strcpy(nova_maquina->responsavel, cJSON_GetObjectItem(item, "resp")->valuestring);

        cJSON *loc_base = cJSON_GetObjectItem(item, "loc_base");
        if (loc_base) {
            const char *cidade = cJSON_GetObjectItem(loc_base, "cidade")->valuestring;
            const char *uf = cJSON_GetObjectItem(loc_base, "uf")->valuestring;
            snprintf(nova_maquina->localizacao, sizeof(nova_maquina->localizacao), "%s - %s", cidade, uf);
        } else {
            strcpy(nova_maquina->localizacao, "Desconhecido");
        }

        adicionar_elemento_lista(lst, nova_maquina);
    }

    cJSON_Delete(json);
    free(json_str);
}


void exibir_dados(MaquinaAutonoma *lst) {
    MaquinaAutonoma *atual = lst;
    while (atual != NULL) {
        printf("%s; %s; %s; %d; %s; %s\n", 
            atual->numero_registro, 
            atual->responsavel, 
            atual->modelo, 
            atual->ano_fabricacao, 
            (atual->status == 1) ? "Ativo" : "Inativo", 
            atual->localizacao
        );
        atual = atual->proximo;
    }
}

void consulta_numero_registro(MaquinaAutonoma *lst, char *num_registro){
    MaquinaAutonoma *p = lst;
    while(p != NULL && strcmp(p->numero_registro, num_registro) != 0){
        p = p->proximo;
    }

    if(p != NULL){
        printf("Máquina encontrada, segue as informações dela:\n\n");
        printf("Número de Registro: %s\n", p->numero_registro);
        printf("Fabricante: %s\n", p->fabricante);
        printf("Modelo: %s\n", p->modelo);
        printf("Categoria: %s\n", p->categoria);
        printf("Aplicação: %s\n", p->aplicacao);
        printf("Ano de Fabricação: %d\n", p->ano_fabricacao);
        printf("Responsável: %s\n", p->responsavel);
        printf("Status: %d\n", p->status);
        printf("Localização: %s\n", p->localizacao);
    } else {
        printf("Máquina não encontrada.\n");
    }
}

void inativacao_numero_registro(MaquinaAutonoma **lst, char *num_registro){
    MaquinaAutonoma *p = *lst;
    int opcao = 999;
    while(p != NULL && strcmp(p->numero_registro, num_registro) != 0){
        p = p->proximo;
    }

    if(p != NULL){
        printf("Máquina %s encontrada com sucesso, deseja desativar a mesma?\n1 - sim\n2 - nao\n", p->numero_registro);
        scanf("%d", &opcao);
        if(opcao == 1){
            p->status = 0;
            printf("Máquina desativada com sucesso!\n");
        }else{
            printf("Processo de desativação cancelado.\n");
        }

    } else {
        printf("Máquina não encontrada.\n");
    }
}

void salvar_dados(MaquinaAutonoma *lst, const char *arquivo) {
    FILE *file = fopen(arquivo, "a"); 
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para gravação.\n");
        return;
    }

    MaquinaAutonoma *atual = lst;
    while (atual != NULL) {
        fprintf(file, "Número de Registro: %s\n", atual->numero_registro);
        fprintf(file, "Fabricante: %s\n", atual->fabricante);
        fprintf(file, "Modelo: %s\n", atual->modelo);
        fprintf(file, "Categoria: %s\n", atual->categoria);
        fprintf(file, "Aplicação: %s\n", atual->aplicacao);
        fprintf(file, "Ano de Fabricação: %d\n", atual->ano_fabricacao);
        fprintf(file, "Responsável: %s\n", atual->responsavel);
        fprintf(file, "Status: %s\n", (atual->status == 1) ? "Ativo" : "Inativo");
        fprintf(file, "Localização: %s\n", atual->localizacao);
        fprintf(file, "----------------------------------\n");        

        atual = atual->proximo;
    }

    fclose(file);
    printf("Dados salvos com sucesso no arquivo de texto!\n");
}

void exibir_maquinas_por_responsavel(MaquinaAutonoma *lst, const char *cpf_cnpj) {
    MaquinaAutonoma *atual = lst;
    MaquinaAutonoma *maquinas_ativas = NULL;
    MaquinaAutonoma *maquinas_inativas = NULL;

    // Filtra as máquinas com base no CPF/CNPJ
    while (atual != NULL) {
        if (strcmp(atual->responsavel, cpf_cnpj) == 0) {
            MaquinaAutonoma *nova_maquina = (MaquinaAutonoma *)malloc(sizeof(MaquinaAutonoma));
            if (!nova_maquina) {
                perror("Erro ao alocar memória para nova máquina");
                return;
            }
            // Copia os dados da máquina atual para o novo nó
            strcpy(nova_maquina->numero_registro, atual->numero_registro);
            strcpy(nova_maquina->fabricante, atual->fabricante);
            strcpy(nova_maquina->modelo, atual->modelo);
            strcpy(nova_maquina->categoria, atual->categoria);
            strcpy(nova_maquina->aplicacao, atual->aplicacao);
            nova_maquina->ano_fabricacao = atual->ano_fabricacao;
            strcpy(nova_maquina->responsavel, atual->responsavel);
            nova_maquina->status = atual->status;
            strcpy(nova_maquina->localizacao, atual->localizacao);
            nova_maquina->proximo = NULL;

            // Adiciona na lista de máquinas ativas ou inativas
            if (nova_maquina->status == 1) {
                // Adiciona na lista de ativas
                adicionar_elemento_lista_relatorio(&maquinas_ativas, nova_maquina);
            } else {
                // Adiciona na lista de inativas
                adicionar_elemento_lista_relatorio(&maquinas_inativas, nova_maquina);
            }
        }
        atual = atual->proximo;
    }

    // Exibe o relatório de máquinas ativas
    printf("\nRelatório de Máquinas Ativas para o CPF/CNPJ: %s\n", cpf_cnpj);
    exibir_dados(maquinas_ativas);  // Função que exibe a lista de máquinas ativas

    // Exibe o relatório de máquinas inativas
    if (maquinas_inativas != NULL) {
        printf("\nRelatório de Máquinas Inativas para o CPF/CNPJ: %s\n", cpf_cnpj);
        exibir_dados(maquinas_inativas);  // Função que exibe a lista de máquinas inativas
    } else {
        printf("Nenhuma máquina inativa encontrada para o CPF/CNPJ: %s\n", cpf_cnpj);
    }
}

int main() {
    MaquinaAutonoma *lst = NULL;

    carregar_json("teste.json", &lst);
    
    // Exibir máquinas por responsável
    //exibir_maquinas_por_responsavel(lst, "18326611807484");
    //exibir_dados(lst);
    relatorio_por_categoria(lst, "103456");
    printf("\n\n\n");


    salvar_dados(lst, "dados_maquinas.txt");

    return 0;
}
