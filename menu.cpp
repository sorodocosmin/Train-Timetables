#include "menu.h"

menu::menu(){
    this->welcome_message = "\n---------------------------------------\n| Hello ! Welcome to the TRAINS APP ! |\n---------------------------------------\n\n";
    this->regular_menu_message = " Please introduce only the number coresponding with the command that you want to execute : \n- 0. Exit \n- 1. Login \n- 2. Show trains from stataion \"X\" to station \"Y\" in a date \"D\" \n- 3. Show the trains which will leave from station \"X\" in the next hour\n- 4. Show the trains wich will arrive to station \"X\" in the next hour\n";
    this->administrator_menu_message = "Please introduce only the number coresponding with the command that you want to execute : \n- 0. Exit \n- 1. Logout \n- 2. Insert delay(or if it arrives sonner) on a train at arrival in station \"X\"\n- 3. Add new train or insert a new station for an existing one \n- 4. Delete train \n- 5. Create another Administrator account \n- 6. Modify username \n- 7. Modify password\n- 8. Show trains from station \"X\" to station \"Y\" in a date \"D\" \n- 9. Show the trains which will leave from station \"X\" in the next hour\n- 10. Show the trains wich will arrive to station \"X\" in the next hour\n- 11. Show all trains for today\n";
    this->bye_message = "\n--------------------------------\n| You closed the APP :( . Bye. |\n--------------------------------\n";
    this->send_to_server_message = "";
    
    this->quit_status = false;
    this->logged_status = false;
}

std::string menu::get_welcome_message(){
    return this->welcome_message;
}
std::string menu::get_regular_menu_message(){
    return this->regular_menu_message;
}
std::string menu::get_administrator_menu_message(){
    return this->administrator_menu_message;
}
std::string menu::get_bye_message(){
    return this->bye_message;
}
std::string menu::get_the_message_which_will_be_send_to_the_server(){
    return this->send_to_server_message;
}
bool menu::get_quit_status(){
    return this->quit_status;
}
bool menu::get_logged_status(){
    return this->logged_status;
}

void menu::handle_client_option(std::string option){

    if( this->logged_status == false ){//regular user
        this->send_to_server_message = this->start_delimiter + "0" + this->stop_delimiter;//<logged_status>
        if(option == "0\n"){//exit
            this->quit_option();
        }
        else if (option == "1\n"){//login
            this->login_option();
        }
        else if(option == "2\n"){//show the trains from station X to Y
            this->trains_from_station_X_to_station_Y_option();
        }
        else if(option == "3\n"){//show the trains which leava in the next hour from a station X
            this->trains_which_leave_in_the_next_hour_option();
        }
        else if(option == "4\n"){//show the trains which arrive in the next hour in a station X
            this->trains_which_leave_in_the_next_hour_option();
        }
        else{
            this->send_to_server_message += this->start_delimiter + "Unknown command" + this->stop_delimiter;
        }
    }
    else{//the user is an Administrator
        this->send_to_server_message = this->start_delimiter + "1" + this->stop_delimiter;
        if(option == "0\n")
            this->quit_option();
        else if (option == "1\n")
            this->logout();
        else if( option == "2\n"){
            this->add_delay_option();
        }
        else if( option == "3\n"){
            this->add_train_option();
        }
        else if (option == "4\n"){
            this->delete_train_option();
        }
        else if(option == "5\n"){
            this->add_new_account_option();
        }
        else if(option == "6\n"){
            this->change_username_option();
        }
        else if(option == "7\n"){
            this->change_password_option();
        }
        else if ( option == "8\n"){
            this->trains_from_station_X_to_station_Y_option();
        }
        else if ( option == "9\n"){
            this->trains_which_leave_in_the_next_hour_option();
        }
        else if ( option == "10\n"){
            this->trains_which_arrive_in_the_next_hour_option();   
        }
        else if (option=="11\n"){
            this->show_all_trains_option();
        }
        else{
            this->send_to_server_message += this->start_delimiter + "Unknown command" + this->stop_delimiter;
        }
    }
    

}

