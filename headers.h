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

/** \brief insere um novo pais e dados na lista
 *
 * \param lista lista_t* lista principal
 * \param pais pais_t* pais novo
 * \param dados dados_t* dados desse pais
 * \return void
 *
 */
void insere_pais_dados_lista(lista_t *lista, pais_t *pais, dados_t *dados);

/** \brief liberta memoria das settings
 *
 * \param settings settings_t*
 * \return void
 *
 */
void liberta_settings(settings_t *settings);

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

//funcionalidades-----------------------------------------------------------

//ordenacao

/** \brief ordena dois nodes de acordo com a populacao decrescente, se a populacao do node right for menor
 *          que a do node right->next, trocam
 *
 * \param atual pais_t* node atual
 * \param comparacao pais_t* node de comparacao
 * \return int diferenca de populacoes
 *
 */
int ordenacao_pop(pais_t *atual, pais_t *comparacao);

/** \brief ordena dois nodes de por ordem alfabetica, se a letra do node atual estiver mais abaixo no alfabeto
 *          que a do node de comparacao, trocam
 *
 * \param atual pais_t* atual
 * \param comparacao pais_t* comparacao
 * \return int strcmp entre os dois paises
 *
 */
int ordenacao_alfa(pais_t *atual, pais_t *comparacao);

/** \brief ordena dois nodes em relacao a quantidade de mortes ou casos numa determinada semana
 *
 * \param settings settings_t* data
 * \param atual pais_t* atual
 * \param comparacao pais_t* comparacao
 * \param indicator char* casos ou mortes
 * \return int diferenca de mortes ou casos entre os dois nodes
 *
 */
int ordenacao_inf_dea(settings_t *settings, pais_t *atual, pais_t *comparacao, char *indicator);

/** \brief menu que verifica que modo de ordenacao foi escolhido
 *
 * \param settings settings_t* qual o modo escolhido
 * \param atual pais_t* atual
 * \param comparacao pais_t* comparacao
 * \return int resultado da ordenacao escolhida
 *
 */
int criterio_ordenacao(settings_t *settings, pais_t *atual, pais_t *comparacao);

/** \brief ordena a lista inteira
 *
 * \param root lista_t* lista principal
 * \param settings settings_t* qual o modo de ordenacao
 * \return lista_t* lista ordenada
 *
 */
lista_t *ordenar_lista(lista_t *root, settings_t *settings);

//restricao

/** \brief apenas dados de países com mais de n mil habitantes
 *
 * \param lista lista_t* primeiro ou ultimo elemento da lista
 * \param node node_t* elemento que estamos neste momento
 * \param settings settings_t* numero de habitantes minimo
 * \return void
 *
 */
void restricao_min(lista_t *lista, node_t *node, settings_t *settings);

/** \brief apenas dados de países com menos de n mil habitantes
 *
 * \param lista lista_t* primeiro ou ultimo elemento da lista
 * \param node node_t* elemento que estamos neste momento
 * \param settings settings_t* numero de habitantes maximo
 * \return void
 *
 */
void restricao_max(lista_t *lista, node_t *node, settings_t *settings);

/** \brief  apenas dados relativos a semana indicada
 *
 * \param lista lista_t* primeiro ou ultimo elemento da lista
 * \param node node_t* elemento que estamos neste momento
 * \param settings settings_t* semana especificada pelo utilizador
 * \return void
 *
 */
void restricao_date(lista_t *lista, node_t *node, settings_t *settings);

/** \brief apenas dados entre as semanas indicadas
 *
 * \param lista lista_t* primeiro ou ultimo elemento da lista
 * \param node node_t* elemento que estamos neste momento
 * \param settings settings_t* semanas que restringem os dados
 * \return void
 *
 */
void restricao_dates(lista_t *lista, node_t *node, settings_t *settings);

/** \brief verifica se a semana do node em que estamos esta entre as datas indicadas
 *
 * \param data yearWeek_t* data do note em que nos encontramos
 * \param data1 yearWeek_t* data mais nova
 * \param data2 yearWeek_t* data mais velha
 * \return short 0 se nao estiver no intervalo, 1 se estiver
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
 * \param p_atual void* atual
 * \param p_comparacao void* comparacao
 * \return int: 0,1-------------------------------------------------------------------------------
 *
 */
int selecao_inf(void *p_atual, void *p_comparacao);

/** \brief selecionar, para cada pais, a semana com menos infetados
 *
 * \param atual dados_t*
 * \param comparacao dados_t*
 * \return int: 0,1------------------------------------------------------------------------
 *
 */
int selecao_dea(void *p_atual, void *p_comparacao);

/** \brief selecionar para cada país a semana com maior rácio de infectados por 100000
 *         habitantes
 *
 * \param p_atual void* atual
 * \param p_comparacao void* comparacao
 * \return int 0,1------------------------------------------------------------------------
 *
 */
int selecao_racio_inf(void *p_atual, void *p_comparacao);

/** \brief selecionar para cada país a semana com maior rácio de mortes por milhão de
 *         habitantes
 *
 * \param p_atual void* atual
 * \param p_comparacao void* comparacao
 * \return int 0,1--------------------------------------------------------------------------------
 *
 */
int selecao_racio_dea(void *p_atual, void *p_comparacao);

/** \brief menu que verifica qual a selecao a fazer
 *
 * \param settings settings_t* para saber qual o tipo de selecao
 * \return compare_fn-----------------------------------------------
 *
 */
compare_fn criterio_selecao(settings_t *settings);

/** \brief loop que faz a selecao da lista
 *
 * \param settings settings_t*
 * \param lista lista_t*
 * \return void
 *
 */
