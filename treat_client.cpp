#include "treat_client.h"
int treat_client::number_of_active_threads = 0;//initialize the number of active threads

treat_client::treat_client (int descriptor, MYSQL *connection) : DB_connection(connection){
    this->desciptor_client = descriptor ;
    this->number_of_active_threads ++;
} 

void treat_client::create_thread(){
    
    pthread_t t;
    pthread_create(&t,NULL,worker_thread, this);

}

std::string treat_client::response(){
    //the message will be recieved in this format  :
    // <status_logged><number_option><aditional_info1_for_option>...etc<...>


    std::string status_logged = this->string_between_2_delimiters_and_erase();

    std::string option_chosen = this->string_between_2_delimiters_and_erase();

    if (status_logged == (start_delimiter + "0" + stop_delimiter) ){//the user isn't an Administrator
        std::string case_exit = this->start_delimiter + "0" + this->stop_delimiter;
        std::string case_login =  this->start_delimiter + "1" + this->stop_delimiter;
        std::string case_trains_from_X_to_Y =  this->start_delimiter + "2" + this->stop_delimiter;
        std::string case_departure_in_next_hour = this->start_delimiter + "3" + this->stop_delimiter;
        std::string case_arrival_in_next_hour = this->start_delimiter + "4" + this->stop_delimiter;

        if(option_chosen == case_exit){//Exit
            return this->option_quit();
        }
        else if(option_chosen == case_trains_from_X_to_Y){//option 2
            return status_logged + this->option_trains_from_station_X_to_station_Y();
        }
        else if (option_chosen == case_departure_in_next_hour ){// option 3
            return status_logged + this->option_trains_which_leave_in_the_next_hour();
        }
        else if(option_chosen == case_arrival_in_next_hour){// option 4
            return status_logged + this->option_trains_which_arrive_in_the_next_hour();
        }
        else if(option_chosen == case_login){
            std::string resp_option_login = this->option_login(); 
            if( resp_option_login== "Logged in successfully!"){
                return this->start_delimiter + "1" + this->stop_delimiter + "Logged in successfully!";
            }
            else{
                return status_logged + resp_option_login;
            }
        }
        else{
            return status_logged + "Unknown command";
        }
        
    }
    else{//the user is an administrator
        std::string case_exit = this->start_delimiter + "0" + this->stop_delimiter;
        std::string case_logout =  this->start_delimiter + "1" + this->stop_delimiter;
        std::string case_add_delay =  this->start_delimiter + "2" + this->stop_delimiter;
        std::string case_add_train =  this->start_delimiter + "3" + this->stop_delimiter;
        std::string case_delete_train = this->start_delimiter + "4" + this->stop_delimiter;
        std::string case_add_new_account = this->start_delimiter + "5" + this->stop_delimiter;
        std::string case_change_username = this->start_delimiter + "6" + this->stop_delimiter;
        std::string case_change_password = this->start_delimiter + "7" + this->stop_delimiter;
        std::string case_trains_from_X_to_Y =  this->start_delimiter + "8" + this->stop_delimiter;
        std::string case_departure_in_next_hour = this->start_delimiter + "9" + this->stop_delimiter;
        std::string case_arrival_in_next_hour = this->start_delimiter + "10" + this->stop_delimiter;
        std::string case_all_trains = this->start_delimiter + "11" + this->stop_delimiter;

        if(option_chosen == case_exit){
            return this->option_quit();
        }
        else if( option_chosen == case_add_delay){
            return status_logged + this->add_delay_option();
        }
        else if( option_chosen == case_add_train){
            return status_logged + this->add_train_option();
        }
        else if( option_chosen == case_delete_train){
            return status_logged + this->delete_train_option();
        }
        else if(option_chosen == case_add_new_account){
            return status_logged + this->add_new_account_option();
        }
        else if(option_chosen == case_change_username){
            return status_logged + this->change_username_option();
        }
        else if(option_chosen == case_change_password){
            return status_logged + this->change_password_option();
        }
        else if ( option_chosen == case_trains_from_X_to_Y){
            return status_logged + this->option_trains_from_station_X_to_station_Y();
        }
        else if ( option_chosen == case_departure_in_next_hour){
            return status_logged + this->option_trains_which_leave_in_the_next_hour();
        }
        else if ( option_chosen == case_arrival_in_next_hour){
            return status_logged + this->option_trains_which_arrive_in_the_next_hour();
        }
        else if (option_chosen == case_logout){
            return this->start_delimiter + "0" + this->stop_delimiter;
        }
        else if (option_chosen == case_all_trains){
            return status_logged + this->show_all_trains_option();
        }
        else{
        return status_logged + "Unknown command";
        }
    }
}


