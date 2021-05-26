#ifndef headers
#define headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define MAX_PALAVRAS_LINHAS 150
#define COUNTRY 35
#define COUNTRY_CODE 4
#define CONTINENT 8
#define INDICATOR 7

typedef struct yearWeek
{
    int year;
    int week;
} yearWeek_t;

typedef struct node
{
    struct node *prev;
    struct node *next;

    void *value;
} node_t;

typedef struct lista
{
    node_t *first;
    node_t *last;
} lista_t;

typedef struct pais
{
    char *country;
    char *country_code;
    char *continent;
    int population;

    lista_t *dados;
} pais_t;

typedef struct dados
{
    char *indicator;
    int weekly_count;
    yearWeek_t *year_week;
    double rate_14_day;
    int cumulative_count;

    pais_t *pais;
} dados_t;

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
    P_NONE,
    P_MIN,
    P_MAX,
    P_DATE,
    P_DATES
};

typedef struct settings
{
    enum leitura criterio_leitura; // -L
    char *leitura_continente;      //continente escolhido
    enum ordenacao criterio_ord;   //-S
    yearWeek_t *ord_date;          //date da ordenacao
    enum selecao criterio_sel;     //-D
    enum restricao criterio_res;   //-P
    int restricao_nmin;            //numero de habitantes minimo
    int restricao_nmax;            //numero de habitantes maximo
    yearWeek_t *restricao_date1;   //data 1
    yearWeek_t *restricao_date2;   //data 2
    char *criterio_file;           //nome do ficheiro a ler
    char *criterio_write;          //nome do ficheiro a escrever
    char *tipo_ficheiro;           //tipo de ficheiro a ler(.csv ou .dat)
    char *tipo_escrita;            //tipo de ficheiro a escrever(.csv ou .dat)
} settings_t;

typedef int (*compare_fn)(void *, void *);

//listas----------------------------------------------------------

/** \brief cria uma lista vazia
 *
 * \return lista_t* lista vazia
 *
 */
lista_t *cria_lista();

/** \brief cria um node de pais novo
 *
 * \return pais_t* pais novo
 *
 */
pais_t *cria_pais();

/** \brief insere um node no final da lista
 *
 * \param lista lista_t* lista na qual vamos adicionar o elemento
 * \param item void* elemento a adicionar na lista
 * \return void
 *
 */
void inserir_elemento_final(lista_t *lista, void *item);

/** \brief apaga um certo elemento da lista
 *
 * \param lista lista_t* primeiro ou ultimo elemento da lista
 * \param elemento dados_t* elemento a ser apagado
 * \return void
 *
 */
void apagar_elemento_lista(lista_t *lista, node_t *elemento, void (*destruir_fn)(void *));

/** \brief liberta a lista da memoria
 *
 * \param lista lista_t* lista a ser libertada
 * \return void
 *
 */
void liberta_lista(lista_t *lista, void (*destruir_fn)(void *));

/** \brief liberta um determinado node
 *
 * \param dados dados_t*
 * \return void
 *
 */
void destruir_dados(void *p_dados);

/** \brief liberta da memoria um determinado node de pais
 *
 * \param p_pais void*
 * \return void
 *
 */
void destruir_pais(void *p_pais);

/** \brief verifica quantos nodes a lista contem
 *
 * \param lista lista_t*
 * \return int numero de nodes da lista
 *
 */
int tamanho_lista(lista_t *lista);

void insere_pais_dados_lista(lista_t *lista, pais_t *pais, dados_t *dados);

//funcionalidades-----------------------------------------------------------

//ordenacao

/** \brief ordena dois nodes de acordo com a populacao decrescente, se a populacao do node right for menor
 *          que a do node right->next, trocam
 *
 * \param right dados_t**
 * \param left dados_t**
 * \param flag int* flag que mantem o loop a verificar a lista inteira
 * \return void
 *
 */
int ordenacao_pop(pais_t *atual, pais_t *comparacao);

/** \brief ordena a lista por ordem alfabetica
 *
 * \param right dados_t**
 * \param left dados_t**
 * \param flag int* flag que mantem o loop a verificar a lista inteira
 * \return void
 *
 */
int ordenacao_alfa(pais_t *atual, pais_t *comparacao);

/** \brief
 *
 * \param settings settings_t*
 * \param atual pais_t*
 * \param comparacao pais_t*
 * \param indicator char*
 * \return int
 *
 */
int ordenacao_inf_dea(settings_t *settings, pais_t *atual, pais_t *comparacao, char *indicator);

/** \brief
 *
 * \param settings settings_t*
 * \param atual pais_t*
 * \param comparacao pais_t*
 * \return int
 *
 */
int criterio_ordenacao(settings_t *settings, pais_t *atual, pais_t *comparacao);

/** \brief ordena a lista
 *
 * \param root lista_t*
 * \param settings settings_t*
 * \return lista_t*
 *
 */
lista_t *ordenar_lista(lista_t *root, settings_t *settings);

//restricao

/** \brief apenas dados de países com mais de n mil habitantes
 *
 * \param lista lista_t* primeiro ou ultimo elemento da lista
 * \param head dados_t** elemento que estamos neste momento
 * \param settings settings_t* numero de habitantes
 * \return void
 *
 */
void restricao_min(lista_t *lista, node_t *node, settings_t *settings);

/** \brief apenas dados de países com menos de n mil habitantes
 *
 * \param lista lista_t* primeiro ou ultimo elemento da lista
 * \param head dados_t** elemento que estamos neste momento
 * \param settings settings_t* n maximo
 * \return void
 *
 */
void restricao_max(lista_t *lista, node_t *node, settings_t *settings);