void selecionar(settings_t *settings, lista_t *lista);

/** \brief ---------------------------------------------------------------------------------
 *
 * \param lista_dados lista_t*
 * \param cmp_fn compare_fn
 * \return node_t*
 *
 */
node_t *selecionar_pais(lista_t *lista_dados, compare_fn cmp_fn);

/** \brief vai buscar um determinado node na semana indicada
 *
 * \param lista lista_t* lista completa
 * \param yearWeek yearWeek_t* semana especifica
 * \param indicator char* mortes ou casos
 * \return dados_t* node com os dados num certa semana
 *
 */
dados_t *obter_dados_semana(lista_t *lista, yearWeek_t *yearWeek, char *indicator);

//lercriarficheiros-------------------------------------------------------------------------------

/** \brief funcao que le o ficheiro e o coloca por completo numa lista, no caso de se ter
 *         escolhido a opcao -L "continente", cria a lista apenas para esse continente
 *
 * \param settings settings_t* definicoes para saber se a lista é ALL ou "continente"
 * \return lista_t* lista criada de acordo com os argumentos da linha de comando
 *
 */
lista_t *ler_ficheiro(settings_t *settings);

/** \brief le a linha, divide-a em varias strings e coloca-as nas diferentes variaveis de um node
 *
 * \param settings settings_t* saber o continente a ler
 * \param lista lista_t* insere na lista principal
 * \param letra char* linha lida do ficheiro
 * \return void
 *
 */
void ler_linha(settings_t *settings, lista_t *lista, char *letra);

/** \brief insere um determinado dado na variavel correta da lista
 *
 * \param dados dados_t* lista para onde vao os dados do ficheiro
 * \param inicio_coluna char* palavra que vai entrar em cada variavel
 * \param coluna int indica em que coluna estamos de modo a colocar o valor na variavel correta
 * \return void
 *
 */
void inserir_dados(pais_t *pais, dados_t *dados, char *inicio_coluna, int coluna);

/** \brief cria um ficheiro
 *
 * \param root lista_t* lista que vai criar o ficheiro
 * \param settings settings_t* saber qual o nome do ficheiro
 * \return void
 *
 */
void cria_ficheiro(lista_t *root, settings_t *settings);

/** \brief le ficheiro .csv
 *
 * \param settings settings_t* para usar noutra funcao
 * \param file FILE* ficheiro
 * \param lista lista_t* onde vai ser colocado os dados do ficheiro
 * \return void
 *
 */
void ler_ficheiro_csv(settings_t *settings, FILE *file, lista_t *lista);

/** \brief le ficheiro .dat
 *
 * \param file FILE* ficheiro
 * \param lista lista_t* onde vai ser colocado os dados do ficheiro
 * \return void
 *
 */
void ler_ficheiro_dat(FILE *file, lista_t *lista);

/** \brief cria um ficheiro .dat
 *
 * \param paises lista_t*
 * \param file FILE*
 * \return void
 *
 */
void escreve_ficheiro_dat(lista_t *paises, FILE *file);

/** \brief cria um ficheiro .csv
 *
 * \param paises lista_t* lista que vai criar o ficheiro
 * \param file FILE* ficherio
 * \return void
 *
 */
void escreve_ficheiro_csv(lista_t *paises, FILE *file);

//main---------------------------------------------------------------------------------------------

/** \brief le os argumentos da linha de comandos
 *
 * \param argc int
 * \param argv[] char*
 * \param settings settings_t* struct onde vao ser guardadas todas as variaveis dos argumentos
 * \return void
 *
 */
void argumentos(int argc, char *argv[], settings_t *settings);

/** \brief verifica se existe algum erro de escrita no ficheiro lido
 *
 * \param lista lista_t* lista com o ficheiro inteiro
 * \param settings settings_t* se houver erro, da free
 * \return void
 *
 */
void erros_ficheiro(lista_t *lista, settings_t *settings);

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

/** \brief verifica se o ficheiro possui alguma letra onde apenas deviam existir numeros
 *
 * \param numero char* string que possui o numero que vai ser verificado
 * \param contador int coluna do ficheiro onde nos encontramos
 * \return int 0 se existir uma letra e 1 se estiver tudo bem
 *
 */
int erro_letra_em_numero(char *numero, int contador);

/** \brief verifica se o continente escolhido e valido
 *
 * \param continente char* continente escolhido
 * \return int 0 se nao for valido e 1 se for valido
 *
 */
void verifica_L(char* continente);

/** \brief verifica se a ordenacao escolhida e valida
 *
 * \param ordenacao char* ordenacao escolhida
 * \return void
 *
 */
void verfica_S(char *ordenacao);

/** \brief verifica se a selecao escolhida e valida
 *
 * \param selecao char* selecao escolhida
 * \return void
 *
 */
void verifica_D(char *selecao);

/** \brief verifica se a restricao escolhida e valida
 *
 * \param restricao char* restricao escolhida
 * \return void
 *
 */
void verifica_P(char *restricao);

/** \brief verifica se existe alguma falta de valores num argumento da linha de comando
 *
 * \param word char* argv[optind]
 * \return void
 *
 */
void erro_argumento(char *word);

/** \brief verifica se possui algum valor em lugares onde nao devia haver
 *
 * \param word char* argv[optind]
 * \return void
 *
 */
void verifica_argumento(char *word);

/** \brief verifica se a linha de argumentos possui -i ou -o
 *
 * \param settings settings_t*
 * \return void
 *
 */
void erros_ficheiros_argumentos(settings_t *settings);

#endif
