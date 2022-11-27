#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "sql.h"

#define PORT 4444

extern int errno;
typedef struct thData{
    int idThread; //id-ul thread-ului tinut in evidenta de acest program
    int cl; //descriptorul intors de accept
}thData;
static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);
SQL BD;
int main ()
{

  if(BD.connect_to_databse() == false){//connect the server to the databse

      printf("The server couldn't connect to the databse!\n ! Try to restart the server ! \n");
      return -1;
    }
  else{
      printf("[+] The server connected to the databse successfully \n");
    }

  printf("The result of the query is :\n%s\n",BD.get_result_of_the_executed_query().c_str() );


//--------------------------SERVER--------------------------------------//
struct sockaddr_in server;	// structura folosita de server
struct sockaddr_in from;
int nr;		//mesajul primit de trimis la client
int sd;		//descriptorul de socket
int pid;
pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
int i=0;


/* crearea unui socket */
if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
{
    perror ("[server]Eroare la socket().\n");
    return errno;
}
/* utilizarea optiunii SO_REUSEADDR */
int on=1;
setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

/* pregatirea structurilor de date */
bzero (&server, sizeof (server));
bzero (&from, sizeof (from));

/* umplem structura folosita de server */
/* stabilirea familiei de socket-uri */
server.sin_family = AF_INET;
/* acceptam orice adresa */
server.sin_addr.s_addr = htonl (INADDR_ANY);
/* utilizam un port utilizator */
server.sin_port = htons (PORT);

/* atasam socketul */
if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
{
    perror ("[server]Eroare la bind().\n");
    return errno;
}

/* punem serverul sa asculte daca vin clienti sa se conecteze */
if (listen (sd, 5) == -1)
{
    perror ("[server]Eroare la listen().\n");
    return errno;
}
/* servim in mod concurent clientii...folosind thread-uri */
while (1)
{
    int client;
    int length = sizeof (from);

    printf ("[server]Asteptam la portul %d...\n",PORT);
    fflush (stdout);

    // client= malloc(sizeof(int));
    /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
    if ( (client = accept (sd, (struct sockaddr *) &from, (socklen_t*)&length)) < 0)
    {
        perror ("[server]Eroare la accept().\n");
        continue;
    }

    /* s-a realizat conexiunea, se astepta mesajul */

    // int idThread; //id-ul threadului
    // int cl; //descriptorul intors de accept
    pthread_t t;//identificatorul thread-ului;
    thData * td; //parametru functia executata de thread
    td=(struct thData*)malloc(sizeof(struct thData));
    td->idThread=i++;
    td->cl=client;

    pthread_create(&t, NULL, &treat, td);

}//while

  return 0;
}

static void *treat(void * arg)
{
    struct thData tdL;
    tdL= *((struct thData*)arg);
    printf ("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
    fflush (stdout);
    pthread_detach(pthread_self());
    raspunde((struct thData*)arg);
    /* am terminat cu acest client, inchidem conexiunea */
    close ((intptr_t)arg);
    return(NULL);

};


void raspunde(void *arg)
{
    char buffer[1024], i=0;
    struct thData tdL;
    tdL= *((struct thData*)arg);
    int quit=0;
    while(!quit){
    if (read (tdL.cl, buffer,sizeof(buffer)) <= 0)
    {
        printf("[Thread %d]\n",tdL.idThread);
        perror ("Eroare la read() de la client.\n");
        quit=1;

    }
    if(strcmp(buffer,"quit\n")==0)
    quit=1;
    printf ("[Thread %d]Mesajul a fost receptionat...%s\n",tdL.idThread, buffer);

    /*pregatim mesajul de raspuns */
    printf("[Thread %d]Trimitem mesajul inapoi...%s\n",tdL.idThread, buffer);

    char result[1024];
    if(strcmp(buffer,"1\n")==0){
    strcpy(result,BD.get_result_of_the_executed_query().c_str());}
    else
    strcpy(result,BD.get_result_of_the_executed_query_2().c_str());
    /* returnam mesajul clientului */
    if (write (tdL.cl, result, sizeof(result)) <= 0)
    {
        printf("[Thread %d] ",tdL.idThread);
        perror ("[Thread]Eroare la write() catre client.\n");
        quit=1;
    }
    else
        printf ("[Thread %d]Mesajul a fost trasmis cu succes.\n",tdL.idThread);
      }//while

}
