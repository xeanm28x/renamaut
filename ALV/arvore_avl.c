#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gov_dev.h"

// Definição da struct Localizacao (exemplo)
typedef struct {
    char cidade[50];
    char uf[3]; // Sigla do estado, ex: "SP"
} Localizacao;

// Estrutura da Máquina Autônoma
typedef struct MaquinaAutonoma {
    char renamaut[20];
    char responsavel[50];
    char categoria[50];
    char fabricante[50];
    char modelo[50];
    int ano_fabricacao;
    int status; // 1 = ativa, 0 = inativa
    Localizacao *localizacao;

    int altura;
    struct MaquinaAutonoma *esq;
    struct MaquinaAutonoma *dir;
} MaquinaAutonoma;

typedef struct EstadoGrupo {
    struct EstadoGrupo *esq;
    struct EstadoGrupo *dir;
    MaquinaAutonoma *maquinas;
} EstadoGrupo;

// Funções auxiliares (protótipos)
void inserir_ordenado(MaquinaAutonoma **lista, MaquinaAutonoma *maquina);
MaquinaAutonoma *copiar_maquina(MaquinaAutonoma *origem);
void inserir_estado(EstadoGrupo **grupo_estados, const char *uf, MaquinaAutonoma *maquina);
const char *get_state_name_by_abbr(const char *uf);
void wait_enter(const char *mensagem);

// Funções AVL
int altura(MaquinaAutonoma *n) {
    return n ? n->altura : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

MaquinaAutonoma *rotacao_direita(MaquinaAutonoma *y) {
    MaquinaAutonoma *x = y->esq;
    MaquinaAutonoma *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;

    return x;
}

MaquinaAutonoma *rotacao_esquerda(MaquinaAutonoma *x) {
    MaquinaAutonoma *y = x->dir;
    MaquinaAutonoma *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;

    return y;
}

int fator_balanceamento(MaquinaAutonoma *n) {
    return n ? altura(n->esq) - altura(n->dir) : 0;
}

// Criação de máquina
MaquinaAutonoma *criar_maquina(const char *renamaut) {
    MaquinaAutonoma *nova = malloc(sizeof(MaquinaAutonoma));
    if (!nova) {
        printf("Erro na alocação.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(nova->renamaut, renamaut);
    strcpy(nova->responsavel, "Desconhecido");
    strcpy(nova->categoria, "Genérica");
    strcpy(nova->fabricante, "Indefinido");
    strcpy(nova->modelo, "ModeloX");
    nova->ano_fabricacao = 2020;
    nova->status = 1;

    nova->localizacao = malloc(sizeof(Localizacao));
    if (!nova->localizacao) {
        printf("Erro na alocação de localização.\n");
        free(nova);
        exit(EXIT_FAILURE);
    }
    strcpy(nova->localizacao->cidade, "CidadeX");
    strcpy(nova->localizacao->uf, "SP");

    nova->altura = 1;
    nova->esq = nova->dir = NULL;
    return nova;
}

// Inserção AVL
MaquinaAutonoma *inserir_avl(MaquinaAutonoma *raiz, MaquinaAutonoma *nova) {
    if (!raiz) return nova;

    int cmp = strcmp(nova->renamaut, raiz->renamaut);
    if (cmp < 0)
        raiz->esq = inserir_avl(raiz->esq, nova);
    else if (cmp > 0)
        raiz->dir = inserir_avl(raiz->dir, nova);
    else
        return raiz;

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    int fb = fator_balanceamento(raiz);

    if (fb > 1 && strcmp(nova->renamaut, raiz->esq->renamaut) < 0)
        return rotacao_direita(raiz);
    if (fb < -1 && strcmp(nova->renamaut, raiz->dir->renamaut) > 0)
        return rotacao_esquerda(raiz);
    if (fb > 1 && strcmp(nova->renamaut, raiz->esq->renamaut) > 0) {
        raiz->esq = rotacao_esquerda(raiz->esq);
        return rotacao_direita(raiz);
    }
    if (fb < -1 && strcmp(nova->renamaut, raiz->dir->renamaut) < 0) {
        raiz->dir = rotacao_direita(raiz->dir);
        return rotacao_esquerda(raiz);
    }

    return raiz;
}

// Impressão pré-ordem
void imprimir_preordem(MaquinaAutonoma *raiz) {
    if (raiz) {
        printf("%s ", raiz->renamaut);
        imprimir_preordem(raiz->esq);
        imprimir_preordem(raiz->dir);
    }
}

// Relatórios
void relatorio_responsavel_avl(char *responsavel, MaquinaAutonoma *arvore, MaquinaAutonoma **ativas, MaquinaAutonoma **inativas) {
    if (!arvore) return;

    if (strcmp(arvore->responsavel, responsavel) == 0) {
        if (arvore->status == 1)
            inserir_ordenado(ativas, copiar_maquina(arvore));
        else
            inserir_ordenado(inativas, copiar_maquina(arvore));
    }

    relatorio_responsavel_avl(responsavel, arvore->esq, ativas, inativas);
    relatorio_responsavel_avl(responsavel, arvore->dir, ativas, inativas);
}

void relatorio_categoria_avl(const char *categoria, MaquinaAutonoma *arvore, EstadoGrupo **grupo_estados) {
    if (!arvore) return;

    if (strcmp(arvore->categoria, categoria) == 0 && arvore->status == 1 && arvore->localizacao && arvore->localizacao->uf)
        inserir_estado(grupo_estados, arvore->localizacao->uf, arvore);

    relatorio_categoria_avl(categoria, arvore->esq, grupo_estados);
    relatorio_categoria_avl(categoria, arvore->dir, grupo_estados);
}

void imprimir_relatorio_fabricante(MaquinaAutonoma *arvore) {
    if (!arvore) return;

    imprimir_relatorio_fabricante(arvore->esq);

    char mensagem[512];
    const char *uf = (arvore->localizacao) ? get_state_name_by_abbr(arvore->localizacao->uf) : "Não informado";
    const char *cidade = (arvore->localizacao) ? arvore->localizacao->cidade : "Não informado";

    sprintf(
        mensagem,
        "%s; %s; %s; %d; %s; %s - %s\n",
        arvore->renamaut,
        arvore->responsavel,
        arvore->modelo,
        arvore->ano_fabricacao,
        arvore->status == 0 ? "Inativa" : "Ativa",
        cidade,
        uf
    );

    wait_enter(mensagem);

    imprimir_relatorio_fabricante(arvore->dir);
}

void imprimir_relatorio_categoria(EstadoGrupo *grupo_estados) {
    if (!grupo_estados) return;

    imprimir_relatorio_categoria(grupo_estados->esq);
    imprimir_relatorio_fabricante(grupo_estados->maquinas);
    imprimir_relatorio_categoria(grupo_estados->dir);
}

// Função principal
int main() {
    MaquinaAutonoma *raiz = NULL;

    char *ids[] = {"50", "30", "70", "20", "40", "60", "80"};
    int n = sizeof(ids) / sizeof(ids[0]);

    for (int i = 0; i < n; i++) {
        MaquinaAutonoma *nova = criar_maquina(ids[i]);
        raiz = inserir_avl(raiz, nova);
    }

    printf("Árvore AVL (pré-ordem): ");
    imprimir_preordem(raiz);
    printf("\n");

    return 0;
}
