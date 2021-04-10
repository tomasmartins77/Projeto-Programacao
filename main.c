#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define DEFAULT_DIMENSOES 9
#define DEFAULT_JOGO_PECA 0
#define DEFAULT_PECAS_TIRO 1

struct peca
{
    char pecas[3][3];
    int tipo;
};
typedef struct
{
    char tabuleiro[15][24] /* = '-' */;
    int linhas, colunas;
} TABULEIRO;
typedef struct
{
    int x;
    int y;
} Celula;
void erro_argumentos(TABULEIRO tabuleiro, int j, int p, int d, int quantidadeTipo[9]);
void utilizacao();
void modos_jogo(TABULEIRO tabuleiro, int j, int p, int d, int quantidadeTipo[9]);
int random_number(int m, int M);
struct peca tipo0();
struct peca tipo1(int variante);
struct peca tipo2(int variante);
struct peca tipo3(int variante);
struct peca tipo4(int variante);
struct peca tipo5(int variante);
struct peca tipo6(int variante);
struct peca tipo7(int variante);
struct peca tipo8();
void modo0_p1(TABULEIRO tabuleiro);
void modo0_p2(TABULEIRO tabuleiro, int quantidadeTipo[9]);
void modo1_p1(TABULEIRO tabuleiro);
void modo1_p2(TABULEIRO tabuleiro, int quantidadeTipo[9]);
void modo_j2(TABULEIRO tabuleiro, int quantidadeTipo[9], int modo_disparo);
void preencher_tabuleiro_p2(TABULEIRO *tabuleiro, int pecas[9]);
struct peca peca_random(int t, int v);
int max_variantes(int tipo);
void meter_peca(char tabuleiro[15][24], int a, int b, struct peca peca);
int selecionar_peca(int pecas[9], int tipos_usados[9]);
void imprimir_tabuleiro(TABULEIRO tabuleiro);
void inicializar_tabuleiro(TABULEIRO *tabuleiro, char peca);
int verificar_final(int quantidadeTipo[9]);
Celula disparar_j2(TABULEIRO *tabuleiro, int modo_disparo, int jogada);
int verificarColisao(char tabuleiro[15][24], int i, int j, struct peca peca);
int restricao2(int count, int linhas, int colunas);
int restricao3(int quantidadeTipo[9]);
int restricao4(int linhas, int colunas, int count);
void print_inicial(int quantidadeTipo[9], TABULEIRO tabuleiro);
int contador_pecas(struct peca);
int contador_peca(int quantidadeTipo[9]);

int main(int argc, char *argv[])
{
    TABULEIRO tabuleiro;
    int opt = 'h';
    opterr = 0;
    int j = DEFAULT_JOGO_PECA,
        p = DEFAULT_PECAS_TIRO, d = DEFAULT_PECAS_TIRO;
    tabuleiro.linhas = DEFAULT_DIMENSOES;
    tabuleiro.colunas = DEFAULT_DIMENSOES;

    int quantidadeTipo[9] = {0};

    srand(time(NULL));

    while ((opt = getopt(argc, argv, "ht:j:p:d:1:2:3:4:5:6:7:8:")) != -1)
    {
        switch (opt)
        {
        case 't':                                                           /* tamanho do tabuleiro*/
            sscanf(optarg, "%dx%d", &tabuleiro.linhas, &tabuleiro.colunas); //predefinicao 9x9??
            break;
        case 'j':                     /* modo de jogo */
            sscanf(optarg, "%d", &j); //modos de jogo sao 0, 1 e 2
            break;
        case 'p': /* modo de posicionamento de pecas */
            sscanf(optarg, "%d", &p);
            break;
        case 'd': /* modo de disparo*/
            sscanf(optarg, "%d", &d);
            break;
        case '1':
            sscanf(optarg, "%d", &quantidadeTipo[1]);
            break;
        case '2':
            sscanf(optarg, "%d", &quantidadeTipo[2]);
            break;
        case '3':
            sscanf(optarg, "%d", &quantidadeTipo[3]);
            break;
        case '4':
            sscanf(optarg, "%d", &quantidadeTipo[4]);
            break;
        case '5':
            sscanf(optarg, "%d", &quantidadeTipo[5]);
            break;
        case '6':
            sscanf(optarg, "%d", &quantidadeTipo[6]);
            break;
        case '7':
            sscanf(optarg, "%d", &quantidadeTipo[7]);
            break;
        case '8':
            sscanf(optarg, "%d", &quantidadeTipo[8]);

            break;
        case 'h': /* help */
        default:  /* opcoes invalidas */
            utilizacao(argv[0]);
            return EXIT_FAILURE;
            break;
        }
    }
    erro_argumentos(tabuleiro, j, p, d, quantidadeTipo);
    modos_jogo(tabuleiro, j, p, d, quantidadeTipo);
    return 0;
}

