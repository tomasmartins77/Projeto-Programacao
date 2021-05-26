#include "headers.h"

int main(int argc, char *argv[])
{
    settings_t *settings = init_settings();
    int opt, binario = 0;
    opterr = 0;
    char yearweek[8], yearweek2[8];
    char criterio_L[20];
    char criterio_S[20];
    char criterio_D[20];
    char criterio_P[20];
    char criterio_FILE[100];
    char criterio_WRITE[100];

    while ((opt = getopt(argc, argv, "L:S:D:P:i:o:")) != -1) // ve ate ao final da linha de comando
    {
        switch (opt)
        {
        case 'L': // verifica se le o ficheiro inteiro ou apenas num certo continente
            sscanf(optarg, "%s", criterio_L);
            if (strcmp(criterio_L, "all") == 0)
                settings->criterio_leitura = L_ALL; // ficheiro inteiro
            else
            {
                settings->criterio_leitura = L_CONTINENTE;
                settings->leitura_continente = (char *)malloc(sizeof(char) * (strlen(criterio_L) + 1));
                if (settings->leitura_continente == NULL)
                {
                    fprintf(stderr, "Erro a alocar memoria para leitura continente");
                    exit(EXIT_FAILURE);
                }
                strcpy(settings->leitura_continente, criterio_L); // continente especifico
            }
            break;
        case 'S':
            sscanf(optarg, "%s", criterio_S);
            if (strcmp(criterio_S, "alfa") == 0)
                settings->criterio_ord = S_ALFA;     // ordenacao por ordem alfabetica
            else if (strcmp(criterio_S, "pop") == 0) // ordenacao por populacao
                settings->criterio_ord = S_POP;
            else if (strcmp(criterio_S, "inf") == 0) // ordenacao por ordem decrescente de casos numa determinada data
            {
                settings->criterio_ord = S_INF;
                strcpy(yearweek, argv[optind]);
                optind++;
                settings->ord_date = parseYearWeek(yearweek);
            }
            else if (strcmp(criterio_S, "dea") == 0) //ordenacao por ordem decrescente de mortes numa determinada data
            {
                settings->criterio_ord = S_DEA;
                strcpy(yearweek, argv[optind]);
                optind++;
                settings->ord_date = parseYearWeek(yearweek);
            }
            break;
        case 'D': // selecao
            sscanf(optarg, "%s", criterio_D);
            if (strcmp(criterio_D, "inf") == 0) // semana com mais infetados
                settings->criterio_sel = D_INF;
            else if (strcmp(criterio_D, "dea") == 0) // semana com mais mortes
                settings->criterio_sel = D_DEA;
            else if (strcmp(criterio_D, "racioinf") == 0) // semana com maior racio de infetados
                settings->criterio_sel = D_RACIOINF;
            else if (strcmp(criterio_D, "raciodea") == 0) // semana com maior racio de mortes
                settings->criterio_sel = D_RACIODEA;
            break;
        case 'P': // restricao
            sscanf(optarg, "%s", criterio_P);
            if (strcmp(criterio_P, "min") == 0) // apenas dados de países com mais de n mil habitantes
            {
                settings->criterio_res = P_MIN;
                settings->restricao_nmin = atoi(argv[optind]);
                optind++;
            }
            else if (strcmp(criterio_P, "max") == 0) // apenas dados de países com menos de n mil habitantes
            {
                settings->criterio_res = P_MAX;
                settings->restricao_nmax = atoi(argv[optind]);
                optind++;
            }
            else if (strcmp(criterio_P, "date") == 0) // apenas dados relativos à semana indicada
            {
                settings->criterio_res = P_DATE;
                strcpy(yearweek, argv[optind]);
                optind++;
                settings->restricao_date1 = parseYearWeek(yearweek);
            }
            else if (strcmp(criterio_P, "dates") == 0) // apenas dados entre as semanas indicadas
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
        case 'i': // leitura do ficheiro
            sscanf(optarg, "%s", criterio_FILE);
            settings->criterio_file = (char *)malloc(sizeof(char) * (strlen(criterio_FILE) + 1));
            if (settings->criterio_file == NULL)
            {
                fprintf(stderr, "Erro a alocar memoria para criterio file");
                exit(EXIT_FAILURE);
            }
            strcpy(settings->criterio_file, criterio_FILE);
            break;
        case 'o': // escrita do ficheiro
            sscanf(optarg, "%s", criterio_WRITE);
            settings->criterio_write = (char *)malloc(sizeof(char) * (strlen(criterio_WRITE) + 1));
            if (settings->criterio_write == NULL)
            {
                fprintf(stderr, "Erro a alocar memoria para criterio write");
                exit(EXIT_FAILURE);
            }
            strcpy(settings->criterio_write, criterio_WRITE);
            break;
        default:
            utilizacao(argv[0]);
            return EXIT_FAILURE;
        }
    }

    settings = verifica_tipo_ficheiro(settings, &binario);
    lista_t *root_principal = ler_ficheiro(settings);
    //erros_ficheiro(root_principal);
    if (binario == 0) // e binario ou nao
    {
        if (settings->criterio_sel != D_NONE)
            selecionar(settings, root_principal);
        if (settings->criterio_res != P_NONE)
            restricao_lista(root_principal, settings);

        root_principal = ordenar_lista(root_principal, settings);
    }
    cria_ficheiro(root_principal, settings);
    liberta_settings(settings);
    liberta_lista(root_principal, destruir_pais);
    return 0;
}

