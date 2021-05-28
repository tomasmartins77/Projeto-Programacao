#include "headers.h"

/** \brief selecionar, para cada pais, a semana com mais infetados*/
int selecao_inf(void *p_atual, void *p_comparacao)
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

/** \brief selecionar, para cada pais, a semana com menos infetados*/
int selecao_dea(void *p_atual, void *p_comparacao)
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

/** \brief selecionar para cada país a semana com maior rácio de infectados por 100000
 *         habitantes*/
int selecao_racio_inf(void *p_atual, void *p_comparacao)
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

/** \brief selecionar para cada país a semana com maior rácio de mortes por milhão de
 *         habitantes*/
int selecao_racio_dea(void *p_atual, void *p_comparacao)
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

/** \brief menu que verifica qual a selecao a fazer*/
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

/** \brief loop que faz a selecao da lista*/
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

/** \brief saber se estamos no mesmo pais*/
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

//--------------------------------------------

/** \brief apenas dados de países com mais de n mil habitantes*/
void restricao_min(lista_t *lista, node_t *node, settings_t *settings)
{
    pais_t *pais = node->value;
    if (pais->population < settings->restricao_nmin) // se menor que o minimo, apaga
        apagar_elemento_lista(lista, node, destruir_pais);
}

/** \brief apenas dados de países com menos de n mil habitantes*/
void restricao_max(lista_t *lista, node_t *node, settings_t *settings)
{
    pais_t *pais = node->value;
    if (pais->population > settings->restricao_nmax) // se maior que o maximo, apaga
        apagar_elemento_lista(lista, node, destruir_pais);
}

/** \brief  apenas dados relativos a semana indicada*/
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
            apagar_elemento_lista(pais->dados, aux, destruir_dados); // se nao for a semana indicada, apaga o node que contem os dados
    }
}

/** \brief verifica se a semana do node em que estamos esta entre as datas indicadas*/
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

/** \brief apenas dados entre as semanas indicadas*/
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
            apagar_elemento_lista(pais->dados, aux, destruir_dados); // se nao estiver entre as semanas indicadas, apaga o node que contem os dados
    }
}

/** \brief restringe a lista de acordo com varias especificacoes referidas pelo utilizador*/
void restricao_lista(lista_t *lista, settings_t *settings)
{
    node_t *curr, *aux;

    settings->restricao_nmin *= 1000; // n mil habitantes
    settings->restricao_nmax *= 1000; //se por acaso nao for escolhido um deles,
    //como sao inicializados a 0, nao muda o valor, e nao sao utilizados na mesma

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
            settings = verifica_datas(settings); // verifica se tem de trocar as datas
            restricao_dates(lista, aux, settings);
        }
    }
}

//----------------------------------------------------

/** \brief vai buscar um determinado node na semana indicada*/
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

/** \brief ordena dois nodes de acordo com a populacao decrescente, se a populacao do node right for menor
 *          que a do node right->next, trocam*/
int ordenacao_pop(pais_t *atual, pais_t *comparacao)
{
    int dif = comparacao->population - atual->population;

    if (dif == 0) //se tiver a mesma populacao
        return ordenacao_alfa(atual, comparacao);

    return dif;
}

/** \brief ordena dois nodes de por ordem alfabetica, se a letra do node atual estiver mais abaixo no alfabeto
 *          que a do node de comparacao, trocam*/
int ordenacao_alfa(pais_t *atual, pais_t *comparacao)
{
    return strcmp(atual->country, comparacao->country);
}

/** \brief ordena dois nodes em relacao a quantidade de mortes ou casos numa determinada semana*/
int ordenacao_inf_dea(settings_t *settings, pais_t *atual, pais_t *comparacao, char *indicator)
{
    dados_t *atual_dados = obter_dados_semana(atual->dados, settings->ord_date, indicator);
    dados_t *comparacao_dados = obter_dados_semana(comparacao->dados, settings->ord_date, indicator);

    int dif = (atual_dados == NULL ? 0 : comparacao_dados->weekly_count) - (comparacao_dados == NULL ? 0 : atual_dados->weekly_count);

    if (dif == 0) //se tiver os mesmos valores
        return ordenacao_alfa(atual, comparacao);

    return dif;
}

/** \brief menu que verifica que modo de ordenacao foi escolhido*/
int criterio_ordenacao(settings_t *settings, pais_t *atual, pais_t *comparacao)
{
    if (settings->criterio_ord == S_ALFA) //ordem alfabetica
        return ordenacao_alfa(atual, comparacao);
    if (settings->criterio_ord == S_POP) //ordem decrescente de populacao
        return ordenacao_pop(atual, comparacao);
    if (settings->criterio_ord == S_INF) //ordem decrescente de casos numa data
        return ordenacao_inf_dea(settings, atual, comparacao, "cases");
    if (settings->criterio_ord == S_DEA) //ordem decrescente de mortes numa data
        return ordenacao_inf_dea(settings, atual, comparacao, "deaths");
    return 0;
}

/** \brief ordena a lista inteira*/
lista_t *ordenar_lista(lista_t *root, settings_t *settings)
{
    node_t *el_atual, *el_comparacao, *aux;

    lista_t *nova_lista = cria_lista();

    el_atual = root->first;

    while (el_atual != NULL)
    {
        aux = el_atual;
        el_comparacao = el_atual->next;

        while (el_comparacao != NULL)
        {
            if (criterio_ordenacao(settings, aux->value, el_comparacao->value) > 0)
                aux = el_comparacao; //el_comparacao é menor do que aux
            el_comparacao = el_comparacao->next;
        }
        inserir_elemento_final(nova_lista, aux->value);
        apagar_elemento_lista(root, aux, NULL);
        el_atual = root->first;
    }
    free(root);
    return nova_lista;
}