/*
 * funcao: erro_argumentos()
 * lê os argumentos da linha de comandos e verifica se existe alguma situação
 * de erro
*/

void erro_argumentos(TABULEIRO tabuleiro, int j, int p, int d, int quantidadeTipo[9])
{
    if (tabuleiro.linhas % 3 != 0 || tabuleiro.colunas % 3 != 0 || tabuleiro.linhas < 9 || tabuleiro.linhas > 15 || tabuleiro.colunas < 9 || tabuleiro.colunas > 24)
    {
        utilizacao();
        exit(-1);
    }
    else if(j == 0 || j == 1)
    {
        if(p < 1 || p > 2 || d != 1)
        {
            utilizacao();
            exit(-1);
        }
        if(p == 2 && quantidadeTipo[1] == 0)
        {
            utilizacao();
            exit(-1);
        }
    }
    else if(j == 2)
    {
        if(d < 1 || d > 3)
        {
            utilizacao();
            exit(-1);
        }
    }
    else if(j < 0 || j > 2)
    {
        utilizacao();
        exit(-1);
    }
}
/*
 * funcao: utilizacao()
 * menu de utilização
*/
void utilizacao()
{
    printf("\t\tMEEC WARS\n\n");
    printf("opcoes validas:\n");
    printf("[-h]\t\t\t  ajuda para os jogadores\n");
    printf("[-t linhas colunas]\t  define as dimensoes do tabuleiro(predifinicao = 9x9)\n");
    printf("[-j (0 a 2)]\t\t  define o modo de jogo(predifinicao = 0)\n");
    printf("[-p (1 ou 2)]\t\t  define o o modo de posicionamento das pecas pelo computador(predefinicao = 1)\n");
    printf("[-d (1 a 3)]\t\t  define o modo de disparo das pecas pelo computador(predefinicao = 1)\n");
    printf("[-1 (minimo 1)]\t\t  numero de pecas do tipo 1\n");
    printf("[-2]\t\t\t  numero de pecas do tipo 2\n");
    printf("[-3]\t\t\t  numero de pecas do tipo 3\n");
    printf("[-4]\t\t\t  numero de pecas do tipo 4\n");
    printf("[-5]\t\t\t  numero de pecas do tipo 5\n");
    printf("[-6]\t\t\t  numero de pecas do tipo 6\n");
    printf("[-7]\t\t\t  numero de pecas do tipo 7\n");
    printf("[-8]\t\t\t  numero de pecas do tipo 8\n");
}

/*
 * funcao: modos_jogo()
 * menu que vai para cada modo diferente de jogo ou posicionamento de pecas
 * ou modo de disparo
*/

void modos_jogo(TABULEIRO tabuleiro, int j, int p, int d, int quantidadeTipo[9])
{
    int i, count, r2, r3, r4;

    for (i = 0, count = 0; i < 8; i++)
    {
        count += quantidadeTipo[i];
    }
    quantidadeTipo[0] = tabuleiro.linhas * tabuleiro.colunas / 9 - count; /* da a quantidade de '-' ou seja, pecas tipo 0*/
    r2 = restricao2(count, tabuleiro.linhas, tabuleiro.colunas);
    r3 = restricao3(quantidadeTipo);
    r4 = restricao4(tabuleiro.linhas, tabuleiro.colunas, count);

    if (j == 0 && r2 == 1) // modo de jogo 0
    {
        if (p == 1) // modo de posicionamento 1
        {
            modo0_p1(tabuleiro);
        }
        else if (p == 2 && r3 == 1 && r4 == 1) // modo de posicionamento 2
        {
            modo0_p2(tabuleiro, quantidadeTipo);
        }
        exit(-1);
    }
    else if (j == 1 && r2 == 1) // modo de jogo 1
    {
        if (p == 1) // modo de posicionamento 1
        {
            modo1_p1(tabuleiro);
        }
        else if (p == 2 && r3 == 1 && r4 == 1) // modo de posicionamento 2
        {
            modo1_p2(tabuleiro, quantidadeTipo);
        }
        exit(-1);
    }
    else if (j == 2 && r2 == 1 && r3 == 1 && r4 == 1) // modo de jogo 2
    {
        modo_j2(tabuleiro, quantidadeTipo, d);
    }
    exit(-1);
}
/*
 * funcoes: struct peca tipo(0 a 8)()
 * construção dos diferentes tipos de peças
*/
struct peca tipo0()
{
    struct peca variante0 = {{{'-', '-', '-'}, {'-', '-', '-'}, {'-', '-', '-'}}, 0};
    return variante0;
}