void * treat_client::worker_thread (void * arg){
    printf("[Active thread number : %d - Asteptam mesajul...\n", treat_client::number_of_active_threads);
    pthread_detach(pthread_self());//the main thread doesn t need to wait for the execution of this thread
    
    char buffer[1024], i=0;
    int ds_cl = ((treat_client*)arg)->desciptor_client;
    int nr_read;
    int quit=0;
    while(!quit){
        if ((nr_read = read (ds_cl, buffer,sizeof(buffer))) <= 0){
            printf("[Thread]\n");
            perror ("Read() Error. The client closed the app unexpectedly\n");
            quit=1;//if the client leaves before giving a response (i.e. closes the console or kills the process)
        }
        else{

            buffer[nr_read] = '\0';
    
            //printf("[Thread] Mesajul a fost receptionat...%s\n",(((treat_client *)arg)->message_received).c_str());
            ((treat_client *)arg)->message_received = buffer;//the message from the Client

            printf("[Thread] The message was received...\n");

            std::string result = ((treat_client *)arg)->response();

            if(result == "The client exited"){//if the command send by the client is exit, then the server won't send to the client any message
                quit = 1;
            }
            else{
                /* write the response to the client */
                if (write (ds_cl, result.c_str(), result.length()) <= 0){
                    // where ds_cl = ((treat_client*)arg)->desciptor_client;
                    perror ("[Thread]Write() Error. The response couldn't send to the client\n");
                    quit=1;//if the client leaves before getting an answer,
                }
                else{
                    printf ("[Thread] The response () was successfully sent.\n");
                }
            }
        }

    }//while
    printf("\nDone serving this client\n");
    close((intptr_t)arg);//we close the connection with this client
    treat_client::number_of_active_threads -- ;
    return (NULL);

}


std::string treat_client::string_between_2_delimiters_and_erase (){
    int pos_start_delim = this->message_received.find(this->start_delimiter);
    int pos_stop_delim = this->message_received.find(this->stop_delimiter);

    std::string str = this->message_received.substr(pos_start_delim,pos_stop_delim+this->stop_delimiter.length());

    this->message_received.erase(pos_start_delim,pos_stop_delim+this->stop_delimiter.length());

    return str;
}

std::string treat_client::delete_the_delimiters_from_a_string (std::string str_with_delimiters){
    if(str_with_delimiters.length()>this->start_delimiter.length()+this->stop_delimiter.length()){//this condition will always be true, but we put it for extra-safety
        str_with_delimiters.erase(0,this->start_delimiter.length());
        str_with_delimiters.erase(str_with_delimiters.length()-this->stop_delimiter.length(),str_with_delimiters.length());
    }

    return str_with_delimiters;
}


bool treat_client::verify_existance_of_station(std::string name_of_station){
    char query_verify_existance_of_stations[200];
    sprintf(query_verify_existance_of_stations,"SELECT 'c' from arrivals_departures WHERE UPPER(TRIM(station_name))=UPPER(TRIM('%s'));",name_of_station.c_str());
    std::string res;
    res = this->DB_connection.get_result_of_the_query(query_verify_existance_of_stations) ; 
    if ( res == "" || res.substr(0,5) == "ERROR"){
        return false;
    }
    else{
        return true;
    }
}


std::string treat_client::option_quit(){
    return "The client exited";
}


 std::string treat_client::option_login(){

    std::string username = this->string_between_2_delimiters_and_erase();
    username = this->delete_the_delimiters_from_a_string(username);

    std::string password = this->string_between_2_delimiters_and_erase();
    password = this->delete_the_delimiters_from_a_string(password);

    char query_verify_existance_of_user_and_psswd[200];
    sprintf(query_verify_existance_of_user_and_psswd,"SELECT 'c' from users WHERE username='%s';",username.c_str());
    std::string res;
    res = this->DB_connection.get_result_of_the_query(query_verify_existance_of_user_and_psswd) ; 
    if ( res == "" || res.substr(0,5) == "ERROR"){
        return "Sorry we couldn't find any user with this username";
    }
    else{
        sprintf(query_verify_existance_of_user_and_psswd,"SELECT 'c' from users WHERE username='%s' AND password='%s';",username.c_str(),password.c_str());
        res = this->DB_connection.get_result_of_the_query(query_verify_existance_of_user_and_psswd) ;
        if ( res == "" || res.substr(0,5) == "ERROR"){
            return "Sorry, the password is incorrect!";
        }
        else{
            return "Logged in successfully!";
        }
    }
}


