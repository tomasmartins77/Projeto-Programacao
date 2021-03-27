#include <stdio.h>
#include <stdlib.h>

void mostra_peca(char peca[3][3]){

    for(int j=0; j < 3; j++){
        for(int i=0; i < 3;i++){
            printf("%c", peca[j][i]);
        }
        printf("\n");
    }
}

void tipo1(){
    char posicao;
    char peca1[3][3]={{'1','-','-'},{'-','-','-'},{'-','-','-'}};
    char peca2[3][3]={{'-','1','-'},{'-','-','-'},{'-','-','-'}};
    char peca3[3][3]={{'-','-','1'},{'-','-','-'},{'-','-','-'}};
    char peca4[3][3]={{'-','-','-'},{'1','-','-'},{'-','-','-'}};
    char peca5[3][3]={{'-','-','-'},{'-','1','-'},{'-','-','-'}};
    char peca6[3][3]={{'-','-','-'},{'-','-','1'},{'-','-','-'}};
    char peca7[3][3]={{'-','-','-'},{'-','-','-'},{'1','-','-'}};
    char peca8[3][3]={{'-','-','-'},{'-','-','-'},{'-','1','-'}};
    char peca9[3][3]={{'-','-','-'},{'-','-','-'},{'-','-','1'}};

    printf("peca ");
    scanf(" %c", &posicao);

    switch(posicao){
        case '1':
                mostra_peca(peca1);
            break;
        case '2':
                mostra_peca(peca2);
            break;
        case '3':
                mostra_peca(peca3);
            break;
        case '4':
                mostra_peca(peca4);
            break;
        case '5':
                mostra_peca(peca5);
            break;
        case '6':
                mostra_peca(peca6);
            break;
        case '7':
                mostra_peca(peca7);
            break;
        case '8':
                mostra_peca(peca8);
            break;
        case '9':
                mostra_peca(peca9);
            break;
    }
}
void tipo2(){
    char posicao;
    char peca10[3][3]={{'2','2','-'},{'-','-','-'},{'-','-','-'}};
    char peca11[3][3]={{'-','2','2'},{'-','-','-'},{'-','-','-'}};
    char peca12[3][3]={{'-','-','-'},{'2','2','-'},{'-','-','-'}};
    char peca13[3][3]={{'-','-','-'},{'-','2','2'},{'-','-','-'}};
    char peca14[3][3]={{'-','-','-'},{'-','-','-'},{'2','2','-'}};
    char peca15[3][3]={{'-','-','-'},{'-','-','-'},{'-','2','2'}};
    char peca16[3][3]={{'2','-','-'},{'2','-','-'},{'-','-','-'}};
    char peca17[3][3]={{'-','-','-'},{'2','-','-'},{'2','-','-'}};
    char peca18[3][3]={{'-','2','-'},{'-','2','-'},{'-','-','-'}};
    char peca19[3][3]={{'-','-','-'},{'-','2','-'},{'-','2','-'}};
    char peca20[3][3]={{'-','-','2'},{'-','-','2'},{'-','-','-'}};
    char peca21[3][3]={{'-','-','-'},{'-','-','2'},{'-','-','2'}};

    printf("peca ");
    scanf(" %c", &posicao);

    switch(posicao){
        case '1':
                mostra_peca(peca10);
            break;
        case '2':
                mostra_peca(peca11);
            break;
        case '3':
                mostra_peca(peca12);
            break;
        case '4':
                mostra_peca(peca13);
            break;
        case '5':
                mostra_peca(peca14);
            break;
        case '6':
                mostra_peca(peca15);
            break;
        case '7':
                mostra_peca(peca16);
            break;
        case '8':
                mostra_peca(peca17);
            break;
        case '9':
                mostra_peca(peca18);
            break;
        case '10':
                mostra_peca(peca19);
            break;
        case '11':
                mostra_peca(peca20);
            break;
        case '12':
                mostra_peca(peca21);
            break;
    }
}

void tipo3(){
    char posicao;
    char peca22[3][3]={{'3','3','3'},{'-','-','-'},{'-','-','-'}};
    char peca23[3][3]={{'-','-','-'},{'3','3','3'},{'-','-','-'}};
    char peca24[3][3]={{'-','-','-'},{'-','-','-'},{'3','3','3'}};
    char peca25[3][3]={{'3','-','-'},{'3','-','-'},{'3','-','-'}};
    char peca26[3][3]={{'-','3','-'},{'-','3','-'},{'-','3','-'}};
    char peca27[3][3]={{'-','-','3'},{'-','-','3'},{'-','-','3'}};

    printf("peca ");
    scanf(" %c", &posicao);

    switch(posicao){
        case '1':
                mostra_peca(peca22);
            break;
        case '2':
                mostra_peca(peca23);
            break;
        case '3':
                mostra_peca(peca24);
            break;
        case '4':
                mostra_peca(peca25);
            break;
        case '5':
                mostra_peca(peca26);
            break;
        case '6':
                mostra_peca(peca27);
            break;
    }
}