struct peca tipo1(int variante)
{
    struct peca variante1[9] = {{{{'1', '-', '-'}, {'-', '-', '-'}, {'-', '-', '-'}}, 1},
        {{{'-', '1', '-'}, {'-', '-', '-'}, {'-', '-', '-'}}, 1},
        {{{'-', '-', '1'}, {'-', '-', '-'}, {'-', '-', '-'}}, 1},
        {{{'-', '-', '-'}, {'1', '-', '-'}, {'-', '-', '-'}}, 1},
        {{{'-', '-', '-'}, {'-', '1', '-'}, {'-', '-', '-'}}, 1},
        {{{'-', '-', '-'}, {'-', '-', '1'}, {'-', '-', '-'}}, 1},
        {{{'-', '-', '-'}, {'-', '-', '-'}, {'1', '-', '-'}}, 1},
        {{{'-', '-', '-'}, {'-', '-', '-'}, {'-', '1', '-'}}, 1},
        {{{'-', '-', '-'}, {'-', '-', '-'}, {'-', '-', '1'}}, 1}
    };
    int k = variante < 0 ? random_number(0, 8) : variante;
    return variante1[k];
}

struct peca tipo2(int variante)
{

    struct peca variante2[12] = {{{{'2', '2', '-'}, {'-', '-', '-'}, {'-', '-', '-'}}, 2},
        {{{'-', '2', '2'}, {'-', '-', '-'}, {'-', '-', '-'}}, 2},
        {{{'-', '-', '-'}, {'2', '2', '-'}, {'-', '-', '-'}}, 2},
        {{{'-', '-', '-'}, {'-', '2', '2'}, {'-', '-', '-'}}, 2},
        {{{'-', '-', '-'}, {'-', '-', '-'}, {'2', '2', '-'}}, 2},
        {{{'-', '-', '-'}, {'-', '-', '-'}, {'-', '2', '2'}}, 2},
        {{{'2', '-', '-'}, {'2', '-', '-'}, {'-', '-', '-'}}, 2},
        {{{'-', '-', '-'}, {'2', '-', '-'}, {'2', '-', '-'}}, 2},
        {{{'-', '2', '-'}, {'-', '2', '-'}, {'-', '-', '-'}}, 2},
        {{{'-', '-', '-'}, {'-', '2', '-'}, {'-', '2', '-'}}, 2},
        {{{'-', '-', '2'}, {'-', '-', '2'}, {'-', '-', '-'}}, 2},
        {{{'-', '-', '-'}, {'-', '-', '2'}, {'-', '-', '2'}}, 2}
    };

    int k = variante < 0 ? random_number(0, 11) : variante;
    return variante2[k];
}

struct peca tipo3(int variante)
{

    struct peca variante3[6] = {{{{'3', '3', '3'}, {'-', '-', '-'}, {'-', '-', '-'}}, 3},
        {{{'-', '-', '-'}, {'3', '3', '3'}, {'-', '-', '-'}}, 3},
        {{{'-', '-', '-'}, {'-', '-', '-'}, {'3', '3', '3'}}, 3},
        {{{'3', '-', '-'}, {'3', '-', '-'}, {'3', '-', '-'}}, 3},
        {{{'-', '3', '-'}, {'-', '3', '-'}, {'-', '3', '-'}}, 3},
        {{{'-', '-', '3'}, {'-', '-', '3'}, {'-', '-', '3'}}, 3}
    };

