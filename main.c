#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PALAVRAS_LINHAS 110
#define MAX_CHAR 20

typedef struct dados
{
    char country[MAX_CHAR];
    char country_code[4];
    char continent[MAX_CHAR];
    int population;
    char indicator[7];
    int weekly_count;
    int year_week;
    int rate_14_day;
    int cumulative_count;

    struct dados *next;
} dados_t;

char ** ler_ficheiro(char * nomeficheiro, int *linhas);
char ** ler_ficheiro_bin(char * nomeficheiro, int *linhas);
char** linhas_continente(char** linhas_lidas, int* linhas, char* continente);
void liberta_memoria(char **linhas, int num_linhas);
void escrever_ficheiro(char* nomeficheiro);
void escrever_ficheiro_bin(char* nomeficheiro);
void imprime_ficheiro(char** linhas_ficheiro, int* linhas);

int main(int argc, char *argv[])
{
    char ** linhas = NULL;
    int num_linhas = 0;




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

dados_t  cria_lista()
{
    FILE* ficheiro;
    char* buffer = NULL;
    dados_t* root = malloc(sizeof(dados_t));
    ficheiro = fopen("covid19_w_t01.csv", "r");

    if(ficheiro == NULL)
    {
        printf("Erro a abrir ficheiro\n");
        exit(EXIT_FAILURE);
    }

    (root) = NULL;
    int linhas = 0;
    while(fgets(buffer, MAX_PALAVRAS_LINHAS, ficheiro) != NULL)
    {
        if(linhas == 0)
        {
            continue;
        }
        int length = strlen(buffer);
        if(buffer[length - 1] == '\n')
            buffer[length - 1] = '\0';


        if((buffer = (char*)malloc(MAX_PALAVRAS_LINHAS * sizeof(char*))) == NULL)
        {
            printf("Erro na alocacao de memoria\n");
            exit(EXIT_FAILURE);
        }
        for(int i = 0; i < 9; i++)
        {
            char* token = strtok(buffer, ",");
            switch(i)
            {
            case 0:
                strcpy(root->country, token);
                break;
            case 1:
                strcpy(root->country_code, token);
                break;
            case 2:
                strcpy(root->continent, token);
                break;
            case 3:
                root->population = token;//ver que ainda e int e nao char
                break;
            case 4:
                strcpy(root->indicator, token);
                break;
            case 5:
                root->weekly_count = token;
                break;
            case 6:
                root->year_week = token;
                break;
            case 7:
                root->rate_14_day = token;
                break;
            case 8:
                root->cumulative_count = token;
                break;
            }
        }
        root = root->next;
        free(buffer);
        linhas++;
    }
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
    char* linhas_novas;

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

void new_node(dados_t** root, char* string)
{
    dados_t* new_node = (char*)malloc(sizeof(dados_t));
    if(new_node == NULL)
    {
        printf("Erro a alocar novo node");
        exit(EXIT_FAILURE);
    }
    new_node->next = NULL;
    new_node->country = string;

    if(*root == NULL)
    {
        *root = new_node;
        return;
    }

    dados_t* curr = *root;
    while(curr->next != NULL)
    {
        curr = curr->next;
    }
    curr->next = new_node;
}
void deallocate_node(node_t** root)
{
    node* curr = *root;
    while(curr != NULL)
    {
        node* aux = *curr;
        curr = curr->next;
        free(aux);
    }
    *root = NULL;
}

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
