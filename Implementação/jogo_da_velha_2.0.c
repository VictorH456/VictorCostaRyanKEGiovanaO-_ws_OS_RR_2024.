#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond[10] = {PTHREAD_COND_INITIALIZER};
int vez_jogador = 0;

// Região crítica
int quantidade_jogada = 0;
int max_jogada = 121;
char ganhou = 'f';

char tabuleiro[11][11] = {
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
};

char verifica(int id)
{
    char simb = id % 2 != 0 ? 'x' : 'o';

    // Verifica linhas e colunas
    for (int i = 0; i < 11; i++)
    {
        if (tabuleiro[i][0] == simb && tabuleiro[i][1] == simb && tabuleiro[i][2] == simb && tabuleiro[i][3] == simb && tabuleiro[i][4] == simb && tabuleiro[i][5] == simb && tabuleiro[i][6] == simb && tabuleiro[i][7] == simb && tabuleiro[i][8] == simb && tabuleiro[i][9] == simb && tabuleiro[i][10] == simb ||
            tabuleiro[0][i] == simb && tabuleiro[1][i] == simb && tabuleiro[2][i] == simb && tabuleiro[3][i] == simb && tabuleiro[4][i] == simb && tabuleiro[5][i] == simb && tabuleiro[6][i] == simb && tabuleiro[7][i] == simb && tabuleiro[8][i] == simb && tabuleiro[9][i] == simb && tabuleiro[10][i] == simb)
        {
            return 'v';
        }
    }

    // Verifica diagonais
    if (tabuleiro[0][0] == simb && tabuleiro[1][1] == simb && tabuleiro[2][2] == simb && tabuleiro[3][3] == simb && tabuleiro[4][4] == simb && tabuleiro[5][5] == simb && tabuleiro[6][6] == simb && tabuleiro[7][7] == simb && tabuleiro[8][8] == simb && tabuleiro[9][9] == simb && tabuleiro[10][10] == simb ||
        tabuleiro[0][10] == simb && tabuleiro[1][9] == simb && tabuleiro[2][8] == simb && tabuleiro[3][7] == simb && tabuleiro[4][6] == simb && tabuleiro[5][5] == simb && tabuleiro[6][4] == simb && tabuleiro[7][3] == simb && tabuleiro[8][2] == simb && tabuleiro[9][1] == simb && tabuleiro[10][0] == simb)
    {
        return 'v';
    }
    return 'f';
}

void *jogada_thread(void *arg)
{
    int id = *((int *)arg);
    int equipeID = id % 2 != 0 ? 1 : 2;

    while (1)
    {
        pthread_mutex_lock(&mutex);
        while (id != vez_jogador)
        {
            pthread_cond_wait(&cond[id], &mutex);
        }

        if (quantidade_jogada != max_jogada && ganhou == 'f')
        {
            while (1)
            {
                int r_l = rand() % 11;
                int r_c = rand() % 11;
                if (tabuleiro[r_l][r_c] == ' ')
                {
                    tabuleiro[r_l][r_c] = id % 2 != 0 ? 'o' : 'x';
                    break;
                }
            }
            for (int i = 0; i < 11; i++)
            {
                for (int j = 0; j < 11; j++)
                {
                    if (j != 10)
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

            printf("Jogador %d da equipe %d joga\n", id, equipeID);

            quantidade_jogada++;

            ganhou = verifica(id);

            if (ganhou == 'v')
            {
                printf("\nEquipe %d ganhou\n", equipeID);
                quantidade_jogada = max_jogada;
            }
            else if (ganhou == 'f' && quantidade_jogada >= max_jogada)
            {
                printf("Empate\n");
                quantidade_jogada = max_jogada;
            }
        }
        //tsleep(1);

        vez_jogador = (vez_jogador + 1) % 10;

        pthread_cond_broadcast(&cond[vez_jogador]);
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    srand(time(NULL));

    pthread_t threads[10];

    for (int i = 0; i < 10; i++)
    {
        int *thread_id = malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&threads[i], NULL, jogada_thread, (void *)thread_id);
    }

    for (int i = 0; i < 10; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_exit(NULL);

    return 0;
}