    int k = variante < 0 ? random_number(0, 5) : variante;
    return variante3[k];
}

struct peca tipo4(int variante)
{

    struct peca variante4[4] = {{{{'4', '4', '-'}, {'4', '4', '-'}, {'-', '-', '-'}}, 4},
        {{{'-', '4', '4'}, {'-', '4', '4'}, {'-', '-', '-'}}, 4},
        {{{'-', '-', '-'}, {'4', '4', '-'}, {'4', '4', '-'}}, 4},
        {{{'-', '-', '-'}, {'-', '4', '4'}, {'-', '4', '4'}}, 4}
    };

    int k = variante < 0 ? random_number(0, 3) : variante;
    return variante4[k];
}

struct peca tipo5(int variante)
{

    struct peca variante5[4] = {{{{'5', '5', '5'}, {'-', '5', '-'}, {'-', '5', '-'}}, 5},
        {{{'5', '-', '-'}, {'5', '5', '5'}, {'5', '-', '-'}}, 5},
        {{{'-', '5', '-'}, {'-', '5', '-'}, {'5', '5', '5'}}, 5},
        {{{'-', '-', '5'}, {'5', '5', '5'}, {'-', '-', '5'}}, 5}
    };

    int k = variante < 0 ? random_number(0, 3) : variante;
    return variante5[k];
}

struct peca tipo6(int variante)
{

    struct peca variante6[4] = {{{{'-', '6', '-'}, {'6', '-', '6'}, {'6', '6', '6'}}, 6},
        {{{'-', '6', '6'}, {'6', '-', '6'}, {'-', '6', '6'}}, 6},
        {{{'6', '6', '6'}, {'6', '-', '6'}, {'-', '6', '-'}}, 6},
        {{{'6', '6', '-'}, {'6', '-', '6'}, {'6', '6', '-'}}, 6}
    };

    int k = variante < 0 ? random_number(0, 3) : variante;
    return variante6[k];
}

struct peca tipo7(int variante)
{

    struct peca variante7[2] = {{{{'7', '-', '7'}, {'7', '7', '7'}, {'7', '-', '7'}}, 7},
        {{{'7', '7', '7'}, {'-', '7', '-'}, {'7', '7', '7'}}, 7}
    };

    int k = variante < 0 ? random_number(0, 1) : variante;
    return variante7[k];
}

struct peca tipo8()
{
    struct peca variante8 = {{{'8', '8', '8'}, {'8', '-', '8'}, {'8', '8', '8'}}, 8};
    return variante8;
}

void modo0_p1(TABULEIRO tabuleiro)
{

    int a, b, k;
    struct peca peca;

    inicializar_tabuleiro(&tabuleiro, '-');

    for (a = 0; a < tabuleiro.linhas; a += 3)
    {
        for (b = 0; b < tabuleiro.colunas; b += 3)
        {
            for (k = 0; k <= 3; k++)
            {
                if (k == 3)
                {
                    peca = tipo1(4);
                }
                else
                {
                    peca = peca_random(-1, -1);
                    if (!verificarColisao(tabuleiro.tabuleiro, a, b, peca))
                    {
                        break;
                    }
                }
            }
            meter_peca(tabuleiro.tabuleiro, a, b, peca);
            //contador_pecas(peca);
        }
    }
    //printf("\n%d\n", contador_pecas(peca));
    printf("%dX%d\n", tabuleiro.linhas, tabuleiro.colunas);
    imprimir_tabuleiro(tabuleiro);
}

/*
 * funcao: peca_random()
 * recebe uma qualquer peca e retorna
 * uma aleatoria entre entre todas as pecas.
*/

struct peca peca_random(int t, int v)
{

    switch (t < 0 ? random_number(0, 8) : t)
    {
    case 0:
        return tipo0();
        break;
    case 1:
        return tipo1(v);
        break;
    case 2:
        return tipo2(v);
        break;
    case 3:
        return tipo3(v);
        break;
    case 4:
        return tipo4(v);
        break;
    case 5:
        return tipo5(v);
        break;
    case 6:
        return tipo6(v);
        break;
    case 7:
        return tipo7(v);
        break;
    case 8:
        return tipo8(v);
    }
}
/*
int contador_pecas(struct peca peca)
{
    struct peca pecas;
    int conta_pecas[8];

    if(pecas.tipo == peca.tipo)
        conta_pecas[0]+=1;
    return conta_pecas;
}*/