std::string treat_client::option_trains_from_station_X_to_station_Y(){
    std::string departure_station = this->string_between_2_delimiters_and_erase();
    //erase the delimitators (departure_station = <name_of_the_station>)
    departure_station = this->delete_the_delimiters_from_a_string(departure_station);
    // now, departure_station = name_of_the_station

    std::string arrival_station = this->string_between_2_delimiters_and_erase();
    arrival_station = this->delete_the_delimiters_from_a_string(arrival_station);

    std::string date = this->string_between_2_delimiters_and_erase();
    date = this->delete_the_delimiters_from_a_string(date);
    
    //verifiy
    if( verify_existance_of_station(departure_station) == false){//it doesn t exist a station with that name
        return "The name of the departure station doesn't exist!";
    }
    
    if( verify_existance_of_station(arrival_station) == false){// --//--
        return "The name of the arrival station doesn't exist!";
    }

    char query_verify_existance_of_date[200];
    sprintf(query_verify_existance_of_date,"SELECT 'c' from arrivals_departures WHERE DATE(arrival)='%s';",date.c_str());
    std::string res;
    res = this->DB_connection.get_result_of_the_query(query_verify_existance_of_date);
    if (res == ""){
        return "We are sorry, but we couldn't find any train on date " + date +  " :("; 
    }
    else if (res.substr(0,5) == "ERROR"){
        //return "The date was written wrong or it doesn't exist!";
        return res;
    }
    //done verifing

    char query_response[1024];

    sprintf(query_response,
    "SELECT a1.id_train AS \"ID\", a1.station_name AS \"Depart. Station\", a1.departure as \"Departure time\", IF((a1.delay IS NULL) OR a1.delay<0,0,a1.delay) as \"Delay\", TIME(a1.departure + INTERVAL IF((a1.delay IS NULL) OR a1.delay<0,0,a1.delay) MINUTE) as \"Actual depart. time\", a2.station_name as \"Arriv. Station\", a2.arrival as \"Arrival time\", IF(a2.delay IS NULL,0,IF(a2.delay<0,0,a2.delay)) as \"Delay\", IF(a2.delay IS NULL,0,IF(a2.delay<0,-a2.delay,0)) AS \"Arrives sonner with:\", TIME(a2.arrival + INTERVAL a2.delay MINUTE) as \"Actual arriv time\" FROM arrivals_departures a1 JOIN arrivals_departures a2 ON a1.id_train=a2.id_train AND UPPER(TRIM(a1.station_name))=UPPER(TRIM('%s')) AND UPPER(TRIM(a2.station_name))=UPPER(TRIM('%s')) AND DATE(a1.departure) = '%s' AND UPPER(TRIM(a1.final_station))=UPPER(TRIM(a2.final_station)) AND a1.arrival < a2.arrival;",
    departure_station.c_str(), arrival_station.c_str(), date.c_str());

    res = this->DB_connection.get_result_of_the_query(query_response);
    
    if ( res != "")
    return res;
    else
    return "We are sorry, but we couldn't find any trains that correspond to your request :(\n";

}

std::string treat_client::option_trains_which_leave_in_the_next_hour(){
    std::string station = this->string_between_2_delimiters_and_erase();
    //erase the delimitators from <station>
    station = this->delete_the_delimiters_from_a_string(station);

    if( verify_existance_of_station(station) == false){
        return "The name of the station doesn't exist!";
    }

    char query_response[1024];
    sprintf(query_response,
    "SELECT id_train, station_name as \"Station Name\", departure as \"Departure time\", IF((delay IS NULL) OR delay<0,0,delay) as \"Delay\", departure + INTERVAL IF((delay IS NULL) OR delay<0,0,delay) MINUTE as \"Actual departure time\" from arrivals_departures WHERE UPPER(TRIM(station_name))='%s' AND TIMEDIFF(departure,SYSDATE())<='01:00:00' AND TIMEDIFF(departure, SYSDATE()) >= 0;"
    ,station.c_str());

    std::string res ;
    res = this->DB_connection.get_result_of_the_query(query_response);

    if ( res != "")
    return res;
    else
    return "We are sorry, but we couldn't find any trains which leave from the station " + station + " in the next hour :( ";   
}

