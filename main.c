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
};
void utilizacao();
void modos_jogo(int linhas, int colunas, int j, int p, int d, int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8);
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
void modo0_p1(int linhas, int colunas);
void modo0_p2(int linhas, int colunas, int n1, int count);
void modo1_p1(int linhas, int colunas);
struct peca peca_random();
int verificarColisao (char tabuleiro [15][24], int i, int j, struct peca peca);
int restricao2(int count, int linhas, int colunas);
int restricao3(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8);
int restricao4(int linhas, int colunas, int count);

int main(int argc, char *argv[])
{
    int opt = 'h';
    opterr= 0;
    int linhas = DEFAULT_DIMENSOES, colunas = DEFAULT_DIMENSOES, j = DEFAULT_JOGO_PECA,\
                 p = DEFAULT_PECAS_TIRO, d = DEFAULT_PECAS_TIRO, n1 = DEFAULT_JOGO_PECA, n2 = DEFAULT_JOGO_PECA, \
                         n3 = DEFAULT_JOGO_PECA, n4 = DEFAULT_JOGO_PECA, n5 = DEFAULT_JOGO_PECA, n6 = DEFAULT_JOGO_PECA, \
                                 n7 = DEFAULT_JOGO_PECA, n8 = DEFAULT_JOGO_PECA;
    srand(time(NULL));

    while ((opt= getopt(argc, argv, "ht:j:p:d:1:2:3:4:5:6:7:8:")) != -1)
    {
        switch (opt)
        {
        case 't': /* tamanho do tabuleiro*/
            sscanf(optarg, "%dX%d", &linhas, &colunas);   //predefinicao 9x9??
            if(linhas % 3 !=0 || colunas % 3 !=0 || linhas < 9 || linhas > 15 || colunas < 9 || colunas > 24)
            {
                utilizacao(argv[0]);
                return EXIT_FAILURE;
            }
            break;
        case 'j': /* modo de jogo */
            sscanf(optarg, "%d", &j);  //modos de jogo sao 0, 1 e 2
            if(j < 0 && j > 2)
            {
                utilizacao(argv[0]);
                return EXIT_FAILURE;
            }
            break;
        case 'p': /* modo de posicionamento de pecas */
            sscanf(optarg, "%d", &p);
            if(j != 0 && j != 1 && d <=0 && d > 0)   //modo de jogo 0 ou 1 tem modos de pecas 1 ou 2
            {
                utilizacao(argv[0]);
                return EXIT_FAILURE;
            }
            else if(p != 1 && p != 2)
            {
                utilizacao(argv[0]);
                return EXIT_FAILURE;
            }
            break;
        case 'd': /* modo de disparo*/
            sscanf(optarg, "%d", &d);
            if(j != 2)
            {
                utilizacao(argv[0]);
                return EXIT_FAILURE;
            }
            else if(p != 1 && p != 2)
            {
                utilizacao(argv[0]);
                return EXIT_FAILURE;
            }
            else if(d != 1 && d != 2 && d != 3)
            {
                utilizacao(argv[0]);
                return EXIT_FAILURE;
            }
            break;
        case '1':
            sscanf(optarg, "%d", &n1);
            break;
        case '2':
            sscanf(optarg, "%d", &n2);
            break;
        case '3':
            sscanf(optarg, "%d", &n3);
            break;
        case '4':
            sscanf(optarg, "%d", &n4);
            break;
        case '5':
            sscanf(optarg, "%d", &n5);
            break;
        case '6':
            sscanf(optarg, "%d", &n6);
            break;
        case '7':
            sscanf(optarg, "%d", &n7);
            break;
        case '8':
            sscanf(optarg, "%d", &n8);
            break;
        case 'h': /* help */
        default: /* opcoes invalidas */
            utilizacao(argv[0]);
            return EXIT_FAILURE;
            break;
        }
    }
    modos_jogo(linhas, colunas, j, p, d, n1, n2, n3, n4, n5, n6, n7, n8);
    return 0;
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
}

