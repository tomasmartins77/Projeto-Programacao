#include "headers.h"

/** \brief verifica quantos nodes a lista contem*/
int tamanho_lista(lista_t *lista)
{
    int count = 0;
    node_t *curr = lista->first;
    while (curr != NULL)
    {
        count++; // contador de nodes
        curr = curr->next;
    }
    return count; // quantidade de nodes da lista
}

/** \brief apaga um certo elemento da lista*/
void apagar_elemento_lista(lista_t *lista, node_t *elemento, void (*destruir_fn)(void *))
{
    if (elemento->prev == NULL) // se for o primeiro, o primeiro passa para o seguinte
        lista->first = elemento->next;
    else
        elemento->prev->next = elemento->next; // o anterior liga ao seguinte

    if (elemento->next == NULL)
        lista->last = elemento->prev; // se for o ultimo, o ultimo passa a ser o anterior
    else
        elemento->next->prev = elemento->prev; // o seguinte liga ao anterior
    if (destruir_fn != NULL)
        destruir_fn(elemento->value);
    free(elemento); // destroi o node
}

/** \brief liberta a lista da memoria*/
void liberta_lista(lista_t *lista, void (*destruir_fn)(void *))
{
    node_t *curr;
    node_t *next = lista->first;

    while (next != NULL) // loop que liberta node a node
    {
        curr = next;
        next = next->next;
        destruir_fn(curr->value);
        free(curr);
    }
    free(lista);
}

/** \brief liberta um determinado node*/
void destruir_dados(void *p_dados)
{
    dados_t *dados = p_dados;
    free(dados->indicator);
    free(dados->year_week);
    free(dados);
}

/** \brief liberta da memoria um determinado node de pais*/
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

/** \brief insere um node no final da lista*/
void inserir_elemento_final(lista_t *lista, void *item)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));

    if (node == NULL)
    {
        fprintf(stderr, "Erro a alocar memoria a node\n");
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
        lista->last->next = node; // insere o node no final
    }
    lista->last = node; // torna esse node o ultimo pois inserimos no final
}

/** \brief cria um node de pais novo*/
pais_t *cria_pais()
{
    pais_t *pais = (pais_t *)malloc(sizeof(pais_t));
    if (pais == NULL)
    {
        fprintf(stderr, "Erro a alocar memoria a pais\n");
        exit(EXIT_FAILURE);
    }
    pais->country = NULL; // inicializa o novo pais vazio
    pais->country_code = NULL;
    pais->continent = NULL;
    pais->population = 0;
    pais->dados = NULL;
    return pais;
}

/** \brief cria uma lista vazia*/
lista_t *cria_lista()
{
    lista_t *novaLista = (lista_t *)malloc(sizeof(lista_t));
    if (novaLista == NULL)
    {
        fprintf(stderr, "Erro a alocar memoria para novalista\n");
        exit(EXIT_FAILURE);
    }
    novaLista->first = NULL; // inicializa a lista vazia
    novaLista->last = NULL;

    return novaLista;
}

/** \brief insere um novo pais e dados na lista*/
void insere_pais_dados_lista(lista_t *lista, pais_t *pais, dados_t *dados)
{
    node_t *aux = lista->first;

    while (aux != NULL && strcmp(pais->country_code, ((pais_t *)aux->value)->country_code) != 0)
    {
        aux = aux->next; // se nao for o pais, passa para o proximo node
    }
    if (aux == NULL) // se a lista estiver vazia
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

/** \brief inicializa todas as settings do programa*/
settings_t *init_settings()
{
    settings_t *settings = (settings_t *)malloc(sizeof(settings_t));
    if (settings == NULL)
    {
        fprintf(stderr, "Erro a alocar memoria a settings\n");
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

    return settings; // inicializa as settings vazias
}

/** \brief  converte o input dado (string) num int para facilitar a comparacao de datas*/
yearWeek_t *parseYearWeek(char *dados)
{
    yearWeek_t *yearWeek = malloc(sizeof(yearWeek_t));
    if (yearWeek == NULL)
    {
        fprintf(stderr, "Erro a alocar memoria a yearweek\n");
        exit(EXIT_FAILURE);
    }
    dados[4] = '\0';                  //'-'
    yearWeek->year = atoi(dados);     //int
    yearWeek->week = atoi(dados + 5); //int

    return yearWeek;
}

/** \brief troca as datas recebidas dos argumentos*/
settings_t *troca_datas(settings_t *datas)
{
    yearWeek_t *aux;
    aux = datas->restricao_date1;                    // guarda a data num auxiliar
    datas->restricao_date1 = datas->restricao_date2; // mete a data do segundo no primeiro
    datas->restricao_date2 = aux;                    // mete a data do aux(data 1) no data 2
    return datas;
}

/** \brief verifica se a data precisa ser trocada*/
settings_t *verifica_datas(settings_t *datas)
{
    if (datas->restricao_date1->year > datas->restricao_date2->year) //ano 1 e maior que o 2
    {
        datas = troca_datas(datas);
    }
    else if (datas->restricao_date1->year == datas->restricao_date2->year) //mesmo ano
    {
        if (datas->restricao_date1->week > datas->restricao_date2->week) //mas semana 1 maior que a 2
        {
            datas = troca_datas(datas);
        }
    }
    //se nao entrar em nenhum, as datas nao precisam ser trocadas
    return datas;
}

/** \brief liberta memoria das settings*/
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
