#include"sql_functions.h"
#include<unistd.h>
#include<string.h>
class treat_client {

    private : 
    
    int desciptor_client ; 
    std::string message_received;
    std::string const start_delimiter = "<", stop_delimiter = ">";

    sql_functions DB_connection;

    static int number_of_active_threads;

    std::string string_between_2_delimiters_and_erase ();
    //Ex: if msg_rcvd = "<1><43>", then the function willreturn <1>,
    // and it will  delete <1> from msg_rcvd (so msg_rcvd="<43>")

    std::string delete_the_delimiters_from_a_string (std::string str_with_delimiters);

    bool verify_existance_of_station(std::string name_of_station);

    std::string option_login();
    std::string option_quit();
    std::string option_trains_from_station_X_to_station_Y();
    std::string option_trains_which_leave_in_the_next_hour();
    std::string option_trains_which_arrive_in_the_next_hour();
    
    //the below methods can't be called if the user isn't an Administrator
    std::string add_delay_option();
    std::string add_train_option();
    std::string delete_train_option();
    std::string add_new_account_option();
    std::string change_username_option();
    std::string change_password_option();
    std::string show_all_trains_option();

    static void * worker_thread(void * arg);
    // ^- we make this function static, as it will be given as a parameter
    // when we will create a new thread ( in function pthread_create() )
    
    public : 

    treat_client (int descriptor, MYSQL *connection); //constructor

    std::string response();
    void create_thread();

};