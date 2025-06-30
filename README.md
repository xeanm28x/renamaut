# 🛠️ Sistema de Cadastro e Gerenciamento de Máquinas Autônomas

Este projeto tem como objetivo realizar o cadastro, a busca e a manipulação de informações sobre máquinas autônomas utilizando diferentes estruturas de dados: **Lista Encadeada**, **Tabela de Dispersão (Hash)**, **Árvore Binária**, **Árvore AVL** e **Árvore Rubro-Negra**. Os dados são importados de um arquivo JSON, analisados com a biblioteca `cJSON`, e organizados de forma ordenada pela chave `renamaut`.

## 📁 Estrutura do Projeto

- 🧾 `main.c`: Arquivo principal que inicia o programa.
- 📂 base_dados/
  - Arquivos de dados manipulados pelo programa.
- 📂 gov_dev/
  - `gov_dev.h`: Arquivo disponibilizado pelo Professor Ronaldo para validações, formatações e outras funcionalidades auxiliares.
- 📂 lista_encadeada/
  - `operacoes_lista_encadeada.h`: Implementação da lista encadeada e funções relacionadas.
  - `file_handler.h`: Manipulação de arquivos e importação JSON para lista encadeada.
- 📂 tabela_dispersao/
  - `operacoes_tabela_dispersao.h`: Implementação da tabela de dispersão (hash) e funções relacionadas.
  - `file_handler.h`: Manipulação de arquivos e importação JSON para tabela de dispersão.
- 📂 arvore_binaria/
  - `operacoes_arvore_binaria.h`: Implementação da árvore binária e funções relacionadas.
  - `file_handler.h`: Manipulação de arquivos e importação JSON para árvore binária.
- 📂 arvore_avl/
  - `operacoes_arvore_avl.h`: Implementação da árvore AVL e funções relacionadas.
  - `file_handler.h`: Manipulação de arquivos e importação JSON para árvore AVL.
- 📂 arvore_rubro_negra/
  - `operacoes_rubro_negra.h`: Implementação da árvore Rubro-Negra e funções relacionadas.
  - `file_handler.h`: Manipulação de arquivos e importação JSON para árvore Rubro-Negra.

## 🔧 Funcionalidades

- Leitura e conversão de dados JSON.
- Operações de registros nas estruturas (inserção, busca, remoção, inativação).
- Relatórios por responsável, categoria e fabricante.
- Exportação e importação de dados em arquivos texto.
- Suporte a múltiplas estruturas de dados para comparação de desempenho e organização.

## 📦 Dependências

- [cJSON](https://github.com/DaveGamble/cJSON): Biblioteca para manipulação de arquivos JSON em C.

## 🚀 Como Executar

1. Clone o repositório:

    ```bash
    git clone https://github.com/seu-usuario/seu-repositorio.git
    cd seu-repositorio
    ```

2. Compile o projeto para a estrutura desejada (exemplo para AVL):

    ```bash
    gcc arvore_avl/main.c -o sistema_avl -lcjson
    ./sistema_avl
    ```

   > Altere o caminho do `main.c` e dos includes conforme a estrutura que deseja testar (lista, hash, binária, AVL, Rubro-Negra).

3. Siga o menu interativo para importar dados, buscar, cadastrar e gerar relatórios.

---

**Observação:**  
Cada pasta de estrutura possui seu próprio `main.c`, `file_handler.h` e arquivos de operações.  
Escolha a estrutura desejada para compilar e executar.
