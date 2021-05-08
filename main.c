#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define MAX_PALAVRAS_LINHAS 110
#define MAX_CHAR 20

typedef struct dados
{
    char country[MAX_CHAR];
    char country_code[MAX_CHAR];
    char continent[MAX_CHAR];
    char population[MAX_CHAR];
    char indicator[MAX_CHAR];
    char weekly_count[MAX_CHAR];
    char year_week[MAX_CHAR];
    char rate_14_day[MAX_CHAR];
    char cumulative_count[MAX_CHAR];

    struct dados *next;
} dados_t;

void menu(char **linhas, int *num_linhas, char *leitura_dados, char *ordenacao_dados, char *selecao_dados, char *restricao_dados, char *leitura_ficheiros, char *escrita_ficheiros);

char **ler_ficheiro(char *nomeficheiro, int *linhas);
char **ler_ficheiro_bin(char *nomeficheiro, int *linhas);
char **linhas_continente(char **linhas_lidas, int *linhas, char *continente);
void liberta_memoria(char **linhas, int num_linhas);
void escrever_ficheiro(char *nomeficheiro);
void escrever_ficheiro_bin(char *nomeficheiro);
void imprime_ficheiro(char **linhas_ficheiro, int *linhas);

int main(int argc, char *argv[])
{
    char **linhas = NULL;
    int num_linhas = 0;
    int opt;
    opterr = 0;
    char leitura_dados[MAX_PALAVRAS_LINHAS] = {0}, ordenacao_dados[MAX_PALAVRAS_LINHAS] = {0}, selecao_dados[MAX_PALAVRAS_LINHAS] = {0}, restricao_dados[MAX_PALAVRAS_LINHAS] = {0},
         leitura_ficheiros[MAX_PALAVRAS_LINHAS] = {0}, escrita_ficheiros[MAX_PALAVRAS_LINHAS] = {0};

    while ((opt = getopt(argc, argv, "hL:S:D:P:i:o:")) != -1)
    {
        switch (opt)
        {
        case 'L':
            sscanf(optarg, "%s", leitura_dados);
            break;
        case 'S':
            sscanf(optarg, "%s", ordenacao_dados);
            break;
        case 'D':
            sscanf(optarg, "%s", selecao_dados);
            break;
        case 'P':
            sscanf(optarg, "%s", restricao_dados);
            break;
        case 'i':
            sscanf(optarg, "%s", leitura_ficheiros);
            break;
        case 'o':
            sscanf(optarg, "%s", escrita_ficheiros);
            break;
        default: /* opcoes invalidas */
            printf("Erro na leitura");
            return EXIT_FAILURE;
            break;
        }
    }
    linhas = ler_ficheiro("covid19_w_t01.csv", &num_linhas);

    linhas = linhas_continente(linhas, &num_linhas, "Africa");
    imprime_ficheiro(linhas, &num_linhas);
    liberta_memoria(linhas, num_linhas);
    return 0;
}