std::string treat_client::option_trains_which_arrive_in_the_next_hour(){
    std::string station = this->string_between_2_delimiters_and_erase();
    //erase the delimitators from <station>
    station = this->delete_the_delimiters_from_a_string(station);

    if( verify_existance_of_station(station) == false){
        return "The name of the station doesn't exist!";
    }

    char query_response[1024];
    sprintf(query_response,
    "SELECT id_train, station_name as \"Station Name\", arrival as \"arrival time\", IF(delay IS NULL,0,IF(delay<0,0,delay)) as \"Delay\", IF(delay IS NULL,0,IF(delay<0,-delay,0)) AS \"Arrives sonner with:\", arrival + INTERVAL delay MINUTE as \"Actual arrival time\" from arrivals_departures WHERE UPPER(TRIM(station_name))='%s' AND TIMEDIFF(arrival,SYSDATE())<='01:00:00' AND TIMEDIFF(arrival, SYSDATE()) >= 0;"
    ,station.c_str());

    std::string res ;
    res = this->DB_connection.get_result_of_the_query(query_response);

    if ( res != "")
    return res;
    else
    return "We are sorry, but we couldn't find any trains which leave from the station " + station + " in the next hour :( ";   

}


std::string treat_client::add_delay_option(){
    //we'll get the message from the client as <id_train><station_name><date><final_station><delay>

//(WE ASSUME  that we cannot have the same train (having the same id) going to the same final_station in the same day);

    std::string id_train = this->string_between_2_delimiters_and_erase();
    id_train = this->delete_the_delimiters_from_a_string(id_train);//erase the delimitators

    std::string station_name = this->string_between_2_delimiters_and_erase();
    station_name = this->delete_the_delimiters_from_a_string(station_name);

    std::string date = this->string_between_2_delimiters_and_erase();
    date=this->delete_the_delimiters_from_a_string(date);

    std::string final_station = this->string_between_2_delimiters_and_erase();
    final_station = this->delete_the_delimiters_from_a_string(final_station);

    std::string delay = this->string_between_2_delimiters_and_erase();
    delay=this->delete_the_delimiters_from_a_string(delay);//erase the delimitators


    char verify_id_train[1024];
    sprintf(verify_id_train,
            "SELECT 'c' from trains WHERE id_train='%s';",
            id_train.c_str());
            
    std::string res ;
    res = this->DB_connection.get_result_of_the_query(verify_id_train);

    if ( res == "" || res.substr(0,5) == "ERROR"){
        return "There is no train with such ID(" + id_train + ");";
    }

    if (this->verify_existance_of_station(station_name) == false){
        return "The name of the STATION doesn't exist!";
    }

    char query_verify_existance_of_date[200];
    sprintf(query_verify_existance_of_date,"SELECT 'c' from arrivals_departures WHERE DATE(arrival)='%s';",date.c_str());
    res = this->DB_connection.get_result_of_the_query(query_verify_existance_of_date);
    if ( res == "" || res.substr(0,5) == "ERROR"){
        //return "The date was written wrong or it doesn't exist!";
        return res;
    }


    if (this->verify_existance_of_station(final_station) == false){
        return "The name of the FINAL STATION doesn't exist!";
    }


    //we already put the condions in the client (so that the given delay is actually a number)

    //int delay_nr = std::stoi(delay);

    //check if there is a train with the specific id, station_name, date and final_station;
    char check_insert[1024];
    sprintf(check_insert,
    "SELECT 'c' from arrivals_departures WHERE UPPER(TRIM(id_train))=UPPER(TRIM('%s')) AND UPPER(TRIM(station_name))=UPPER(TRIM('%s')) AND DATE(arrival)='%s' AND UPPER(TRIM(final_station))=UPPER(TRIM('%s'));",
    id_train.c_str(),station_name.c_str(),date.c_str(),final_station.c_str());

    res= this->DB_connection.get_result_of_the_query(check_insert);
    if( res == "" || res.substr(0,5) == "ERROR"){
        return "Sorry, the update couldn t be made as there is no train with id: "+ id_train +" which arrives in station " + station_name + " (date : " + date + ") and has as final station: " + final_station + " :(";
    }
    else{
    //insert the delay
        char insert_delay[1024];

        sprintf(insert_delay,
        "UPDATE arrivals_departures SET delay = %s WHERE UPPER(TRIM(id_train))=UPPER(TRIM('%s')) AND UPPER(TRIM(station_name))=UPPER(TRIM('%s')) AND DATE(arrival)='%s' AND UPPER(TRIM(final_station))=UPPER(TRIM('%s'));",
        delay.c_str(),id_train.c_str(),station_name.c_str(),date.c_str(),final_station.c_str());

        //execute the query
        res = this->DB_connection.get_result_of_the_query(insert_delay);

        return res;
    }

}

