#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

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

    struct dados *prev;
    struct dados *next;
} dados_t;

typedef struct lista
{
    dados_t *first;
    dados_t *last;
} lista_t;

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
    D_NONE,
    D_INF,
    D_DEA,
    D_RACIOINF,
    D_RACIODEA
};
enum restricao
{
    P_MIN,
    P_MAX,
    P_DATE,
    P_DATES
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
lista_t *ler_ficheiro();
dados_t *ler_linha(char *letra);
void inserir_dados(dados_t *dados, char *inicio_coluna, int coluna);
yearWeek_t *parseYearWeek(char *dados);
lista_t *cria_lista();
void inserir_elemento_final(lista_t *lista, dados_t *item);
void destruir_dados(dados_t *dados);
int selecao_inf(dados_t *atual, dados_t *comparacao);

lista_t *cria_lista()
{
    lista_t *novaLista = malloc(sizeof(lista_t));

    novaLista->first = NULL;
    novaLista->last = NULL;

    return novaLista;
}

void inserir_elemento_final(lista_t *lista, dados_t *item)
{
    item->next = NULL;        //inicia o next como null porque é o ultimo elemento
    item->prev = lista->last; //o item anterior é o "antigo" ultimo elemento da lista

    if (lista->first == NULL) //se a lista estiver vazia
    {
        lista->first = item; //o novo elemento torna-se o primeiro
    }
    else
    {
        lista->last->next = item;
    }
    lista->last = item;
}

lista_t *ler_ficheiro()
{
    FILE *ficheiro;
    char buffer[MAX_PALAVRAS_LINHAS];
    lista_t *lista = cria_lista();

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
        inserir_elemento_final(lista, item);
    }

    fclose(ficheiro);
    return lista;
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

/** \brief  converte o input dado (string) num int para facilitar a comparacao de datas
 *
 * \param data *char
 * \return yearWeek_t
 *
 */
yearWeek_t *parseYearWeek(char *dados)
{
    yearWeek_t *yearWeek = malloc(sizeof(yearWeek_t));

    dados[4] = '\0';
    yearWeek->year = atoi(dados);        //int
    yearWeek->week = atoi(dados + 5);     //int

    return yearWeek;
}

settings_t *trocadatas(settings_t *datas)
{
    settings_t *aux = NULL;
    if(datas->restricao_date1 < datas->restricao_date2)
    {
        datas->restricao_date1 = aux->restricao_date2;
        datas->restricao_date1 = datas->restricao_date2;
        datas->restricao_date2 = aux->restricao_date1;
    }
    return datas;
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

void restricao_min(dados_t **right, dados_t **left, int *flag, settings_t* anosemana)
{
    dados_t *aux;

    anosemana->restricao_n *= 1000;
    if ((*right)->population < anosemana->restricao_n)
    {
        aux = (*right);
        (*left)->next = remove_do_inicio(aux);
        (*flag) = 1;
        (*right) = aux;
    }
}

void restricao_max(dados_t **right, dados_t **left, int *flag, settings_t* anosemana)
{
    dados_t *aux;

    anosemana->restricao_n *= 1000;
    if ((*right)->population > anosemana->restricao_n)
    {
        aux = (*right);
        (*left)->next = remove_do_inicio(aux);
        (*flag) = 1;
        (*right) = aux;
    }
}
//--------------------------------------------------------------------------------------------
void restricao_date(dados_t **right, dados_t **left, int *flag, settings_t* anosemana)
{
    dados_t *aux;


    if ((*right)->year_week != anosemana->restricao_date1)
    {
        aux = (*right);
        (*left)->next = remove_do_inicio(aux);
        (*flag) = 1;
        (*right) = aux;
    }
}

void restricao_dates(dados_t **right, dados_t **left, int *flag, settings_t* anosemana)
{
    dados_t *aux;

    anosemana = trocadatas(anosemana);

    if ((*right)->year_week > anosemana->restricao_date1 && (*right)->year_week < anosemana->restricao_date2)
    {
        aux = (*right);
        (*left)->next = remove_do_inicio(aux);
        (*flag) = 1;
        (*right) = aux;
    }
}
//--------------------------------------------------------------------------------------


dados_t *restricao_lista(dados_t *root, settings_t* anosemana)
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
                //restricao_min(&right, &left, &flag, anosemana);
                restricao_max(&right, &left, &flag, anosemana);
                restricao_date(&right, &left, &flag, anosemana);
                restricao_dates(&right, &left, &flag, anosemana);
                left = right;
                if (right->next != NULL)
                    right = right->next;
            }
        }
    }
    root = head->next;
    return root;
}

