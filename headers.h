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

typedef int (*compare_fn)(void *, void *);

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
void troca(lista_t *lista, node_t *el1, node_t *el2);
dados_t *remove_do_inicio(dados_t *headlist);
void cria_ficheiro(lista_t *root, settings_t *settings);
void apagar_elemento_lista(lista_t *lista, node_t *elemento, void (*destruir_fn)(void *));
void liberta_lista(lista_t *lista, void (*destruir_fn)(void *));
void destruir_dados(void *p_dados);
void destruir_pais(void *p_pais);
//funcionalidades------------
int ordenacao_pop(pais_t *atual, pais_t *comparacao);
int ordenacao_alfa(pais_t *atual, pais_t *comparacao);
int ordenacao_inf_dea(settings_t *settings, pais_t *atual, pais_t *comparacao, char *indicator);
lista_t *ordenar_lista(lista_t *root, settings_t *settings);
void restricao_min(lista_t *lista, node_t *node, settings_t *settings);
void restricao_max(lista_t *lista, node_t *node, settings_t *settings);
void restricao_date(lista_t *lista, node_t *node, settings_t *settings);
void restricao_dates(lista_t *lista, node_t *node, settings_t *settings);
void restricao_lista(lista_t *lista, settings_t *settings);
int selecao_inf(void *p_atual, void *p_comparacao);
compare_fn criterio_selecao(settings_t *settings);
int criterio_ordenacao(settings_t *settings, pais_t *atual, pais_t *comparacao);
void selecionar(settings_t *settings, lista_t *lista);
node_t *selecionar_pais(lista_t *lista_dados, compare_fn cmp_fn);
dados_t *obter_dados_semana(lista_t *lista, yearWeek_t *yearWeek, char *indicator);
//main-----------------------
void liberta_settings(settings_t *settings);
void erros_ficheiro(lista_t *lista);
settings_t *verifica_tipo_ficheiro(settings_t *settings, int *binario);
void utilizacao();
settings_t *init_settings();

void imprime_lista_paises(lista_t *lista);
//---------------------------
void imprime_lista(lista_t *lista); //para apagar

void escreve_ficheiro_dat(lista_t *paises, FILE *file);
void escreve_ficheiro_csv(lista_t *paises, FILE *file);
int tamanho_lista(lista_t *lista);
void ler_ficheiro_csv(settings_t *settings, FILE *file, lista_t *lista);
void ler_ficheiro_dat(settings_t *settings, FILE *file, lista_t *lista);

#endif
