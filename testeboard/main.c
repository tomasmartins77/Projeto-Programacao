#include <stdio.h>
#include <stdlib.h>

void boardgame(int linhas, int colunas);

int main()
{
    int linhas, colunas;
    boardgame(linhas, colunas);
    return 0;
}





void boardgame(int linhas, int colunas){
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
