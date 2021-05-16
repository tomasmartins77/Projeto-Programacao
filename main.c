#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

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
    INF,
    DEA,
    RACIOINF,
    RACIODEA
};
enum restricao
{
    MIN,
    MAX,
    DATE,
    DATES
};

typedef struct settings
{
    enum leitura criterio_leitura; // -L
    char *leitura_continente;
    enum ordenacao criterio_ord; //-S
    yearWeek_t *ord_date;
    enum selecao criterio_sel;   //-D
    enum restricao criterio_res; //-P
    short restricao_n;
    yearWeek_t *restricao_date1;
    yearWeek_t *restricao_date2;
} settings_t;

//headers
dados_t *cria_lista();
dados_t *ler_linha(char *letra);
void inserir_dados(dados_t *dados, char *inicio_coluna, int coluna);
yearWeek_t *parseYearWeek(char *data);

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

dados_t *troca(dados_t *left, dados_t *right)
{
    left->next = right->next;
    right->next = left;
    return right;
}

void ordenacao_pop(dados_t **right, dados_t **left, int *flag)
{
    if ((*right)->population < (*right)->next->population)
    {
        (*left)->next = troca((*right), (*right)->next);
        (*flag) = 1;
    }
}

void ordenacao_alfa(dados_t **right, dados_t **left, int *flag)
{
    if (strcmp((*right)->country, (*right)->next->country) > 0)
    {
        (*left)->next = troca((*right), (*right)->next);
        (*flag) = 1;
    }
}

