#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int random_number(int m, int M);
void mostra_peca(char peca[9][3][3]);
char tipo1(int i, int j);
char tipo2(int i, int j);
char tipo3(int i, int j);
char tipo4(int i, int j);
char tipo5(int i, int j);
char tipo6(int i, int j);
char tipo7(int i, int j);
char tipo8(int i, int j);
void init_boardgame(int linhas, int colunas);
void modo0_peca1(int r);

int main(int argc, char *argv[])
{

    int linhas, colunas;
    init_boardgame(linhas, colunas);
    return 0;
}

void mostra_peca(char peca[9][3][3]){
    int j, i, k;
    for(j=0; j < 3; j++){
        for(i=0; i < 3;i++){
            printf("%c", peca[k][j][i]);
        }
        printf("\n");
    }
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
    char variante8[1][3][3]={{'8','8','8'},{'8','-','8'},{'8','8','8'}};
    return variante8[0][i][j];
}


void init_boardgame(int linhas, int colunas){
    char alfa[24]="ABCDEFGHIJKLMNOPQRSTUVWX";
    int nums[16]= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    char tabuleiro[15][24];
    char variante[9][3][3];
    int i, j, max, k;

    do{
        printf("Insira as dimensoes (linhas)(colunas): "); //escolher as dimensoes do tabuleiro
        scanf("%d %d", &linhas, &colunas);
            if(linhas % 3 != 0 || colunas % 3 != 0 || linhas < 9 || linhas > 15 || colunas < 9 || colunas > 24){
                printf("Dimensoes invalidas\n");
            }
    }while(linhas % 3 !=0 || colunas % 3 !=0 || linhas < 9 || linhas > 15 || colunas < 9 || colunas > 24);

    max = linhas;

        for (i=0; i<15; i++){

        for (j=0; j<24; j++){
            tabuleiro [i][j]='-';
        }

   }
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            tabuleiro[i][j] = tipo1(i, j);
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
    for(j = 1; j <= colunas; j++){
        printf(" %c", alfa[j-1]);
    }
}

int random_number(int m, int M){
    time_t t;
    int i = 0;
    srand((unsigned) time(&t));
    i = rand() % (m - M)+m;
    return i;
}

/*void modo0_peca1(int r){
    int m = 0, M = 7;
    int t = random_number(0, 7);

    switch(t){
    case 0:

        break;
    case 1:
        tipo2();
        break;
    case 2:
        tipo3();
        break;

    case 3:
        tipo4();
        break;

    case 4:
        tipo4();
        break;

    case 5:
        tipo5();
        break;

    case 6:
        tipo6();
        break;

    case 7:
        tipo7();
        break;
    }
}*/
