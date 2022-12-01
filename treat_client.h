#include"sql.h"
#include<unistd.h>
#include<string.h>
class treat_client {

    private : 
    
    int desciptor_client ; 
    std::string message_received;
    MYSQL * DB;
    static int number_of_active_threads;

    static void * worker_thread(void * arg);
    // ^- we make this function static, as it will be given as a parameter
    // when we will create a new thread ( in function pthread_create() )
    
    public : 
    treat_client (int descriptor, MYSQL *DB); //constructor

    std::string response();
    void create_thread();

};