#include <cjson/cJSON.h>
#include <stdio.h>

// Lê um arquivo ERMAUF (.json) e carrega os registros na tabela hash
#include <cjson/cJSON.h>

void importar_json(HashTable* ht, const char* nome_arquivo) {
    FILE* f = fopen(nome_arquivo, "r");
    if (!f) {
        perror("Erro ao abrir JSON");
        return;
    }

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* data = malloc(len + 1);
    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);

    cJSON* root = cJSON_Parse(data);
    free(data);
    if (!root) {
        printf("Erro ao parsear JSON.\n");
        return;
    }

    cJSON* lista = cJSON_GetObjectItem(root, "dados");
    if (!cJSON_IsArray(lista)) {
        printf("JSON não contém 'dados' como array.\n");
        cJSON_Delete(root);
        return;
    }

    cJSON* item;
    cJSON_ArrayForEach(item, lista) {
        Registro reg;
        memset(&reg, 0, sizeof(Registro));

        strcpy(reg.renamaut, cJSON_GetObjectItem(item, "renamaut")->valuestring);
        strcpy(reg.fabricante, cJSON_GetObjectItem(item, "fab")->valuestring);
        strcpy(reg.modelo, cJSON_GetObjectItem(item, "mod")->valuestring);
        strcpy(reg.categoria, cJSON_GetObjectItem(item, "cat")->valuestring);
        strcpy(reg.aplicacao, cJSON_GetObjectItem(item, "apl")->valuestring);
        reg.ano = cJSON_GetObjectItem(item, "ano")->valueint;
        strcpy(reg.responsavel, cJSON_GetObjectItem(item, "resp")->valuestring);
        strcpy(reg.status, "ativo");  // por padrão

        cJSON* loc = cJSON_GetObjectItem(item, "loc_base");
        if (loc) {
            strcpy(reg.cidade, cJSON_GetObjectItem(loc, "cidade")->valuestring);
            strcpy(reg.estado, cJSON_GetObjectItem(loc, "uf")->valuestring);
        }
        inserir(ht, reg);
    }

    cJSON_Delete(root);
}