void menu::clear_message_send(){

    this->send_to_server_message = "";
}
void menu::quit_option(){

    this->send_to_server_message += this->start_delimiter + "0" + this->stop_delimiter;//option 0
    this->quit_status = true;

}

std::string menu::text_between_delimiters(std::string text){

    return this->start_delimiter+ text + this->stop_delimiter;

}
std::string menu::read_the_extra_info_from_the_user(std::string default_err_message,std::string err_message_nr_of_chars_is_bigger, int max_nr_of_chars_which_can_be_read){

    char read_input[1024];

    int nr_bytes_read;
    nr_bytes_read = read (0,read_input,sizeof(read_input));

    while(nr_bytes_read < 1 || nr_bytes_read > max_nr_of_chars_which_can_be_read){// if the input from the users is "too big" or it cannot be read
        if(nr_bytes_read == -1){
            perror(default_err_message.c_str());
            fflush(stdout); 
        }
        else{
            printf("%s",err_message_nr_of_chars_is_bigger.c_str());
            fflush(stdout);
        }

        printf("Please try to introduce it again : ");
        fflush(stdout);
        nr_bytes_read = read (0,read_input,sizeof(read_input));
    }

    read_input[nr_bytes_read-1] = '\0';

    return read_input;

}
std::string menu::read_date_from_the_user (){
    char date[1024];
    int nr_bytes_read = read (0,date,sizeof(date));
    while(nr_bytes_read == -1 || nr_bytes_read != 11){
        if( nr_bytes_read == -1){
            perror("Sorry, it seems like there was an error while reading the DATE :");
            printf("Please try to introduce it again (YYYY-MM-DD) : ");
            fflush(stdout);
        }
        else{
            printf("Sorry, the length of a date has to be exactly 10(YYYY-MM-DD)\n");
            printf("Please try to introduce it again (YYYY-MM-DD) : ");
            fflush(stdout); 
        }
        nr_bytes_read = read (0,date,sizeof(date));
    }
    date[nr_bytes_read-1] = '\0';
    return date;
}

std::string menu::read_date_and_time_from_the_user(){
    char date[1024];
    int nr_bytes_read = read (0,date,sizeof(date));
    while(nr_bytes_read == -1 || nr_bytes_read != 17){
        if( nr_bytes_read == -1){
            perror("Sorry, it seems like there was an error while reading the DATE :");
            printf("Please try to introduce it again (YYYY-MM-DD HH:MM) : ");
            fflush(stdout);
        }
        else{
            printf("Sorry, the length of a date has to be exactly 16(YYYY-MM-DD HH:MM)\n");
            printf("Please try to introduce it again (YYYY-MM-DD HH:MM) : ");
            fflush(stdout); 
        }
        nr_bytes_read = read (0,date,sizeof(date));
    }
    date[nr_bytes_read-1] = '\0';
    return date;
}

void menu::login_option(){

    this->send_to_server_message += this->start_delimiter + "1" + this->stop_delimiter;//option1

    std::string username,password;

    printf(" Introduce the username: ");
    fflush(stdout);
    username = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the username :(",
        "Sorry, the maximum length of a username is 20\n",
        20);

    this->send_to_server_message += this->text_between_delimiters(username);//+<username>

    printf(" Introduce the password: ");
    fflush(stdout);
    password = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the password :( ",
        "Sorry, the maximum length of a password is 30\n",
        30
    );

    this->send_to_server_message += this->text_between_delimiters(password);//+<psswd>

}