void preencher_tabuleiro_p2(TABULEIRO *tabuleiro, int pecas[9])
{
    int pecas_duplicado[9];
    int i, j, k, a, b;

    for (i = 0; i < 1000; i++)
    {
        int sem_erros = 1;

        for (j = 0; j < 9; j++)
        {
            pecas_duplicado[j] = pecas[j];
        }

        for (a = 0; a < 15; a++) /*reiniciar o tabuleiro a zeros*/
        {
            for (b = 0; b < 24; b++)
            {
                tabuleiro->tabuleiro[a][b] = '-';
            }
        }

        for (a = 0; a < tabuleiro->linhas && sem_erros; a += 3)
        {
            for (b = 0; b < tabuleiro->colunas && sem_erros; b += 3)
            {
                int success_peca = 0;
                int tipos_usados[9] = {0};
                for (j = 0; j < 8; j++)
                {
                    int tipo_peca = selecionar_peca(pecas_duplicado, tipos_usados);
                    int max;
                    if (tipo_peca < 0)
                    {
                        break;
                    }
                    max = max_variantes(tipo_peca);

                    for (k = -1; k < max; k++)
                    {
                        struct peca tentativa_peca = peca_random(tipo_peca, k); /*peca que vamos tentar por no tabuleiro*/
                        if (!verificarColisao(tabuleiro->tabuleiro, a, b, tentativa_peca))
                        {
                            success_peca = 1;
                            meter_peca(tabuleiro->tabuleiro, a, b, tentativa_peca);
                            pecas_duplicado[tipo_peca]--;
                            break;
                        }
                    }

                    tipos_usados[tipo_peca] = 1;

                    if (success_peca == 1)
                    {
                        break;
                    }
                }
                if (success_peca == 0)
                {
                    sem_erros = 0; /*deu erro , nao foi possivel colocar a peca*/
                }
            }
        }
        if (sem_erros == 1)
        {
            return;
        }
    }
    exit(-1);
}

/*
 * funcao: meter_peca()
 * coloca uma peça recebida numa posição expecífica
 * do tabuleiro
*/

void meter_peca(char tabuleiro[15][24], int a, int b, struct peca peca)
{
    int i, j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            tabuleiro[a + i][b + j] = peca.pecas[i][j];
        }
    }
}

/*
 * funcao: inicializar_tabuleiro()
 * inicializa o tabuleiro inteiro com algo à escolha
*/

void inicializar_tabuleiro(TABULEIRO *tabuleiro, char peca)
{
    int a, b;
    for (a = 0; a < 15; a++)
    {
        for (b = 0; b < 24; b++)
        {
            tabuleiro->tabuleiro[a][b] = peca;
        }
    }
}

/*
 * funcao: max_variantes()
 * indica o numero de variantes escolhidas de cada tipo de barco
*/

int max_variantes(int tipo)
{
    int numero_variantes[] = {1, 9, 12, 6, 4, 4, 4, 2, 1};
    return numero_variantes[tipo];
}

int selecionar_peca(int pecas[9], int tipos_usados[9])
{
    int possibilidades[40] = {0};
    int count = 0, i, j;

    for (i = 0; i < 9; i++)
    {
        if (pecas[i] > 0 && !tipos_usados[i])
        {
            for (j = 0; j < pecas[i]; j++)
            {
                possibilidades[count++] = i;
            }
        }
    }
    if (count == 0)
    {
        return -1;
    }
    return possibilidades[random_number(0, count - 1)];
}

/*
 * funcao: print_inicial()
 * imprime os dados escolhidos pelo utilizador em
 * relação ao tamanho do tabuleiro e à quantidade de tipo
 * de peças
*/

void print_inicial(int quantidadeTipo[9], TABULEIRO tabuleiro)
{
    printf("%dx%d %d %d %d %d %d %d %d %d\n", tabuleiro.linhas, tabuleiro.colunas, quantidadeTipo[1], quantidadeTipo[2], quantidadeTipo[3], quantidadeTipo[4], quantidadeTipo[5], quantidadeTipo[6], quantidadeTipo[7], quantidadeTipo[8]);
}

