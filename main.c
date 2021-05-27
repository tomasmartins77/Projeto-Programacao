#include "headers.h"

int main(int argc, char *argv[])
{
    settings_t *settings = init_settings();
    int binario = 0;

    argumentos(argc, argv, settings); // argumentos da linha de comandos

    settings = verifica_tipo_ficheiro(settings, &binario); // verifica se o ficheiro e .dat ou .csv

    lista_t *root_principal = ler_ficheiro(settings); // cria a lista inteira ou apenas com um continente

    erros_ficheiro(root_principal, settings); // verifica se o ficheiro possui erros a partir da lista criada

    if (binario == 0) // e binario ou nao
    {
        if (settings->criterio_sel != D_NONE)
            selecionar(settings, root_principal); // selecao
        if (settings->criterio_res != P_NONE)
            restricao_lista(root_principal, settings); // restricao

        root_principal = ordenar_lista(root_principal, settings); // ordenacao
    }
    cria_ficheiro(root_principal, settings); // cria ficheiro na extensao pretendida
    //liberta toda a memoria
    liberta_settings(settings);
    liberta_lista(root_principal, destruir_pais);
    return 0;
}

void argumentos(int argc, char *argv[], settings_t *settings)
{
    int opt;
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
            if(!verifica_L(criterio_L))
            {
                fprintf(stderr, "continente invalido");
                exit(EXIT_FAILURE);
            }
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
        case 'S'://ordenacao
            sscanf(optarg, "%s", criterio_S);
            if(!verfica_S(criterio_S))
            {
                fprintf(stderr, "ordenacao indisponivel");
                exit(EXIT_FAILURE);
            }
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
            if(!verifica_D(criterio_D))
            {
                fprintf(stderr, "selecao indisponivel");
                exit(EXIT_FAILURE);
            }
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
            if(!verifica_P(criterio_P))
            {
                fprintf(stderr, "restricao indisponivel");
                exit(EXIT_FAILURE);
            }
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
            exit(EXIT_FAILURE);
        }
    }
}

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

yearWeek_t *parseYearWeek(char *dados)
{
    yearWeek_t *yearWeek = malloc(sizeof(yearWeek_t));
    if (yearWeek == NULL)
    {
        fprintf(stderr, "Erro a alocar memoria a yearweek");
        exit(EXIT_FAILURE);
    }
    dados[4] = '\0'; // '-'
    yearWeek->year = atoi(dados);     //int
    yearWeek->week = atoi(dados + 5); //int

    return yearWeek;
}

settings_t *troca_datas(settings_t *datas)
{
    yearWeek_t *aux;
    aux = datas->restricao_date1;
    datas->restricao_date1 = datas->restricao_date2;
    datas->restricao_date2 = aux;
    return datas;
}

settings_t *verifica_datas(settings_t *datas)
{
    if (datas->restricao_date1->year > datas->restricao_date2->year)//ano 1 e maior que o 2
    {
        datas = troca_datas(datas);
    }
    else if (datas->restricao_date1->year == datas->restricao_date2->year)//mesmo ano
    {
        if (datas->restricao_date1->week > datas->restricao_date2->week)//mas semana 1 maior que a 2
        {
            datas = troca_datas(datas);
        }
    }
    //se nao entrar em nenhum, as datas nao precisam ser trocadas
    return datas;
}

void liberta_settings(settings_t *settings)
{
    free(settings->criterio_file);
    free(settings->criterio_write);
    free(settings->leitura_continente);
    if (settings->ord_date != NULL) //se nao forem utilizados, nao precisam dar free
        free(settings->ord_date);
    if (settings->restricao_date1 != NULL)
        free(settings->restricao_date1);
    if (settings->restricao_date2 != NULL)
        free(settings->restricao_date2);
    free(settings);
}

int erro_letra_em_numero(char *numero, int contador)
{
    char i;

    if(contador == 4 || (contador >= 6 && contador <= 9)) // colunas onde existem numeros
    {
        for(i = 'A'; i <= 'z'; i++)
        {
            if(strchr(numero, i) != NULL) // se existe alguma letra no ficheiro
                return 0;
        }
    }
    return 1;
}

