#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define DEFAULT_DIMENSOES 9
#define DEFAULT_JOGO 0
#define DEFAULT_PECAS_TIRO 1

struct peca
{
    char pecas[3][3];
    int tipo;
};
typedef struct
{
    char tabuleiro[15][24];
    int linhas, colunas;
} TABULEIRO;
typedef struct
{
    int x;
    int y;
} Celula;
void erro_argumentos(TABULEIRO tabuleiro, int jogo, int pecas, int disparos, int quantidadeTipo[9]);
void utilizacao();
void modos_jogo(TABULEIRO tabuleiro, int jogo, int pecas, int disparos, int quantidadeTipo[9]);
struct peca tipo0();
struct peca tipo1(int variante);
struct peca tipo2(int variante);
struct peca tipo3(int variante);
struct peca tipo4(int variante);
struct peca tipo5(int variante);
struct peca tipo6(int variante);
struct peca tipo7(int variante);
struct peca tipo8();
void modo0_p2(TABULEIRO tabuleiro, int quantidadeTipo[9]);
void modo_p1(TABULEIRO tabuleiro, int jogo);
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
void afundar_navio(TABULEIRO *tabuleiro, int matriz);
Celula modo_d1(TABULEIRO *tabuleiro);
Celula modo_d2(TABULEIRO *tabuleiro, int *jogada);
Celula disparar_j2(TABULEIRO *tabuleiro, int modo_disparo, int *jogada);
int verificarColisao(char tabuleiro[15][24], int i, int j, struct peca peca);
int restricao2(int count, int linhas, int colunas);
int restricao3(int quantidadeTipo[9]);
int restricao4(int linhas, int colunas, int count);
void print_inicial(int quantidadeTipo[9], TABULEIRO tabuleiro);
void contador_pecas_p1(int tipo, int conta_pecas[9]);
int contador_pecas_p2(int quantidadeTipo[9]);
int random_number(int m, int M);

int main(int argc, char *argv[])
{
    TABULEIRO tabuleiro;
    int opt = 'h';
    opterr = 0;
    int jogo = DEFAULT_JOGO,
        pecas = DEFAULT_PECAS_TIRO, disparos = DEFAULT_PECAS_TIRO;
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
        case 'j':                        /* modo de jogo */
            sscanf(optarg, "%d", &jogo); //modos de jogo sao 0, 1 e 2
            break;
        case 'p':                         /* modo de posicionamento de pecas */
            sscanf(optarg, "%d", &pecas); // modos de posicionamento de pecas sao 1 ou 2
            break;
        case 'd':                            /* modo de disparo*/
            sscanf(optarg, "%d", &disparos); // modos de disparo sao 1, 2, 3
            break;
        case '1':
            sscanf(optarg, "%d", &quantidadeTipo[1]); //minimo 1 peca tipo 1
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
    erro_argumentos(tabuleiro, jogo, pecas, disparos, quantidadeTipo);
    modos_jogo(tabuleiro, jogo, pecas, disparos, quantidadeTipo);
    return 0;
}

/** \brief * le os argumentos da linha de comandos e verifica se existe alguma situacao
 * de erro
 *
 * \param tabuleiro TABULEIRO: tabuleiro com dimensoes possivelmente incorretas
 * \param jogo int: modo de jogo possivelmente incorreto
 * \param pecas int: modo de posicionamento de pecas possivelmente incorreto
 * \param disparos int: modo de disparo possivelmente incorreto
 * \param quantidadeTipo[9] int: quantidade de pecas de cada tipo possivelmente incorreto
 * \return void
 *
 */