void menu::trains_from_station_X_to_station_Y_option(){
    if ( this->logged_status == false){
        this->send_to_server_message += this->start_delimiter + "2" + this->stop_delimiter;//+<option_chosen>
    }
    else{//the user is already logged in and he/she is an Administrator
        this->send_to_server_message += this->start_delimiter + "8" + this->stop_delimiter;
    }

    std::string departure_station,arrival_station,date;
    int nr_bytes_read;

    printf(" Introduce the name of the DEPARTURE STATION : ");
    fflush(stdout);
    departure_station = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the name of the DEPARTURE STATION :",
        "Sorry, the maximum length of a STATION is 30\n",
        30
    );

    this->send_to_server_message += this->text_between_delimiters(departure_station);//+<depart_staion>

    printf(" Introduce the name of the ARRIVAL STATION : ");
    fflush(stdout);
    arrival_station = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the name of the ARRIVAL STATION :",
        "Sorry, the maximum length of a STATION is 30\n",
        30
    );
    
    this->send_to_server_message += this->text_between_delimiters(arrival_station);//+<arrival_station>

    printf(" Introduce the DATE (YYYY-MM-DD) : ");
    fflush(stdout);

    date = this->read_date_from_the_user();

    this->send_to_server_message += this->text_between_delimiters(date);//+<date>

}

void menu::trains_which_leave_in_the_next_hour_option(){

    if ( this->logged_status == false){
    this->send_to_server_message += this->start_delimiter + "3" + this->stop_delimiter;//+<option_chosen>
    }
    else{//the user is already logged in and he/she is an Administrator
        this->send_to_server_message += this->start_delimiter + "9" + this->stop_delimiter;
    }

    std::string station;
    printf(" Introduce the name of the STATION : ");
    fflush(stdout);
    station = this-> read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the name of the STATION :",
        "Sorry, the maximum length of a STATION is 30\n",
        30
    );

    this->send_to_server_message += this->text_between_delimiters(station);//+<_station>

}
void menu::trains_which_arrive_in_the_next_hour_option(){

    if ( this->logged_status == false){
    this->send_to_server_message += this->start_delimiter + "4" + this->stop_delimiter;//+<option_chosen>
    }
    else{//the user is already logged in and he/she is an Administrator
        this->send_to_server_message += this->start_delimiter + "10" + this->stop_delimiter;
    }

    std::string station;
    printf(" Introduce the name of the STATION : ");
    fflush(stdout);
    station = this-> read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the name of the STATION :",
        "Sorry, the maximum length of a STATION is 30\n",
        30
    );

    this->send_to_server_message += this->text_between_delimiters(station);//+<_station>

}

std::string menu::read_the_delay(){

    int nr_bytes_read;
    char delay[1024];
    
    nr_bytes_read = read (0,delay,sizeof(delay));

    while(nr_bytes_read < 2 || nr_bytes_read > 5){
        if(nr_bytes_read == -1){
            perror("Sorry, it seems like there was an error while reading the name of the STATION :");
        }
        else if( nr_bytes_read == 1){
            printf("Sorry, you didn t introduce any value\n");
            fflush(stdout);
        }
        else {
            printf("Sorry, the number of minutes which you inserted is too big (max nr of minutes = 9999)\n");
            fflush(stdout);
        }
        printf("Please try to introduce it again : ");
        fflush(stdout);
        nr_bytes_read = read (0,delay,sizeof(delay));
    }
    delay[nr_bytes_read-1] = '\0';

    return delay;
}