std::string treat_client::add_train_option(){
    //we'll get the message as: <id_train><station><arrival_time><departure_time><final_station>

    std::string id_train = this->string_between_2_delimiters_and_erase();
    id_train = this->delete_the_delimiters_from_a_string(id_train);//erase the delimitators

    std::string station_name = this->string_between_2_delimiters_and_erase();
    station_name = this->delete_the_delimiters_from_a_string(station_name);

    std::string arrival = this->string_between_2_delimiters_and_erase();
    arrival=this->delete_the_delimiters_from_a_string(arrival);

    std::string departure = this->string_between_2_delimiters_and_erase();
    departure = this->delete_the_delimiters_from_a_string(departure);

    std::string final_station = this->string_between_2_delimiters_and_erase();
    final_station=this->delete_the_delimiters_from_a_string(final_station);//erase the delimitators

    //if it's a new train (the id_train doesn t exist in trains table, then we have to add it there too)
    char insert_id_train[1024];
    sprintf(insert_id_train,
    "INSERT INTO trains VALUES (UPPER(TRIM('%s')));",
    id_train.c_str()
    );
    std::string res,response_to_client;
    res = this->DB_connection.get_result_of_the_query(insert_id_train);
    printf("The response in TRAINS is %s\n", res.c_str());
    if(res=="The insert of the train was successfully made!"){
        response_to_client = "The train which you want to add is a new one ( the ID: " + id_train + " didn't exist )";
    }
    else{//res.substr(0,5)="ERROR"
        response_to_client = "It already exists a train with ID: " + id_train;
    }

    // now we add it in arrivals_departures

    char insert_train[1024];
    sprintf(insert_train,
    "INSERT INTO arrivals_departures VALUES ('%s',upper(trim('%s')),'%s','%s',0,'%s');",
    station_name.c_str(),id_train.c_str(),arrival.c_str(),departure.c_str(),final_station.c_str()
    );
    res = this->DB_connection.get_result_of_the_query(insert_train);

    if(res=="The insert of the train was successfully made!"){
        response_to_client += "\n The train added successfully";
    }
    else{
        response_to_client += "\n The train couldn't be added as you didn't respect the format of the arrival/departure time";
    }

    return response_to_client;
}

std::string treat_client::delete_train_option(){

    std::string id_train = this->string_between_2_delimiters_and_erase();
    id_train = this->delete_the_delimiters_from_a_string(id_train);

    char verify_existance_of_id_train[1024];

    sprintf(verify_existance_of_id_train,
    "SELECT 'c' FROM trains where upper(trim(id_train))=upper(trim('%s'));",
    id_train.c_str());
    std::string res;

    res= this->DB_connection.get_result_of_the_query(verify_existance_of_id_train);
    if(res == "" ){
        return "There is no train with such ID (" + id_train + ");";
    }
    else{
        char delete_train[1024];
        sprintf(delete_train,
        "DELETE FROM trains WHERE upper(trim(id_train))=upper(trim('%s'));",
        id_train.c_str());
    
        res = this->DB_connection.get_result_of_the_query(delete_train);
        if(res == "ERROR"){
            return "There was an Error deleting this train:(\n Please try again";
        }
        else{
        return "The train with the ID("+id_train+ ") deleted successfully;";
        }
    }

}


