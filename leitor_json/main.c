
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>

// Function to read file content
char *read_file(const char *filename, size_t *length) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "Error opening file\n");
    return NULL;
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  *length = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Allocate memory for file content
  char *buffer = (char *)malloc(*length + 1);
  if (!buffer) {
    fprintf(stderr, "Memory allocation failed\n");
    fclose(file);
    return NULL;
  }

  // Read file content
  size_t read_size = fread(buffer, 1, *length, file);
  buffer[read_size] = '\0';
  fclose(file);

  return buffer;
}

// Function to process JSON data
void process_json(const char *json_data) {
    cJSON *root = cJSON_Parse(json_data);
    if (!root) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr) {
            fprintf(stderr, "Error parsing JSON: %s\n", error_ptr);
        }
        return;
    }

    // Get the "dados" array
    cJSON *dados = cJSON_GetObjectItem(root, "dados");
    if (dados && cJSON_IsArray(dados)) {
        cJSON *item = NULL;
        int count = 0;

        // Iterate through array elements
        cJSON_ArrayForEach(item, dados) {
            if (cJSON_IsObject(item)) {
                count++;
                // Print all fields for each object
                cJSON *renamaut = cJSON_GetObjectItem(item, "renamaut");
                cJSON *fab = cJSON_GetObjectItem(item, "fab");
                cJSON *mod = cJSON_GetObjectItem(item, "mod");
                cJSON *loc_base = cJSON_GetObjectItem(item, "loc_base");

                printf("\nObject %d:\n", count);
                printf("  Renamaut: %s\n", renamaut->valuestring);
                printf("  Fabricante: %s\n", fab->valuestring);
                printf("  Modelo: %s\n", mod->valuestring);
                
                if (loc_base && cJSON_IsObject(loc_base)) {
                    cJSON *cidade = cJSON_GetObjectItem(loc_base, "cidade");
                    cJSON *uf = cJSON_GetObjectItem(loc_base, "uf");
                    printf("  Localização: %s - %s\n", cidade->valuestring, uf->valuestring);
                }
            }
        }
        printf("\nTotal de objetos processados: %d\n", count);
    }

    cJSON_Delete(root); // Clean up
}

int main(void) {
  const char *filename = "teste.json";
  size_t file_length;

  // Read JSON file
  char *json_data = read_file(filename, &file_length);
  if (!json_data) {
    return 1;
  }

  // Process JSON data
  process_json(json_data);

  // Clean up
  free(json_data);
  return 0;
}
