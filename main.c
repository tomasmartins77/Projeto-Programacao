#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PALAVRAS_LINHAS 110
#define COUNTRY 25
#define COUNTRY_CODE 4
#define CONTINENT 8
#define POPULATION 10
#define WEEKLY_COUNT 6
#define INDICATOR 7
#define RATE_14_DAY 13
#define CUMULATIVE_COUNT 7
#define FILEPATH_COVID "covid19_w_t01.csv"

typedef struct dados
{
    char country[COUNTRY];
    char country_code[COUNTRY_CODE];
    char continent[CONTINENT];
    char population[POPULATION];
    char indicator[INDICATOR];
    char weekly_count[WEEKLY_COUNT];
    char year_week[CONTINENT];
    char rate_14_day[RATE_14_DAY];
    char cumulative_count[CUMULATIVE_COUNT];

    struct dados *next;
} dados_t;

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
                    printf("Erro na alocacao de memoria\n");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                if((linha_lida = (char**)realloc(linha_lida, *linhas * sizeof(char*))) == NULL)
                {
                    printf("Erro no realloc\n");
                    exit(EXIT_FAILURE);
                }

            }
            linha_lida[(*linhas) - 1] = (char*)malloc(sizeof(char*) * length + 1);
            strcpy(linha_lida[(*linhas) - 1], buffer);
        }
        (*linhas)++;
    }
    (*linhas)--;
    fclose(ficheiro);
    return linha_lida;
}

dados_t* cria_lista()
{
    FILE* ficheiro;
    char buffer[MAX_PALAVRAS_LINHAS];
    dados_t* root = malloc(sizeof(dados_t));
    if(root == NULL)
    {
        printf("Erro na no struct");
        exit(-1);
    }
    ficheiro = fopen(FILEPATH_COVID, "r");

    if(ficheiro == NULL)
    {
        printf("Erro a abrir ficheiro\n");
        exit(EXIT_FAILURE);
    }

    root->next = NULL;
    int linhas = 0;
    printf("%s", buffer);
    while(fgets(buffer, MAX_PALAVRAS_LINHAS, ficheiro) != NULL)
    {
        novo_node(&root);
        printf("%s", buffer);
        if(linhas == 0)
        {
            linhas++;
            continue;
        }
        int length = strlen(buffer);
        if(buffer[length - 1] == '\n')
            buffer[length - 1] = '\0';

        for(int i = 0; i < 9; i++)
        {
            char* token = strtok(buffer, ",");
            int j;
            char token2[MAX_PALAVRAS_LINHAS];
            for(j = 0; j < strlen(token); j++)
            {
                token2[j] = token[j];
            }
            switch(i)
            {
            case 0:
                strcpy(root->country, token2);
                break;
            case 1:
                strcpy(root->country_code, token2);
                break;
            case 2:
                strcpy(root->continent, token2);
                break;
            case 3:
                strcpy(root->population, token2);
                break;
            case 4:
                strcpy(root->indicator, token2);
                break;
            case 5:
                strcpy(root->weekly_count, token2);
                break;
            case 6:
                strcpy(root->year_week, token2);
                break;
            case 7:
                strcpy(root->rate_14_day, token2);
                break;
            case 8:
                strcpy(root->cumulative_count, token2);
                break;
            }
        }
        root = root->next;
        linhas++;
    }
    return root;
}


/*
char ** ler_ficheiro_bin(char * nomeficheiro, int *linhas)
{
    FILE * ficheiro;
    char buffer[MAX_PALAVRAS_LINHAS];

    char ** linha_lida;
    ficheiro = fopen(nomeficheiro, "rb");

    if(ficheiro == NULL)
    {
        printf("Erro a abrir ficheiro\n");
        exit(EXIT_FAILURE);
    }
    *linhas = 1;
    while(fgets(buffer, MAX_PALAVRAS_LINHAS, ficheiro) != NULL)
    {
        int length = strlen(buffer);
        if(buffer[length - 1] == '\n')
            buffer[length - 1] = '\0';
        if(*linhas == 1)
        {
            if((linha_lida = (char**)malloc(sizeof(char*))) == NULL)
            {
                printf("Erro na alocacao de memoria\n");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            if((linha_lida = (char**)realloc(linha_lida, *linhas * sizeof(char*))) == NULL)
            {
                printf("Erro no realloc\n");
                exit(EXIT_FAILURE);
            }

        }
        linha_lida[(*linhas) - 1] = (char*)malloc(sizeof(char*) * length + 1);
        strcpy(linha_lida[(*linhas) - 1], buffer);
        (*linhas)++;
    }
    (*linhas)--;
    fclose(ficheiro);
    return linha_lida;
}*/