dados_t *remove_do_inicio(dados_t *headlist)
{
    if (headlist == NULL)
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

void restricao_min(dados_t **right, dados_t **left, int *flag, int n)
{
    dados_t* aux;
    if((*right)->population < n)
    {
        aux = (*right);
        (*left)->next = remove_do_inicio(aux);
        (*flag) = 1;
        (*right) = aux;
    }
}

void restricao_max(dados_t **right, dados_t **left, int *flag, int n)
{
    dados_t* aux;
    if((*right)->population > n)
    {
        aux = (*right);
        (*left)->next = remove_do_inicio(aux);
        (*flag) = 1;
        (*right) = aux;
    }
}

dados_t* restricao_lista(dados_t *root, int n)
{
    int flag = 1;
    dados_t *left, *right, *head, aux;

    n *= 1000;

    head = &aux;
    head = root;
    if (root != NULL && root->next != NULL)
    {
        while (flag)
        {
            flag = 0;
            left = head;
            right = head->next;
            while(right->next != NULL)
            {
                //restricao_min(&right, &left, &flag, n);
                restricao_max(&right, &left, &flag, n);

                left = right;
                if (right->next != NULL)
                    right = right->next;
            }
        }
    }
    root = head->next;
    return root;
}

void selecao_inf(dados_t **right, dados_t **left, int *flag)
{
    dados_t *aux;

    if (strcmp((*right)->indicator, "cases") == 0)
    {
        if (strcmp((*right)->country, (*right)->next->country) == 0)
        {
            if ((*right)->weekly_count < (*right)->next->weekly_count)
            {
                aux = (*right);
                (*left)->next = remove_do_inicio(aux);
                (*flag) = 1;
            }
            else
            {
                (*left) = (*right);
                if ((*right)->next != NULL)
                    (*right) = (*right)->next;
                aux = (*right);
                (*left)->next = remove_do_inicio(aux);
                (*flag) = 1;
            }
        }
    }
    else
    {
        aux = (*right);
        (*left)->next = remove_do_inicio(aux);
        (*flag) = 1;
    }
}

void selecao_dea(dados_t **right, dados_t **left, int *flag)
{
    dados_t *aux;

    if (strcmp((*right)->indicator, "deaths") == 0)
    {
        if (strcmp((*right)->country, (*right)->next->country) == 0)
        {
            if ((*right)->weekly_count < (*right)->next->weekly_count)
            {
                aux = (*right);
                (*left)->next = remove_do_inicio(aux);
                (*flag) = 1;
            }
            else
            {
                (*left) = (*right);
                if ((*right)->next != NULL)
                    (*right) = (*right)->next;
                aux = (*right);
                (*left)->next = remove_do_inicio(aux);
                (*flag) = 1;
            }
        }
    }
    else
    {
        aux = (*right);
        (*left)->next = remove_do_inicio(aux);
        (*flag) = 1;
    }
}

void selecao_racioinf(dados_t **right, dados_t **left, int *flag)
{
    dados_t *aux;

    if (strcmp((*right)->indicator, "cases") == 0)
    {
        if (strcmp((*right)->country, (*right)->next->country) == 0)
        {
            if ((*right)->rate_14_day < (*right)->next->rate_14_day)
            {
                aux = (*right);
                (*left)->next = remove_do_inicio(aux);
                (*flag) = 1;
            }
            else
            {
                (*left) = (*right);
                if ((*right)->next != NULL)
                    (*right) = (*right)->next;
                aux = (*right);
                (*left)->next = remove_do_inicio(aux);
                (*flag) = 1;
            }
        }
    }
    else
    {
        aux = (*right);
        (*left)->next = remove_do_inicio(aux);
        (*flag) = 1;
    }
}

void selecao_raciodea(dados_t **right, dados_t **left, int *flag)
{
    dados_t *aux;

    if (strcmp((*right)->indicator, "deaths") == 0)
    {
        if (strcmp((*right)->country, (*right)->next->country) == 0)
        {
            if ((*right)->rate_14_day < (*right)->next->rate_14_day)
            {
                aux = (*right);
                (*left)->next = remove_do_inicio(aux);
                (*flag) = 1;
            }
            else
            {
                (*left) = (*right);
                if ((*right)->next != NULL)
                    (*right) = (*right)->next;
                aux = (*right);
                (*left)->next = remove_do_inicio(aux);
                (*flag) = 1;
            }
        }
    }
    else
    {
        aux = (*right);
        (*left)->next = remove_do_inicio(aux);
        (*flag) = 1;
    }
}

dados_t *selecao_lista(dados_t *root)
{
    int flag = 1;
    dados_t *left, *right, *head, aux;

    head = &aux;
    head = root;
    if (root != NULL && root->next != NULL)
    {
        while (flag)
        {
            flag = 0;
            left = head;
            right = head->next;
            while (right->next != NULL)
            {
                //selecao_inf(&right, &left, &flag);
                //selecao_dea(&right, &left, &flag);
                selecao_racioinf(&right, &left, &flag);
                //selecao_raciodea(&right, &left, &flag);
                left = right;
                if (right->next != NULL)
                    right = right->next;
            }
        }
    }
    root = head->next;
    return root;
}

dados_t *ordenar_lista(dados_t *root)
{
    int flag = 1;
    dados_t *left, *right, *head, aux;

    head = &aux;
    head = root;
    if (root != NULL && root->next != NULL)
    {
        while (flag)
        {
            flag = 0;
            left = head;
            right = head->next;
            while (right->next != NULL)
            {
                //ordenacao_pop(&right, &left, &flag);
                ordenacao_alfa(&right, &left, &flag);
                left = right;
                if (right->next != NULL)
                    right = right->next;
            }
        }
    }
    root = head->next;
    return root;
}

void imprime_lista(dados_t *root)
{
    dados_t *curr = root;
    while (curr->next != NULL)
    {
        printf("%s - %s - %s - %d - %s - %d - %d-%d - %f - %d\n", curr->country, curr->country_code, curr->continent, curr->population, curr->indicator,
               curr->weekly_count, curr->year_week->year, curr->year_week->week, curr->rate_14_day, curr->cumulative_count);
        curr = curr->next;
    }
}

void cria_ficheiro(dados_t* root)
{
    FILE* fp;
    dados_t* curr;
    fp = fopen("teste.csv", "w");

    curr = root;
    while(curr->next != NULL)
    {
        fprintf(fp, "%s, %s, %s, %d, %s, %d, %d-%d, %f, %d\n", curr->country, curr->country_code, curr->continent, curr->population, curr->indicator,
                curr->weekly_count, curr->year_week->year, curr->year_week->week, curr->rate_14_day, curr->cumulative_count);
        curr = curr->next;
    }
    fclose(fp);
}

void liberta_lista(dados_t *head)
{
    dados_t *curr;

    while (head != NULL)
    {
        curr = head;
        head = head->next;
        free(curr);
    }
}

int main(int argc, char *argv[])
{
    settings_t *settings = malloc(sizeof(settings_t));
    int opt;
    opterr = 0;
    char criterio_L[20];
    char criterio_S[20];
    char criterio_D[20];
    char criterio_P[20];
    while ((opt = getopt(argc, argv, "L:S:D:P:i:o:")) != -1)
    {
        switch (opt)
        {
        case 'L':
            sscanf(optarg, "%s", criterio_L);
            if (strcmp(criterio_L, "all") == 0)
                settings->criterio_leitura = L_ALL;

            else
            {
                settings->criterio_leitura = L_CONTINENTE;
                settings->leitura_continente = malloc(sizeof(char) * (strlen(criterio_L) + 1));
                strcpy(settings->leitura_continente, criterio_L);
            }

            break;

        case 'S':
            sscanf(optarg, "%s", criterio_S);
            if (strcmp(criterio_S, "alfa") == 0)
                settings->criterio_ord = S_ALFA;

            else if (strcmp(criterio_S, "pop") == 0)
                settings->criterio_ord = S_POP;

            else if (strcmp(criterio_S, "inf") == 0)
            {
                settings->criterio_ord = S_INF;
                settings->ord_date = parseYearWeek("2020-15");
            }
            //separar ano e semana

            else if (strcmp(criterio_S, "dea") == 0)
            {
                settings->criterio_ord = S_DEA;
                settings->ord_date = parseYearWeek("2020-15");
            }
            //separar ano e semana
            break;

        case 'D':
            sscanf(optarg, "%s", criterio_D);
            if (strcmp(criterio_D, "inf") == 0)
                settings->criterio_sel = INF;

            else if (strcmp(criterio_D, "dea") == 0)
                settings->criterio_sel = DEA;

            else if (strcmp(criterio_D, "dea") == 0)
                settings->criterio_sel = RACIOINF;

            else if (strcmp(criterio_D, "dea") == 0)
                settings->criterio_sel = RACIODEA;
            break;
        case 'P': //nenhum destes ta a funcionar caralho
            sscanf(optarg, "%s", criterio_P);
            if (strcmp(criterio_P, "min") == 0)
                settings->criterio_res = MIN;

            else if (strcmp(criterio_P, "max") == 0)
                settings->criterio_res = MAX;

            else if (strcmp(criterio_P, "date") == 0)
            {
                settings->criterio_res = DATE;
                settings->restricao_date1 = parseYearWeek("2020-15");
            }

            else if (strcmp(criterio_P, "dates") == 0)
            {
                settings->criterio_res = DATES;
                settings->restricao_date1 = parseYearWeek("2020-14");
                settings->restricao_date2 = parseYearWeek("2020-15");
            }
            break;
        case 'i':
            break;
        case 'o':
            break;
        }
    }
    int n = 10000;
    dados_t *root_principal = cria_lista();
    //root_principal = selecao_lista(root_principal);
    //root_principal = ordenar_lista(root_principal);
    root_principal = restricao_lista(root_principal, n);
    cria_ficheiro(root_principal);
    imprime_lista(root_principal);
    liberta_lista(root_principal);
    return 0;
}