void erros_ficheiro(lista_t *lista, settings_t *settings)
{
    char i;
    node_t *curr = lista->first;

    while (curr != NULL)
    {
        pais_t *aux_pais = curr->value;

        node_t *aux_dados = aux_pais->dados->first;

        if (aux_pais->population < 0)
        {
            fprintf(stderr, "nao existem colunas suficientes para um ficheiro valido");
            liberta_settings(settings);
            liberta_lista(lista, destruir_pais);
            exit(EXIT_FAILURE);
        }
        for (i = '0'; i <= '9'; i++)
        {
            if (strchr(aux_pais->country, i) != NULL || strchr(aux_pais->continent, i) != NULL || strchr(aux_pais->country_code, i) != NULL)
            {
                fprintf(stderr, "nao existem colunas suficientes para um ficheiro valido");
                liberta_settings(settings);
                liberta_lista(lista, destruir_pais);
                exit(EXIT_FAILURE);
            }
        }

        while (aux_dados != NULL)
        {
            dados_t *dados = aux_dados->value;

            if (dados->cumulative_count < 0 || dados->rate_14_day < 0 || dados->weekly_count < 0)
            {
                fprintf(stderr, "nao existem colunas suficientes para um ficheiro valido");
                liberta_settings(settings);
                liberta_lista(lista, destruir_pais);
                exit(EXIT_FAILURE);
            }
            else if (dados->year_week->week <= 0 || dados->year_week->week > 53 || dados->year_week->year < 0)
            {
                fprintf(stderr, "nao existem colunas suficientes para um ficheiro valido");
                liberta_settings(settings);
                liberta_lista(lista, destruir_pais);
                exit(EXIT_FAILURE);
            }
            for (i = '0'; i <= '9'; i++)
            {
                if (strchr(dados->indicator, i) != NULL)
                {
                    fprintf(stderr, "nao existem colunas suficientes para um ficheiro valido");
                    liberta_settings(settings);
                    liberta_lista(lista, destruir_pais);
                    exit(EXIT_FAILURE);
                }
            }
            aux_dados = aux_dados->next;
        }
        curr = curr->next;
    }
}

int verifica_L(char* continente)
{
    if(strcmp(continente, "all") == 0 || strcmp(continente, "Europe") == 0 || strcmp(continente, "Africa") == 0 || strcmp(continente, "Asia") == 0 || strcmp(continente, "America") == 0 || strcmp(continente, "Oceania") == 0)
        return 1;
    return 0;
}

int verfica_S(char *ordenacao)
{
    if(strcmp(ordenacao, "alfa") == 0 || strcmp(ordenacao, "pop") == 0 || strcmp(ordenacao, "inf") == 0 || strcmp(ordenacao, "dea") == 0)
        return 1;
    return 0;
}

int verifica_D(char *selecao)
{
    if(strcmp(selecao, "inf") == 0 || strcmp(selecao, "dea") == 0 || strcmp(selecao, "racioinf") == 0 || strcmp(selecao, "raciodea") == 0)
        return 1;
    return 0;
}

int verifica_P(char *restricao)
{
    if(strcmp(restricao, "min") == 0 || strcmp(restricao, "max") == 0 || strcmp(restricao, "date") == 0 || strcmp(restricao, "dates") == 0)
        return 1;
    return 0;
}

settings_t *verifica_tipo_ficheiro(settings_t *settings, int *binario)
{
    char csv[4] = "csv";
    char dat[4] = "dat";
    if (strstr(settings->criterio_file, csv) != 0)//se for ficheiro csv
    {
        settings->tipo_ficheiro = "r";
    }
    else if (strstr(settings->criterio_file, dat) != 0)//se for ficheiro dat
    {
        settings->tipo_ficheiro = "rb";
        (*binario) = 1;
    }
    if (strstr(settings->criterio_write, csv) != 0)//se criar ficheiro csv
    {
        settings->tipo_escrita = "w";
    }
    else if (strstr(settings->criterio_write, dat) != 0)//se criar ficheiro dat
    {
        settings->tipo_escrita = "wb";
    }
    else
    {
        fprintf(stderr, "extensao indisponivel");
        exit(EXIT_FAILURE);
    }
    return settings;
}

void utilizacao()
{
    printf("\t\tPROJETO FINAL\n\n");
    printf("opcoes validas:\n");
    printf("[-L all ou \"continente\"]\t\t\t\t  le o ficheiro inteiro ou dos paises em relacao ao continente\n");
    printf("[-S (alfa, pop, inf yyyy-ww, dea yyyy-ww)]\t\t  Ordenacao dos dados\n");
    printf("[-D (inf, dea, racioinf, raciodea)]\t\t\t  Selecao dos dados\n");
    printf("[-P (min n, max n, date yyyy-ww, dates yyyy-ww yyyy-ww)]  Restricao dos dados\n");
    printf("[-i \"nomedoficheiro.extensao\"]\t\t\t\t  Leitura do ficheiro\n");
    printf("[-o \"nomedoficheiro.extensao\"]\t\t\t\t  Escrita do ficheiro\n");
}
