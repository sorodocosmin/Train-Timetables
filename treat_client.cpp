#include "treat_client.h"
int treat_client::number_of_active_threads = 0;//initialize the number of active threads

treat_client::treat_client (int descriptor, MYSQL *DB){
    this->desciptor_client = descriptor ;
    this->DB = DB;
    this->number_of_active_threads ++;
    //printf("CONSTRUCTOR TREAT CLIENT \n");
} 

void treat_client::create_thread(){
    
    pthread_t t;
    pthread_create(&t,NULL,worker_thread, this);

}

std::string treat_client::response(){
    std::string case_1 = "1\n";
    if (this->message_received == case_1)
        return "Yes";
    else
     return "Something else";//we need to do a function for each case!
}


void * treat_client::worker_thread (void * arg){
    printf("[Aactive thread no : %d - Asteptam mesajul...\n", treat_client::number_of_active_threads);
    pthread_detach(pthread_self());//the main thread doesn t need to wait for the execution of this thread
    
    char buffer[1024], i=0;
    int ds_cl = ((treat_client*)arg)->desciptor_client;
    int nr_read;
    int quit=0;
    while(!quit){
    if ((nr_read = read (ds_cl, buffer,sizeof(buffer))) <= 0)
    {
        printf("[Thread]\n");
        perror ("Eroare la read() de la client.\n");
        quit=1;//if the client leaves before giving a response (i.e. closes the console or kills the process)

    }
    buffer[nr_read] = '\0';
    if(strcmp(buffer,"0\n")==0)
    quit=1;
    printf ("[Thread] Mesajul a fost receptionat...%s\n", buffer);

    ((treat_client *)arg)->message_received = buffer;

    printf("Copy-ul a fost facut : %s\n",(((treat_client *)arg)->message_received).c_str());
    /*pregatim mesajul de raspuns */
    printf("[Thread] Trimitem mesajul inapoi...%s\n", buffer);

    char result[1024];
    strcpy(result,(((treat_client *)arg)->response()).c_str());
    /* returnam mesajul clientului */
    if (write (ds_cl, result, sizeof(result)) <= 0)
    {
        printf("[Thread] ");
        perror ("[Thread]Eroare la write() catre client.\n");
        quit=1;//if the client leaves before getting an answer,
    }
    else
        printf ("[Thread] Mesajul a fost trasmis cu succes.\n");
      }//while

    close((intptr_t)arg);//we close the connection with this client
    treat_client::number_of_active_threads -- ;
    return (NULL);

}