/** \brief
 *
 * \return settings_t*
 *
 */
settings_t *init_settings()
{
    settings_t *settings = (settings_t *)malloc(sizeof(settings_t));
    if (settings == NULL)
    {
        fprintf(stderr, "Erro a alocar memoria a settings");
        exit(EXIT_FAILURE);
    }
    settings->criterio_leitura = L_ALL; //predefinicao all
    settings->criterio_ord = S_ALFA;    // predefinicao alfa
    settings->criterio_sel = D_NONE;
    settings->criterio_res = P_NONE;
    settings->criterio_file = NULL;
    settings->criterio_write = NULL;
    settings->leitura_continente = NULL;
    settings->ord_date = NULL;
    settings->restricao_date1 = NULL;
    settings->restricao_date2 = NULL;
    settings->restricao_nmax = 0;
    settings->restricao_nmin = 0;
    settings->tipo_escrita = NULL;
    settings->tipo_ficheiro = NULL;

    return settings;
}

/** \brief
 *
 * \param dados char*
 * \return yearWeek_t*
 *
 */
yearWeek_t *parseYearWeek(char *dados)
{
    yearWeek_t *yearWeek = malloc(sizeof(yearWeek_t));
    if (yearWeek == NULL)
    {
        fprintf(stderr, "Erro a alocar memoria a yearweek");
        exit(EXIT_FAILURE);
    }
    dados[4] = '\0';
    yearWeek->year = atoi(dados);     //int
    yearWeek->week = atoi(dados + 5); //int

    return yearWeek;
}

/** \brief troca as datas
 *
 * \param datas settings_t* data a ser trocada
 * \return settings_t* datas data trocada
 *
 */
settings_t *troca_datas(settings_t *datas)
{
    yearWeek_t *aux;
    aux = datas->restricao_date1;
    datas->restricao_date1 = datas->restricao_date2;
    datas->restricao_date2 = aux;
    return datas;
}

/** \brief verifica se a data precisa ser trocada
 *
 * \param datas settings_t* data a ser verificada
 * \return settings_t* data trocada
 *
 */
settings_t *verifica_datas(settings_t *datas)
{
    if (datas->restricao_date1->year > datas->restricao_date2->year)
    {
        datas = troca_datas(datas);
    }
    else if (datas->restricao_date1->year == datas->restricao_date2->year)
    {
        if (datas->restricao_date1->week > datas->restricao_date2->week)
        {
            datas = troca_datas(datas);
        }
    }
    return datas;
}