void imprime_ficheiro(char** linhas_ficheiro, int* linhas)
{
    int i, j;

    for(i=0; i<*linhas; i++)
    {
        for(j = 0; j < strlen(linhas_ficheiro[i]); j++)
        {
            printf("%c", linhas_ficheiro[i][j]);
        }
        printf("\n");
    }
}

void imprime_lista(dados_t* root)
{
    dados_t* curr = root;
    while(curr->next != NULL)
    {
        printf("%s - %s - %s - %s - %s - %s - %s - %s - %s", curr->country, curr->country_code, curr->continent, curr->population\
               , curr->indicator, curr->weekly_count, curr->year_week, curr->rate_14_day, curr->cumulative_count);
        curr = curr->next;
    }
}

void escrever_ficheiro(char* nomeficheiro)
{
    char buffer[MAX_PALAVRAS_LINHAS];

    FILE* ficheiro = fopen("covid19_w_t01", "w");
    if(ficheiro == NULL)
    {
        printf("Erro a abrir ficheiro");
        exit(EXIT_FAILURE);
    }
    printf("Para terminar de escrever escrever: EOF\n---------------------------------------\n");
    while(fgets(buffer, MAX_PALAVRAS_LINHAS, stdin) != NULL)
    {
        if(strstr(buffer,"EOF") != 0)
        {
            int length = strlen(buffer);
            for(int i = 0; i < length - 4; i++)
                fputc(buffer[i], ficheiro);
            break;
        }
        fputs(buffer, ficheiro);
    }
    fclose(ficheiro);
}

char* string_tok(char* linha, int* linhas_totais)
{
    char* token;
    char* linhas_novas = NULL;

    token = strtok(linha, ",");

    while (token != NULL)
    {
        token = strtok(NULL, ",");
    }
    return linhas_novas;
}
/*
void escrever_ficheiro_bin(char* nomeficheiro)
{
    char buffer[MAX_PALAVRAS_LINHAS];

    FILE* ficheiro = fopen(nomeficheiro, "wb");
    if(ficheiro == NULL)
    {
        printf("Erro a abrir ficheiro");
        exit(EXIT_FAILURE);
    }
    printf("Para terminar de escrever escrever: EOF\n---------------------------------------\n");
    while(fgets(buffer, MAX_PALAVRAS_LINHAS, stdin) != NULL)
    {
        if(strstr(buffer,"EOF") != 0)
        {
            int length = strlen(buffer);
            for(int i = 0; i < length - 4; i++)
                fputc(buffer[i], ficheiro);
            break;
        }
        fputs(buffer, ficheiro);
    }
    fclose(ficheiro);
}

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
}
*/
void novo_node(dados_t** root)
{
    dados_t* new_node = malloc(sizeof(dados_t));
    if(new_node == NULL)
    {
        printf("Erro a alocar novo node");
        exit(EXIT_FAILURE);
    }
    new_node->next = NULL;

    if(*root == NULL)
    {
        *root = new_node;
        return;
    }

    dados_t* curr = *root;
    while(curr->next != NULL) //erroooooooo
    {
        curr = curr->next;
    }
    curr->next = new_node;
}

/*
void deallocate_node(dados_t** root)
{
    dados_t* curr = *root;
    while(curr != NULL)
    {
        dados_t* aux = *curr;
        curr = curr->next;
        free(aux);
    }
    *root = NULL;
}*/
/*
insert_beginning(dados_t** root, char* linha)
{
    dados_t* new_node = malloc(sizeof(dados_t));
    if(new_node == NULL)
    {
        printf("Erro na alocacao");
        exit(-1);
    }
    new_node->? = linha;
    new_node->next = *root;

    *root = new_node;
}

void insert_after(dados_t* node, char* string)
{
    dados_t* new_node = malloc(sizeof(char*));
    if(new_node == NULL)
    {
        printf("Erro na alocacao");
        exit(-1);
    }
    new_node->? = string;
    new_node->next = node->next;
    node->next = new_node;
}
*/
void liberta_memoria(char **linhas, int num_linhas)
{
    int i;
    for(i = 0; i < num_linhas; i++)
    {
        free(linhas[i]);
    }
    free(linhas);
}

int main(int argc, char *argv[])
{
    char ** linhas = NULL;
    int num_linhas = 0;

    dados_t* root_principal = cria_lista();
    imprime_lista(root_principal);

    return 0;
}
