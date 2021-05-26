#include "headers.h"

/** \brief selecionar, para cada pais, a semana com mais infetados
 *
 * \param atual dados_t*
 * \param comparacao dados_t*
 * \return int: 0,1,2
 *
 */
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

/** \brief
 *
 * \param p_atual void*
 * \param p_comparacao void*
 * \return int
 *
 */
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

/** \brief
 *
 * \param p_atual void*
 * \param p_comparacao void*
 * \return int
 *
 */
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

/** \brief
 *
 * \param p_atual void*
 * \param p_comparacao void*
 * \return int
 *
 */
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

/** \brief
 *
 * \param lista_dados lista_t*
 * \param cmp_fn compare_fn
 * \return node_t*
 *
 */
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
            apagar_elemento_lista(pais->dados, aux, destruir_dados); // se nao for a semana indicada, apaga o node que contem os dados
    }
}

/** \brief
 *
 * \param data yearWeek_t*
 * \param data1 yearWeek_t*
 * \param data2 yearWeek_t*
 * \return short
 *
 */
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
            apagar_elemento_lista(pais->dados, aux, destruir_dados); // se nao for a semana indicada, apaga o node que contem os dados
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
//----------------------------------------------------

/** \brief ordena dois nodes de acordo com a populacao decrescente, se a populacao do node right for menor
 *          que a do node right->next, trocam
 *
 * \param right dados_t**
 * \param left dados_t**
 * \param flag int* flag que mantem o loop a verificar a lista inteira
 * \return void
 *
 */
int ordenacao_pop(pais_t *atual, pais_t *comparacao)
{
    int dif = atual->population - comparacao->population;

    if (dif == 0)
        return ordenacao_alfa(atual, comparacao);

    return dif;
}

/** \brief ordena a lista por ordem alfabetica
 *
 * \param right dados_t**
 * \param left dados_t**
 * \param flag int* flag que mantem o loop a verificar a lista inteira
 * \return void
 *
 */
int ordenacao_alfa(pais_t *atual, pais_t *comparacao)
{
    return strcmp(atual->country, comparacao->country);
}

/** \brief
 *
 * \param settings settings_t*
 * \param atual pais_t*
 * \param comparacao pais_t*
 * \param indicator char*
 * \return int
 *
 */
int ordenacao_inf_dea(settings_t *settings, pais_t *atual, pais_t *comparacao, char *indicator)
{

    dados_t *atual_dados = obter_dados_semana(atual->dados, settings->ord_date, indicator);
    dados_t *comparacao_dados = obter_dados_semana(comparacao->dados, settings->ord_date, indicator);

    int dif = (atual_dados == NULL ? 0 : atual_dados->weekly_count) - (comparacao_dados == NULL ? 0 : comparacao_dados->weekly_count);

    if (dif == 0)
        return ordenacao_alfa(atual, comparacao);

    return dif;
}

/** \brief
 *
 * \param settings settings_t*
 * \param atual pais_t*
 * \param comparacao pais_t*
 * \return int
 *
 */
int criterio_ordenacao(settings_t *settings, pais_t *atual, pais_t *comparacao)
{
    if (settings->criterio_ord == S_ALFA)
        return ordenacao_alfa(atual, comparacao);
    if (settings->criterio_ord == S_POP)
        return ordenacao_pop(atual, comparacao);
    if (settings->criterio_ord == S_INF)
        return ordenacao_inf_dea(settings, atual, comparacao, "cases");
    if (settings->criterio_ord == S_DEA)
        return ordenacao_inf_dea(settings, atual, comparacao, "deaths");
    return 0;
}

/** \brief ordena a lista
 *
 * \param root lista_t*
 * \param settings settings_t*
 * \return lista_t*
 *
 */
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