void tipo4(){
    char posicao;
    char peca28[3][3]={{'4','4','-'},{'4','4','-'},{'-','-','-'}};
    char peca29[3][3]={{'-','4','4'},{'-','4','4'},{'-','-','-'}};
    char peca30[3][3]={{'-','-','-'},{'4','4','-'},{'4','4','-'}};
    char peca31[3][3]={{'-','-','-'},{'-','4','4'},{'-','4','4'}};

    printf("peca ");
    scanf(" %c", &posicao);

    switch(posicao){
        case '1':
                mostra_peca(peca28);
            break;
        case '2':
                mostra_peca(peca29);
            break;
        case '3':
                mostra_peca(peca30);
            break;
        case '4':
                mostra_peca(peca31);
            break;
    }
}

void tipo5(){
    char posicao;
    char peca32[3][3]={{'5','5','5'},{'-','5','-'},{'-','5','-'}};
    char peca33[3][3]={{'5','-','-'},{'5','5','5'},{'5','-','-'}};
    char peca34[3][3]={{'-','5','-'},{'-','5','-'},{'5','5','5'}};
    char peca35[3][3]={{'-','-','5'},{'5','5','5'},{'-','-','5'}};

    printf("peca ");
    scanf(" %c", &posicao);

    switch(posicao){
        case '1':
                mostra_peca(peca32);
            break;
        case '2':
                mostra_peca(peca33);
            break;
        case '3':
                mostra_peca(peca34);
            break;
        case '4':
                mostra_peca(peca35);
            break;
    }
}

void tipo6(){
    char posicao;
    char peca36[3][3]={{'-','6','-'},{'6','-','6'},{'6','6','6'}};
    char peca37[3][3]={{'-','6','6'},{'6','-','6'},{'-','6','6'}};
    char peca38[3][3]={{'6','6','6'},{'6','-','6'},{'-','6','-'}};
    char peca39[3][3]={{'6','6','-'},{'6','-','6'},{'6','6','-'}};

    printf("peca ");
    scanf(" %c", &posicao);

    switch(posicao){
        case '1':
                mostra_peca(peca36);
            break;
        case '2':
                mostra_peca(peca37);
            break;
        case '3':
                mostra_peca(peca38);
            break;
        case '4':
                mostra_peca(peca39);
            break;
    }
}

void tipo7(){
    char posicao;
    char peca40[3][3]={{'7','-','7'},{'7','7','7'},{'7','-','7'}};
    char peca41[3][3]={{'7','7','7'},{'-','7','-'},{'7','7','7'}};

    printf("peca ");
    scanf(" %c", &posicao);

    switch(posicao){
        case '1':
                mostra_peca(peca40);
            break;
        case '2':
                mostra_peca(peca41);
            break;
    }
}

void tipo8(){
    char posicao;
    char peca42[3][3]={{'8','8','8'},{'8','-','8'},{'8','8','8'}};
    printf("peca ");
    scanf(" %c", &posicao);

    switch(posicao){
        case '1':
                mostra_peca(peca42);
            break;
    }
}

void init_boardgame(int linhas, int colunas){
    char alfa[25]={{'A'},{'B'},{'C'},{'D'},{'E'},{'F'},{'G'},{'H'},{'I'},{'J'},{'K'},\
    {'L'},{'M'},{'N'},{'O'},{'P'},{'Q'},{'R'},{'S'},{'T'},{'U'},{'W'},{'V'},{'X'},{'W'},{'Z'},{'\0'}};
    int nums[16]= {0,1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    char tabuleiro[15][24];

    int i, j, max;

    do{
        printf("Insira as dimesoes (linhas)(colunas): "); //escolher as dimensoes do tabuleiro
        scanf("%d %d", &linhas, &colunas);
            if(linhas % 3 != 0 || colunas % 3 != 0 || linhas < 9 || linhas > 15 || colunas < 9 || colunas > 24){
                printf("Dimensoes invalidas\n");
            }
    }while(linhas % 3 !=0 || colunas % 3 !=0 || linhas < 9 || linhas > 15 || colunas < 9 || colunas > 24);

    max = linhas;

    for(i = 0; i < max; i++){
            printf("%2d ", nums[linhas--]);  //imprime as linhas do tabuleiro
          for(j = 0; j < colunas; j++){     //imprime as colunas do tabuleiro
            printf("- ");
          }
            printf("\n");
    }
    printf("  ");
    for(j = 1; j <= colunas; j++){
        printf(" %c", alfa[j-1]);
    }
}

int main()
{
    int linhas, colunas;
    init_boardgame(linhas, colunas);
    tipo8();

    return 0;
}
