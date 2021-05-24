#ifndef headers
#define headers

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

typedef struct dados
{
    char *indicator;
    int weekly_count;
    yearWeek_t *year_week;
    double rate_14_day;
    int cumulative_count;

} dados_t;

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

enum leitura
{
    L_ALL,
    L_CONTINENTE //---------------------nao estamos a utilizar----------------------------
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
    yearWeek_t *ord_date;
    enum selecao criterio_sel;   //-D
    enum restricao criterio_res; //-P
    int restricao_nmin;          //numero de habitantes
    int restricao_nmax;
    yearWeek_t *restricao_date1; //data 1
    yearWeek_t *restricao_date2; //data 2
    char *criterio_file;         //nome do ficheiro a ler
    char *criterio_write;        //nome do ficheiro a escrever
    char *tipo_ficheiro;         //tipo de ficheiro a ler(.csv ou .dat)
    char *tipo_escrita;          //tipo de ficheiro a escrever(.csv ou .dat)
} settings_t;
//listas--------------------
lista_t *cria_lista();
pais_t *cria_pais();
void inserir_elemento_final(lista_t *lista, void *item);
lista_t *ler_ficheiro(settings_t *settings);
void ler_linha(settings_t *settings, lista_t *lista, char *letra);
void inserir_dados(pais_t *pais, dados_t *dados, char *inicio_coluna, int coluna);
yearWeek_t *parseYearWeek(char *dados);
settings_t *troca_datas(settings_t *datas);
settings_t *verifica_datas(settings_t *datas);
node_t *troca(node_t *left, node_t *right);
dados_t *remove_do_inicio(dados_t *headlist);
void cria_ficheiro(lista_t *root, settings_t *settings);
void apagar_elemento_lista(lista_t *lista, node_t *elemento, void (*destruir_fn)(void *));
void liberta_lista(lista_t *lista, void (*destruir_fn)(void *));
void destruir_dados(void *p_dados);
void destruir_pais(void *p_pais);
//funcionalidades------------
void ordenacao_pop(dados_t **right, dados_t **left, int *flag);
void ordenacao_alfa(dados_t **right, dados_t **left, int *flag);
lista_t *ordenar_lista(lista_t *root, settings_t *anosemana);
void restricao_min(lista_t *lista, dados_t **head, settings_t *settings);
void restricao_max(lista_t *lista, dados_t **head, settings_t *settings);
void restricao_date(lista_t *lista, dados_t **head, settings_t *settings);
void restricao_dates(lista_t *lista, dados_t **head, settings_t *settings);
lista_t *restricao_lista(lista_t *root, settings_t *anosemana);
short selecao_inf(void *p_atual, void *p_comparacao);
short (*criterio_selecao(settings_t *settings))(void *, void *);
void selecionar(settings_t *settings, lista_t *lista);
dados_t *selecionar_pais(lista_t *lista_dados, short (*cmp_fn)(void *, void *));
//main-----------------------
void liberta_settings(settings_t *settings);
void erros_ficheiro(lista_t *lista);
settings_t *verifica_tipo_ficheiro(settings_t *settings, int *binario);
void utilizacao();
settings_t *init_settings();

void imprime_lista_paises(lista_t *lista);
//---------------------------
void imprime_lista(lista_t *lista); //para apagar

#endif