void menu::add_delay_option(){//this method can be calld only if the user is logged in
    this->send_to_server_message += this->start_delimiter + "2" + this->stop_delimiter;

    std::string id_train, station,date,final_station;

    printf(" Introduce the ID of the Train : ");
    fflush(stdout);
    id_train = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the ID of the TRAIN",
        "Sorry, the m length of a train's ID is 10\n",
        10
    );
    
    this->send_to_server_message += this->text_between_delimiters(id_train);//+<id_train>

    printf(" Introduce the name of the STATION : ");
    fflush(stdout);
    station = this-> read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the name of the STATION :",
        "Sorry, the maximum length of a STATION is 30\n",
        30
    );

    this->send_to_server_message += this->text_between_delimiters(station);//+<_station>

    printf(" Introduce the initial date for this train (without the delay) : ");
    fflush(stdout);
    date = this->read_date_from_the_user();

    this->send_to_server_message += this->text_between_delimiters(date);//+<date>

    printf(" Introduce the final station for this Train : ");
    fflush(stdout);
    final_station = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the name of the FINAL STATION :",
        "Sorry, the maximum length of a STATION is 30\n",
        30
    );

    this->send_to_server_message += this->text_between_delimiters(final_station);//+<final_station>

    printf("Does the train have a delay(1) or it arrives earlier(2) ?\nPlease introduce only the number between brackets\n");
    std::string option_delay = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading your option",
        "Sorry, the only numbers which you can choose are 1/2\n",
        2
    );

    while(option_delay !="1" && option_delay!="2"){
        printf("Please introduce a valid option(1/2)! : ");
        fflush(stdout);
        option_delay = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading your option",
        "Sorry, the only numbers which you can choose are 1/2\n",
        2
    );  
    }
    
    if(option_delay == "1"){
        printf("Insert (in minutes) what delay does the train have : ");
        fflush(stdout);
    }
    else{
        printf("Insert the number of minutes with wich the train arrives earlier: ");
        fflush(stdout);
    }
    std::string delay_string = this->read_the_delay();

    while ( delay_string.find_first_not_of("0123456789\n") != std::string::npos || (delay_string.substr(0,1) == "0" && delay_string.length()>1)){//the nr of minutes has to be only forrmen by nnumbers
        if(delay_string.find_first_not_of("0123456789\n") != std::string::npos){
            printf("The number of minutes cannot contain any other character except(0,1,2,3,4,5,6,7,8,9)\n");
            printf("Introduce the number of minutes again : ");
            fflush(stdout);
            delay_string = this->read_the_delay();
        }
        else{
            printf("You cannot introduce a number of multiple digits starting with 0 :) \n");
            printf("Introduce the number of minutes again : ");
            fflush(stdout);
            delay_string = this->read_the_delay();
        }
    }
    
    if(option_delay == "1"){//the train has a delay
        this->send_to_server_message += this->start_delimiter + delay_string + this->stop_delimiter;
    }
    else{//the train arrives earlier
        this->send_to_server_message += this->start_delimiter + "-" + delay_string + this->stop_delimiter;
    }//+<dely>

    //the message to the server will be sent as <status_logged><option><id_train><station_name><date><final_station><delay>


}
void menu::add_train_option(){
    this->send_to_server_message += this->start_delimiter + "3" + this->stop_delimiter;

    std::string id_train, station,date,final_station;

    printf(" Introduce the ID of the Train : ");
    fflush(stdout);
    id_train = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the ID of the TRAIN",
        "Sorry, the maximum length of a train's ID is 10\n",
        10
    );
    
    this->send_to_server_message += this->text_between_delimiters(id_train);//+<id_station>

    printf(" Introduce the name of the STATION : ");
    fflush(stdout);
    station = this-> read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the name of the STATION :",
        "Sorry, the maximum length of a STATION is 30\n",
        30
    );

    this->send_to_server_message += this->text_between_delimiters(station);//+<_station>

    printf(" Introduce the date and hour (YYYY-MM-DD HH:MM) at arrival in the mentioned station: ");
    fflush(stdout);
    date = this->read_date_and_time_from_the_user();
    date += ":00";

    this->send_to_server_message += this->text_between_delimiters(date);//+<arrival_time>

    printf(" Introduce the date and hour (YYYY-MM-DD HH:MM) at departure from the mentioned station: ");
    fflush(stdout);
    date = this->read_date_and_time_from_the_user();
    date += ":00";

    this->send_to_server_message += this->text_between_delimiters(date);//+<departure_time>

    printf(" Introduce the final station for this Train : ");
    fflush(stdout);
    final_station = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the name of the FINAL STATION :",
        "Sorry, the maximum length of a STATION is 30\n",
        30
    );

    this->send_to_server_message += this->text_between_delimiters(final_station);//+<final_station>