void modo0_p2(TABULEIRO tabuleiro, int quantidadeTipo[9])
{
    preencher_tabuleiro_p2(&tabuleiro, quantidadeTipo);
    print_inicial(quantidadeTipo, tabuleiro);
    imprimir_tabuleiro(tabuleiro);
}

void modo1_p1(TABULEIRO tabuleiro)
{
    char c;
    TABULEIRO tabuleiroinvi;
    tabuleiroinvi.linhas = tabuleiro.linhas;
    tabuleiroinvi.colunas = tabuleiro.colunas;
    int a, b, k, t, l, pecas_em_jogo = 0, pecas_matriz[5][8] = {{0}};
    struct peca peca;
    int max_jogadas = tabuleiro.linhas * tabuleiro.colunas;

    time_t inicio, fim;
    double tempo_jogo;

    inicializar_tabuleiro(&tabuleiro, '-');

    inicializar_tabuleiro(&tabuleiroinvi, ' ');

    for (a = 0; a < tabuleiro.linhas; a += 3)
    {
        for (b = 0; b < tabuleiro.colunas; b += 3)
        {
            for (k = 0; k <= 3; k++)
            {
                if (k == 3)
                {
                    peca = tipo1(4);
                }
                else
                {
                    peca = peca_random(-1, -1);
                    if (!verificarColisao(tabuleiro.tabuleiro, a, b, peca))
                    {
                        break;
                    }
                }
            }
            pecas_matriz[a / 3][b / 3] = peca.tipo;
            pecas_em_jogo += peca.tipo;
            meter_peca(tabuleiro.tabuleiro, a, b, peca);
        }
    }

    printf("%dx%d\n", tabuleiro.linhas, tabuleiro.colunas);
    time(&inicio);
    for (t = 0; t < max_jogadas; t++)
    {
        imprimir_tabuleiro(tabuleiroinvi);
        printf("\n");

        scanf(" %c %d", &c, &l);

        if (l <= 0 || l > tabuleiro.linhas || c - 'A' < 0 || c - 'A' >= tabuleiro.colunas || tabuleiroinvi.tabuleiro[tabuleiro.linhas - l][c - 'A'] != ' ')
        {
            t--;
        }

        else
        {
            int num = c - 'A';
            int num2 = tabuleiro.linhas - l;
            tabuleiroinvi.tabuleiro[num2][num] = tabuleiro.tabuleiro[num2][num];

            if (tabuleiro.tabuleiro[num2][num] != '-')
            {
                pecas_em_jogo--;
                pecas_matriz[num2 / 3][num / 3]--;

                if (pecas_em_jogo == 0)
                {
                    break;
                }
            }
        }
    }
    imprimir_tabuleiro(tabuleiro);

    time(&fim);
    tempo_jogo = difftime(fim, inicio);
    printf("\nNumero de tentativas: %d\n", t);
    printf("Tempo de Jogo: %.2lf segundos", tempo_jogo);
}

void modo1_p2(TABULEIRO tabuleiro, int quantidadeTipo[9])
{
    char c;
    TABULEIRO tabuleiroinvi;
    tabuleiroinvi.linhas = tabuleiro.linhas;
    tabuleiroinvi.colunas = tabuleiro.colunas;
    int a, b, t, l, pecas_matriz[5][8] = {{0}}, pecas_em_jogo = contador_peca(quantidadeTipo);

    int max_jogadas = tabuleiro.linhas * tabuleiro.colunas;

    time_t inicio, fim;
    double tempo_jogo;

    inicializar_tabuleiro(&tabuleiroinvi, ' ');

    preencher_tabuleiro_p2(&tabuleiro, quantidadeTipo);

    time(&inicio);
    for (t = 0; t < max_jogadas; t++)
    {
        print_inicial(quantidadeTipo, tabuleiro);
        imprimir_tabuleiro(tabuleiroinvi);
        printf("\n");

        scanf(" %c %d", &c, &l);

        if (l <= 0 || l > tabuleiro.linhas || c - 'A' < 0 || c - 'A' >= tabuleiro.colunas || tabuleiroinvi.tabuleiro[tabuleiro.linhas - l][c - 'A'] != ' ')
        {
            t--;
        }

        else
        {
            int num = c - 'A';
            int num2 = tabuleiro.linhas - l;
            tabuleiroinvi.tabuleiro[num2][num] = tabuleiro.tabuleiro[num2][num];

            if (tabuleiro.tabuleiro[num2][num] != '-')
            {
                pecas_em_jogo--;
                pecas_matriz[num2 / 3][num / 3]--;
                if (pecas_em_jogo == 0)
                {
                    break;
                }
            }
        }
    }
    print_inicial(quantidadeTipo, tabuleiro);
    imprimir_tabuleiro(tabuleiro);

    time(&fim);
    tempo_jogo = difftime(fim, inicio);
    printf("\nNumero de tentativas: %d\n", t);
    printf("Tempo de Jogo: %.2lf segundos", tempo_jogo);
}

