#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <pthread.h>
#define TIME 1000000
#define LOOP 500

long int iContador = 0;
long int saldo_esperado = 0;

void atrasar_operacao(){
    for (long int i = 0; i < TIME; i++){}
}

void *somar_saldo(void *pArg)
{
    printf("[    SOMA    ] TID: %lu\n", pthread_self());
    long int i, old;
    old = iContador;
    atrasar_operacao();
    for (i = 0; i < LOOP; i++){
        old = old + 1;
    }
    iContador = old;
    pthread_exit(0);
}

void *subtrair_saldo(void *pArg)
{
    printf("[ SUBTRACAO  ] TID: %lu\n", pthread_self());
    long int i, old;
    old = iContador;
    atrasar_operacao();
    for (i = 0; i < LOOP; i++){
        old = old - 1;
    }
    iContador = old;
    pthread_exit(0);
}

void *imprimir_saldo(void *pArg)
{
    printf("\n########### RACE CONDITION TEST #############\n");
    printf("SALDO GERADO: %ld\n", iContador);
    printf("SALDO ESPERADO: %ld\n", saldo_esperado);
    printf("[ + ] 500 UD\n[ - ] 500 UD\n[ i ]IMPRIMIR\n[ s ]SAIR\n");
    pthread_exit(0);
}

int main(void)
{
    pthread_t tidA, tidB, tidC;
    char c;
    while (1)
    {
        pthread_create(&tidC, NULL, imprimir_saldo,NULL);
        c = getchar();
        if(c == '+'){saldo_esperado += 500 ;pthread_create(&tidA, NULL, somar_saldo, NULL);}
        else if(c == '-'){saldo_esperado -= 500 ;pthread_create(&tidB, NULL, subtrair_saldo, NULL);}
        else if(c == 'i'){pthread_create(&tidC, NULL, imprimir_saldo,NULL);}
        else if(c == 's'){break;}
    }

    pthread_join(tidA, NULL);
    pthread_join(tidB, NULL);
    pthread_join(tidC, NULL);
    return 0;
}