/** \brief selecionar, para cada pais, a semana com mais infetados
 *
 * \param atual dados_t*
 * \param comparacao dados_t*
 * \return int: 0,1,2
 *
 */
int selecao_inf(dados_t *atual, dados_t *comparacao)
{
    //se os paises forem diferentes
    if (comparacao != NULL && strcmp(atual->country_code, comparacao->country_code) != 0)
        return 0;

    //se o elemento atual nao tiver o indicador "cases"
    if (strcmp(atual->indicator, "cases") != 0)
        return 2; //"discartar" o elemento atual

    //se o elemento comparacao for nulo, ignoramo-lo
    if (comparacao == NULL)
        return 0;

    //se o elemento de comparacao nao tiver o indicador "cases"
    if (strcmp(comparacao->indicator, "cases") != 0)
        return 1; //"discartar" o elemento de comparacao

    if (atual->weekly_count > comparacao->weekly_count)
        return 1; //discarta o elemento comparacao
    else
        return 2; //discarta o elemento atual
}

//to do funcao que compara datas para, em caso de empate, escolher a semana mais recente

// pegar no parseWeek e ver qual dos numeros é mais (pequeno? ou maior? dunno)

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

dados_t *ordenar_lista(dados_t *root, settings_t *anosemana)
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
                ordenacao_pop(&right, &left, &flag);
                //ordenacao_alfa(&right, &left, &flag);

                left = right;
                if (right->next != NULL)
                    right = right->next;
            }
        }
    }
    root = head->next;
    return root;
}

void imprime_lista(lista_t *lista)
{
    dados_t *curr = lista->first;
    while (curr != NULL)
    {
        printf("%s - %s - %s - %d - %s - %d - %d-%d - %f - %d\n", curr->country, curr->country_code, curr->continent, curr->population, curr->indicator,
               curr->weekly_count, curr->year_week->year, curr->year_week->week, curr->rate_14_day, curr->cumulative_count);
        curr = curr->next;
    }
}

void cria_ficheiro(dados_t *root)
{
    FILE *fp;
    dados_t *curr;
    fp = fopen("teste.csv", "w");

    curr = root;
    while (curr->next != NULL)
    {
        fprintf(fp, "%s, %s, %s, %d, %s, %d, %d-%d, %f, %d\n", curr->country, curr->country_code, curr->continent, curr->population, curr->indicator,
                curr->weekly_count, curr->year_week->year, curr->year_week->week, curr->rate_14_day, curr->cumulative_count);
        curr = curr->next;
    }
    fclose(fp);
}

void apagar_elemento_lista(lista_t *lista, dados_t *elemento)
{

    if (elemento->prev == NULL)
    {
        lista->first = elemento->next;
    }
    else
    {
        elemento->prev->next = elemento->next;
    }

    if (elemento->next == NULL)
    {
        lista->last = elemento->prev;
    }
    else
    {
        elemento->next->prev = elemento->prev;
    }
    destruir_dados(elemento);
}

void liberta_lista(lista_t *lista)
{
    dados_t *curr;
    dados_t *next = lista->first;

    while (next != NULL)
    {
        curr = next;
        next = next->next;
        destruir_dados(curr);
    }
    free(lista);
}

void destruir_dados(dados_t *dados)
{
    free(dados->year_week);
    free(dados);
}

int criterio_selecao(settings_t *settings, dados_t *atual, dados_t *comparacao)
{
    if (settings->criterio_sel == D_INF)
        return selecao_inf(atual, comparacao);
    // else if (...)
    return 0;
}

void selecionar(settings_t *settings, lista_t *lista)
{
    dados_t *el_atual = lista->first;
    dados_t *el_comparacao;

    while (el_atual != NULL)
    {
        dados_t *aux_atual = el_atual;
        el_atual = el_atual->next;
        el_comparacao = aux_atual->next;

        while (el_comparacao != NULL)
        {
            int result = criterio_selecao(settings, aux_atual, el_comparacao); //0-ignorar, 1-apagar comparacao, 2-apagar atual
            dados_t *aux_comparacao = el_comparacao;
            el_comparacao = el_comparacao->next;
            if (result == 1)
            {
                if (aux_comparacao == el_atual)
                    el_atual = el_atual->next;
                apagar_elemento_lista(lista, aux_comparacao);
            }
            else if (result == 2)
            {
                if (aux_atual == el_atual)
                    el_atual = el_atual->next;
                apagar_elemento_lista(lista, aux_atual);
                aux_atual = aux_comparacao;
            }
        }

        if (criterio_selecao(settings, aux_atual, NULL) == 2)
        {
            if (aux_atual == el_atual)
                el_atual = el_atual->next;
            apagar_elemento_lista(lista, aux_atual);
        }
    }
}