void erro_argumentos(TABULEIRO tabuleiro, int jogo, int pecas, int disparos, int quantidadeTipo[9])
{
    int k;
    if (tabuleiro.linhas % 3 != 0 || tabuleiro.colunas % 3 != 0 || tabuleiro.linhas < 9 || tabuleiro.linhas > 15 || tabuleiro.colunas < 9 || tabuleiro.colunas > 24)
    {
        utilizacao();
        exit(-1);
    }
    else if (jogo == 0 || jogo == 1)
    {
        if (pecas < 1 || pecas > 2 || disparos != 1)
        {
            utilizacao();
            exit(-1);
        }
        if (pecas == 2 && quantidadeTipo[1] == 0)
        {
            utilizacao();
            exit(-1);
        }
        if (pecas == 1)
        {
            for (k = 1; k < 9; k++)
            {
                if (quantidadeTipo[k] != 0)
                {
                    utilizacao();
                    exit(-1);
                }
            }
        }
    }
    else if (jogo == 2)
    {
        if (disparos < 1 || disparos > 3)
        {
            utilizacao();
            exit(-1);
        }
    }
    else if (jogo < 0 || jogo > 2)
    {
        utilizacao();
        exit(-1);
    }
}

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
    printf("exemplos validos de invocacao de programa:");
    printf("\nExemplo 1: ./wargame -j 1 -p 1\nExemplo 2: ./wargame -j 2 -p 1 -d 3\nExemplo 3: ./wargame -d 3 -j 1 -p 2 -1 5 -2 3");
    printf("\nExemplos invalidos de invocacao de programa:\nExemplo 1: ./wargame -j 1 -p 1 -d 3\nExemplo 2: ./wargame -j 1 -d 2\n\
Exemplo 3: ./wargame -j 2 -d 1 -1 5 -2 4 -3 5\n");
}

/** \brief  menu que vai para cada modo diferente de jogo ou posicionamento de pecas
 * ou modo de disparo
 *
 * \param tabuleiro TABULEIRO: tabuleiro utilizado em todos os modos de jogo
 * \param jogo int: modo de jogo escolhido
 * \param pecas int: tipo de posicionamento de pecas
 * \param disparos int: modo de disparo
 * \param quantidadeTipo[9] int: quantidade de cada tipo de barco
 * \return void
 *
 */
void modos_jogo(TABULEIRO tabuleiro, int jogo, int pecas, int disparos, int quantidadeTipo[9])
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

    if (jogo == 0 && r2 == 1) // modo de jogo 0
    {
        if (pecas == 1) // modo de posicionamento 1
        {
            modo_p1(tabuleiro, jogo);
        }
        else if (pecas == 2 && r3 == 1 && r4 == 1) // modo de posicionamento 2
        {
            modo0_p2(tabuleiro, quantidadeTipo);
        }
        exit(-1);
    }
    else if (jogo == 1 && r2 == 1) // modo de jogo 1
    {
        if (pecas == 1) // modo de posicionamento 1
        {
            modo_p1(tabuleiro, jogo);
        }
        else if (pecas == 2 && r3 == 1 && r4 == 1) // modo de posicionamento 2
        {
            modo1_p2(tabuleiro, quantidadeTipo);
        }
        exit(-1);
    }
    else if (jogo == 2 && r2 == 1 && r3 == 1 && r4 == 1) // modo de jogo 2
    {
        modo_j2(tabuleiro, quantidadeTipo, disparos);
    }
    exit(-1);
}

/** \brief funcoes: struct peca tipo(0 a 8)()
 * construcao dos diferentes tipos de pecas
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

/** \brief  recebe uma qualquer peca e retorna
 * uma aleatoria entre entre todas as pecas
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
    default:
        return tipo8(v);
        break;
    }
}

void contador_pecas_p1(int tipo, int conta_pecas[9])
{
    switch(tipo)
    {
    case 1:
        conta_pecas[1] += 1;
        break;
    case 2:
        conta_pecas[2] += 1;
        break;
    case 3:
        conta_pecas[3] += 1;
        break;
    case 4:
        conta_pecas[4] += 1;
        break;
    case 5:
        conta_pecas[5] += 1;
        break;
    case 6:
        conta_pecas[6] += 1;
        break;
    case 7:
        conta_pecas[7] += 1;
        break;
    case 8:
        conta_pecas[8] += 1;
        break;
    }
}

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

/** \brief coloca uma peca recebida numa posicao expecifica do tabuleiro
 *
 * \param tabuleiro[15][24] char: tabuleiro na qual vai ser colocada a peca
 * \param a int: linha onde nos encontramos
 * \param b int: coluna onde nos encontramos
 * \param peca struct peca: peca a ser colocada
 * \return void
 *
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

/** \brief inicializa o tabuleiro inteiro com algo a escolha
 *
 * \param tabuleiro TABULEIRO*
 * \param peca char: char que vai inicializar o tabuleiro
 * \return void
 *
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

/** \brief indica o numero de variantes escolhidas de cada tipo de barco
 *
 * \param tipo int: recebe o tipo de peca
 * \return int: array com o numero de variantes de cada tipo de barco
 *
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

/** \brief imprime os dados escolhidos pelo utilizador em
 * relacao ao tamanho do tabuleiro e a quantidade de tipo
 * de pecas
 *
 * \param quantidadeTipo[9] int: quantidade inicial de cada tipo de barco
 * \param tabuleiro TABULEIRO: tamanho do tabuleiro
 * \return void
 *
 */

