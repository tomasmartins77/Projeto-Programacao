#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void utilizacao();
void modos_jogo(int linhas, int colunas, int j, int p, int d, int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8);
int random_number(int m, int M);
void mostra_peca(char peca[9][3][3]);
void tipo0(char tabuleiro[15][24], int i, int j);
void tipo1(char tabuleiro[15][24], int i, int j);
char tipo2(char tabuleiro[15][24], int i, int j);
char tipo3(char tabuleiro[15][24], int i, int j);
char tipo4(char tabuleiro[15][24], int i, int j);
char tipo5(char tabuleiro[15][24], int i, int j);
char tipo6(char tabuleiro[15][24], int i, int j);
char tipo7(char tabuleiro[15][24], int i, int j);
char tipo8(char tabuleiro[15][24], int i, int j);
void init_boardgame(int linhas, int colunas);
void peca_random(char tabuleiro[15][24], int i, int j);

int main(int argc, char *argv[]){
     srand(time(NULL));
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

void tipo0(char tabuleiro[15][24], int i, int j){
     int x,y;
     char variante0[3][3]={{'-','-','-'},{'-','-','-'},{'-','-','-'}};
     for (x=0; x<3; x++) {
        for (y=0; y<3; y++) {
            tabuleiro[x+i][y+j]=variante0[x][y];
        }
     }

}

void tipo1(char tabuleiro[15][24], int i, int j){
    int x,y;
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

    for (x=0; x<3; x++) {
        for (y=0; y<3; y++) {
            tabuleiro[x+i][y+j]=variante1[k][x][y];

        }
    }


}

char tipo2(char tabuleiro[15][24],int i, int j){
      int x,y;
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
    for (x=0; x<3; x++) {
        for (y=0; y<3; y++) {
            tabuleiro[x+i][y+j]=variante2[k][x][y];

        }
    }
}

char tipo3(char tabuleiro[15][24],int i, int j){
    int x,y;
    char variante3[6][3][3] =  {{{'3','3','3'},{'-','-','-'},{'-','-','-'}},
                                {{'-','-','-'},{'3','3','3'},{'-','-','-'}},
                                {{'-','-','-'},{'-','-','-'},{'3','3','3'}},
                                {{'3','-','-'},{'3','-','-'},{'3','-','-'}},
                                {{'-','3','-'},{'-','3','-'},{'-','3','-'}},
                                {{'-','-','3'},{'-','-','3'},{'-','-','3'}}};
    int k = random_number(0, 5);

     for (x=0; x<3; x++) {
        for (y=0; y<3; y++) {
            tabuleiro[x+i][y+j]=variante3[k][x][y];

        }
    }
}

char tipo4(char tabuleiro[15][24],int i, int j){
      int x,y;
    char variante4[4][3][3]=   {{{'4','4','-'},{'4','4','-'},{'-','-','-'}},
                                {{'-','4','4'},{'-','4','4'},{'-','-','-'}},
                                {{'-','-','-'},{'4','4','-'},{'4','4','-'}},
                                {{'-','-','-'},{'-','4','4'},{'-','4','4'}}};
    int k = random_number(0, 3);
     for (x=0; x<3; x++) {
        for (y=0; y<3; y++) {
            tabuleiro[x+i][y+j]=variante4[k][x][y];

        }
    }
}

char tipo5(char tabuleiro[15][24],int i, int j){
    int x,y;
    char variante5[4][3][3]=   {{{'5','5','5'},{'-','5','-'},{'-','5','-'}},
                                {{'5','-','-'},{'5','5','5'},{'5','-','-'}},
                                {{'-','5','-'},{'-','5','-'},{'5','5','5'}},
                                {{'-','-','5'},{'5','5','5'},{'-','-','5'}}};
    int k = random_number(0, 3);
      for (x=0; x<3; x++) {
        for (y=0; y<3; y++) {
            tabuleiro[x+i][y+j]=variante5[k][x][y];

        }
    }
    }


char tipo6(char tabuleiro[15][24],int i, int j){
    int x,y;
    char variante6[4][3][3]=   {{{'-','6','-'},{'6','-','6'},{'6','6','6'}},
                                {{'-','6','6'},{'6','-','6'},{'-','6','6'}},
                                {{'6','6','6'},{'6','-','6'},{'-','6','-'}},
                                {{'6','6','-'},{'6','-','6'},{'6','6','-'}}};
    int k = random_number(0, 3);
      for (x=0; x<3; x++) {
        for (y=0; y<3; y++) {
            tabuleiro[x+i][y+j]=variante6[k][x][y];

        }
    }
}

char tipo7(char tabuleiro[15][24],int i, int j){
    int x,y;
    char variante7[2][3][3]=   {{{'7','-','7'},{'7','7','7'},{'7','-','7'}},
                                {{'7','7','7'},{'-','7','-'},{'7','7','7'}}};
    int k = random_number(0, 1);
    for (x=0; x<3; x++) {
        for (y=0; y<3; y++) {
            tabuleiro[x+i][y+j]=variante7[k][x][y];

        }
    }
}

char tipo8(char tabuleiro[15][24],int i, int j){
    int x,y;
    char variante8[3][3]={{'8','8','8'},{'8','-','8'},{'8','8','8'}};
    for (x=0; x<3; x++) {
        for (y=0; y<3; y++) {
            tabuleiro[x+i][y+j]=variante8[x][y];

        }
    }
}


void init_boardgame(int linhas, int colunas){
    char alfa[24]="ABCDEFGHIJKLMNOPQRSTUVWX";
    int nums[16]= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    char tabuleiro[15][24];
    int i, j, max, a, b, c, d, k;

    do{
        printf("Insira as dimensoes (linhas)(colunas): "); //escolher as dimensoes do tabuleiro
        scanf("%d %d", &linhas, &colunas);
            if(linhas % 3 != 0 && colunas % 3 != 0 && linhas < 9 && linhas > 15 && colunas < 9 && colunas > 24){
                printf("Dimensoes invalidas\n");
            }

    }while(linhas % 3 !=0 && colunas % 3 !=0 && linhas < 9 && linhas > 15 && colunas < 9 && colunas > 24);

    max = linhas;
    printf("\n");
    for(a = 0; a < linhas; a+=3){
        for(b = 0; b < colunas; b+=3){
            peca_random(tabuleiro,a,b);


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
 funcao: peca_random()

  recebe uma qualquer peca e retorna
  uma aleatoria entre entre todas as pecas.

*/
void peca_random(char tabuleiro[15][24], int i, int j){

  /* char peca_ran[3][3];
  /*  char pecas [10][3][3]; */

    switch(random_number(0, 8)){
    case 0:
        tipo0(tabuleiro,i,j);
        break;
    case 1:
        tipo1(tabuleiro,i,j);
        break;
    case 2:
        tipo2(tabuleiro,i,j);
        break;
    case 3:
        tipo3(tabuleiro,i,j);
        break;
    case 4:
        tipo4(tabuleiro,i,j);
        break;
    case 5:
        tipo5(tabuleiro,i,j);
        break;
    case 6:
        tipo6(tabuleiro,i,j);
        break;
    case 7:
        tipo7(tabuleiro,i,j);
        break;
    case 8:
        tipo8(tabuleiro,i,j);
   }



}

/*int random_number(int m, int M, int count) {
    count=2;
    int i;
    for (i=0;i<count;i++) {
        int num=(rand() % (M - m + 1) + m);
        printf("%d", num);
    }
}

/*
  funcao: random_number()

  recebe um minimo e um maximo e retorna
  um numero aleatorio entre esses dois valores. */



int random_number(int m, int M){
    int n;

    /*for (n=0; n<7; n++) { */
    int i = rand() % (M - m + 1) + m;

    return i;
    }

