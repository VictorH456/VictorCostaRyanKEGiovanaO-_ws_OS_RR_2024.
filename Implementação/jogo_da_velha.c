#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
/*
    [TEMA - 1] Jogo da Velha
    O projeto a ser desenvolvido, consiste na criação do jogo da velha. Neste caso, o jogo será feito por
    dois usuários artificiais (ou seja duas threads) no mesmo jogo da velha. Cada usuário deve operar o
    jogo no mesmo tabuleiro do jogo da velha 3 por 3. Assim, o recurso compartilhado será tabuleiro do
    jogo da velha, logo os jogadores devem esperar a sua vez de jogar e obdecer a regras do jogo.
*/
/*
    Ideias:
    - Utilizar número de jogadas para saber qual tread utilizar(par - tread 1, impar - tread 2)
    - Interface gráfica
    -
*/

// v = Pode jogar, x = tread 1, o = tread 2.
char tabuleiro[3][3] = {{' ', ' ', ' '},
                        {' ', ' ', ' '},
                        {' ', ' ', ' '}};

int quantidade_jogada = 0;
int max_jogada = 9;

void verifica(int id)
{
    int ganhou = 0;
    char simb = ' ';
    if (id == 1)
    {
        simb = 'x';
    }
    if (id == 2)
    {
        simb = 'o';
    }

    if (simb == ((tabuleiro[0][0] == tabuleiro[1][0]) == tabuleiro[2][0]))
    {
        ganhou = 1;
    }
    if (simb == ((tabuleiro[0][1] == tabuleiro[1][1]) == tabuleiro[2][1]))
    {
        ganhou = 1;
    }
    if (simb == ((tabuleiro[0][2] == tabuleiro[1][2]) == tabuleiro[2][2]))
    {
        ganhou = 1;
    }
    if (simb == ((tabuleiro[0][0] == tabuleiro[0][1]) == tabuleiro[0][2]))
    {
        ganhou = 1;
    }
    if (simb == ((tabuleiro[1][0] == tabuleiro[1][1]) == tabuleiro[1][2]))
    {
        ganhou = 1;
    }
    if (simb == ((tabuleiro[2][0] == tabuleiro[2][1]) == tabuleiro[2][2]))
    {
        ganhou = 1;
    }
    if (simb == ((tabuleiro[0][0] == tabuleiro[1][1]) == tabuleiro[2][2]))
    {
        ganhou = 1;
    }
    if (simb == ((tabuleiro[2][0] == tabuleiro[2][1]) == tabuleiro[0][2]))
    {
        ganhou = 1;
    }
    if (quantidade_jogada == 9)
    {
        pthread_exit(NULL);
        printf("Empatou");
    }
    else if (quantidade_jogada >= 5)
    {
        if (ganhou == 1)
        {
            printf("Jogador %d ganhou\n", id);
        }
    }
}
void *jogada_tread1(void *arg)
{
    int *id = (int *)arg;
    while (1)
    {
        verifica(*id);
        if (quantidade_jogada % 2 == 0 && quantidade_jogada != max_jogada)
        {
            while (1)
            {
                int r_l = 0 + rand() % (2 - 0 + 1);
                int r_c = 0 + rand() % (2 - 0 + 1);
                if (tabuleiro[r_l][r_c] == ' ')
                {
                    tabuleiro[r_l][r_c] = 'x';
                    break;
                }
            }
            for (size_t i = 0; i < 3; i++)
            {
                for (size_t j = 0; j < 3; j++)
                {
                    if (j != 2)
                    {
                        printf("| %c ", tabuleiro[i][j]);
                    }
                    else
                    {
                        printf("| %c |", tabuleiro[i][j]);
                    }
                }
                printf("\n");
            }
            printf("jogador 1 executa\n");
            quantidade_jogada++;
        }
    }
}

void *jogada_tread2(void *arg){
    int *id = (int *)arg;

    while (1)
    {
        verifica(*id);
        if (quantidade_jogada % 2 != 0 && quantidade_jogada != max_jogada)
        {
            while (1)
            {
                int r_l = 0 + rand() % (2 - 0 + 1);
                int r_c = 0 + rand() % (2 - 0 + 1);
                if (tabuleiro[r_l][r_c] == ' ')
                {
                    tabuleiro[r_l][r_c] = 'o';
                    break;
                }
            }
            for (size_t i = 0; i < 3; i++)
            {
                for (size_t j = 0; j < 3; j++)
                {
                    if (j != 2)
                    {
                        printf("| %c ", tabuleiro[i][j]);
                    }
                    else
                    {
                        printf("| %c |", tabuleiro[i][j]);
                    }
                }
                printf("\n");
            }
            printf("Jogador 2 executa\n");
            quantidade_jogada++;
        }
    }
}

int main()
{
    srand(time(NULL)); // Inicializa o gerador de números aleatórios com um valor de "semente" diferente

    pthread_t t1;
    pthread_t t2;
    int t1_id = 1;
    int t2_id = 2;

    pthread_create(&t1, NULL, jogada_tread1, &t1_id);
    pthread_create(&t2, NULL, jogada_tread2, &t2_id);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_exit(NULL);

    return 0;
}
