#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include "headers.h"

/** \brief cria a lista principal
 *
 * \return lista_t* lista vazia
 *
 */
lista_t *cria_lista()
{
    lista_t *novaLista = malloc(sizeof(lista_t));

    novaLista->first = NULL;
    novaLista->last = NULL;

    return novaLista;
}

/** \brief insere um node no final da lista
 *
 * \param lista lista_t* lista na qual vamos adicionar o elemento
 * \param item dados_t* elemento a adicionar na lista
 * \return void
 *
 */
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

    if (ficheiro == NULL) // verifica se ouve algum erro na abertura do ficheiro
    {
        printf("Erro a abrir ficheiro\n");
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
        if(settings->criterio_leitura == L_ALL) // se for ALL queremos uma lista do ficheiro completo
        {
            dados_t *item = ler_linha(buffer);
            inserir_elemento_final(lista, item);
        }
        else
        {
            if(strstr(buffer, settings->leitura_continente) != 0) // so queremos lista com um determinado continente
            {
                dados_t *item = ler_linha(buffer);
                inserir_elemento_final(lista, item);
            }
            else
            {
                continue; // se nao for o continente passa para a linha seguinte
            }
        }
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
dados_t *ler_linha(char *letra)
{
    char *inicio_coluna = letra;
    int coluna = 0;
    dados_t *dados = malloc(sizeof(dados_t));

    dados->next = NULL;

    while (*letra != '\0' && *letra != '\n')
    {
        if (*letra == ',') // troca as "," por \0 para melhor divisao de elementos
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

/** \brief insere um determinado dado na variavel correta da lista
 *
 * \param dados dados_t* lista para onde vao os dados do ficheiro
 * \param inicio_coluna char* palavra que vai entrar em cada variavel
 * \param coluna int indica em que coluna estamos de modo a colocar o valor na variavel correta
 * \return void
 *
 */
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
 * \param dados *char string com a data
 * \return yearWeek_t data convertida em dois ints
 *
 */
yearWeek_t *parseYearWeek(char *dados)
{
    yearWeek_t *yearWeek = malloc(sizeof(yearWeek_t));

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
settings_t *troca_datas(settings_t* datas)
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
 * \param left dados_t*
 * \param right dados_t*
 * \return dados_t* nodes trocados
 *
 */
dados_t *troca(dados_t *left, dados_t *right)
{
    left->next = right->next;
    right->next = left;
    return right;
}

/** \brief nao sei se vamos precisar desta funcao
 *
 * \param headlist dados_t*
 * \return dados_t*
 *
 */
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
void ordenacao_pop(dados_t **right, dados_t **left, int *flag)
{
    if ((*right)->population < (*right)->next->population) // se menor, troca
    {
        (*left)->next = troca((*right), (*right)->next);
        (*flag) = 1;
    }
}

/** \brief ordena a lista por ordem alfabetica
 *
 * \param right dados_t**
 * \param left dados_t**
 * \param flag int* flag que mantem o loop a verificar a lista inteira
 * \return void
 *
 */
void ordenacao_alfa(dados_t **right, dados_t **left, int *flag)
{
    if (strcmp((*right)->country, (*right)->next->country) > 0) // se > 0, o node right->next tem uma letra
    {
        // mais abaixo no alfabeto
        (*left)->next = troca((*right), (*right)->next);
        (*flag) = 1;
    }
}

/** \brief menu que verifica qual a ordenacao escolhida
 *
 * \param right dados_t**
 * \param left dados_t**
 * \param flag int*
 * \param settings settings_t* onde esta guardado o tipo de ordenacao
 * \return void
 *
 */
void menu_ordenacao(dados_t **right, dados_t **left, int *flag, settings_t *settings)
{
    if (settings->criterio_ord == S_ALFA) // ordenacao por ordem alfabetica
    {
        ordenacao_alfa(right, left, flag);
    }
    else if (settings->criterio_ord == S_POP) // ordenacao por populacao
    {
        ordenacao_pop(right, left, flag);
    }
//elseif(...)
}

/** \brief ordena a lista inteira
 *
 * \param root dados_t*
 * \param anosemana settings_t*
 * \return dados_t*
 *
 */
lista_t *ordenar_lista(lista_t *root, settings_t *settings)
{
    int flag = 1;
    dados_t *left, *right, *head, aux;

    head = &aux;
    head = root->first;
    if (root != NULL && root->first->next != NULL)
    {
        while (flag)
        {
            flag = 0;
            left = head;
            right = head->next;
            while (right->next != NULL)
            {
                //menu_ordenacao(&right, &left, &flag, anosemana);
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
}

//--------------------------------------------------------------------------------------------

/** \brief
 *
 * \param right dados_t**
 * \param left dados_t**
 * \param flag int*
 * \param settings settings_t*
 * \return void
 *
 */
void restricao_max(lista_t *lista, dados_t **head, settings_t *settings)
{
    dados_t *temp;

    if ((*head)->population > settings->restricao_nmax)
    {
        temp = (*head)->next;

        apagar_elemento_lista(lista, (*head));

        (*head) = temp;
    }
    else
    {
        (*head) = (*head)->next;
    }
}

/** \brief
 *
 * \param right dados_t**
 * \param left dados_t**
 * \param flag int*
 * \param anosemana settings_t*
 * \return void
 *
 */
void restricao_date(lista_t *lista, dados_t **head, settings_t *anosemana)
{
    dados_t *temp;

    if ((*head)->year_week->week != anosemana->restricao_date1->week || (*head)->year_week->year != anosemana->restricao_date1->year)
    {
        temp = (*head)->next;

        apagar_elemento_lista(lista, (*head));

        (*head) = temp;
    }
    else
    {
        (*head) = (*head)->next;
    }
}

/** \brief
 *
 * \param right dados_t**
 * \param left dados_t**
 * \param flag int*
 * \param anosemana settings_t*
 * \return void
 *
 */
void restricao_dates(lista_t *lista, dados_t **head, settings_t *anosemana)
{
    dados_t *temp;

    if((*head)->year_week->year <= anosemana->restricao_date1->year || (*head)->year_week->year >= anosemana->restricao_date2->year)
    {
        if((*head)->year_week->year == anosemana->restricao_date1->year  && (*head)->year_week->year == anosemana->restricao_date2->year)
        {
            if((*head)->year_week->week >= anosemana->restricao_date1->week && (*head)->year_week->week <= anosemana->restricao_date2->week)
            {
                (*head) = (*head)->next;
                return;
            }
        }
        else if((*head)->year_week->year == anosemana->restricao_date1->year)
        {
            if((*head)->year_week->week >= anosemana->restricao_date1->week)
            {
                (*head) = (*head)->next;
                return;
            }
        }
        else if((*head)->year_week->year == anosemana->restricao_date2->year)
        {
            if((*head)->year_week->week <= anosemana->restricao_date2->week)
            {
                (*head) = (*head)->next;
                return;
            }
        }
        temp = (*head)->next;

        apagar_elemento_lista(lista, (*head));

        (*head) = temp;
        return;
    }
    (*head) = (*head)->next;
}

/** \brief
 *
 * \param right dados_t**
 * \param left dados_t**
 * \param flag int*
 * \param settings settings_t*
 * \return void
 *
 */
void menu_restricao(dados_t **right, dados_t **left, int *flag, settings_t *settings)
{
    if (settings->criterio_res == P_MIN)
    {
        //restricao_min(right, flag, settings);
    }
    else if (settings->criterio_res == P_MAX)
    {
//        restricao_max(right, left, flag, settings);
    }
    else if (settings->criterio_res == P_DATE)
    {
        //   restricao_date(right, left, flag, settings);
    }
    else if (settings->criterio_res == P_DATES)
    {
        // restricao_dates(right, left, flag, settings);
    }
}

void restricao_min(lista_t *lista, dados_t **head, settings_t *settings)
{
    dados_t *temp;

    if ((*head)->population < settings->restricao_nmin)
    {
        temp = (*head)->next;

        apagar_elemento_lista(lista, (*head));

        (*head) = temp;
    }
    else
    {
        (*head) = (*head)->next;
    }
}

/** \brief
 *
 * \param root dados_t*
 * \param settings settings_t*
 * \return dados_t*
 *
 */

lista_t *restricao_lista(lista_t *lista, settings_t *settings)
{
    dados_t *head, aux;

    settings->restricao_nmin *= 1000;
    settings->restricao_nmax *= 1000;

    head = &aux;
    head = lista->first;

    settings = verifica_datas(settings);

    printf("%d %d\n", settings->restricao_date2->year, settings->restricao_date2->week);
    printf("%d %d\n", settings->restricao_date1->year, settings->restricao_date1->week);

    if (lista != NULL && lista->first->next != NULL)
    {
        while (head != NULL)
        {
            if(settings->criterio_res == P_MIN)
                restricao_min(lista, &head, settings);
            else if(settings->criterio_res == P_MAX)
                restricao_max(lista, &head, settings);
            else if (settings->criterio_res == P_DATE)
                restricao_date(lista, &head, settings);
            else if (settings->criterio_res == P_DATES)
                restricao_dates(lista, &head, settings);
        }
    }
    lista->last = head;
    return lista;
}
//--------------------------------------------------------------------------------------

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

    //no caso de empate, para desempatar escolhe-se a semana mais recente
    if (atual->weekly_count == comparacao->weekly_count)
    {

        if (atual->year_week->year == comparacao->year_week->year) //se for o mesmo ano
        {

            if (atual->year_week->week > comparacao->year_week->week)
                return 2;

            else
                return 1;
        }
        if (atual->year_week->year > comparacao->year_week->year)
        {
            return 1; //o elemento atual é mais recente, discartamos o elemento comparacao
        }
        else
            return 2;
    }
}
/*
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
}*/
//----------------------------------------------------------------------------------
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
//----------------------------------^^^^eliminar no final^^^^-------------------------------------------------
/** \brief cria um ficheiro csv
 *
 * \param root lista_t* lista que vai criar o ficheiro
 * \param settings settings_t* saber qual o nome do ficheiro
 * \return void
 *
 */
void cria_ficheiro(lista_t *root, settings_t *settings)
{
    FILE *fp;
    dados_t *curr = root->first;
    fp = fopen(settings->criterio_write, settings->tipo_escrita);
    if (fp == NULL)
    {
        printf("Erro a criar ficheiro");
        exit(EXIT_FAILURE);
    }
    if(strcmp(settings->tipo_escrita, "w") != 0)
    {
        fprintf(fp, "country,country_code,continent,population,indicator,weekly_count,year_week,rate_14_day,cumulative_count\n");
    }
    while (curr != NULL)
    {
        fprintf(fp, "%s, %s, %s, %d, %s, %d, %d-%d, %f, %d\n", curr->country, curr->country_code, curr->continent, curr->population, curr->indicator,
                curr->weekly_count, curr->year_week->year, curr->year_week->week, curr->rate_14_day, curr->cumulative_count);
        curr = curr->next;
    }
    fclose(fp);
}

/** \brief apaga um certo elemento da lista
 *
 * \param lista lista_t*
 * \param elemento dados_t*
 * \return void
 *
 */
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

/** \brief liberta a lista da memoria
 *
 * \param lista lista_t*
 * \return void
 *
 */
void liberta_lista(lista_t *lista)
{
    dados_t *curr;
    dados_t *next = lista->first;

    while (next != NULL) // loop que liberta linha a linha
    {
        curr = next;
        next = next->next;
        destruir_dados(curr);
    }
    free(lista);
}

/** \brief liberta um determinado node
 *
 * \param dados dados_t*
 * \return void
 *
 */
void destruir_dados(dados_t *dados)
{
    free(dados->year_week);
    free(dados);
}

/** \brief menu que verifica qual a selecao a fazer
 *
 * \param settings settings_t* para saber qual o tipo de selecao
 * \param atual dados_t*
 * \param comparacao dados_t*
 * \return int
 *
 */
int criterio_selecao(settings_t *settings, dados_t *atual, dados_t *comparacao)
{
    if (settings->criterio_sel == D_INF) // selecao inf
        return selecao_inf(atual, comparacao);
    // else if (...)
    return 0;
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

/** \brief verifica se existe algum erro de escrita no ficheiro lido
 *
 * \param lista lista_t*
 * \return void
 *
 */
void erros_ficheiro(lista_t *lista)
{
    dados_t *head = lista->first;
    char i;

    while (head != NULL)
    {
        if (head->population < 0 || head->cumulative_count < 0 || head->rate_14_day < 0 || head->weekly_count < 0)
        {
            exit(-1);
        }
        else if (head->year_week->week <= 0 || head->year_week->week > 53 || head->year_week->year < 0)
        {
            exit(-1);
        }
        for (i = '0'; i <= '9'; i++)
        {
            if (strchr(head->country, i) != NULL || strchr(head->continent, i) != NULL || strchr(head->country_code, i) != NULL || strchr(head->indicator, i) != NULL)
            {
                exit(-1);
            }
        }
        head = head->next;
    }
}

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
    printf("[-P]\t\t\t\t  Restricicao dos dados\n");
    printf("[-i]\t\t\t\t  Leitura do ficheiro\n");
    printf("[-o]\t\t\t\t  Escrita do ficheiro\n");
}

int main(int argc, char *argv[])
{
    settings_t *settings = malloc(sizeof(settings_t));
    int opt, binario = 0;
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
    settings->criterio_res = P_NONE;

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
                settings->leitura_continente = malloc(sizeof(char) * (strlen(criterio_L) + 1));
                strcpy(settings->leitura_continente, criterio_L); // continente especifico
            }
            break;
        case 'S':
            sscanf(optarg, "%s", criterio_S);
            if (strcmp(criterio_S, "alfa") == 0)
                settings->criterio_ord = S_ALFA; // ordenacao por ordem alfabetica
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
            settings->criterio_file = malloc(sizeof(char) * (strlen(criterio_FILE) + 1));
            strcpy(settings->criterio_file, criterio_FILE);
            break;
        case 'o': // escrita do ficheiro
            sscanf(optarg, "%s", criterio_WRITE);
            settings->criterio_write = malloc(sizeof(char) * (strlen(criterio_WRITE) + 1));
            strcpy(settings->criterio_write, criterio_WRITE);
            break;
        default:
            utilizacao(argv[0]);
            return EXIT_FAILURE;
        }
    }

    settings = verifica_tipo_ficheiro(settings, &binario);
    lista_t *root_principal = ler_ficheiro(settings);
    erros_ficheiro(root_principal);
    if(binario == 0)// e binario
    {
        if (settings->criterio_sel != D_NONE)
            selecionar(settings, root_principal);
        if(settings->criterio_res != P_NONE)
            root_principal = restricao_lista(root_principal, settings);
        root_principal = ordenar_lista(root_principal, settings);
    }
    cria_ficheiro(root_principal, settings);
    imprime_lista(root_principal);
    liberta_lista(root_principal);
    return 0;
}