void modos_jogo(int linhas, int colunas, int j, int p, int d, int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8)
{
    int count = n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8;
    int r2 = restricao2(count, linhas, colunas);
    int r3 = restricao3(n1, n2, n3, n4, n5, n6, n7, n8);
    int r4 = restricao4(linhas, colunas, count);
    if(j == 0 && r2 == 1)                                    // modo de jogo 0
    {
        if(p == 1)                                           // modo de posicionamento 1
        {
            modo0_p1(linhas, colunas);
        }
        else if(p == 2 && r3 == 1 && r4 == 1)                // modo de posicionamento 2
        {
            modo0_p2(linhas, colunas, n1, count);
        }
        exit(0);
    }
    else if(j == 1 && r2 == 1)                               // modo de jogo 1
    {
        if(p == 1)                                           // modo de posicionamento 1
        {
            modo1_p1(linhas, colunas);
        }
        else if(p == 2 && r3 == 1 && r4 == 1)                // modo de posicionamento 2
        {

        } exit(0);
    }
    else if(j == 2 && r2 == 1 && r3 == 1 && r4 == 1)         // modo de jogo 2
    {
        if(d == 1)                                           // modo de disparo 1
        {

        }
        else if(d == 2)                                      // modo de disparo 2
        {

        }
        else if(d == 3)                                      // modo de disparo 3
        {

        }
    } exit(0);
}

struct peca tipo0()
{
    struct peca variante0 = {{{'-','-','-'},{'-','-','-'},{'-','-','-'}}};
    return variante0;
}

struct peca tipo1(int variante)
{
    struct peca variante1[9] = {{{{'1','-','-'},{'-','-','-'},{'-','-','-'}}},
        {{{'-','1','-'},{'-','-','-'},{'-','-','-'}}},
        {{{'-','-','1'},{'-','-','-'},{'-','-','-'}}},
        {{{'-','-','-'},{'1','-','-'},{'-','-','-'}}},
        {{{'-','-','-'},{'-','1','-'},{'-','-','-'}}},
        {{{'-','-','-'},{'-','-','1'},{'-','-','-'}}},
        {{{'-','-','-'},{'-','-','-'},{'1','-','-'}}},
        {{{'-','-','-'},{'-','-','-'},{'-','1','-'}}},
        {{{'-','-','-'},{'-','-','-'},{'-','-','1'}}}
    };
    int k = variante <0? random_number(0, 8):variante;
    return variante1[k];
}

struct peca tipo2(int variante)
{

    struct peca variante2[12] = {{{{'2','2','-'},{'-','-','-'},{'-','-','-'}}},
        {{{'-','2','2'},{'-','-','-'},{'-','-','-'}}},
        {{{'-','-','-'},{'2','2','-'},{'-','-','-'}}},
        {{{'-','-','-'},{'-','2','2'},{'-','-','-'}}},
        {{{'-','-','-'},{'-','-','-'},{'2','2','-'}}},
        {{{'-','-','-'},{'-','-','-'},{'-','2','2'}}},
        {{{'2','-','-'},{'2','-','-'},{'-','-','-'}}},
        {{{'-','-','-'},{'2','-','-'},{'2','-','-'}}},
        {{{'-','2','-'},{'-','2','-'},{'-','-','-'}}},
        {{{'-','-','-'},{'-','2','-'},{'-','2','-'}}},
        {{{'-','-','2'},{'-','-','2'},{'-','-','-'}}},
        {{{'-','-','-'},{'-','-','2'},{'-','-','2'}}}
    };

    int k = variante <0? random_number(0, 11):variante;
    return variante2[k];
}

struct peca tipo3(int variante)
{

    struct peca variante3[6] =  {{{{'3','3','3'},{'-','-','-'},{'-','-','-'}}},
        {{{'-','-','-'},{'3','3','3'},{'-','-','-'}}},
        {{{'-','-','-'},{'-','-','-'},{'3','3','3'}}},
        {{{'3','-','-'},{'3','-','-'},{'3','-','-'}}},
        {{{'-','3','-'},{'-','3','-'},{'-','3','-'}}},
        {{{'-','-','3'},{'-','-','3'},{'-','-','3'}}}
    };

    int k = variante <0? random_number(0, 5):variante;
    return variante3[k];
}

struct peca tipo4(int variante)
{

    struct peca variante4[4]=  {{{{'4','4','-'},{'4','4','-'},{'-','-','-'}}},
        {{{'-','4','4'},{'-','4','4'},{'-','-','-'}}},
        {{{'-','-','-'},{'4','4','-'},{'4','4','-'}}},
        {{{'-','-','-'},{'-','4','4'},{'-','4','4'}}}
    };

    int k = variante <0? random_number(0, 3):variante;
    return variante4[k];

}

struct peca tipo5(int variante)
{

