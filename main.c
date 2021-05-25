#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
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
                settings->criterio_leitura = L_CONTINENTE; //-------------------nao utilizado------------------
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
    //  erros_ficheiro(root_principal);
    if (binario == 0) // e binario
    {
        if (settings->criterio_sel != D_NONE)
            selecionar(settings, root_principal);
        if (settings->criterio_res != P_NONE)
            restricao_lista(root_principal, settings);

        // root_principal = ordenar_lista(root_principal, settings);
    }

    //  cria_ficheiro(root_principal, settings);
    imprime_lista_paises(root_principal);
    /*   liberta_settings(settings);
    liberta_lista(root_principal, destruir_pais); */
    return 0;
}

/** \brief cria uma lista vazia
 *
 * \return lista_t* lista vazia
 *
 */
lista_t *cria_lista()
{
    lista_t *novaLista = (lista_t *)malloc(sizeof(lista_t));
    if (novaLista == NULL)
    {
        fprintf(stderr, "Erro a alocar memoria para novalista");
        exit(EXIT_FAILURE);
    }
    novaLista->first = NULL;
    novaLista->last = NULL;

    return novaLista;
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

pais_t *cria_pais()
{
    pais_t *pais = (pais_t *)malloc(sizeof(pais_t));
    if (pais == NULL)
    {
        fprintf(stderr, "Erro a alocar memoria a pais");
        exit(EXIT_FAILURE);
    }
    pais->country = NULL;
    pais->country_code = NULL;
    pais->continent = NULL;
    pais->population = 0;
    pais->dados = NULL;
    return pais;
}

void insere_pais_dados_lista(lista_t *lista, pais_t *pais, dados_t *dados)
{
    node_t *aux = lista->first;

    while (aux != NULL && strcmp(pais->country_code, ((pais_t *)aux->value)->country_code) != 0)
    {
        aux = aux->next;
    }
    if (aux == NULL)
    {
        inserir_elemento_final(lista, pais);
        pais->dados = cria_lista();
        aux = lista->last;
    }
    else //se ja existir pais
    {
        destruir_pais(pais);
    }
    dados->pais = aux->value;
    inserir_elemento_final(((pais_t *)aux->value)->dados, dados); //inserir os novos dados na lista do pais
}

/** \brief insere um node no final da lista
 *
 * \param lista lista_t* lista na qual vamos adicionar o elemento
 * \param item void* elemento a adicionar na lista
 * \return void
 *
 */
void inserir_elemento_final(lista_t *lista, void *item)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));

    if (node == NULL)
    {
        fprintf(stderr, "Erro a alocar memoria a node");
        exit(EXIT_FAILURE);
    }

    node->next = NULL;        //inicia o next como null porque é o ultimo elemento
    node->prev = lista->last; //o item anterior é o "antigo" ultimo elemento da lista
    node->value = item;
    if (lista->first == NULL) //se a lista estiver vazia
    {
        lista->first = node; //o novo elemento torna-se o primeiro
    }
    else
    {
        lista->last->next = node;
    }
    lista->last = node;
}

/** \brief funcao que le o ficheiro e o coloca por completo numa lista, no caso de se ter
 *         escolhido a opcao -L "continente", cria a lista apenas para esse continente
 *
 * \param settings settings_t* definicoes para saber se a lista é ALL ou "continente"
 * \return lista_t* lista criada de acordo com os argumentos da linha de comando
 *
 */
lista_t *ler_ficheiro(settings_t *settings)
{
    FILE *ficheiro;
    char buffer[MAX_PALAVRAS_LINHAS];
    lista_t *lista = cria_lista(); //cria lista vazia

    ficheiro = fopen(settings->criterio_file, settings->tipo_ficheiro); // abre o ficheiro
    if (ficheiro == NULL)                                               // verifica se ouve algum erro na abertura do ficheiro
    {
        fprintf(stderr, "Erro a abrir ficheiro\n");
        exit(EXIT_FAILURE);
    }
    int linhas = 0; // contador de linhas, para nao ler a primeira, pois e o cabecalho
    while (fgets(buffer, MAX_PALAVRAS_LINHAS, ficheiro) != NULL)
    {
        if (linhas == 0)
        {
            linhas++;
            continue; // primeira linha queremos ignorar
        }
        ler_linha(settings, lista, buffer);
    }

    fclose(ficheiro);
    return lista;
}

