#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void utilizacao();
void modos_jogo(int linhas, int colunas, int j, int p, int d, int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8);
int random_number(int m, int M);
void mostra_peca(char peca[9][3][3]);
char tipo0(int i, int j);
char tipo1(int i, int j);
char tipo2(int i, int j);
char tipo3(int i, int j);
char tipo4(int i, int j);
char tipo5(int i, int j);
char tipo6(int i, int j);
char tipo7(int i, int j);
char tipo8(int i, int j);
void init_boardgame(int linhas, int colunas);
char peca_random(int i, int j);

int main(int argc, char *argv[]){

    int linhas = 0, colunas = 0;
    init_boardgame(linhas, colunas);
    return 0;
}

void utilizacao(){
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

void modos_jogo(int linhas, int colunas, int j, int p, int d, int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8){
    if(j == 0){            // modo de jogo 0
        if(p == 1){         // modo de posicionamento 1

        }else{              // modo de posicionamento 2

        }
    }else if(j == 1){       // modo de jogo 1
        if(p == 1){         // modo de posicionamento 1

        }else{              // modo de posicionamento 2

        }
    }else if(j == 2){       // modo de jogo 2
        if(d == 1){         // modo de disparo 1

        }else if(d == 2){   // modo de disparo 2

        }else{              // modo de disparo 3

        }
    }
}

void mostra_peca(char peca[9][3][3]){
    int j, i;
    int k = random_number(0, 8);
    for(j=0; j < 3; j++){
        for(i=0; i < 3;i++){
            printf("%c", peca[k][j][i]);
        }
        printf("\n");
    }
}

char tipo0(int i, int j){
     char variante0[3][3]={{'-','-','-'},{'-','-','-'},{'-','-','-'}};
     return variante0[i][j];
}

char tipo1(int i, int j){
    char variante1[9][3][3]={{{'1','-','-'},{'-','-','-'},{'-','-','-'}},
                             {{'-','1','-'},{'-','-','-'},{'-','-','-'}},
                             {{'-','-','1'},{'-','-','-'},{'-','-','-'}},
                             {{'-','-','-'},{'1','-','-'},{'-','-','-'}},
                             {{'-','-','-'},{'-','1','-'},{'-','-','-'}},
                             {{'-','-','-'},{'-','-','1'},{'-','-','-'}},
                             {{'-','-','-'},{'-','-','-'},{'1','-','-'}},
                             {{'-','-','-'},{'-','-','-'},{'-','1','-'}},
                             {{'-','-','-'},{'-','-','-'},{'-','-','1'}}};
    int k = random_number(0, 8);
    return variante1[k][i][j];
}

char tipo2(int i, int j){
    char variante2[12][3][3] = {{{'2','2','-'},{'-','-','-'},{'-','-','-'}},
                                {{'-','2','2'},{'-','-','-'},{'-','-','-'}},
                                {{'-','-','-'},{'2','2','-'},{'-','-','-'}},
                                {{'-','-','-'},{'-','2','2'},{'-','-','-'}},
                                {{'-','-','-'},{'-','-','-'},{'2','2','-'}},
                                {{'-','-','-'},{'-','-','-'},{'-','2','2'}},
                                {{'2','-','-'},{'2','-','-'},{'-','-','-'}},
                                {{'-','-','-'},{'2','-','-'},{'2','-','-'}},
                                {{'-','2','-'},{'-','2','-'},{'-','-','-'}},
                                {{'-','-','-'},{'-','2','-'},{'-','2','-'}},
                                {{'-','-','2'},{'-','-','2'},{'-','-','-'}},
                                {{'-','-','-'},{'-','-','2'},{'-','-','2'}}};
    int k = random_number(0, 11);
    return variante2[k][i][j];
}

char tipo3(int i, int j){
    char variante3[6][3][3] =  {{{'3','3','3'},{'-','-','-'},{'-','-','-'}},
                                {{'-','-','-'},{'3','3','3'},{'-','-','-'}},
                                {{'-','-','-'},{'-','-','-'},{'3','3','3'}},
                                {{'3','-','-'},{'3','-','-'},{'3','-','-'}},
                                {{'-','3','-'},{'-','3','-'},{'-','3','-'}},
                                {{'-','-','3'},{'-','-','3'},{'-','-','3'}}};
    int k = random_number(0, 5);
    return variante3[k][i][j];
}

char tipo4(int i, int j){
    char variante4[4][3][3]=   {{{'4','4','-'},{'4','4','-'},{'-','-','-'}},
                                {{'-','4','4'},{'-','4','4'},{'-','-','-'}},
                                {{'-','-','-'},{'4','4','-'},{'4','4','-'}},
                                {{'-','-','-'},{'-','4','4'},{'-','4','4'}}};
    int k = random_number(0, 3);
    return variante4[k][i][j];
}

char tipo5(int i, int j){
    char variante5[4][3][3]=   {{{'5','5','5'},{'-','5','-'},{'-','5','-'}},
                                {{'5','-','-'},{'5','5','5'},{'5','-','-'}},
                                {{'-','5','-'},{'-','5','-'},{'5','5','5'}},
                                {{'-','-','5'},{'5','5','5'},{'-','-','5'}}};
    int k = random_number(0, 3);
    return variante5[k][i][j];
}

char tipo6(int i, int j){
    char variante6[4][3][3]=   {{{'-','6','-'},{'6','-','6'},{'6','6','6'}},
                                {{'-','6','6'},{'6','-','6'},{'-','6','6'}},
                                {{'6','6','6'},{'6','-','6'},{'-','6','-'}},
                                {{'6','6','-'},{'6','-','6'},{'6','6','-'}}};
    int k = random_number(0, 3);
    return variante6[k][i][j];
}

char tipo7(int i, int j){
    char variante7[2][3][3]=   {{{'7','-','7'},{'7','7','7'},{'7','-','7'}},
                                {{'7','7','7'},{'-','7','-'},{'7','7','7'}}};
    int k = random_number(0, 1);
    return variante7[k][i][j];
}

char tipo8(int i, int j){
    char variante8[3][3]={{'8','8','8'},{'8','-','8'},{'8','8','8'}};
    return variante8[i][j];
}


void init_boardgame(int linhas, int colunas){
    char alfa[24]="ABCDEFGHIJKLMNOPQRSTUVWX";
    int nums[16]= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    char tabuleiro[15][24];
    int i, j, max, a, b, c, d, K;

    do{
        printf("Insira as dimensoes (linhas)(colunas): "); //escolher as dimensoes do tabuleiro
        scanf("%d %d", &linhas, &colunas);
            if(linhas % 3 != 0 && colunas % 3 != 0 && linhas < 9 && linhas > 15 && colunas < 9 && colunas > 24){
                printf("Dimensoes invalidas\n");
            }
    }while(linhas % 3 !=0 && colunas % 3 !=0 && linhas < 9 && linhas > 15 && colunas < 9 && colunas > 24);

    max = linhas;

    for(a = 0; a < linhas; a++){
        for(b = 0; b < colunas; b++){

            for (i = 0; i < 3; i++){
                for (j = 0; j < 3; j++){
                    tabuleiro[i][j] = peca_random(i, j);
                }
            }
        }
    }


    for(i = 0; i < max; i++){
        printf("%2d ", nums[linhas--]);  //imprime as linhas do tabuleiro
          for(j = 0; j < colunas; j++){  //imprime as colunas do tabuleiro
               printf("%c ", tabuleiro[i][j]);
          }
            printf("\n");
    }
        printf("  ");
            for(j = 0; j < colunas; j++){
                printf(" %c", alfa[j]);
            }
}

/*
 * funcao: peca_random()
 *
 * recebe uma qualquer peca e retorna
 * uma aleatoria entre entre todas as pecas.
 *
*/
char peca_random(int i, int j){
    char peca_ran[3][3];

    switch(random_number(0, 8)){
    case 0:
        peca_ran[i][j] = tipo0(i, j);
        break;
    case 1:
        peca_ran[i][j] = tipo1(i, j);
        break;
    case 2:
        peca_ran[i][j] = tipo2(i, j);
        break;
    case 3:
        peca_ran[i][j] = tipo3(i, j);
        break;
    case 4:
        peca_ran[i][j] = tipo4(i, j);
        break;
    case 5:
        peca_ran[i][j] = tipo5(i, j);
        break;
    case 6:
        peca_ran[i][j] = tipo6(i, j);
        break;
    case 7:
        peca_ran[i][j] = tipo7(i, j);
        break;
    case 8:
        peca_ran[i][j] = tipo8(i, j);
    }

    return peca_ran[i][j];
}

/*
 *  funcao: random_number()
 *
 *  recebe um minimo e um maximo e retorna
 *  um numero aleatorio entre esses dois valores.
 *
*/

int random_number(int m, int M){
    time_t t;
    srand((unsigned) time(&t));
    return rand() % (m - M - 1) + m;
}