void erros_ficheiro(lista_t *lista)
{
    dados_t *head = lista->first;

    while (head != NULL)
    {
        if(head->population < 0)
        {
            printf("populacao impossivel");
            exit(-1);
        }
        else if(head->cumulative_count < 0)
        {
            printf("valor impossivel");
            exit(-1);
        }
        else if(head->rate_14_day < 0)
        {
            printf("valor impossivel");
            exit(-1);
        }
        else if(head->weekly_count < 0)
        {
            printf("valor impossivel");
            exit(-1);
        }
        else if(head->year_week->week <= 0 && head->year_week->week > 52 && head->year_week->year < 0)
        {
            printf("valor impossivel");
            exit(-1);
        }

        head = head->next;
    }
}

void utilizacao()
{
    printf("\t\tPROJETO FINAL\n\n");
    printf("opcoes validas:\n");
    printf("[-L all ou \"continente\"]\t  le o ficheiro inteiro ou dos paises em ralacao ao continente\n");
    printf("[-S]\t\t\t\t  Ordenacao dos dados\n");
    printf("[-D]\t\t\t\t  Selecao dos dados\n");
    printf("[-P]\t\t\t\t  Restricicao dos dados\n");
    printf("[-i]\t\t\t\t  Leitura do ficheiro\n");
    printf("[-o]\t\t\t\t  Escrita do ficheiro\n");
}

int main(int argc, char *argv[])
{
    settings_t *settings = malloc(sizeof(settings_t));
    int opt;
    opterr = 0;
    char yearweek[8], yearweek2[8];
    char criterio_L[20];
    char criterio_S[20];
    char criterio_D[20];
    char criterio_P[20];
    char criterio_FILE[100];
    char criterio_WRITE[100];

    settings->criterio_leitura = L_ALL;
    settings->criterio_ord = S_ALFA;
    settings->criterio_sel = D_NONE;
    settings->criterio_res = D_NONE;

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
                strcpy(yearweek, argv[optind]);
                optind++;
                settings->ord_date = parseYearWeek(yearweek);
            }

            else if (strcmp(criterio_S, "dea") == 0)
            {
                settings->criterio_ord = S_DEA;
                strcpy(yearweek, argv[optind]);
                optind++;
                settings->ord_date = parseYearWeek(yearweek);
            }
            break;
        case 'D':
            sscanf(optarg, "%s", criterio_D);
            if (strcmp(criterio_D, "inf") == 0)
                settings->criterio_sel = D_INF;

            else if (strcmp(criterio_D, "dea") == 0)
                settings->criterio_sel = D_DEA;

            else if (strcmp(criterio_D, "racioinf") == 0)
                settings->criterio_sel = D_RACIOINF;

            else if (strcmp(criterio_D, "raciodea") == 0)
                settings->criterio_sel = D_RACIODEA;
            break;
        case 'P':
            sscanf(optarg, "%s", criterio_P);
            if (strcmp(criterio_P, "min") == 0)
            {
                settings->criterio_res = P_MIN;
                settings->restricao_n = atoi(argv[optind]);
                optind++;
            }

            else if (strcmp(criterio_P, "max") == 0)
            {
                settings->criterio_res = P_MAX;
                settings->restricao_n = atoi(argv[optind]);
                optind++;
            }

            else if (strcmp(criterio_P, "date") == 0)
            {
                settings->criterio_res = P_DATE;
                strcpy(yearweek, argv[optind]);
                optind++;
                settings->restricao_date1 = parseYearWeek(yearweek);
            }
            else if (strcmp(criterio_P, "dates") == 0)
            {
                settings->criterio_res = P_DATES;
                strcpy(yearweek, argv[optind]);
                optind++;
                strcpy(yearweek2, argv[optind]);
                optind++;
                settings->restricao_date1 = parseYearWeek(yearweek);
                settings->restricao_date2 = parseYearWeek(yearweek2);
            }
            break;
        case 'i':
            sscanf(optarg, "%s", criterio_FILE);
            break;
        case 'o':
            sscanf(optarg, "%s", criterio_WRITE);
            break;
        default:
            utilizacao(argv[0]);
            return EXIT_FAILURE;
            break;
        }
    }

    lista_t *root_principal = ler_ficheiro();
    if (settings->criterio_sel != D_NONE)
        selecionar(settings, root_principal);
    //root_principal = selecao_lista(root_principal);
    //root_principal = ordenar_lista(root_principal, settings);
    //root_principal = restricao_lista(root_principal, settings);
    cria_ficheiro(root_principal);
    imprime_lista(root_principal);
    liberta_lista(root_principal);
    return 0;
}