/** \brief le a linha, divide-a em varias strings e coloca-as nas diferentes variaveis de um node
 *
 * \param letra char* linha lida do ficheiro
 * \return dados_t* node com os elementos da linha nas suas variaveis
 *
 */
void ler_linha(settings_t *settings, lista_t *lista, char *letra)
{
    char *inicio_coluna = letra;
    int coluna = 0;
    pais_t *pais = cria_pais();

    dados_t *dados = malloc(sizeof(dados_t));
    if (dados == NULL)
    {
        fprintf(stderr, "Erro a alocar memoria para dados");
        exit(EXIT_FAILURE);
    }

    while (*letra != '\0' && *letra != '\n')
    {
        if (*letra == ',') // troca as "," por "\0" para dividir a string em varias substrings
        {
            *letra = '\0';

            inserir_dados(pais, dados, inicio_coluna, coluna);

            coluna++;
            inicio_coluna = letra + 1;
        }
        letra++;
    }

    *letra = '\0';
    inserir_dados(pais, dados, inicio_coluna, coluna);

    if (settings->criterio_leitura == L_CONTINENTE && strcmp(pais->continent, settings->leitura_continente) != 0)
    {
        //descartar linha
        destruir_pais(pais);
        destruir_dados(dados);
    }
    else
    {
        insere_pais_dados_lista(lista, pais, dados);
    }
}

/** \brief insere um determinado dado na variavel correta da lista
 *
 * \param dados dados_t* lista para onde vao os dados do ficheiro
 * \param inicio_coluna char* palavra que vai entrar em cada variavel
 * \param coluna int indica em que coluna estamos de modo a colocar o valor na variavel correta
 * \return void
 *
 */