std::string treat_client::add_new_account_option(){

    std::string username = this->string_between_2_delimiters_and_erase();
    username = this->delete_the_delimiters_from_a_string(username);

    std::string password = this->string_between_2_delimiters_and_erase();
    password = this->delete_the_delimiters_from_a_string(password);

    char query_verify_existance_of_user_and_psswd[200];
    sprintf(query_verify_existance_of_user_and_psswd,"SELECT 'c' from users WHERE username='%s';",username.c_str());
    std::string res;
    res = this->DB_connection.get_result_of_the_query(query_verify_existance_of_user_and_psswd) ; 
    if ( res == "" ) {
        char insert_user_and_password[1024];
        sprintf(insert_user_and_password,"INSERT INTO users VALUES ('%s','%s');",username.c_str(),password.c_str());
        res = this->DB_connection.get_result_of_the_query(insert_user_and_password) ;
        if ( res.substr(0,5) == "ERROR"){
            return "Sorry, it couldn't create a new account, please try again!";
        }
        else{
            return "New account created successfully";
        }
    }
    else if (res.substr(0,5) == "ERROR"){
        return "Sorry, it couldn't create a new account, please try again!";
    }
    else{
        return "Sorry, this username is already taken";
    }
}

std::string treat_client::change_username_option(){
printf("USERNAME\n");
    std::string username = this->string_between_2_delimiters_and_erase();
    username = this->delete_the_delimiters_from_a_string(username);

    std::string password = this->string_between_2_delimiters_and_erase();
    password = this->delete_the_delimiters_from_a_string(password);

    std::string new_username = this->string_between_2_delimiters_and_erase();
    new_username = this->delete_the_delimiters_from_a_string(new_username);

    std::string res;
    char query_verify_existance_of_user_and_psswd[1024];
    sprintf(query_verify_existance_of_user_and_psswd,
    "SELECT 'c' from users WHERE username='%s' AND password='%s';",
    username.c_str(),password.c_str()
    );

    res = this->DB_connection.get_result_of_the_query(query_verify_existance_of_user_and_psswd) ;
    if ( res == "" || res.substr(0,5) == "ERROR"){
        return "Sorry, password or username incorrect";
    }
    else{
        char update_username[1024];
        sprintf(update_username,
        "UPDATE users SET username='%s' WHERE username='%s' AND password='%s';",
        new_username.c_str(),username.c_str(),password.c_str()
        );
        res = this->DB_connection.get_result_of_the_query(update_username);
        printf("The result is : %s\n", res.c_str());
        if(res.substr(0,5) == "ERROR"){
            return "Sorry, the username couldn't be changed:(\n Please try again;";
        }
        else {
            return "The username modified successfully;";
        }
    }

}


std::string treat_client::change_password_option(){
printf("PASSWORD\n");
    std::string username = this->string_between_2_delimiters_and_erase();
    username = this->delete_the_delimiters_from_a_string(username);

    std::string password = this->string_between_2_delimiters_and_erase();
    password = this->delete_the_delimiters_from_a_string(password);

    std::string new_password = this->string_between_2_delimiters_and_erase();
    new_password = this->delete_the_delimiters_from_a_string(new_password);

    std::string res;
    char query_verify_existance_of_user_and_psswd[1024];
    sprintf(query_verify_existance_of_user_and_psswd,
    "SELECT 'c' from users WHERE username='%s' AND password='%s';",
    username.c_str(),password.c_str()
    );

    res = this->DB_connection.get_result_of_the_query(query_verify_existance_of_user_and_psswd) ;
    if ( res == "" || res.substr(0,5) == "ERROR"){
        return "Sorry, password or username incorrect";
    }
    else{
        char update_password[1024];
        sprintf(update_password,
        "UPDATE users SET password='%s' WHERE username='%s' AND password='%s';",
        new_password.c_str(),username.c_str(),password.c_str()
        );
        res= this->DB_connection.get_result_of_the_query(update_password);
        if(res.substr(0,5) == "ERROR"){
            return "Sorry, the password couldn't be changed:(\n Please try again;";
        }
        else {
            return "The password modified successfully;";
        }
    }
}
std::string treat_client::show_all_trains_option(){
    std::string res;
    char query_show_all_trains[1024];
    sprintf(query_show_all_trains,"SELECT id_train AS \"ID\", station_name as \"Station Name\", arrival, IF(delay IS NULL,0,IF(delay<0,0,delay)) as \"Delay\", IF(delay IS NULL,0,IF(delay<0,-delay,0)) AS \"Arrives sonner with:\",departure,final_station as \"Final Station\" from arrivals_departures WHERE date(arrival)=date(sysdate());");
    res = this->DB_connection.get_result_of_the_query(query_show_all_trains);
    if( res=="" ){
        return "Sorry, there are no trains today";
    }
    else if(res.substr(0,5) == "ERROR"){
        return "Sorry, it couldn't show all trains:(\n Please try again;";
    }
    else {
        return res;
    }
}
