#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define MAX_PALAVRAS_LINHAS 100

void menu(char **linhas, int* num_linhas, char* leitura_dados, char* ordenacao_dados, char* selecao_dados, char* restricao_dados, char* leitura_ficheiros, char* escrita_ficheiros);

char ** ler_ficheiro(char * nomeficheiro, int *linhas);
void liberta_memoria(char **linhas, int num_linhas);

int main(int argc, char *argv[])
{
    char ** linhas = NULL;
    int num_linhas = 0;
    int opt;
    opterr = 0;
    char leitura_dados[MAX_PALAVRAS_LINHAS], ordenacao_dados[MAX_PALAVRAS_LINHAS], selecao_dados[MAX_PALAVRAS_LINHAS], restricao_dados[MAX_PALAVRAS_LINHAS], leitura_ficheiros[MAX_PALAVRAS_LINHAS], escrita_ficheiros[MAX_PALAVRAS_LINHAS];

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
        default:  /* opcoes invalidas */
            printf("Erro na leitura");
            return EXIT_FAILURE;
            break;
        }
    }
    menu(linhas, &num_linhas, leitura_dados, ordenacao_dados, selecao_dados, restricao_dados, leitura_ficheiros, leitura_dados);

    liberta_memoria(linhas, num_linhas);
    return 0;
}

void menu(char **linhas, int* num_linhas, char* leitura_dados, char* ordenacao_dados, char* selecao_dados, char* restricao_dados, char* leitura_ficheiros, char* escrita_ficheiros)
{
       if(strcmp(leitura_dados, "all") == 0){

         linhas = ler_ficheiro("covid19_w_t01.csv", &num_linhas);
        printf("%d", num_linhas);
    }

}

char ** ler_ficheiro(char * nomeficheiro, int *linhas)
{
    FILE * ficheiro;
    char buffer[MAX_PALAVRAS_LINHAS];
    char ** linha_lida;
    ficheiro = fopen(nomeficheiro, "r");

    if(ficheiro == NULL)
    {
        printf("Erro a abrir ficheiro\n");
        exit(EXIT_FAILURE);
    }
    *linhas = 0;
    while(fgets(buffer, MAX_PALAVRAS_LINHAS, ficheiro) != NULL)
    {
        if(*linhas != 0)
        {
            int length = strlen(buffer);
            if(buffer[length - 1] == '\n')
                buffer[length - 1] = '\0';
            if(*linhas == 1)
            {
                if((linha_lida = (char**)malloc(sizeof(char*))) == NULL)
                {
                    printf("Erro na aloccao de memoria\n");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    if((linha_lida = (char**)realloc(linha_lida, *linhas * sizeof(char*))) == NULL)
                    {
                        printf("Erro no realloc\n");
                        exit(EXIT_FAILURE);
                    }
                    linha_lida[*linhas - 1] = (char*)malloc(sizeof(char) * length + 1);
                }
                strcpy(linha_lida[*linhas - 1], buffer);
            }
        }
        (*linhas)++;
    }
    fclose(ficheiro);
    return linha_lida;
}



void liberta_memoria(char **linhas, int num_linhas)
{
    int i;
    for(i = 0; i < num_linhas; i++)
    {
        free(linhas[i]);
    }
    free(linhas);
}