    struct peca variante5[4]=   {{{{'5','5','5'},{'-','5','-'},{'-','5','-'}}},
        {{{'5','-','-'},{'5','5','5'},{'5','-','-'}}},
        {{{'-','5','-'},{'-','5','-'},{'5','5','5'}}},
        {{{'-','-','5'},{'5','5','5'},{'-','-','5'}}}
    };

    int k = variante <0? random_number(0, 3):variante;
    return variante5[k];
}

struct peca tipo6(int variante)
{

    struct peca variante6[4] =   {{{{'-','6','-'},{'6','-','6'},{'6','6','6'}}},
        {{{'-','6','6'},{'6','-','6'},{'-','6','6'}}},
        {{{'6','6','6'},{'6','-','6'},{'-','6','-'}}},
        {{{'6','6','-'},{'6','-','6'},{'6','6','-'}}}
    };

    int k = variante <0? random_number(0, 3):variante;
    return variante6[k];
}

struct peca tipo7(int variante)
{

    struct peca variante7[2] =   {{{{'7','-','7'},{'7','7','7'},{'7','-','7'}}},
        {{{'7','7','7'},{'-','7','-'},{'7','7','7'}}}
    };

    int k = variante <0? random_number(0, 1):variante;
    return variante7[k];
}

struct peca tipo8()
{

    struct peca variante8 = {{{'8','8','8'},{'8','-','8'},{'8','8','8'}}};
    return variante8;
}

