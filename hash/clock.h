#include <time.h>
#include <stdio.h>

static inline double medir_tempo(clock_t inicio, clock_t fim, const char* operacao) {
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    // Exibe no terminal
    printf("[Tempo %s] %.6f segundos\n", operacao, tempo);

    // Grava no arquivo de log
    FILE* log = fopen("relatorio_tempo.log", "a");
    if (log) {
        time_t agora = time(NULL);
        struct tm* t = localtime(&agora);
        fprintf(log, "[%02d/%02d/%04d %02d:%02d:%02d] %s: %.6f segundos\n",
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
            t->tm_hour, t->tm_min, t->tm_sec,
            operacao, tempo);
        fclose(log);
    }

    return tempo;
}
