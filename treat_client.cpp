#include "treat_client.h"
int treat_client::number_of_active_threads = 0;//initialize the number of active threads

treat_client::treat_client (int descriptor, MYSQL *connection) : DB_connection(connection){
    this->desciptor_client = descriptor ;
    this->number_of_active_threads ++;
    //printf("CONSTRUCTOR TREAT CLIENT \n");
} 

void treat_client::create_thread(){
    
    pthread_t t;
    pthread_create(&t,NULL,worker_thread, this);

}

std::string treat_client::response(){
    //the message will be recieved in this format  :
    // <status_logged><number_option><aditional_info1_for_option>...etc 


    std::string status_logged = this->string_between_2_delimiters_and_erase();
    printf("Status logged is : %s \n", status_logged.c_str());
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
        std::string case_trains_from_X_to_Y =  this->start_delimiter + "7" + this->stop_delimiter;
        std::string case_departure_in_next_hour = this->start_delimiter + "8" + this->stop_delimiter;
        std::string case_arrival_in_next_hour = this->start_delimiter + "9" + this->stop_delimiter;

        if(option_chosen == case_exit){
            return status_logged + this->option_quit();
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
        else{
        return status_logged + "idk yet";
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
            printf ("[Thread] Mesajul a fost receptionat...%s\n", buffer);
    
            ((treat_client *)arg)->message_received = buffer;

            printf("Copy-ul a fost facut : %s\n",(((treat_client *)arg)->message_received).c_str());
            char result[1024];
            strcpy(result,(((treat_client *)arg)->response()).c_str());

            if(strcmp(result,"The client exited") == 0){//if the command send by the client is exit, then the server won t send to the client any message
                quit = 1;
            }
            else{
                /* write the response to the client */
                //printf("\n\n\nMEsajul care se trimite : %s\n\n\nAre lungime : %ld",result,strlen(result));
                if (write (ds_cl, result, sizeof(result)) <= 0){
                    // where ds_cl = ((treat_client*)arg)->desciptor_client;
                    perror ("[Thread]Write() Error. The response couldn't send to the client\n");
                    quit=1;//if the client leaves before getting an answer,
                }
                else
                    printf ("[Thread] The response () was successfully sent.\n");
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


bool treat_client::verify_existance_of_station(std::string name_of_station){
    char query_verify_existance_of_stations[200];
    sprintf(query_verify_existance_of_stations,"SELECT 'c' AS \"col\" from arrivals_departures WHERE UPPER(TRIM(station_name))=UPPER(TRIM('%s'));",name_of_station.c_str());
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
    username.erase(0,this->start_delimiter.length());
    username.erase(username.length()-this->stop_delimiter.length(),username.length());

    std::string password = this->string_between_2_delimiters_and_erase();
    password.erase(0,this->start_delimiter.length());
    password.erase(password.length()-this->stop_delimiter.length(),password.length());

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
    departure_station.erase(0,this->start_delimiter.length());
    departure_station.erase(departure_station.length()-this->stop_delimiter.length(),departure_station.length());
    // now, departure_station = name_of_the_station

    std::string arrival_station = this->string_between_2_delimiters_and_erase();
    arrival_station.erase(0,this->start_delimiter.length());
    arrival_station.erase(arrival_station.length()-this->stop_delimiter.length(),arrival_station.length());

    std::string date = this->string_between_2_delimiters_and_erase();
    date.erase(0,this->start_delimiter.length());
    date.erase(date.length()-this->stop_delimiter.length(),date.length());
    
    //verifiy
    if( verify_existance_of_station(departure_station) == false){//it doesn t exist a station with that name
        return "The name of the departure station doesn't exist!";
    }
    
    if( verify_existance_of_station(arrival_station) == false){// --//--
        return "The name of the arrival station doesn't exist!";
    }

    char query_verify_existance_of_date[200];
    sprintf(query_verify_existance_of_date,"SELECT 'c' AS \"col\" from arrivals_departures WHERE DATE(arrival)='%s';",date.c_str());
    std::string res;
    res = this->DB_connection.get_result_of_the_query(query_verify_existance_of_date);
    if ( res == "" || res.substr(0,5) == "ERROR"){
        //return "The date was written wrong or it doesn't exist!";
        return res;
    }
    //done verifing

    char query_response[1024];

    sprintf(query_response,
    "SELECT a1.id_train, a1.station_name AS \"Departure Station\",a1.departure, a1.delay, a2.station_name as \"Arrival Station\",a2.arrival, a2.delay FROM arrivals_departures a1 JOIN arrivals_departures a2 ON a1.id_train=a2.id_train AND UPPER(TRIM(a1.station_name))=UPPER(TRIM('%s')) AND UPPER(TRIM(a2.station_name))=UPPER(TRIM('%s')) AND DATE(a1.departure) = '%s' AND UPPER(TRIM(a1.final_station))=UPPER(TRIM(a2.final_station)) AND a1.arrival < a2.arrival;",
    departure_station.c_str(), arrival_station.c_str(), date.c_str());

    //sprintf(query_response, "SELECT * from arrivals_departures;");
    res = this->DB_connection.get_result_of_the_query(query_response);
    if ( res != "")
    return res;
    else
    return "We are sorry, but we couldn't find any trains that correspond to your request :(\n";

}

std::string treat_client::option_trains_which_leave_in_the_next_hour(){
    std::string station = this->string_between_2_delimiters_and_erase();
    //erase the delimitators from <station>
    station.erase(0,this->start_delimiter.length());
    station.erase(station.length()-this->stop_delimiter.length(),station.length());

    if( verify_existance_of_station(station) == false){
        return "The name of the station doesn't exist!";
    }

    char query_response[1024];
    sprintf(query_response,
    "SELECT id_train, station_name as \"Station Name\", departure as \"Departure time\", IF((delay IS NULL) OR delay<0,0,delay) as \"Delay\", departure + INTERVAL delay MINUTE as \"Actual departure time\" from arrivals_departures WHERE UPPER(TRIM(station_name))='%s' AND TIMEDIFF(departure,SYSDATE())<='01:00:00' AND TIMEDIFF(departure, SYSDATE()) >= 0;"
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
    station.erase(0,this->start_delimiter.length());
    station.erase(station.length()-this->stop_delimiter.length(),station.length());

    if( verify_existance_of_station(station) == false){
        return "The name of the station doesn't exist!";
    }

    char query_response[1024];
    sprintf(query_response,
    "SELECT id_train, station_name as \"Station Name\", departure as \"Departure time\", IF(delay IS NULL,0,IF(delay<0,0,delay)) as \"Delay\", IF(delay IS NULL,0,IF(delay<0,-delay,0)) AS \"Comes sonner with:\", departure + INTERVAL delay MINUTE as \"Actual arrival time\" from arrivals_departures WHERE UPPER(TRIM(station_name))='%s' AND TIMEDIFF(departure,SYSDATE())<='01:00:00' AND TIMEDIFF(departure, SYSDATE()) >= 0;"
    ,station.c_str());

    std::string res ;
    res = this->DB_connection.get_result_of_the_query(query_response);

    if ( res != "")
    return res;
    else
    return "We are sorry, but we couldn't find any trains which leave from the station " + station + " in the next hour :( ";   

}