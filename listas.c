#include "headers.h"

int tamanho_lista(lista_t *lista)
{
    int count = 0;
    node_t *curr = lista->first;
    while (curr != NULL)
    {
        count++; // contador de nodes
        curr = curr->next;
    }
    return count;
}

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
