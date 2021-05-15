#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PALAVRAS_LINHAS 110
#define COUNTRY 35
#define COUNTRY_CODE 4
#define CONTINENT 8
#define INDICATOR 7
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
}

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
    while (fgets(buffer, MAX_PALAVRAS_LINHAS, ficheiro) != NULL)
    {
        if (linhas == 0)
        {
            linhas++;
            continue;
        }
        dados_t *item = ler_linha(buffer);
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
void menu_ordenar_lista(dados_t* lista,char* tipo, yearWeek_t* ano_semana)
{
    if(tipo == "alfa")
    {
       // ordenar_alfa();
    }
    else if(tipo == "pop")
    {
     //   ordenar_pop();
    }
    else if(tipo == "inf")
    {
      //  ordenar_inf();
    }
    else if(tipo == "dea")
    {
     //   ordenar_dea();
    }
    else
    {
        printf("Erro de tipo de ordenacao");
        exit(-1);
    }
}*/
dados_t* troca(dados_t* left, dados_t* right)
{
    left->next = right->next;
    right->next = left;
    return right;
}

void ordenacao_pop(dados_t** right, dados_t** left, int* flag)
{
    if((*right)->population < (*right)->next->population)
    {
        (*left)->next = troca((*right), (*right)->next);
        (*flag) = 1;
    }
}

void ordenacao_alfa(dados_t** right, dados_t** left, int* flag)
{
    if(strcmp((*right)->country, (*right)->next->country) > 0 )
    {
        (*left)->next = troca((*right), (*right)->next);
        (*flag) = 1;
    }
}

dados_t* remove_do_inicio(dados_t* headlist)
{
    if(headlist == NULL)
    {
        printf("A lista esta vazia");
    }
    else
    {
        dados_t *temp = headlist;
        headlist = headlist->next;
        free(temp);
        temp = NULL;
    }
    return headlist;
}

dados_t* selecao_lista_inf(dados_t* root)
{
    dados_t *left, *right, *head, aux;

    head = &aux;
    head = root;
    if(root != NULL && root->next != NULL)
    {
        left = head;
        right = head->next;
        while(right->next != NULL)
        {
            printf("%s\n", right->country);
            printf("%s\n", right->next->country);
            printf("%d\n", right->weekly_count);
            printf("%d\n", right->next->weekly_count);
            while(strcmp(right->country, right->next->country) == 0)
            {

                if(strcmp(right->indicator, "cases") == 0)
                {
                    if(right->weekly_count < right->next->weekly_count)
                    {
                        left->next = remove_do_inicio(right);

                    }
                }
                else
                {
                    left->next = remove_do_inicio(right);
                }

                left = right;
                if(right->next != NULL)
                    right = right->next;
            }
            left = right;
            if(right->next != NULL)
                right = right->next;
        }

    }
    root = head->next;
    return root;
}

dados_t* ordenar_lista(dados_t* root)
{
    int flag = 1;
    dados_t *left, *right, *head, aux;

    head = &aux;
    head = root;
    if(root != NULL && root->next != NULL)
    {
        while(flag)
        {
            flag = 0;
            left = head;
            right = head->next;
            while(right->next != NULL)
            {
                //ordenacao_pop(&right, &left, &flag);
                ordenacao_alfa(&right, &left, &flag);
                left = right;
                if(right->next != NULL)
                    right = right->next;
            }
        }
    }
    root = head->next;
    return root;
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

void imprime_lista(dados_t *root)
{
    dados_t *curr = root;
    while (curr->next != NULL)
    {
        // yearweek a nao imprimir certo
        printf("%s - %s - %s - %d - %s - %d - %d-%d - %f - %d\n", curr->country, curr->country_code, curr->continent, curr->population, curr->indicator,\
               curr->weekly_count, curr->year_week->year, curr->year_week->week, curr->rate_14_day, curr->cumulative_count);
        curr = curr->next;
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
}
*/



void liberta_lista(dados_t* head)
{
    dados_t* curr;

    while (head != NULL)
    {
        curr = head;
        head = head->next;
        free(curr);
    }
}

void liberta_memoria(char **linhas, int num_linhas)
{
    int i;
    for (i = 0; i < num_linhas; i++)
    {
        free(linhas[i]);
    }
    free(linhas);
}

int main(int argc, char *argv[])
{
    dados_t *root_principal = cria_lista();
    //root_principal = ordenar_lista(root_principal);
    root_principal = selecao_lista_inf(root_principal);
    imprime_lista(root_principal);
    liberta_lista(root_principal);
    return 0;
}