char **ler_ficheiro(char *nomeficheiro, int *linhas)
{
    FILE *ficheiro;
    char buffer[MAX_PALAVRAS_LINHAS];
    char **linha_lida;
    ficheiro = fopen(nomeficheiro, "r");

    if (ficheiro == NULL)
    {
        printf("Erro a abrir ficheiro\n");
        exit(EXIT_FAILURE);
    }
    *linhas = 0;
    while (fgets(buffer, MAX_PALAVRAS_LINHAS, ficheiro) != NULL)
    {
        if (*linhas != 0)
        {
            int length = strlen(buffer);
            if (buffer[length - 1] == '\n')
                buffer[length - 1] = '\0';
            if (*linhas == 1)
            {
                if ((linha_lida = (char **)malloc(sizeof(char **))) == NULL)
                {
                    printf("Erro na alocacao de memoria\n");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                if ((linha_lida = (char **)realloc(linha_lida, *linhas * sizeof(char *))) == NULL)
                {
                    printf("Erro no realloc\n");
                    exit(EXIT_FAILURE);
                }
            }
            linha_lida[(*linhas) - 1] = (char *)malloc(sizeof(char *) * length + 1);
            strcpy(linha_lida[(*linhas) - 1], buffer);
        }
        (*linhas)++;
    }
    (*linhas)--;
    fclose(ficheiro);
    return linha_lida;
}

char **ler_ficheiro_bin(char *nomeficheiro, int *linhas)
{
    FILE *ficheiro;
    char buffer[MAX_PALAVRAS_LINHAS];

    char **linha_lida;
    ficheiro = fopen(nomeficheiro, "rb");

    if (ficheiro == NULL)
    {
        printf("Erro a abrir ficheiro\n");
        exit(EXIT_FAILURE);
    }
    *linhas = 1;
    while (fgets(buffer, MAX_PALAVRAS_LINHAS, ficheiro) != NULL)
    {
        int length = strlen(buffer);
        if (buffer[length - 1] == '\n')
            buffer[length - 1] = '\0';
        if (*linhas == 1)
        {
            if ((linha_lida = (char **)malloc(sizeof(char **))) == NULL)
            {
                printf("Erro na alocacao de memoria\n");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            if ((linha_lida = (char **)realloc(linha_lida, *linhas * sizeof(char *))) == NULL)
            {
                printf("Erro no realloc\n");
                exit(EXIT_FAILURE);
            }
        }
        linha_lida[(*linhas) - 1] = (char *)malloc(sizeof(char *) * length + 1);
        strcpy(linha_lida[(*linhas) - 1], buffer);
        (*linhas)++;
    }
    (*linhas)--;
    fclose(ficheiro);
    return linha_lida;
}

void imprime_ficheiro(char **linhas_ficheiro, int *linhas)
{
    int i, j;

    for (i = 0; i < *linhas; i++)
    {
        for (j = 0; j < strlen(linhas_ficheiro[i]); j++)
        {
            printf("%c", linhas_ficheiro[i][j]);
        }
        printf("\n");
    }
}

void escrever_ficheiro(char *nomeficheiro)
{
    char buffer[MAX_PALAVRAS_LINHAS];

    FILE *ficheiro = fopen("covid19_w_t01", "w");
    if (ficheiro == NULL)
    {
        printf("Erro a abrir ficheiro");
        exit(EXIT_FAILURE);
    }
    printf("Para terminar de escrever escrever: EOF\n---------------------------------------\n");
    while (fgets(buffer, MAX_PALAVRAS_LINHAS, stdin) != NULL)
    {
        if (strstr(buffer, "EOF") != 0)
        {
            int length = strlen(buffer);
            for (int i = 0; i < length - 4; i++)
                fputc(buffer[i], ficheiro);
            break;
        }
        fputs(buffer, ficheiro);
    }
    fclose(ficheiro);
}

void escrever_ficheiro_bin(char *nomeficheiro)
{
    char buffer[MAX_PALAVRAS_LINHAS];

    FILE *ficheiro = fopen(nomeficheiro, "wb");
    if (ficheiro == NULL)
    {
        printf("Erro a abrir ficheiro");
        exit(EXIT_FAILURE);
    }
    printf("Para terminar de escrever escrever: EOF\n---------------------------------------\n");
    while (fgets(buffer, MAX_PALAVRAS_LINHAS, stdin) != NULL)
    {
        if (strstr(buffer, "EOF") != 0)
        {
            int length = strlen(buffer);
            for (int i = 0; i < length - 4; i++)
                fputc(buffer[i], ficheiro);
            break;
        }
        fputs(buffer, ficheiro);
    }
    fclose(ficheiro);
}
/*
char** linhas_continente(char** linhas_lidas, int* linhas, char* continente)
{
    int i;
    char** novas_linhas = NULL;
    char buffer1[MAX_PALAVRAS_LINHAS];

    for (i = 0; i < *linhas; i++)
    {
        printf("%s", buffer1);
        printf("%s", linhas_lidas[i]);
        int length = strlen(linhas_lidas[i]);

        buffer1 = strstr(continente, linhas_lidas[i]);
        printf("%s", buffer1);
        if(strcmp(buffer1, "Europe") != 0)
        {
            novas_linhas[i] = linhas_lidas[i];

        }
    }

    return novas_linhas;
}*/

void liberta_memoria(char **linhas, int num_linhas)
{
    int i;
    for (i = 0; i < num_linhas; i++)
    {
        free(linhas[i]);
    }
    free(linhas);
}