//the message which will be send to the server will be : <status_logged><option_menu><id_train><station><arrival_time><departure_time><final_station>

}

void menu::delete_train_option(){
    this->send_to_server_message += this->start_delimiter + "4" + this->stop_delimiter;

    std::string id_train;
    printf(" Introduce the ID of the Train : ");
    fflush(stdout);
    id_train = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the ID of the TRAIN",
        "Sorry, the maximum length of a train's ID is 10\n",
        10
    );
    
    this->send_to_server_message += this->text_between_delimiters(id_train);//+<id_station>

}

void menu::add_new_account_option(){
    this->send_to_server_message += this->start_delimiter + "5" + this->stop_delimiter;

    std::string username,password;

    printf(" Introduce the username: ");
    fflush(stdout);
    username = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the username :(",
        "Sorry, the maximum length of a username is 20\n",
        20);

    this->send_to_server_message += this->text_between_delimiters(username);//+<username>

    printf(" Introduce the password: ");
    fflush(stdout);
    password = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the password :( ",
        "Sorry, the maximum length of a password is 30\n",
        30
    );

    this->send_to_server_message += this->text_between_delimiters(password);//+<psswd> 
}
void menu::change_username_option(){
    this->send_to_server_message += this->start_delimiter + "6" + this->stop_delimiter;

    std::string username,password;

    printf(" Introduce your current username: ");
    fflush(stdout);
    username = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the username :(",
        "Sorry, the maximum length of a username is 20\n",
        20);

    this->send_to_server_message += this->text_between_delimiters(username);//+<username>

    printf(" Introduce the password: ");
    fflush(stdout);
    password = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the password :( ",
        "Sorry, the maximum length of a password is 30\n",
        30
    );

    this->send_to_server_message += this->text_between_delimiters(password);

    printf(" Introduce the new username: ");
    fflush(stdout);
    username = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the username :(",
        "Sorry, the maximum length of a username is 20\n",
        20);

    this->send_to_server_message += this->text_between_delimiters(username);//+<new_username>

}
void menu::change_password_option(){
    this->send_to_server_message += this->start_delimiter + "7" + this->stop_delimiter;

    std::string username,password;

    printf(" Introduce the username: ");
    fflush(stdout);
    username = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the username :(",
        "Sorry, the maximum length of a username is 20\n",
        20);

    this->send_to_server_message += this->text_between_delimiters(username);//+<username>

    printf(" Introduce your current password: ");
    fflush(stdout);
    password = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the password :( ",
        "Sorry, the maximum length of a password is 30\n",
        30
    );

    this->send_to_server_message += this->text_between_delimiters(password);

    printf(" Introduce the new password: ");
    fflush(stdout);
    password = this->read_the_extra_info_from_the_user(
        "Sorry, it seems like there was an error while reading the password :( ",
        "Sorry, the maximum length of a password is 30\n",
        30
    );

    this->send_to_server_message += this->text_between_delimiters(password);//+<new_passowrd>

}

void menu::show_all_trains_option(){
    this->send_to_server_message += this->start_delimiter + "11" + this->stop_delimiter;
}

void menu::logout(){
    this->send_to_server_message += this->start_delimiter + "1" + this->stop_delimiter;
}

void menu::set_received_message(std::string rcv_msg){
    this->received_message = rcv_msg ; 
}
void menu::set_logged_status(){
    if(this->received_message.substr(0,1 + this->start_delimiter.length()+this->stop_delimiter.length()) ==
    this->start_delimiter + "0" + this->stop_delimiter){
        this->logged_status = false;
    }
    else{
        this->logged_status = true;
    }
}
void menu::print_received_message(){
    //before we print the message, we delete the <logged_status> part
    this->received_message.erase(0,1 + this->start_delimiter.length()+this->stop_delimiter.length());

    printf("\n\n%s\n\n", (this->received_message).c_str());
}