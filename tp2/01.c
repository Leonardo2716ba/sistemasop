#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
//Leonardo de Souza Gomes 21.2.4054
#define READ 0
#define WRITE 1

void display();
int ler_caractere();
void criar_pipes(int saldo[], int opcao[]);
pid_t criar_fork();
void processo_pai(int saldo[], int opcao[], pid_t filho1, pid_t filho2);
void processo_filho_soma(int opcao[], int saldo[]);
void processo_filho_remove(int opcao[], int saldo[]);
void imprimir_saldo(int saldo[]);

int main()
{
    display();

    int saldo[2];
    int opcao[2];

    criar_pipes(saldo, opcao);

    int aux = 0;
    if (write(saldo[WRITE], &aux, sizeof(int)) == -1)
    {
        printf("Erro ao escrever no pipe\n");
        exit(1);
    }

    pid_t filho1 = criar_fork();
    pid_t filho2 = -1;

    if (filho1 > 0)
    {
        filho2 = criar_fork();
        if (filho2 == 0)
        {
            filho1 = -1;
        }
    }

    if (filho1 > 0 && filho2 > 0)
    {
        processo_pai(saldo, opcao, filho1, filho2);
    }
    else if (filho1 == 0)
    {
        processo_filho_soma(opcao, saldo);
    }
    else if (filho2 == 0)
    {
        processo_filho_remove(opcao, saldo);
    }

    return 0;
}

void display()
{
    printf("\n[ + ] 500 UD;\n[ - ] 500 UD;\n[ i ] Imprimir Saldo;\n[ k ] Finalizar\n");
}

int ler_caractere()
{
    int caractere;
    do
    {
        caractere = (int)getchar();
    } while (caractere != '+' && caractere != 'i' &&
             caractere != 'k' && caractere != '-');
    return caractere;
}

void criar_pipes(int saldo[], int opcao[])
{
    if (pipe(saldo) < 0 || pipe(opcao) < 0)
    {
        printf("Erro ao criar pipe\n");
        exit(1);
    }
}

pid_t criar_fork()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        printf("Erro ao criar fork\n");
        exit(1);
    }
    return pid;
}

void processo_pai(int saldo[], int opcao[], pid_t filho1, pid_t filho2)
{
    int caractere = 0;
    while (caractere != 'k')
    {
        caractere = ler_caractere();
        if (caractere == 'i')
        {
            imprimir_saldo(saldo);
        }
        else
        {
            write(opcao[WRITE], &caractere, sizeof(int));
        }
    }
    if (caractere == 'k')
    {
        printf("-----------------------------\n");
        //Encerra os processos filhos
        kill(filho1, SIGKILL);
        kill(filho2, SIGKILL);
        exit(0);
    }
}

void processo_filho_soma(int opcao[], int saldo[])
{
    int operacao;
    int x;
    do
    {
        read(opcao[READ], &operacao, sizeof(int));
        if (operacao == '+')
        {
            printf("SOMA PID  : %d\n", getpid());
            read(saldo[READ], &x, sizeof(int));
            x += 500;
            write(saldo[WRITE], &x, sizeof(int));
        }
        else
        {
            //escreve comando de volta no pipe
            write(opcao[WRITE], &operacao, sizeof(int));
        }
    } while (operacao != 'k');
}

void processo_filho_remove(int opcao[], int saldo[])
{
    int operacao;
    int x;
    do
    {
        read(opcao[READ], &operacao, sizeof(int));
        if (operacao == '-')
        {
            printf("SUBTRACAO PID: %d\n", getpid());
            read(saldo[READ], &x, sizeof(int));
            x -= 500;
            write(saldo[WRITE], &x, sizeof(int));
        }
        else
        {
            write(opcao[WRITE], &operacao, sizeof(int));
        }
    } while (operacao != 'k');
}

void imprimir_saldo(int saldo[])
{
    int x;
    read(saldo[READ], &x, sizeof(int));
    printf("IMPRIME PID  : %d\n", getpid());
    printf("Saldo        : %d UD\n", x);
    write(saldo[WRITE], &x, sizeof(int));
}