/** \brief
 *
 * \param lista lista_t*
 * \param yearWeek yearWeek_t*
 * \param indicator char*
 * \return dados_t*
 *
 */
dados_t *obter_dados_semana(lista_t *lista, yearWeek_t *yearWeek, char *indicator)
{
    node_t *aux = lista->first;

    dados_t *aux_dados;

    while (aux != NULL)
    {
        aux_dados = aux->value;

        if (aux_dados->year_week->year == yearWeek->year && aux_dados->year_week->week == yearWeek->week && strcmp(aux_dados->indicator, indicator) == 0)
            return aux_dados;
        aux = aux->next;
    }
    return NULL;
}

/** \brief
 *
 * \param settings settings_t*
 * \return void
 *
 */
void liberta_settings(settings_t *settings)
{
    free(settings->criterio_file);
    free(settings->criterio_write);
    free(settings->leitura_continente);
    if (settings->ord_date != NULL)
        free(settings->ord_date);
    if (settings->restricao_date1 != NULL)
        free(settings->restricao_date1);
    if (settings->restricao_date2 != NULL)
        free(settings->restricao_date2);
    free(settings);
}

/** \brief verifica se existe algum erro de escrita no ficheiro lido
 *
 * \param lista lista_t* lista com o ficheiro inteiro
 * \return void
 *
 */
/*
void erros_ficheiro(lista_t *lista)
{
   dados_t *head = lista->first;
   char i;

   while (head != NULL)
   {
       if (head->population < 0 || head->cumulative_count < 0 || head->rate_14_day < 0 || head->weekly_count < 0)
       {
           fprintf(stderr, "dados de ficheiros com numero(s) negativo(s)");
           exit(EXIT_FAILURE);
       }
       else if (head->year_week->week <= 0 || head->year_week->week > 53 || head->year_week->year < 0)
       {
           fprintf(stderr, "dados de ficheiros com semana(s) impossivei(s)");
           exit(EXIT_FAILURE);
       }
       for (i = '0'; i <= '9'; i++)
       {
           if (strchr(head->country, i) != NULL || strchr(head->continent, i) != NULL || strchr(head->country_code, i) != NULL || strchr(head->indicator, i) != NULL)
           {
               fprintf(stderr, "dados de ficheiros com numero(s) em nome(s)");
               exit(EXIT_FAILURE);
           }
       }
       head = head->next;
   }
}*/

/** \brief verifica se o ficheiro e .dat ou .csv
 *
 * \param settings settings_t*
 * \param binario int* flag para saber se o ficheiro e binario
 * \return settings_t*
 *
 */
settings_t *verifica_tipo_ficheiro(settings_t *settings, int *binario)
{
    char csv[4] = "csv";
    char dat[4] = "dat";
    if (strstr(settings->criterio_file, csv) != 0)
    {
        settings->tipo_ficheiro = "r";
    }
    else if (strstr(settings->criterio_file, dat) != 0)
    {
        settings->tipo_ficheiro = "rb";
        (*binario) = 1;
    }
    if (strstr(settings->criterio_write, csv) != 0)
    {
        settings->tipo_escrita = "w";
    }
    else if (strstr(settings->criterio_write, dat) != 0)
    {
        settings->tipo_escrita = "wb";
    }
    return settings;
}

/** \brief menu de utilizacao
 *
 * \return void
 *
 */
void utilizacao()
{
    printf("\t\tPROJETO FINAL\n\n");
    printf("opcoes validas:\n");
    printf("[-L all ou \"continente\"]\t  le o ficheiro inteiro ou dos paises em ralacao ao continente\n");
    printf("[-S]\t\t\t\t  Ordenacao dos dados\n");
    printf("[-D]\t\t\t\t  Selecao dos dados\n");
    printf("[-P]\t\t\t\t  Restricao dos dados\n");
    printf("[-i]\t\t\t\t  Leitura do ficheiro\n");
    printf("[-o]\t\t\t\t  Escrita do ficheiro\n");
}
