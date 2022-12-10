#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include "menu.h"

int main (int argc, char *argv[])
{ 
  int port;
  int sd;			// socket's descriptor
  struct sockaddr_in server;	// struct used for connecting
  int nr1=0;
  char buffer[1024];
  char ip_adress[100];
  /*if there aren't 3 arguments when running the client*/
  if (argc != 3){
    printf ("The default value for the IP is : 127.0.0.1\n and for the Port : 4444 \n");
    printf ("If you want to specify other IP/Port please use this syntax :\n%s <IP_adress> <Port>\n", argv[0]);
    strcpy(ip_adress,"127.0.0.1");
    port = 4444;
  }
  else{
    strcpy(ip_adress,argv[1]);
    /* the specified port */
    port = atoi (argv[2]);
  }

  /* create the socket*/
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Socket Error.\n");
      return errno;
    }

  /* IPV4 */
  server.sin_family = AF_INET;

  /* the server's IP adress */
  if(inet_pton(AF_INET, ip_adress, &server.sin_addr)<=0){
    perror("[-]Invalid IP address!\n");
    exit(-1);
  }

  /* the port which will make the connection */
  server.sin_port = htons (port);

  /* connect to the Server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1){
    perror ("[-]Connect() Error.\n");
    return errno;
  }

  menu Client;

  printf("%s", (Client.get_welcome_message()).c_str());

  while( Client.get_quit_status() == false ){

    if (Client.get_logged_status() == false){
      printf ("%s",(Client.get_regular_menu_message()).c_str());
    }
    else{
      printf ("%s",(Client.get_administrator_menu_message()).c_str());
    }

    int nr_read = read (0, buffer, sizeof(buffer));

    if( -1 == nr_read ){
      perror("Couldn t read the option which you have chosen\n");
      printf("\nPlease try again\n");
    }
    else{//there was no error reading the option
      buffer[nr_read] = '\0';
      printf("[client] Am citit %s\n",buffer);

      Client.handle_client_option(buffer);

      strcpy(buffer,(Client.get_the_message_which_will_be_send_to_the_server()).c_str());

      if (write (sd,buffer,sizeof(buffer)) <= 0){
        perror ("[-]Write() Error. The message couldn't be send to the server.\n");
        return errno;
      }
    
      char buffer_received[1024];

      if (Client.get_quit_status() == false ){//the server won t send any response back to the client for the Quit command
      
        Client.clear_message_send();//we clear the message from Client instance
        /* read the response from the server
        (it's in a blocking state, until the server sends the response) */
        if ((nr_read=read (sd, buffer_received,sizeof(buffer))) < 0){
          perror ("[-]Read() Error, couldn't read the response from the server.\n");
          return errno;
        }

        buffer_received[nr_read]='\0';
        //printf ("[+]The response is: %s\n", buffer_received);
        Client.set_received_message(buffer_received);
        Client.set_logged_status();
        Client.print_received_message();

      }
    }
  }//while

  /* close the connection */
  close (sd);
}
