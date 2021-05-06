#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PALAVRAS_LINHAS 100

char ** ler_ficheiro(char * nomeficheiro, int *linhas);
void liberta_memoria(char **linhas, int num_linhas);

int main()
{
    char ** linhas = NULL;
    int num_linhas = 0;
    linhas = ler_ficheiro("covid19_w_t01.csv", &num_linhas);
    liberta_memoria(linhas, num_linhas);
    return 0;
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
        if(*linhas != 0)
        {
            int length = strlen(buffer);
            if(buffer[length - 1] == '\n')
                buffer[length - 1] = '\0';
            if(*linhas == 1)
            {
                if(linha_lida = (char**)malloc(sizeof(char*)) == NULL)
                {
                    printf("Erro na aloccao de memoria\n");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    if(linha_lida = (char**)realloc(*linhas, sizeof(char*)) == NULL)
                    {
                        printf("Erro no realloc\n");
                        exit(EXIT_FAILURE);
                    }
                    linha_lida[*linhas - 1] = (char*)malloc(sizeof(char) * length + 1);
                }
                strcpy(linha_lida[*linhas - 1], buffer);
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
