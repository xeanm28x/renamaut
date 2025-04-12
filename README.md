# 🛠️ Sistema de Cadastro e Gerenciamento de Máquinas Autônomas

Este projeto tem como objetivo realizar o cadastro, a busca e a manipulação de informações sobre máquinas autônomas utilizando **Lista Encadeada**, **Tabela de Dispersão** e **Árvore Binária** como estruturas principais de dados. Os dados são importados de um arquivo JSON, analisados com a biblioteca `cJSON`, e organizados de forma ordenada pela chave `renamaut`.

## 📁 Estrutura do Projeto

- 🧾 `main.c`: Arquivo principal que inicia o programa.
- 📂 base_dados/
  - Aqui são contidos os arquivos manipulados no programa.
- 📂 gov_dev/
  - `gov_dev.h`: Arquivo disponibilizado pelo Professor Ronaldo para auxiliar na implementação do programa (validações, formatações, entre outras funcionalidades).
- 📂 lista_encadeada/
  - `operacoes_lista_encadeada.h`: Implementação da lista encadeada e funções relacionadas (inserção, busca, remoção etc.).
  - `file_handler.h`: Implementação do arquivo de persistência de dados + arquivos de importação JSON adaptados para lista encadeada.
- 📂 tabela_dispersao/
  - `operacoes_tabela_dispersao.h`: Implementação da tabela de dispersão e funções relacionadas (inserção, busca, remoção etc.).
  - `file_handler.h`: Implementação do arquivo de persistência de dados + arquivos de importação JSON adaptados para tabela de dispersão.
- 📂 arvore_binaria/
  - `operacoes_arvore_binaria.h`: Implementação da árvore binária e funções relacionadas (inserção, busca, remoção etc.).
  - `file_handler.h`: Implementação do arquivo de persistência de dados + arquivos de importação JSON adaptados para árvore binária.

## 🔧 Funcionalidades

- Leitura e conversão de dados JSON.
- Operações de registros nas estruturas (inserção, busca e remoção).

## 📦 Dependências

- [cJSON](https://github.com/DaveGamble/cJSON): Biblioteca para manipulação de arquivos JSON em C.

## 🚀 Como Executar

1. Clone o repositório:

```bash
git clone https://github.com/seu-usuario/seu-repositorio.git
cd seu-repositorio
```

🚧👷 ...