/*
 * funcao. contador_peca()
 * conta todas as posições possíveis de posicionamento de partes dos
 * barcos em relação aos barcos escolhidos pelo utilizador
*/

int contador_peca(int quantidadeTipo[9])
{
    int a, pecas_em_jogo = 0;
    for (a = 1; a < 9; a++)
    {
        switch (a)
        {
        case 1:
            pecas_em_jogo += quantidadeTipo[a];
            break;
        case 2:
            pecas_em_jogo += quantidadeTipo[a] * 2;
            break;
        case 3:
            pecas_em_jogo += quantidadeTipo[a] * 3;
            break;
        case 4:
            pecas_em_jogo += quantidadeTipo[a] * 4;
            break;
        case 5:
            pecas_em_jogo += quantidadeTipo[a] * 5;
            break;
        case 6:
            pecas_em_jogo += quantidadeTipo[a] * 6;
            break;
        case 7:
            pecas_em_jogo += quantidadeTipo[a] * 7;
            break;
        case 8:
            pecas_em_jogo += quantidadeTipo[a] * 8;
            break;
        }
    }
    return pecas_em_jogo;
}

/*
 * funcao: imprimir_tabuleiro()
 * imprime o tabuleiro recebido como argumento
*/

void imprimir_tabuleiro(TABULEIRO tabuleiro)
{
    char h;
    int i, j;
    int max = tabuleiro.linhas;
    for (i = 0; i < max; i++)
    {
        printf("%2d ", tabuleiro.linhas--);     //imprime as linhas do tabuleiro
        for (j = 0; j < tabuleiro.colunas; j++) //imprime as colunas do tabuleiro
        {
            printf("%c ", tabuleiro.tabuleiro[i][j]);
        }
        printf("\n");
    }
    printf("  ");
    for (h = 'A'; h < 'A' + tabuleiro.colunas; h++)
    {
        printf(" %c", h);
    }
}

Celula modo_d1(TABULEIRO *tabuleiro)
{
    // 15 linhas * 24 colunas = 360 quadriculas
    Celula possibilidades[360];
    int a, b, i = 0;

    for (a = 0; a < tabuleiro->linhas; a++)
    {
        for (b = 0; b < tabuleiro->colunas; b++)
        {
            if (tabuleiro->tabuleiro[a][b] == ' ')
            {
                possibilidades[i].x = a;
                possibilidades[i].y = b;
                i++;
            }
        }
    }
    if (i == 0)
    {
        //quer dizer que o tabuleiro está cheio
        possibilidades[0].x = -1;
        possibilidades[0].y = -1;
        return possibilidades[0];
    }
    return possibilidades[random_number(0, i - 1)];
}

Celula modo_d2(TABULEIRO *tabuleiro, int jogada)
{
    int ordem[] = {4, 1, 7, 3, 5, 0, 8, 2, 6};
    int matriz = jogada / 9;
    int a, b, x, y;
    Celula disparo;

    a = (matriz / (tabuleiro->colunas / 3)) * 3; //
    b = (matriz % (tabuleiro->colunas / 3)) * 3;

    x = (ordem[jogada % 9]) / 3;
    y = (ordem[jogada % 9]) % 3;

    disparo.x = a + x;
    disparo.y = b + y;

    if (disparo.x >= tabuleiro->linhas || disparo.y >= tabuleiro->colunas) //quando tiver fora do tabuleiro, o jogo acaba
    {
        disparo.x = -1;
        disparo.y = -1;
    }

    return disparo;
}

