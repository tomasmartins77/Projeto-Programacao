#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

struct peca{
    char pecas[3][3];
};
void utilizacao();
void modos_jogo(int linhas, int colunas, int j, int p, int d, int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8);
int random_number(int m, int M);
void mostra_peca(char peca[9][3][3]);
struct peca tipo0(int variante);
struct peca tipo1(int variante);
struct peca tipo2(int variante);
struct peca tipo3(int variante);
struct peca tipo4(int variante);
struct peca tipo5(int variante);
struct peca tipo6(int variante);
struct peca tipo7(int variante);
struct peca tipo8(int variante);
void init_boardgame(int linhas, int colunas);
struct peca peca_random();
int verificarColisao (char tabuleiro [15][24], int i, int j, struct peca peca);

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

struct peca tipo0(int variante){
     struct peca variante0={{{'-','-','-'},{'-','-','-'},{'-','-','-'}}};
     return variante0;
}

struct peca tipo1(int variante){
    struct peca variante1[9]={{{{'1','-','-'},{'-','-','-'},{'-','-','-'}}},
                             {{{'-','1','-'},{'-','-','-'},{'-','-','-'}}},
                             {{{'-','-','1'},{'-','-','-'},{'-','-','-'}}},
                             {{{'-','-','-'},{'1','-','-'},{'-','-','-'}}},
                             {{{'-','-','-'},{'-','1','-'},{'-','-','-'}}},
                             {{{'-','-','-'},{'-','-','1'},{'-','-','-'}}},
                             {{{'-','-','-'},{'-','-','-'},{'1','-','-'}}},
                             {{{'-','-','-'},{'-','-','-'},{'-','1','-'}}},
                             {{{'-','-','-'},{'-','-','-'},{'-','-','1'}}}};
    int k = variante <0? random_number(0, 8):variante;
    return variante1[k];
}

struct peca tipo2(int variante){

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
                                {{{'-','-','-'},{'-','-','2'},{'-','-','2'}}}};

    int k = variante <0? random_number(0, 11):variante;
    return variante2[k];

}

struct peca tipo3(int variante){

    struct peca variante3[6] =  {{{{'3','3','3'},{'-','-','-'},{'-','-','-'}}},
                                {{{'-','-','-'},{'3','3','3'},{'-','-','-'}}},
                                {{{'-','-','-'},{'-','-','-'},{'3','3','3'}}},
                                {{{'3','-','-'},{'3','-','-'},{'3','-','-'}}},
                                {{{'-','3','-'},{'-','3','-'},{'-','3','-'}}},
                                {{{'-','-','3'},{'-','-','3'},{'-','-','3'}}}};

    int k = variante <0? random_number(0, 5):variante;
    return variante3[k];

}

struct peca tipo4(int variante){

    struct peca variante4[4]=  {{{{'4','4','-'},{'4','4','-'},{'-','-','-'}}},
                                {{{'-','4','4'},{'-','4','4'},{'-','-','-'}}},
                                {{{'-','-','-'},{'4','4','-'},{'4','4','-'}}},
                                {{{'-','-','-'},{'-','4','4'},{'-','4','4'}}}};

    int k = variante <0? random_number(0, 3):variante;
    return variante4[k];

}

struct peca tipo5(int variante){

    struct peca variante5[4]=   {{{{'5','5','5'},{'-','5','-'},{'-','5','-'}}},
                                {{{'5','-','-'},{'5','5','5'},{'5','-','-'}}},
                                {{{'-','5','-'},{'-','5','-'},{'5','5','5'}}},
                                {{{'-','-','5'},{'5','5','5'},{'-','-','5'}}}};

    int k = variante <0? random_number(0, 3):variante;
    return variante5[k];

    }


struct peca tipo6(int variante){

    struct peca variante6[4]=   {{{{'-','6','-'},{'6','-','6'},{'6','6','6'}}},
                               {{{'-','6','6'},{'6','-','6'},{'-','6','6'}}},
                                {{{'6','6','6'},{'6','-','6'},{'-','6','-'}}},
                                {{{'6','6','-'},{'6','-','6'},{'6','6','-'}}}};

   int k = variante <0? random_number(0, 3):variante;
    return variante6[k];
}

struct peca tipo7(int variante){

    struct peca variante7[2]=   {{{{'7','-','7'},{'7','7','7'},{'7','-','7'}}},
                                {{{'7','7','7'},{'-','7','-'},{'7','7','7'}}}};

   int k = variante <0? random_number(0, 2):variante;
    return variante7[k];
}

struct peca tipo8(int variante){

    struct peca variante8={{{'8','8','8'},{'8','-','8'},{'8','8','8'}}};
   return variante8;
}


void init_boardgame(int linhas, int colunas){
    char alfa[24]="ABCDEFGHIJKLMNOPQRSTUVWX";
    int nums[16]= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    char tabuleiro[15][24];
    int i, j, max, a, b, c, d, k;
    struct peca peca;

    do{
        printf("Insira as dimensoes (linhas)(colunas): "); //escolher as dimensoes do tabuleiro
        scanf("%d %d", &linhas, &colunas);
            if(linhas % 3 != 0 && colunas % 3 != 0 && linhas < 9 && linhas > 15 && colunas < 9 && colunas > 24){
                printf("Dimensoes invalidas\n");
            }

    }while(linhas % 3 !=0 && colunas % 3 !=0 && linhas < 9 && linhas > 15 && colunas < 9 && colunas > 24);

    for(a = 0; a < 15; a++){
        for(b = 0; b < 24; b++){
                tabuleiro[a][b]='-';
        }
    }


    max = linhas;
    printf("\n");
    for(a = 0; a < linhas; a+=3){
        for(b = 0; b < colunas; b+=3){

          for (k=0; k<=3; k++) {
            if (k==3) {
                peca = tipo1(4);
            }
            else {
                peca = peca_random();
                if (!verificarColisao(tabuleiro, a, b, peca)) {
                    break;
                }
            }
          }

          for (i=0; i<3; i++) {
            for (j=0; j<3; j++) {
                tabuleiro[a+i][b+j]=peca.pecas[i][j];
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
 funcao: peca_random()

  recebe uma qualquer peca e retorna
  uma aleatoria entre entre todas as pecas.

*/
struct peca peca_random(){

  /* char peca_ran[3][3];
  /*  char pecas [10][3][3]; */

    switch(random_number(0, 8)){
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

int verificarColisao (char tabuleiro [15][24], int i, int j, struct peca peca) {
    int x,y,a,b, offset_x, offset_y;
    for (x=0; x<3; x++) {
        for (y=0; y<3; y++) {
            if (peca.pecas[x][y]!='-') {
                for (a=-1; a<2; a++) {
                        for (b=-1; b<2; b++) {
                            offset_x= i + x + a;
                            offset_y= j + y + b;
                            if (offset_x>=0 && offset_x<15 && offset_y>=0 && offset_y<24) {
                                if (tabuleiro[offset_x][offset_y]!='-') {
                                    return 1;
                                }
                            }
                        }

                }
            }
        }
    } return 0;
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

