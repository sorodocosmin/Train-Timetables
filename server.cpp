#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "treat_client.h"
#include "sql.h"
#define PORT 4444



int main ()
{
    SQL BD("localhost","root","password","trains_database");

    if(BD.connect_to_databse() == false){//connect the server to the databse
        printf("The server couldn't connect to the databse!\n ! Try to restart the server ! \n");
        return -1;
    }
    else{
        printf("[+] The server connected to the databse successfully \n");
    }

//--------------------------SERVER--------------------------------------//
struct sockaddr_in server;	// structura folosita de server
struct sockaddr_in from;
int sd;		//descriptorul de socket

/* crearea unui socket */
if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)//  TCP
{
    perror ("[-]Socket error\n");
    return errno;
}
/* utilizarea optiunii SO_REUSEADDR */
int on=1;
setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));//in case that the server crashes, we won't have to wait a long tim till the port becomes reusable

/* pregatirea structurilor de date */
bzero (&server, sizeof (server));
bzero (&from, sizeof (from));

/* umplem structura folosita de server */
/* stabilirea familiei de socket-uri */
server.sin_family = AF_INET;

server.sin_addr.s_addr = htonl (INADDR_ANY); //we accept the connection from any adress

server.sin_port = htons (PORT); //the port which the client will have to specify

/* atasam socketul */
if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
{
    perror ("[server]Eroare la bind().\n");
    return errno;
}

/* punem serverul sa asculte daca vin clienti sa se conecteze */
if (listen (sd, 10) == -1)
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

    /* Accept a client (blocking state untill a client connects*/
    if ( (client = accept (sd, (struct sockaddr *) &from, (socklen_t*)&length)) < 0){
        perror ("[-]Accept() Error\n");
        continue;//the client will have to try to connect again
    }
    
    treat_client CLIENT(client, BD.getConnection());
    if ( CLIENT.create_thread() == false ){//the thread created successfully
        CLIENT.failed_creating_thread();
    }

}//while

  return 0;
}
