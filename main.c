#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PALAVRAS_LINHAS 110
#define COUNTRY 35
#define COUNTRY_CODE 4
#define CONTINENT 8
#define POPULATION 10
#define WEEKLY_COUNT 6
#define INDICATOR 7
#define RATE_14_DAY 13
#define CUMULATIVE_COUNT 7
#define FILEPATH_COVID "covid19_w_t01.csv"

typedef struct yearWeek
{
    short year;
    short week;
} yearWeek_t;

typedef struct dados
{
    char country[COUNTRY];
    char country_code[COUNTRY_CODE];
    char continent[CONTINENT];
    int population;
    char indicator[INDICATOR];
    int weekly_count;
    yearWeek_t *year_week;
    double rate_14_day;
    int cumulative_count;

    struct dados *next;
} dados_t;

enum leitura
{
    L_ALL,
    L_CONTINENTE
};
enum ordenacao
{
    S_ALFA,
    S_POP,
    S_INF,
    S_DEA
};
enum selecao
{
    inf,
    dea,
    racioinf,
    raciodea
};
enum restricao
{
    min,
    max,
    date,
    dates
};

typedef struct settings
{
    enum leitura criterio_leitura; // -L
    char *leitura_continente;
    enum ordenacao criterio_ord; //-S
    short ord_year;
    short ord_week;
    enum selecao criterio_sel;   //-D
    enum restricao criterio_res; //-P
    short restricao_n;
    short restricao_ano1;
    short restricao_semana1;
    short restricao_ano2;
    short restricao_semana2;
} settings_t;

//headers
dados_t *cria_lista();
dados_t *ler_linha(char *letra);
void inserir_dados(dados_t *dados, char *inicio_coluna, int coluna);
yearWeek_t *parseYearWeek(char *data);

/* char **ler_ficheiro(char *nomeficheiro, int *linhas)
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
                if ((linha_lida = (char **)malloc(sizeof(char *))) == NULL)
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
} */

dados_t *cria_lista()
{
    FILE *ficheiro;
    char buffer[MAX_PALAVRAS_LINHAS];
    dados_t *root = NULL;
    dados_t *last = NULL;

    ficheiro = fopen(FILEPATH_COVID, "r");

    if (ficheiro == NULL)
    {
        printf("Erro a abrir ficheiro\n");
        exit(EXIT_FAILURE);
    }

    int linhas = 0;
    printf("%s", buffer);
    while (fgets(buffer, MAX_PALAVRAS_LINHAS, ficheiro) != NULL)
    {
        printf("%s", buffer);
        if (linhas == 0)
        {
            linhas++;
            continue;
        }
        dados_t *item = ler_linha(buffer);
        printf("%d", item->population);
        if (root == NULL)
        {
            root = item;
        }
        else
        {
            last->next = item;
        }
        last = item;

        linhas++;
    }
    return root;
}

//passar da string para o elemento da lista
dados_t *ler_linha(char *letra)
{
    char *inicio_coluna = letra;
    int coluna = 0;
    dados_t *dados = malloc(sizeof(dados_t));

    dados->next = NULL;

    while (*letra != '\0' && *letra != '\n')
    {

        if (*letra == ',')
        {
            *letra = '\0';

            inserir_dados(dados, inicio_coluna, coluna);

            coluna++;
            inicio_coluna = letra + 1;
        }

        letra++;
    }

    *letra = '\0';
    inserir_dados(dados, inicio_coluna, coluna);
    return dados;
}

void inserir_dados(dados_t *dados, char *inicio_coluna, int coluna)
{
    switch (coluna)
    {

    case 0:
        strcpy(dados->country, inicio_coluna);
        break;
    case 1:
        strcpy(dados->country_code, inicio_coluna);
        break;
    case 2:
        strcpy(dados->continent, inicio_coluna);
        break;
    case 3:
        dados->population = atoi(inicio_coluna);
        break;
    case 4:
        strcpy(dados->indicator, inicio_coluna);
        break;
    case 5:
        dados->weekly_count = atoi(inicio_coluna);
        break;
    case 6:
        dados->year_week = parseYearWeek(inicio_coluna);
        break;
    case 7:
        dados->rate_14_day = atof(inicio_coluna);
        break;
    case 8:
        dados->cumulative_count = atoi(inicio_coluna);
        break;
    }
}

yearWeek_t *parseYearWeek(char *data)
{
    yearWeek_t *yearWeek = malloc(sizeof(yearWeek_t));

    data[4] = '\0';                  // yyyy-ww
    yearWeek->year = atoi(data);     //int
    yearWeek->week = atoi(data + 5); //int
    return yearWeek;
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

/* void imprime_ficheiro(char **linhas_ficheiro, int *linhas)
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
} */
/* 
void imprime_lista(dados_t *root)
{
    dados_t *curr = root;
    while (curr->next != NULL)
    {
        printf("%s - %s - %s - %s - %s - %s - %s - %s - %s", curr->country, curr->country_code, curr->continent, curr->population, curr->indicator, curr->weekly_count, curr->year_week, curr->rate_14_day, curr->cumulative_count);
        curr = curr->next;
    }
} */

/* void escrever_ficheiro(char *nomeficheiro)
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

char *string_tok(char *linha, int *linhas_totais)
{
    char *token;
    char *linhas_novas = NULL;

    token = strtok(linha, ",");

    while (token != NULL)
    {
        token = strtok(NULL, ",");
    }
    return linhas_novas;
} */
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
/* void novo_node(dados_t **root)
{
    dados_t *new_node = malloc(sizeof(dados_t));
    if (new_node == NULL)
    {
        printf("Erro a alocar novo node");
        exit(EXIT_FAILURE);
    }
    new_node->next = NULL;

    if (*root == NULL)
    {
        *root = new_node;
        return;
    }

    dados_t *curr = *root;
    while (curr->next != NULL) //erroooooooo
    {
        curr = curr->next;
    }
    curr->next = new_node;
} */

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
/* void liberta_memoria(char **linhas, int num_linhas)
{
    int i;
    for (i = 0; i < num_linhas; i++)
    {
        free(linhas[i]);
    }
    free(linhas);
} */

int main(int argc, char *argv[])
{

    /*dados_t *root_principal = */ cria_lista();
    // imprime_lista(root_principal);

    return 0;
}