Celula disparar_j2(TABULEIRO *tabuleiro, int modo_disparo, int jogada)
{
    if (modo_disparo == 1)
    {
        return modo_d1(tabuleiro);
    }
    else
    {
        return modo_d2(tabuleiro, jogada);
    }
}

void modo_j2(TABULEIRO tabuleiro, int quantidadeTipo[9], int modo_disparo)
{
    time_t inicio, fim;
    print_inicial(quantidadeTipo, tabuleiro);
    Celula disparo;
    char resposta;
    int i, a, b, count = 0;
    double tempo_jogo;
    time(&inicio);

    for (i = 0; i < 9; i++)
    {
        quantidadeTipo[i] *= i;
    }

    inicializar_tabuleiro(&tabuleiro, ' ');

    while ((disparo = disparar_j2(&tabuleiro, modo_disparo, count)).x >= 0 && verificar_final(quantidadeTipo))
    {
        printf("%c%d\n", disparo.y + 'A', tabuleiro.linhas - disparo.x);
        scanf(" %c", &resposta);

        if (resposta > '0' && resposta < '9')
        {
            quantidadeTipo[resposta - '0']--;
        }
        tabuleiro.tabuleiro[disparo.x][disparo.y] = resposta;
        count++;
    }
    time(&fim);
    tempo_jogo = difftime(fim, inicio);
    printf("Fim de Jogo: %d jogadas em %.2lf segundos\n", count, tempo_jogo);

    for (a = 0; a < 15; a++)
    {
        for (b = 0; b < 24; b++)
        {
            if (tabuleiro.tabuleiro[a][b] == ' ')
            {
                tabuleiro.tabuleiro[a][b] = '-';
            }
        }
    }
    imprimir_tabuleiro(tabuleiro);
}

int verificar_final(int quantidadeTipo[9])
{
    int i;
    for (i = 1; i < 9; i++)
    {
        if (quantidadeTipo[i] > 0)
        {
            return 1;
        }
    }
    return 0;
}

/*
 * funcao verificarcolisao()
 * As peças nunca podem ter arestas ou vértices de contacto com outras peças
*/

int verificarColisao(char tabuleiro[15][24], int i, int j, struct peca peca)
{
    int x, y, a, b, offset_x, offset_y;
    for (x = 0; x < 3; x++)
    {
        for (y = 0; y < 3; y++)
        {
            if (peca.pecas[x][y] != '-')
            {
                for (a = -1; a < 2; a++)
                {
                    for (b = -1; b < 2; b++)
                    {
                        offset_x = i + x + a;
                        offset_y = j + y + b;
                        if (offset_x >= 0 && offset_x < 15 && offset_y >= 0 && offset_y < 24)
                        {
                            if (tabuleiro[offset_x][offset_y] != '-')
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/*
 * funcao: restricao2()
 * O número máximo de peças por tabuleiro está limitado a: linhas * colunas / 9
*/

int restricao2(int count, int linhas, int colunas)
{
    int i = linhas * colunas / 9;
    if (count <= i)
    {
        return 1;
    }
    return 0;
}

/*
 * funcao: restricao3()
 * Num jogo, segue-se a regra de nunca ter mais peças de um tipo de maior dimensão
 * do que peças de um tipo de menor dimensão
*/

int restricao3(int quantidadeTipo[9])
{
    int i;

    for (i = 2; i < 9; i++)
    {
        if (quantidadeTipo[i] > quantidadeTipo[i - 1])
        {
            return 0;
        }
    }
    return 1;
}

/*
 * funcao: restricao4()
 * o número total de peças não deve exceder 50% do número de matrizes 3x3 num tabuleiro
*/

int restricao4(int linhas, int colunas, int count)
{
    int i = (linhas * colunas / 9) / 2;
    if (count <= i)
    {
        return 1;
    }
    return 0;
}

/*
 * funcao: random_number()
 * recebe um minimo e um maximo e retorna
 * um numero aleatorio entre esses dois valores.
*/

int random_number(int m, int M)
{
    return rand() % (M - m + 1) + m;
}