/** \brief  apenas dados relativos à semana indicada
 *
 * \param lista lista_t* primeiro ou ultimo elemento da lista
 * \param head dados_t** elemento que estamos neste momento
 * \param anosemana settings_t* semana especificada pelo utilizador
 * \return void
 *
 */
void restricao_date(lista_t *lista, node_t *node, settings_t *settings);

/** \brief apenas dados entre as semanas indicadas
 *
 * \param lista lista_t* primeiro ou ultimo elemento da lista
 * \param head dados_t** elemento que estamos neste momento
 * \param anosemana settings_t* semanas que restringem os dados
 * \return void
 *
 */
void restricao_dates(lista_t *lista, node_t *node, settings_t *settings);

/** \brief
 *
 * \param data yearWeek_t*
 * \param data1 yearWeek_t*
 * \param data2 yearWeek_t*
 * \return short
 *
 */
short entre_semanas(yearWeek_t *data, yearWeek_t *data1, yearWeek_t *data2);

/** \brief restringe a lista de acordo com varias especificacoes referidas pelo utilizador
 *
 * \param lista lista_t* lista a ser restringida
 * \param settings settings_t* para saber qual o tipo de restricao
 * \return lista_t* lista restringida
 *
 */
void restricao_lista(lista_t *lista, settings_t *settings);

//selecao

/** \brief selecionar, para cada pais, a semana com mais infetados
 *
 * \param atual dados_t*
 * \param comparacao dados_t*
 * \return int: 0,1,2
 *
 */
int selecao_inf(void *p_atual, void *p_comparacao);

/** \brief selecionar, para cada pais, a semana com menos infetados
 *
 * \param atual dados_t*
 * \param comparacao dados_t*
 * \return int: 0,1,2
 *
 */
int selecao_dea(void *p_atual, void *p_comparacao);

/** \brief
 *
 * \param p_atual void*
 * \param p_comparacao void*
 * \return int
 *
 */
int selecao_racio_inf(void *p_atual, void *p_comparacao);

/** \brief
 *
 * \param p_atual void*
 * \param p_comparacao void*
 * \return int
 *
 */
int selecao_racio_dea(void *p_atual, void *p_comparacao);

/** \brief menu que verifica qual a selecao a fazer
 *
 * \param settings settings_t* para saber qual o tipo de selecao
 * \param atual dados_t*
 * \param comparacao dados_t*
 * \return int
 *
 */
compare_fn criterio_selecao(settings_t *settings);

/** \brief
 *
 * \param settings settings_t*
 * \param lista lista_t*
 * \return void
 *
 */
void selecionar(settings_t *settings, lista_t *lista);

/** \brief
 *
 * \param lista_dados lista_t*
 * \param cmp_fn compare_fn
 * \return node_t*
 *
 */
node_t *selecionar_pais(lista_t *lista_dados, compare_fn cmp_fn);

/** \brief
 *
 * \param lista lista_t*
 * \param yearWeek yearWeek_t*
 * \param indicator char*
 * \return dados_t*
 *
 */
dados_t *obter_dados_semana(lista_t *lista, yearWeek_t *yearWeek, char *indicator);

//lercriarficheiros-------------------------------------------------------------------------------

lista_t *ler_ficheiro(settings_t *settings);

void ler_linha(settings_t *settings, lista_t *lista, char *letra);

void inserir_dados(pais_t *pais, dados_t *dados, char *inicio_coluna, int coluna);

void cria_ficheiro(lista_t *root, settings_t *settings);

void ler_ficheiro_csv(settings_t *settings, FILE *file, lista_t *lista);

void ler_ficheiro_dat(settings_t *settings, FILE *file, lista_t *lista);

void escreve_ficheiro_dat(lista_t *paises, FILE *file);

void escreve_ficheiro_csv(lista_t *paises, FILE *file);

//main---------------------------------------------------------------------------------------------

void argumentos(int argc, char *argv[], settings_t *settings);

/** \brief liberta memoria das settings
 *
 * \param settings settings_t*
 * \return void
 *
 */
void liberta_settings(settings_t *settings);

/** \brief verifica se existe algum erro de escrita no ficheiro lido
 *
 * \param lista lista_t* lista com o ficheiro inteiro
 * \param flag int muda se tiver algum erro no ficheiro
 * \return int
 *
 */
int erros_ficheiro(lista_t *lista, int flag);

/** \brief verifica se o ficheiro e .dat ou .csv
 *
 * \param settings settings_t* onde vai ser guardado o tipo de ficheiro
 * \param binario int* flag para saber se o ficheiro e binario
 * \return settings_t*
 *
 */
settings_t *verifica_tipo_ficheiro(settings_t *settings, int *binario);

/** \brief menu de utilizacao
 *
 * \return void
 *
 */
void utilizacao();

/** \brief inicializa todas as settings do programa
 *
 * \return settings_t*
 *
 */
settings_t *init_settings();

/** \brief troca as datas recebidas dos argumentos
 *
 * \param datas settings_t* data a ser trocada
 * \return settings_t* datas data trocada
 *
 */
settings_t *troca_datas(settings_t *datas);

/** \brief verifica se a data precisa ser trocada
 *
 * \param datas settings_t* data a ser verificada
 * \return settings_t* data trocada
 *
 */
settings_t *verifica_datas(settings_t *datas);

/** \brief  converte o input dado (string) num int para facilitar a comparacao de datas
 *
 * \param dados *char string com a data
 * \return yearWeek_t data convertida em dois ints
 *
 */
yearWeek_t *parseYearWeek(char *dados);

int erro_letra_em_numero(char *numero, int contador);

int verifica_L(char* continente);

int verfica_S(char *ordenacao);

int verifica_D(char *selecao);

int verifica_P(char *restricao);

#endif