void inserir_dados(pais_t *pais, dados_t *dados, char *inicio_coluna, int coluna)
{
    switch (coluna)
    {
    case 0:
        pais->country = malloc(sizeof(char) * (strlen(inicio_coluna) + 1));
        strcpy(pais->country, inicio_coluna);
        break;
    case 1:
        pais->country_code = malloc(sizeof(char) * (strlen(inicio_coluna) + 1));
        strcpy(pais->country_code, inicio_coluna);

        break;
    case 2:
        pais->continent = malloc(sizeof(char) * (strlen(inicio_coluna) + 1));
        strcpy(pais->continent, inicio_coluna);
        break;
    case 3:
        pais->population = atoi(inicio_coluna);
        break;
    case 4:
        dados->indicator = malloc(sizeof(char) * (strlen(inicio_coluna) + 1));
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
 * \param dados *char string com a data
 * \return yearWeek_t data convertida em dois ints
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

/** \brief troca determinados nodes da lista
 *
 * \param left node_t*
 * \param right node_t*
 * \return node_t* nodes trocados
 *
 */
node_t *troca(node_t *left, node_t *right)
{
    left->next = right->next;
    right->next = left;
    return right;
}

//-------------------------------------------------------------------------------------
/** \brief ordena dois nodes de acordo com a populacao decrescente, se a populacao do node right for menor
 *          que a do node right->next, trocam
 *
 * \param right dados_t**
 * \param left dados_t**
 * \param flag int* flag que mantem o loop a verificar a lista inteira
 * \return void
 *
 */
/* void ordenacao_pop(dados_t **right, dados_t **left, int *flag)
{
    if ((*right)->population < (*right)->next->population) // se menor, troca
    {
        (*left)->next = troca((*right), (*right)->next);
        (*flag) = 1;
    }
}
 */
/** \brief ordena a lista por ordem alfabetica
 *
 * \param right dados_t**
 * \param left dados_t**
 * \param flag int* flag que mantem o loop a verificar a lista inteira
 * \return void
 *
 */
/* void ordenacao_alfa(dados_t **right, dados_t **left, int *flag)
{
    if (strcmp((*right)->country, (*right)->next->country) > 0) // se > 0, o node right->next tem uma letra
    {
        // mais abaixo no alfabeto
        (*left)->next = troca((*right), (*right)->next);
        (*flag) = 1;
    }
} */

/** \brief ordena a lista
 *
 * \param root lista_t*
 * \param settings settings_t*
 * \return lista_t*
 *
 */
/* lista_t *ordenar_lista(lista_t *root, settings_t *settings)
{
    int flag = 1;
    dados_t *left, *right, *head, aux;
    head = &aux;
    head->next = root->first;
    if (root != NULL && root->first->next != NULL)
    {
        while (flag)
        {
            flag = 0;
            left = head;
            right = head->next;
            while (right->next != NULL)
            {
                if (settings->criterio_ord == S_ALFA)
                    ordenacao_alfa(&right, &left, &flag);
                else if (settings->criterio_ord == S_POP)
                    ordenacao_pop(&right, &left, &flag);

                left = right;
                if (right->next != NULL)
                    right = right->next;
            }
        }
    }
    root->first = head->next;
    return root;
} */

//--------------------------------------------------------------------------------------------

/** \brief apaga o elemento que nao pretendemos para a lista
 *
 * \param lista lista_t* primeiro ou ultimo elemento da lista
 * \param head dados_t** elemento a apagar
 * \return dados_t* lista sem elemento
 *
 */
/* dados_t *apagar_elemento_restricao(lista_t *lista, dados_t *head)
{
    dados_t *temp;

    temp = head->next;
    apagar_elemento_lista(lista, head);
    head = temp;
    return head;
} */

/** \brief apenas dados de países com mais de n mil habitantes
 *
 * \param lista lista_t* primeiro ou ultimo elemento da lista
 * \param head dados_t** elemento que estamos neste momento
 * \param settings settings_t* numero de habitantes
 * \return void
 *
 */
void restricao_min(lista_t *lista, node_t *node, settings_t *settings)
{
    pais_t *pais = node->value;
    if (pais->population < settings->restricao_nmin) // se menor que o minimo, apaga
        apagar_elemento_lista(lista, node, destruir_pais);
}

/** \brief apenas dados de países com menos de n mil habitantes
 *
 * \param lista lista_t* primeiro ou ultimo elemento da lista
 * \param head dados_t** elemento que estamos neste momento
 * \param settings settings_t* n maximo
 * \return void
 *
 */
void restricao_max(lista_t *lista, node_t *node, settings_t *settings)
{
    pais_t *pais = node->value;
    if (pais->population > settings->restricao_nmax) // se maior que o maximo, apaga
        apagar_elemento_lista(lista, node, destruir_pais);
}
/** \brief  apenas dados relativos à semana indicada
 *
 * \param lista lista_t* primeiro ou ultimo elemento da lista
 * \param head dados_t** elemento que estamos neste momento
 * \param anosemana settings_t* semana especificada pelo utilizador
 * \return void
 *
 */
void restricao_date(lista_t *lista, node_t *node, settings_t *settings)
{
    pais_t *pais = node->value;

    if (pais->dados == NULL)
        return;

    node_t *curr = pais->dados->first, *aux;
    dados_t *aux_dados;

    while (curr != NULL)
    {
        aux = curr;
        curr = curr->next;
        aux_dados = aux->value;

        if (aux_dados->year_week->week != settings->restricao_date1->week || aux_dados->year_week->year != settings->restricao_date1->year)
            apagar_elemento_lista(lista, node, destruir_dados); // se nao for a semana indicada, apaga o node que contem os dados
    }
}

short entre_semanas(yearWeek_t *data, yearWeek_t *data1, yearWeek_t *data2)
{
    if (data->year <= data1->year || data->year >= data2->year)
    {
        // se nao e um ano dentro do intervalo, nao entra
        if (data->year == data1->year && data->year == data2->year)
        {
            // se tiver no mesmo ano
            if (data->week >= data1->week && data->week <= data2->week)
                // se tiver dentro do intervalo indicado, retorna
                return 1;
        }
        else if (data->year == data1->year)
        {
            // ano da data 1 e diferente do da data 2
            if (data->week >= data1->week)
                // se tiver dentro do intervalo, retorna
                return 1;
        }
        else if (data->year == data2->year)
        {
            // ano da data 2 e diferente do da data 1
            if (data->week <= data2->week)
                // se tiver dentro do intervalo, retorna
                return 1;
        }
        // se nao entrar nos outros ifs, entao nao esta no intervalo
        return 0;
    }
    return 1;
}

/** \brief apenas dados entre as semanas indicadas
 *
 * \param lista lista_t* primeiro ou ultimo elemento da lista
 * \param head dados_t** elemento que estamos neste momento
 * \param anosemana settings_t* semanas que restringem os dados
 * \return void
 *
 */
void restricao_dates(lista_t *lista, node_t *node, settings_t *settings)
{
    pais_t *pais = node->value;

    if (pais->dados == NULL)
        return;

    node_t *curr = pais->dados->first, *aux;
    dados_t *aux_dados;

    while (curr != NULL)
    {
        aux = curr;
        curr = curr->next;
        aux_dados = aux->value;

        if (!entre_semanas(aux_dados->year_week, settings->restricao_date1, settings->restricao_date2))
            apagar_elemento_lista(lista, node, destruir_dados); // se nao for a semana indicada, apaga o node que contem os dados
    }
}

/** \brief restringe a lista de acordo com varias especificacoes referidas pelo utilizador
 *
 * \param lista lista_t* lista a ser restringida
 * \param settings settings_t* para saber qual o tipo de restricao
 * \return lista_t* lista restringida
 *
 */
void restricao_lista(lista_t *lista, settings_t *settings)
{
    node_t *curr, *aux;

    settings->restricao_nmin *= 1000; // n mil habitantes
    settings->restricao_nmax *= 1000;

    curr = lista->first;

    while (curr != NULL) // loop que verifica a lista inteira
    {
        aux = curr;
        curr = curr->next;
        if (settings->criterio_res == P_MIN) //apenas dados de países com mais de n mil habitantes
            restricao_min(lista, aux, settings);
        else if (settings->criterio_res == P_MAX) //apenas dados de países com menos de n mil habitantes
            restricao_max(lista, aux, settings);
        else if (settings->criterio_res == P_DATE) //apenas dados relativos à semana indicada
            restricao_date(lista, aux, settings);
        else if (settings->criterio_res == P_DATES) //apenas dados entre as semanas indicadas
        {
            settings = verifica_datas(settings);
            restricao_dates(lista, aux, settings);
        }
    }
}
//--------------------------------------------------------------------------------------

/** \brief selecionar, para cada pais, a semana com mais infetados
 *
 * \param atual dados_t*
 * \param comparacao dados_t*
 * \return int: 0,1,2
 *
 */

short selecao_inf(void *p_atual, void *p_comparacao)
{
    dados_t *atual = p_atual;
    dados_t *comparacao = p_comparacao;

    //se o elemento novo nao tiver o indicador "cases"
    if (strcmp(comparacao->indicator, "cases") != 0)
        return 0; //"descartar" o novo elemento

    //se o elemento atual for nulo, ficamos com o comparacao
    if (atual == NULL)
        return 1;

    if (atual->weekly_count < comparacao->weekly_count)
        return 1; //descarta o elemento atual

    //no caso de empate, para desempatar escolhe-se a semana mais recente
    if (atual->weekly_count == comparacao->weekly_count)
    {

        if (atual->year_week->year == comparacao->year_week->year) //se for o mesmo ano
        {
            if (atual->year_week->week < comparacao->year_week->week)
                return 1; //descartar o elemento atual
        }
        if (atual->year_week->year < comparacao->year_week->year)
            return 1; //o elemento atual é mais antigo, descartamos o elemento atual
    }
    return 0;
}

short selecao_dea(void *p_atual, void *p_comparacao)
{
    dados_t *atual = p_atual;
    dados_t *comparacao = p_comparacao;

    //se o elemento novo nao tiver o indicador "deaths"
    if (strcmp(comparacao->indicator, "deaths") != 0)
        return 0; //"descartar" o novo elemento

    //se o elemento atual for nulo, ficamos com o comparacao
    if (atual == NULL)
        return 1;

    if (atual->weekly_count < comparacao->weekly_count)
        return 1; //descarta o elemento atual

    //no caso de empate, para desempatar escolhe-se a semana mais recente
    if (atual->weekly_count == comparacao->weekly_count)
    {

        if (atual->year_week->year == comparacao->year_week->year) //se for o mesmo ano
        {
            if (atual->year_week->week < comparacao->year_week->week)
                return 1; //descartar o elemento atual
        }
        if (atual->year_week->year < comparacao->year_week->year)
            return 1; //o elemento atual é mais antigo, descartamos o elemento atual
    }
    return 0;
}

short selecao_racio_inf(void *p_atual, void *p_comparacao)
{
    dados_t *atual = p_atual;
    dados_t *comparacao = p_comparacao;

    //se o elemento novo nao tiver o indicador "cases"
    if (strcmp(comparacao->indicator, "cases") != 0)
        return 0; //"descartar" o novo elemento

    //se o elemento atual for nulo, ficamos com o comparacao
    if (atual == NULL)
        return 1;

    if (atual->rate_14_day < comparacao->rate_14_day)
        return 1; //descarta o elemento atual

    //no caso de empate, para desempatar escolhe-se a semana mais recente
    if (atual->rate_14_day == comparacao->rate_14_day)
    {
        if (atual->year_week->year == comparacao->year_week->year) //se for o mesmo ano
        {
            if (atual->year_week->week < comparacao->year_week->week)
                return 1; //descartar o elemento atual
        }
        if (atual->year_week->year < comparacao->year_week->year)
            return 1; //o elemento atual é mais antigo, descartamos o elemento atual
    }
    return 0;
}

short selecao_racio_dea(void *p_atual, void *p_comparacao)
{
    dados_t *atual = p_atual;
    dados_t *comparacao = p_comparacao;

    //se o elemento novo nao tiver o indicador "deaths"
    if (strcmp(comparacao->indicator, "deaths") != 0)
        return 0; //"descartar" o novo elemento

    //se o elemento atual for nulo, ficamos com o comparacao
    if (atual == NULL)
        return 1;

    if (atual->rate_14_day < comparacao->rate_14_day)
        return 1; //descarta o elemento atual

    //no caso de empate, para desempatar escolhe-se a semana mais recente
    if (atual->rate_14_day == comparacao->rate_14_day)
    {
        if (atual->year_week->year == comparacao->year_week->year) //se for o mesmo ano
        {
            if (atual->year_week->week < comparacao->year_week->week)
                return 1; //descartar o elemento atual
        }
        if (atual->year_week->year < comparacao->year_week->year)
            return 1; //o elemento atual é mais antigo, descartamos o elemento atual
    }
    return 0;
}
//----------------------------------------------------------------------------------
void imprime_lista(lista_t *lista)
{
    node_t *curr = lista->first;
    while (curr != NULL)
    {
        dados_t *dados = curr->value;
        printf("- %s - %d - %d-%d - %f - %d\n", dados->indicator,
               dados->weekly_count, dados->year_week->year, dados->year_week->week, dados->rate_14_day, dados->cumulative_count);
        curr = curr->next;
    }
}

void imprime_lista_paises(lista_t *lista)
{
    node_t *curr = lista->first;
    while (curr != NULL)
    {
        pais_t *pais = curr->value;
        printf("%s - %s - %s - %d\n", pais->country,
               pais->country_code, pais->continent, pais->population);
        imprime_lista(pais->dados);
        curr = curr->next;
    }
}
//----------------------------------^^^^eliminar no final^^^^-------------------------------------------------
/** \brief cria um ficheiro csv
 *
 * \param root lista_t* lista que vai criar o ficheiro
 * \param settings settings_t* saber qual o nome do ficheiro
 * \return void
 *
 */
/* void cria_ficheiro(lista_t *root, settings_t *settings)
{
    FILE *fp;
    dados_t *curr = root->first;
    fp = fopen(settings->criterio_write, settings->tipo_escrita);
    if (fp == NULL)
    {
        fprintf(stderr, "Erro a criar ficheiro");
        exit(EXIT_FAILURE);
    }
    if (strcmp(settings->tipo_escrita, "w") == 0)
    {
        fprintf(fp, "country,country_code,continent,population,indicator,weekly_count,year_week,rate_14_day,cumulative_count\n");
    }
    while (curr != NULL)
    {
        if (strcmp(settings->tipo_escrita, "w") == 0)
        {
            fprintf(fp, "%s, %s, %s, %d, %s, %d, %d-%d, %f, %d\n", curr->country, curr->country_code, curr->continent, curr->population, curr->indicator,
                    curr->weekly_count, curr->year_week->year, curr->year_week->week, curr->rate_14_day, curr->cumulative_count);
        }
        else
        {
            fwrite(curr->country, sizeof(curr->country), 1, fp);
            fwrite(curr->country_code, sizeof(curr->country_code), 1, fp);
            fwrite(curr->continent, sizeof(curr->continent), 1, fp);
            fwrite(&curr->population, sizeof(curr->population), 1, fp);
            fwrite(curr->indicator, sizeof(curr->indicator), 1, fp);
            fwrite(&curr->weekly_count, sizeof(curr->weekly_count), 1, fp);
            fwrite(&curr->year_week->year, sizeof(curr->year_week->year), 1, fp);
            fwrite(&curr->year_week->week, sizeof(curr->year_week->week), 1, fp);
            fwrite(&curr->rate_14_day, sizeof(curr->rate_14_day), 1, fp);
            fwrite(&curr->cumulative_count, sizeof(curr->cumulative_count), 1, fp);
        }
        curr = curr->next;
    }
    fclose(fp);
} */

/** \brief apaga um certo elemento da lista
 *
 * \param lista lista_t*
 * \param elemento dados_t*
 * \return void
 *
 */
void apagar_elemento_lista(lista_t *lista, node_t *elemento, void (*destruir_fn)(void *))
{
    if (elemento->prev == NULL)
        lista->first = elemento->next;
    else
        elemento->prev->next = elemento->next;

    if (elemento->next == NULL)
        lista->last = elemento->prev;
    else
        elemento->next->prev = elemento->prev;
    if (destruir_fn != NULL)
        destruir_fn(elemento->value);
    free(elemento);
}

/** \brief liberta a lista da memoria
 *
 * \param lista lista_t*
 * \return void
 *
 */
void liberta_lista(lista_t *lista, void (*destruir_fn)(void *))
{
    node_t *curr;
    node_t *next = lista->first;

    while (next != NULL) // loop que liberta linha a linha
    {
        curr = next;
        next = next->next;
        destruir_fn(curr->value);
        free(curr);
    }
    free(lista);
}

/** \brief liberta um determinado node
 *
 * \param dados dados_t*
 * \return void
 *
 */
void destruir_dados(void *p_dados)
{
    dados_t *dados = p_dados;
    free(dados->indicator);
    free(dados->year_week);
    free(dados);
}

void destruir_pais(void *p_pais)
{
    pais_t *pais = p_pais;
    free(pais->country_code);
    free(pais->country);
    free(pais->continent);
    if (pais->dados != NULL)
        liberta_lista(pais->dados, destruir_dados);
    free(pais);
}

/** \brief liberta memoria das settings
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

/** \brief menu que verifica qual a selecao a fazer
 *
 * \param settings settings_t* para saber qual o tipo de selecao
 * \param atual dados_t*
 * \param comparacao dados_t*
 * \return int
 *
 */
compare_fn criterio_selecao(settings_t *settings)
{
    if (settings->criterio_sel == D_INF) // selecao infetados
        return selecao_inf;
    if (settings->criterio_sel == D_DEA) //selecao deaths
        return selecao_dea;
    if (settings->criterio_sel == D_RACIOINF) // selecao racio infetados / 100 mil de habitantes
        return selecao_racio_inf;
    if (settings->criterio_sel == D_RACIODEA) // selecao racio mortes / 1 milhao de habitantes
        return selecao_racio_dea;
    return NULL;
}

/** \brief
 *
 * \param settings settings_t*
 * \param lista lista_t*
 * \return void
 *
 */
void selecionar(settings_t *settings, lista_t *lista)
{
    node_t *el_atual = lista->first;
    compare_fn cmp_fn = criterio_selecao(settings);

    while (el_atual != NULL)
    {
        node_t *aux = el_atual;
        pais_t *pais_atual = el_atual->value;
        node_t *resultado = selecionar_pais(pais_atual->dados, cmp_fn);
        el_atual = el_atual->next;

        if (resultado == NULL)
            apagar_elemento_lista(lista, aux, destruir_pais);
        else
        {
            dados_t *resultado_dados = resultado->value;
            apagar_elemento_lista(pais_atual->dados, resultado, NULL); //remove elemento da lista, sem o destruir
            liberta_lista(pais_atual->dados, destruir_dados);          // destroi o restante da lista
            pais_atual->dados = cria_lista();                          // cria uma nova lista com apenas o elemento selecionado
            inserir_elemento_final(pais_atual->dados, resultado_dados);
        }
    }
}

node_t *selecionar_pais(lista_t *lista_dados, compare_fn cmp_fn)
{
    node_t *resultado = NULL;
    node_t *aux = lista_dados->first;

    while (aux != NULL)
    {
        if (cmp_fn(resultado == NULL ? NULL : resultado->value, aux->value))
        {
            resultado = aux;
        }
        aux = aux->next;
    }
    return resultado;
}

/** \brief verifica se existe algum erro de escrita no ficheiro lido
 *
 * \param lista lista_t*
 * \return void
 *
 */
/* void erros_ficheiro(lista_t *lista)
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
} */

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