void modo0_p1(int linhas, int colunas)
{
    char alfa[24]="ABCDEFGHIJKLMNOPQRSTUVWX";
    int nums[16]= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    char tabuleiro[15][24];
    int i, j, max, a, b, k;
    struct peca peca;
    max = linhas;

    for(a = 0; a < 15; a++)
    {
        for(b = 0; b < 24; b++)
        {
            tabuleiro[a][b] = '-';
        }
    }

    for(a = 0; a < linhas; a+=3)
    {
        for(b = 0; b < colunas; b+=3)
        {
            for (k=0; k<=3; k++)
            {
                if (k==3)
                {
                    peca = tipo1(4);
                }
                else
                {
                    peca = peca_random();
                    if (!verificarColisao(tabuleiro, a, b, peca))
                    {
                        break;
                    }
                }
            }
            for (i=0; i<3; i++)
            {
                for (j=0; j<3; j++)
                {
                    tabuleiro[a+i][b+j]=peca.pecas[i][j];
                }
            }
        }
    }

    printf("%dX%d\n", linhas, colunas);

    for(i = 0; i < max; i++)
    {
        printf("%2d ", nums[linhas--]);  //imprime as linhas do tabuleiro
        for(j = 0; j < colunas; j++)   //imprime as colunas do tabuleiro
        {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
    printf("  ");
    for(j = 0; j < colunas; j++)
    {
        printf(" %c", alfa[j]);
    }
}

/*
 * funcao: peca_random()
 * recebe uma qualquer peca e retorna
 * uma aleatoria entre entre todas as pecas.
*/

struct peca peca_random()
{

    switch(random_number(0, 8))
    {
    case 0:
        return tipo0(-1);
        break;
    case 1:
        return tipo1(-1);
        break;
    case 2:
        return tipo2(-1);
        break;
    case 3:
        return tipo3(-1);
        break;
    case 4:
        return tipo4(-1);
        break;
    case 5:
        return tipo5(-1);
        break;
    case 6:
        return tipo6(-1);
        break;
    case 7:
        return tipo7(-1);
        break;
    case 8:
        return tipo8(-1);
    }
}

void modo0_p2(int linhas, int colunas, int n1, int count)
{
    char alfa[24]="ABCDEFGHIJKLMNOPQRSTUVWX";
    int nums[16]= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    char tabuleiro[15][24];
    int i, j, max, a, b, k=0;
    max = linhas;

    int p = 0, g;
    struct peca pecas[40];
    struct peca peca;

    for(g=0; g<40; g++)
    {
        if(g>count)
        {
            pecas[g+p] = tipo0();
        }
        else
        {
            for(j = 0; j<n1; j++)
                pecas[p] = tipo1(-1);
            p++;
        }
    }

    for(a = 0; a < 15; a++)
    {
        for(b = 0; b < 24; b++)
        {
            tabuleiro[a][b] = '-';
        }
    }

    for(g=0; g < 40; g++)
    {
        for(a = 0; a < linhas; a+=3)
        {
            for(b = 0; b < colunas; b+=3)
            {
                peca = pecas[g];
                for (i=0; i<3; i++)
                {
                    for (j=0; j<3; j++)
                    {
                        tabuleiro[a+i][b+j]=peca.pecas[i][j];
                    }
                }
                k++;
            }
        }
    }


    for(i = 0; i < max; i++)
    {
        printf("%2d ", nums[linhas--]);  //imprime as linhas do tabuleiro
        for(j = 0; j < colunas; j++)   //imprime as colunas do tabuleiro
        {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
    printf("  ");
    for(j = 0; j < colunas; j++)
    {
        printf(" %c", alfa[j]);
    }
}

void modo1_p1(int linhas, int colunas)
{
    char alfa[24]="ABCDEFGHIJKLMNOPQRSTUVWX";
    int nums[16]= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    char tabuleiro[15][24], c, tabuleiroinvi[15][24];
    int i, j, max, a, b, k, l, t;
    struct peca peca;
    max = linhas;

    for(a = 0; a < 15; a++)
    {
        for(b = 0; b < 24; b++)
        {
            tabuleiro[a][b] = '-';
        }
    }

    for(a = 0; a < 15; a++)
    {
        for(b = 0; b < 24; b++)
        {
            tabuleiroinvi[a][b] = ' ';
        }
    }

    for(a = 0; a < linhas; a+=3)
    {
        for(b = 0; b < colunas; b+=3)
        {
            for (k=0; k<=3; k++)
            {
                if (k==3)
                {
                    peca = tipo1(4);
                }
                else
                {
                    peca = peca_random();
                    if (!verificarColisao(tabuleiro, a, b, peca))
                    {
                        break;
                    }
                }
            }
            for (i=0; i<3; i++)
            {
                for (j=0; j<3; j++)
                {
                    tabuleiro[a+i][b+j]=peca.pecas[i][j];
                }
            }
        }
    }
//-------------------------------------------------------------------------------------
    printf("%dX%d\n", linhas, colunas);

    for(i = 0; i < max; i++)
    {
        printf("%2d ", nums[linhas--]);  //imprime as linhas do tabuleiro
        for(j = 0; j < colunas; j++)   //imprime as colunas do tabuleiro
        {
            printf("%c ", tabuleiroinvi[i][j]);
        }
        printf("\n");
    }
    printf("  ");
    for(j = 0; j < colunas; j++)
    {
        printf(" %c", alfa[j]);
    }


}

/*
 * funcao verificarcolisao()
 * As peças nunca podem ter arestas ou vértices de contacto com outras peças
 *
*/

int verificarColisao (char tabuleiro [15][24], int i, int j, struct peca peca)
{
    int x, y, a, b, offset_x, offset_y;
    for (x = 0; x < 3; x++)
    {
        for (y = 0; y < 3; y++)
        {
            if (peca.pecas[x][y] != '-')
            {
                for (a=-1; a<2; a++)
                {
                    for (b = -1; b < 2; b++)
                    {
                        offset_x= i + x + a;
                        offset_y= j + y + b;
                        if (offset_x>=0 && offset_x<15 && offset_y>=0 && offset_y<24)
                        {
                            if (tabuleiro[offset_x][offset_y]!='-')
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
 *
*/

int restricao2(int count, int linhas, int colunas)
{
    int i = linhas * colunas / 9;
    if(count <= i)
    {
        return 1;
    }
    return 0;
}

/*
 * funcao: restricao3()
 * Num jogo, segue-se a regra de nunca ter mais peças de um tipo de maior dimensão
 * do que peças de um tipo de menor dimensão
 *
*/

int restricao3(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8)
{
    if(n1 >= n2)
    {
        if(n2>=n3)
        {
            if(n3 >= n4)
            {
                if(n4>=n5)
                {
                    if(n5>=n6)
                    {
                        if(n6>=n7)
                        {
                            if (n7>=n8)
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
 * funcao: restricao4()
 * o número total de peças não deve exceder 50% do número de matrizes 3x3 num tabuleiro
 *
*/

int restricao4(int linhas, int colunas, int count)
{
    int i = (linhas*colunas/9)/2;
    if(count <= i)
    {
        return 1;
    }
    return 0;
}

/*
 * funcao: random_number()
 * recebe um minimo e um maximo e retorna
 * um numero aleatorio entre esses dois valores.
 *
*/

int random_number(int m, int M)
{
    return rand() % (M - m + 1) + m;
}