void print_inicial(int quantidadeTipo[9], TABULEIRO tabuleiro)
{
    int k;
    printf("%dx%d", tabuleiro.linhas, tabuleiro.colunas);
    for(k=1; k<9; k++)
    {
        printf(" %d", quantidadeTipo[k]);
    }
    printf("\n");
}


void modo0_p2(TABULEIRO tabuleiro, int quantidadeTipo[9])
{
    preencher_tabuleiro_p2(&tabuleiro, quantidadeTipo);
    print_inicial(quantidadeTipo, tabuleiro);
    imprimir_tabuleiro(tabuleiro);
}

void modo_p1(TABULEIRO tabuleiro, int jogo)
{
    char coluna;
    TABULEIRO tabuleiroinvi;
    tabuleiroinvi.linhas = tabuleiro.linhas;
    tabuleiroinvi.colunas = tabuleiro.colunas;
    int a, b, k, jogadas, linha, pecas_em_jogo = 0, pecas_matriz[5][8] = {{0}}, pecas[9] = {0};
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
            contador_pecas_p1(peca.tipo, pecas);
        }
    }

    if (jogo == 0)
    {
        print_inicial(pecas, tabuleiro);
        imprimir_tabuleiro(tabuleiro);
        exit(-1);
    }

    time(&inicio);
    for (jogadas = 0; jogadas < max_jogadas; jogadas++)
    {
        print_inicial(pecas, tabuleiro);
        imprimir_tabuleiro(tabuleiroinvi);
        printf("\n");

        scanf(" %c %d", &coluna, &linha);

        if (linha <= 0 || linha > tabuleiro.linhas || coluna - 'A' < 0 || coluna - 'A' >= tabuleiro.colunas || tabuleiroinvi.tabuleiro[tabuleiro.linhas - linha][coluna - 'A'] != ' ')
        {
            jogadas--;
        }
        else
        {
            int num = coluna - 'A';
            int num2 = tabuleiro.linhas - linha;
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
    printf("\nNumero de tentativas: %d\n", jogadas);
    printf("Tempo de Jogo: %.2lf segundos", tempo_jogo);
}

void modo1_p2(TABULEIRO tabuleiro, int quantidadeTipo[9])
{
    char coluna;
    TABULEIRO tabuleiroinvi;
    tabuleiroinvi.linhas = tabuleiro.linhas;
    tabuleiroinvi.colunas = tabuleiro.colunas;
    int jogadas, linha, pecas_matriz[5][8] = {{0}}, pecas_em_jogo = contador_pecas_p2(quantidadeTipo);

    int max_jogadas = tabuleiro.linhas * tabuleiro.colunas;

    time_t inicio, fim;
    double tempo_jogo;

    inicializar_tabuleiro(&tabuleiroinvi, ' ');

    preencher_tabuleiro_p2(&tabuleiro, quantidadeTipo);

    time(&inicio);
    for (jogadas = 0; jogadas < max_jogadas; jogadas++)
    {
        print_inicial(quantidadeTipo, tabuleiro);
        imprimir_tabuleiro(tabuleiroinvi);
        printf("\n");

        scanf(" %c %d", &coluna, &linha);
        // jogadas invalidas
        if (linha <= 0 || linha > tabuleiro.linhas || coluna - 'A' < 0 || coluna - 'A' >= tabuleiro.colunas || tabuleiroinvi.tabuleiro[tabuleiro.linhas - linha][coluna - 'A'] != ' ')
        {
            jogadas--;
        }

        else
        {
            int num = coluna - 'A';
            int num2 = tabuleiro.linhas - linha;
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
    printf("\nNumero de tentativas: %d\n", jogadas);
    printf("Tempo de Jogo: %.2lf segundos", tempo_jogo);
}

/** \brief conta todas as posicoes possiveis de posicionamento de partes dos barcos em relacao
 * aos barcos escolhidos pelo utilizador
 *
 * \param quantidadeTipo[9] int: quantidade de cada tipo de barco
 * \return int: quantidade de pecas ainda em jogo
 *
 */
int contador_pecas_p2(int quantidadeTipo[9])
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

/** \brief imprime o tabuleiro no ecra
 *
 * \param tabuleiro TABULEIRO: tabuleiro que se pretende imprimir
 * \return void
 *
 */
void imprimir_tabuleiro(TABULEIRO tabuleiro)
{
    char h;
    int i, j;
    int max = tabuleiro.linhas;
    for (i = 0; i < max; i++) //cria as linhas do tabuleiro
    {
        printf("%2d ", tabuleiro.linhas--);
        for (j = 0; j < tabuleiro.colunas; j++) //cria as colunas do tabuleiro
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
        //quer dizer que o tabuleiro esta cheio
        possibilidades[0].x = -1;
        possibilidades[0].y = -1;
        return possibilidades[0];
    }
    return possibilidades[random_number(0, i - 1)];
}

Celula modo_d2(TABULEIRO *tabuleiro, int *jogada)
{
    int ordem[] = {4, 1, 7, 3, 5, 0, 8, 2, 6};
    int a, b, x, y, matriz;
    Celula disparo;
    (*jogada)--;
    do
    {
        (*jogada)++;
        matriz = *jogada / 9;

        a = (matriz / (tabuleiro->colunas / 3)) * 3;
        b = (matriz % (tabuleiro->colunas / 3)) * 3;

        x = (ordem[*jogada % 9]) / 3;
        y = (ordem[*jogada % 9]) % 3;

        disparo.x = a + x;
        disparo.y = b + y;

        if (disparo.x >= tabuleiro->linhas || disparo.y >= tabuleiro->colunas) //quando tiver fora do tabuleiro, o jogo acaba
        {
            disparo.x = -1;
            disparo.y = -1;
        }
    }
    while (disparo.x >= 0 && tabuleiro->tabuleiro[disparo.x][disparo.y] != ' ');

    return disparo;
}

Celula disparar_j2(TABULEIRO *tabuleiro, int modo_disparo, int *jogada)
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

void afundar_navio(TABULEIRO *tabuleiro, int matriz)
{
    int i = (matriz / (tabuleiro->colunas / 3)) * 3;
    int j = (matriz % (tabuleiro->colunas / 3)) * 3;
    int x, y, a, b, offset_x, offset_y;
    for (x = 0; x < 3; x++)
    {
        for (y = 0; y < 3; y++)
        {
            // seleciona apenas barcos, ou seja, celulas que tenham numeros
            if (tabuleiro->tabuleiro[x + i][y + j] != '-' || tabuleiro->tabuleiro[x + i][y + j] != ' ')
            {
                for (a = -1; a < 2; a++) // verifica a volta de uma quadricula especifica do tipo de peca
                {
                    for (b = -1; b < 2; b++)
                    {
                        offset_x = i + x + a; // offset_(x e y): verifica as bordas do tabuleiro
                        offset_y = j + y + b;
                        if (offset_x >= 0 && offset_x < 15 && offset_y >= 0 && offset_y < 24)
                        {
                            //colocar '-' nos espacos em branco que rodeiam o barco depois de este ser atingido na totalidade
                            if (tabuleiro->tabuleiro[offset_x][offset_y] == ' ')
                            {
                                tabuleiro->tabuleiro[offset_x][offset_y] = '-';
                            }
                        }
                    }
                }
            }
        }
    }
}

void modo_j2(TABULEIRO tabuleiro, int quantidadeTipo[9], int modo_disparo)
{
    time_t inicio, fim;
    print_inicial(quantidadeTipo, tabuleiro);
    Celula disparo;
    char resposta;
    int i, a, b, count = 0, jogada = 0;
    double tempo_jogo;
    int matriz_por_encontrar[40] = {0};
    time(&inicio);

    for (i = 0; i < 9; i++)
    {
        quantidadeTipo[i] *= i;
    }

    inicializar_tabuleiro(&tabuleiro, ' ');

    while ((disparo = disparar_j2(&tabuleiro, modo_disparo, &jogada)).x >= 0 && verificar_final(quantidadeTipo))
    {
        printf("%c%d\n", disparo.y + 'A', tabuleiro.linhas - disparo.x);
        scanf(" %c", &resposta);

        if (resposta > '0' && resposta < '9')
        {
            quantidadeTipo[resposta - '0']--;
            if (matriz_por_encontrar[jogada / 9] == 0)
            {
                matriz_por_encontrar[jogada / 9] = resposta - '0';
            }
            matriz_por_encontrar[jogada / 9]--;
            if (matriz_por_encontrar[jogada / 9] == 0)
            {
                if (modo_disparo == 3)
                {
                    afundar_navio(&tabuleiro, jogada / 9);
                }
                jogada = (jogada / 9) * 9 + 8;
            }
        }
        tabuleiro.tabuleiro[disparo.x][disparo.y] = resposta;
        count++;
        jogada++;
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

/** \brief verifica se ainda existe pecas no tabuleiro
 *
 * \param quantidadeTipo[9] int: verifica se o array se encontra vazio
 * \return int: se vazio retorna 0 senao retorna 1
 *
 */
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

/** \brief as pecas nunca podem ter arestas ou vertices de contacto com outras pecas
 *
 * \param tabuleiro[15][24] char: tabuleiro
 * \param i int: linha na qual nos encontramos no tabuleiro--------------------------------------------------------
 * \param j int: coluna na qual nos encontramos no tabuleiro
 * \param peca struct peca: peca na qual vai ser verificada a colisao
 * \return int: 1 se houver alguma colisao e 0 se nao houver
 *
 */
int verificarColisao(char tabuleiro[15][24], int i, int j, struct peca peca)
{
    int x, y, a, b, offset_x, offset_y;
    for (x = 0; x < 3; x++)
    {
        for (y = 0; y < 3; y++)
        {
            if (peca.pecas[x][y] != '-') // apenas ve posicoes da matriz com peca de barco
            {
                for (a = -1; a < 2; a++) // verifica a volta de uma quadricula especifica do tipo de peca
                {
                    for (b = -1; b < 2; b++)
                    {
                        offset_x = i + x + a; // offset_(x e y): verifica as bordas do tabuleiro
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

/** \brief o numero maximo de pecas por tabuleiro esta limitado a. linhas * colunas / 9
 *
 * \param count int: quantidade de pecas no tabuleiro
 * \param linhas int: numero de linhas
 * \param colunas int: numero de colunas
 * \return int: 1 se se o numero de pecas e menor ou igual a linhas * colunas / 9 e 0 se nao se verificar isso
 *
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

/** \brief nunca se pode ter mais pecas de um tipo de maior dimensao que pecas de um tipo de menor dimensao
 *
 * \param quantidadeTipo[9] int: quantidade de pecas de cada tipo
 * \return int: 1 se a restricao nao se verificar e 0 se a restricao se verificar
 *
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

/** \brief o numero total de pecas nao deve exceder 50% do numero de matrizes 3x3 num tabuleiro
 *
 * \param linhas int: numero de linhas do tabuleiro
 * \param colunas int: numero de colunas do tabuleiro
 * \param count int: numero total de pecas escolhidas
 * \return int: 1 se houver menos pecas que matrizes e 0 se houver mais pecas que matrizes
 *
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

/** \brief recebe um minimo e um maximo e retorna um numero aleatorio entre esses dois valores
 *
 * \param m int: valor mais baixo possivel para o numero aleatorio
 * \param M int: valor mais alto possivel para o numero aleatorio
 * \return int: numero aleatorio entre minimo o e maximo
 *
 */
int random_number(int m, int M)
{
    return rand() % (M - m + 1) + m;
}
