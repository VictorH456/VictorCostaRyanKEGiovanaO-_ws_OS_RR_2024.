#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

//Varivaveis globais
char simb;
int r_l;
int r_c;
char ganhou = 'f';

/*
 [0][0] [1][0] [2][0]
 [0][1] [1][1] [2][1]
 [0][2] [1][2] [2][2]
*/

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Regiao critica
int should_sleep = 1; // variável de condição
int quantidade_jogada = 0;
int max_jogada = 9;
char tabuleiro[3][3] = {{' ', ' ', ' '},
                        {' ', ' ', ' '},
                        {' ', ' ', ' '}};

char verifica(int id)
{
    simb = id == 1 ? 'x' : 'o';

    // Verifica linhas e colunas
    for (int i = 0; i < 3; i++)
    {
        if (tabuleiro[i][0] == simb && tabuleiro[i][1] == simb && tabuleiro[i][2] == simb ||
            tabuleiro[0][i] == simb && tabuleiro[1][i] == simb && tabuleiro[2][i] == simb)
        {
            return 'v';
        }
    }

    // Verifica diagonais
    if (tabuleiro[0][0] == simb && tabuleiro[1][1] == simb && tabuleiro[2][2] == simb ||
        tabuleiro[0][2] == simb && tabuleiro[1][1] == simb && tabuleiro[2][0] == simb)
    {
        return 'v';
    }
    return 'f';
}

void *jogada_thread(void *arg)
{
    int *id = (int *)arg;
    while (1)
    {
        // Sistema para acordar a thread
        pthread_mutex_lock(&mutex);
        while (*id == 1 ? !should_sleep : should_sleep)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);
        // acaba aqui

        if (quantidade_jogada != max_jogada)
        {
            while (1)
            {
                int r_l = 0 + rand() % (2 - 0 + 1);
                int r_c = 0 + rand() % (2 - 0 + 1);
                if (tabuleiro[r_l][r_c] == ' ')
                {
                    tabuleiro[r_l][r_c] = *id == 1 ? 'x' : 'o';
                    break;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
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

            printf("Jogador %d Joga\n", *id);
            quantidade_jogada++;

            ganhou = verifica(*id);
            if (ganhou == 'v')
            {
                printf("\nJogador %d ganhou\n", *id);
                quantidade_jogada = max_jogada; // Encerra o jogo
            }
            else if (ganhou == 'f' && quantidade_jogada >= max_jogada)
            {
                printf("Empatou\n");
                quantidade_jogada = max_jogada; // Encerra o jogo
            }
        }

        sleep(1);
        pthread_mutex_lock(&mutex);
        should_sleep = !should_sleep; // thread 1 ou 2 dorme
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
    }
}

int main()
{
    // Inicializa o gerador aleatório com uma seed diferente
    srand(time(NULL)); 

    //Criação das threads
    pthread_t t1, t2;
    int t1_id = 1;
    int t2_id = 2;

    pthread_create(&t1, NULL, jogada_thread, &t1_id);
    pthread_create(&t2, NULL, jogada_thread, &t2_id);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_exit(NULL);

    return 0;
}
