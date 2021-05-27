#include "headers.h"

int main(int argc, char *argv[])
{
    settings_t *settings = init_settings();
    int binario = 0;

    argumentos(argc, argv, settings); // argumentos da linha de comandos

    erros_ficheiros_argumentos(settings); // verifica se -i e -o estao especificados

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
    fprintf(stdout, "Ficheiro %s criado com sucesso", settings->criterio_write);
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
            verifica_L(criterio_L);// verifica se e uma opcao valida
            if (strcmp(criterio_L, "all") == 0)
            {
                verifica_argumento(argv[optind]); //verifica se possui valores
                settings->criterio_leitura = L_ALL; // ficheiro inteiro
            }
            else
            {
                verifica_argumento(argv[optind]); //verifica se possui valores
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
            verfica_S(criterio_S);// verifica se e uma opcao valida
            if (strcmp(criterio_S, "alfa") == 0)
            {
                verifica_argumento(argv[optind]); //verifica se possui valores
                settings->criterio_ord = S_ALFA;     // ordenacao por ordem alfabetica
            }
            else if (strcmp(criterio_S, "pop") == 0) // ordenacao por populacao
            {
                verifica_argumento(argv[optind]); //verifica se possui valores
                settings->criterio_ord = S_POP;
            }
            else if (strcmp(criterio_S, "inf") == 0) // ordenacao por ordem decrescente de casos numa determinada data
            {
                erro_argumento(argv[optind]); // verifica se nao possui valores

                settings->criterio_ord = S_INF;
                strcpy(yearweek, argv[optind]);
                optind++;
                settings->ord_date = parseYearWeek(yearweek);
            }
            else if (strcmp(criterio_S, "dea") == 0) //ordenacao por ordem decrescente de mortes numa determinada data
            {
                erro_argumento(argv[optind]); // verifica se nao possui valores

                settings->criterio_ord = S_DEA;
                strcpy(yearweek, argv[optind]);
                optind++;
                settings->ord_date = parseYearWeek(yearweek);
            }
            break;
        case 'D': // selecao
            sscanf(optarg, "%s", criterio_D);
            verifica_D(criterio_D); // verifica se e uma opcao valida
            if (strcmp(criterio_D, "inf") == 0) // semana com mais infetados
            {
                verifica_argumento(argv[optind]); //verifica se possui valores
                settings->criterio_sel = D_INF;
            }
            else if (strcmp(criterio_D, "dea") == 0) // semana com mais mortes
            {
                verifica_argumento(argv[optind]); //verifica se possui valores
                settings->criterio_sel = D_DEA;
            }
            else if (strcmp(criterio_D, "racioinf") == 0) // semana com maior racio de infetados
            {
                verifica_argumento(argv[optind]); //verifica se possui valores
                settings->criterio_sel = D_RACIOINF;
            }
            else if (strcmp(criterio_D, "raciodea") == 0) // semana com maior racio de mortes
            {
                verifica_argumento(argv[optind]); //verifica se possui valores
                settings->criterio_sel = D_RACIODEA;
            }
            break;
        case 'P': // restricao
            sscanf(optarg, "%s", criterio_P);
            verifica_P(criterio_P);// verifica se e uma opcao valida
            if (strcmp(criterio_P, "min") == 0) // apenas dados de países com mais de n mil habitantes
            {
                erro_argumento(argv[optind]); // verifica se nao possui valores

                settings->criterio_res = P_MIN;
                settings->restricao_nmin = atoi(argv[optind]);
                optind++;
            }
            else if (strcmp(criterio_P, "max") == 0) // apenas dados de países com menos de n mil habitantes
            {
                erro_argumento(argv[optind]); // verifica se nao possui valores

                settings->criterio_res = P_MAX;
                settings->restricao_nmax = atoi(argv[optind]);
                optind++;
            }
            else if (strcmp(criterio_P, "date") == 0) // apenas dados relativos à semana indicada
            {
                erro_argumento(argv[optind]);  // verifica se nao possui valores

                settings->criterio_res = P_DATE;
                strcpy(yearweek, argv[optind]);
                optind++;
                settings->restricao_date1 = parseYearWeek(yearweek);
            }
            else if (strcmp(criterio_P, "dates") == 0) // apenas dados entre as semanas indicadas
            {
                erro_argumento(argv[optind]);  // verifica se nao possui valores

                settings->criterio_res = P_DATES;
                strcpy(yearweek, argv[optind]);
                optind++;

                erro_argumento(argv[optind]); // verifica se nao possui valores

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

        if (aux_pais->population < 0) // populacao menor que 0
        {
            fprintf(stderr, "pais com populacao negativa");
            liberta_settings(settings);
            liberta_lista(lista, destruir_pais);
            exit(EXIT_FAILURE);
        }
        for (i = '0'; i <= '9'; i++)// ve cada numero nas palavras
        {
            if (strchr(aux_pais->country, i) != NULL || strchr(aux_pais->continent, i) != NULL || strchr(aux_pais->country_code, i) != NULL)
            {
                fprintf(stderr, "valor so com letras possui numeros"); // se algum valor possui numeros
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
                fprintf(stderr, "dados variaveis com numeros negativos"); // valores negativos
                liberta_settings(settings);
                liberta_lista(lista, destruir_pais);
                exit(EXIT_FAILURE);
            }
            else if (dados->year_week->week <= 0 || dados->year_week->week > 53 || dados->year_week->year < 0)
            {
                fprintf(stderr, "semanas impossiveis"); // semanas impossiveis
                liberta_settings(settings);
                liberta_lista(lista, destruir_pais);
                exit(EXIT_FAILURE);
            }
            for (i = '0'; i <= '9'; i++)
            {
                if (strchr(dados->indicator, i) != NULL) // ve se o indicador tem numeros
                {
                    fprintf(stderr, "indicador de cases ou deaths possui numeros");
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

void verifica_L(char* continente)
{
    if(strcmp(continente, "all") == 0 || strcmp(continente, "Europe") == 0 || strcmp(continente, "Africa") == 0 || strcmp(continente, "Asia") == 0 || strcmp(continente, "America") == 0 || strcmp(continente, "Oceania") == 0)
    {
        return; // se for opcao correta, retorna
    }
    fprintf(stderr, "continente invalido");
    exit(EXIT_FAILURE);
}

void verfica_S(char *ordenacao)
{
    if(strcmp(ordenacao, "alfa") == 0 || strcmp(ordenacao, "pop") == 0 || strcmp(ordenacao, "inf") == 0 || strcmp(ordenacao, "dea") == 0)
    {
        return;// se for opcao correta, retorna
    }
    fprintf(stderr, "ordenacao indisponivel");
    exit(EXIT_FAILURE);
}

void verifica_D(char *selecao)
{
    if(strcmp(selecao, "inf") == 0 || strcmp(selecao, "dea") == 0 || strcmp(selecao, "racioinf") == 0 || strcmp(selecao, "raciodea") == 0)
    {
        return; // se for opcao correta, retorna
    }
    fprintf(stderr, "selecao indisponivel");
    exit(EXIT_FAILURE);
}

void verifica_P(char *restricao)
{
    if(strcmp(restricao, "min") == 0 || strcmp(restricao, "max") == 0 || strcmp(restricao, "date") == 0 || strcmp(restricao, "dates") == 0)
    {
        return; // se for opcao correta, retorna
    }
    fprintf(stderr, "restricao indisponivel");
    exit(EXIT_FAILURE);
}

void erro_argumento(char *word)
{
    if(!strcmp(word,"-L") || !strcmp(word, "-S") || !strcmp(word,"-D")  || !strcmp(word,"-P") || !strcmp(word,"-i")  || !strcmp(word,"-o"))
    {
        fprintf(stderr, "opcao precisa de um valor"); // se nao tiver um numero ou data a seguir a uma opcao que precisa
        exit(EXIT_FAILURE);
    }
}

void verifica_argumento(char *word)
{
    if(!strcmp(word,"-L") || !strcmp(word, "-S") || !strcmp(word,"-D")  || !strcmp(word,"-P") || !strcmp(word,"-i")  || !strcmp(word,"-o"))
    {
        return; // se tiver um numero ou data onde nao pode haver
    }
    fprintf(stderr, "opcao nao permite valores");
    exit(EXIT_FAILURE);
}

void erros_ficheiros_argumentos(settings_t *settings)
{
    if(settings->criterio_file == NULL) // se tem -i
    {
        fprintf(stderr, "opcao -i e obrigatoria");
        liberta_settings(settings);
        exit(EXIT_FAILURE);
    }
    if(settings->criterio_write == NULL) // se tem -o
    {
        fprintf(stderr, "opcao -o e obrigatoria");
        liberta_settings(settings);
        exit(EXIT_FAILURE);
    }
}

settings_t *verifica_tipo_ficheiro(settings_t *settings, int *binario)
{
    char csv[5] = ".csv";
    char dat[5] = ".dat";
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
        fprintf(stderr, "extensao indisponivel"); // se nao for nenhuma destas, a extensao esta errada 
        exit(EXIT_FAILURE);
    }
    return settings;
}

void utilizacao()
{
    fprintf(stderr, "\t\tPROJETO FINAL\n\n");
    fprintf(stderr, "opcoes validas:\n");
    fprintf(stderr, "[-L all ou \"continente\"]\t\t\t\t  le o ficheiro inteiro ou dos paises em relacao ao continente\n");
    fprintf(stderr, "[-S (alfa, pop, inf yyyy-ww, dea yyyy-ww)]\t\t  Ordenacao dos dados\n");
    fprintf(stderr, "[-D (inf, dea, racioinf, raciodea)]\t\t\t  Selecao dos dados\n");
    fprintf(stderr, "[-P (min n, max n, date yyyy-ww, dates yyyy-ww yyyy-ww)]  Restricao dos dados\n");
    fprintf(stderr, "[-i \"nomedoficheiro.extensao\"]\t\t\t\t  Leitura do ficheiro\n");
    fprintf(stderr, "[-o \"nomedoficheiro.extensao\"]\t\t\t\t  Escrita do ficheiro\n");
}
