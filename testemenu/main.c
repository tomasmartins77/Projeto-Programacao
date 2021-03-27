#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void utilizacao()
{
    printf("\t\tMEEC WARS\n\n");
    printf("opcoes validas:\n");
    printf("[-h]\t\t\t  ajuda para os jogadores\n");
    printf("[-t linhas colunas]\t  define as dimensoes do tabuleiro\n");
    printf("[-j (0 a 2)]\t\t  define o modo de jogo)\n");
    printf("[-p (1 a 2)]\t\t  define o o modo de posicionamento das pecas pelo computador\n");
    printf("[-d (1 a 3)]\t\t  define o modo de disparo das pecas pelo computador\n");
    printf("[-1 (minimo 1)]\t\t  numero de pecas do tipo 1\n");
    printf("[-2]\t\t\t  numero de pecas do tipo 2\n");
    printf("[-3]\t\t\t  numero de pecas do tipo 3\n");
    printf("[-4]\t\t\t  numero de pecas do tipo 4\n");
    printf("[-5]\t\t\t  numero de pecas do tipo 5\n");
    printf("[-6]\t\t\t  numero de pecas do tipo 6\n");
    printf("[-7]\t\t\t  numero de pecas do tipo 7\n");
    printf("[-8]\t\t\t  numero de pecas do tipo 8\n");
}

int main(int argc, char *argv[])
{
    int linhas, colunas, j, p, d, n1, n2, n3, n4, n5, n6, n7, n8;
    int opt= 'h';
    opterr= 0;

    while ((opt= getopt(argc, argv, "ht:j:p:d:1:2:3:4:5:6:7:8:")) != -1) {
        switch (opt) {
            case 't': /* tamanho do tabuleiro*/
                linhas = atoi(argv[2]);
                colunas = atoi(argv[3]);
                break;
            case 'j': /* modo de jogo */
                sscanf(optarg, "%d", &j);
                break;
            case 'p': /* modo de posicionamento de pecas */
                sscanf(optarg, "%d", &p);
                break;
            case 'd': /* modo de disparo*/
                sscanf(optarg, "%d", &d);
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
    return 0;
}
