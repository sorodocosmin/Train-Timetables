#include "menu.h"

menu::menu(){
    this->welcome_message = "\n---------------------------------------\n| Hello ! Welcome to the TRAINS APP ! |\n---------------------------------------\n\n";
    this->regular_menu_message = " Please introduce only the number coresponding with the command that you want to execute : \n- 0. Exit \n- 1. Login \n- 2. Show trains from stataion \"X\" to station \"Y\" in a date \"D\" \n- 3. Show the trains which will leave from station \"X\" in the next hour\n- 4. Show the trains wich will arrive to station \"X\" in the next hour\n";
    this->administrator_menu_message = "Please introduce only the number coresponding with the command that you want to execute : \n- 0. Exit \n- 1. Logout \n- 2. Add train \n- 3. Delete train \n- 4. Create another Administrator account \n- 5. Modify username \n- 6. Modify password\n- 7. Show trains from stataion \"X\" to station \"Y\" in a date \"D\" \n- 8. Show the trains which will leave from station \"X\" in the next hour\n- 9. Show the trains wich will arrive to station \"X\" in the next hour";
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
        else if(option == "3\n"){//show the trains which arrive in the next hour in a station X
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
        else
            this->send_to_server_message += this->start_delimiter + "Unknown command" + this->stop_delimiter;
    }
    

}

void menu::clear_message_send(){

    this->send_to_server_message = "";
}
void menu::quit_option(){

    this->send_to_server_message += this->start_delimiter + "0" + this->stop_delimiter;//option 0
    this->quit_status = true;

}

void menu::login_option(){

    this->send_to_server_message += this->start_delimiter + "1" + this->stop_delimiter;//option1
}

void menu::trains_from_station_X_to_station_Y_option(){
    
    this->send_to_server_message += this->start_delimiter + "2" + this->stop_delimiter;//+<option_chosen>

    char departure_station[1024],arrival_station[1024], date[1024];
    int nr_bytes_read;

    printf(" Introduce the name of the DEPARTURE STATION : ");
    fflush(stdout);
    nr_bytes_read = read (0,departure_station,sizeof(departure_station));
    while(nr_bytes_read == -1 || nr_bytes_read > 30){
        perror("Sorry, it seems like there was an error while reading the name of the DEPARTURE STATION :");
        printf("Please try to introduce it again : ");
        fflush(stdout);
        nr_bytes_read = read (0,departure_station,sizeof(departure_station));
    }
    departure_station[nr_bytes_read-1] = '\0';
    this->send_to_server_message += this->start_delimiter;
    this->send_to_server_message += departure_station;
    this->send_to_server_message += this->stop_delimiter;//+<depart_staion>

    printf(" Introduce the name of the ARRIVAL STATION : ");
    fflush(stdout);
    nr_bytes_read = read (0,arrival_station,sizeof(arrival_station));
    while(nr_bytes_read == -1 || nr_bytes_read > 30){
        perror("Sorry, it seems like there was an error while reading the name of the ARRIVAL STATION :");
        printf("Please try to introduce it again : ");
        fflush(stdout);
        nr_bytes_read = read (0,arrival_station,sizeof(arrival_station));
    }
    arrival_station[nr_bytes_read-1] = '\0';
    this->send_to_server_message += this->start_delimiter;
    this->send_to_server_message += arrival_station;
    this->send_to_server_message += this->stop_delimiter;//+<arrival_station>

    printf(" Introduce the DATE (YYYY-MM-DD) : ");
    fflush(stdout);
    nr_bytes_read = read (0,date,sizeof(date));
    while(nr_bytes_read == -1 || nr_bytes_read > 11){
        perror("Sorry, it seems like there was an error while reading the DATE :");
        printf("Please try to introduce it again (YYYY-MM-DD) : ");
        fflush(stdout);
        nr_bytes_read = read (0,date,sizeof(date));
    }
    date[nr_bytes_read-1] = '\0';
    this->send_to_server_message += this->start_delimiter;
    this->send_to_server_message += date;
    this->send_to_server_message += this->stop_delimiter;//+<date>

}

void menu::trains_which_leave_in_the_next_hour_option(){

    this->send_to_server_message += this->start_delimiter + "3" + this->stop_delimiter;//+<option_chosen>

    char station[1024];
    int nr_bytes_read;
    printf(" Introduce the name of the STATION : ");
    fflush(stdout);
    nr_bytes_read = read (0,station,sizeof(station));
    while(nr_bytes_read == -1 || nr_bytes_read > 30){
        perror("Sorry, it seems like there was an error while reading the name of the STATION :");
        printf("Please try to introduce it again : ");
        fflush(stdout);
        nr_bytes_read = read (0,station,sizeof(station));
    }
    station[nr_bytes_read-1] = '\0';
    this->send_to_server_message += this->start_delimiter;
    this->send_to_server_message += station;
    this->send_to_server_message += this->stop_delimiter;//+<_station>

}
void menu::trains_which_arrive_in_the_next_hour_option(){

    this->send_to_server_message += this->start_delimiter + "4" + this->stop_delimiter;//+<option_chosen>

    char station[1024];
    int nr_bytes_read;
    printf(" Introduce the name of the STATION : ");
    fflush(stdout);
    nr_bytes_read = read (0,station,sizeof(station));
    while(nr_bytes_read == -1 || nr_bytes_read > 30){
        perror("Sorry, it seems like there was an error while reading the name of the STATION :");
        printf("Please try to introduce it again : ");
        fflush(stdout);
        nr_bytes_read = read (0,station,sizeof(station));
    }
    station[nr_bytes_read-1] = '\0';
    this->send_to_server_message += this->start_delimiter;
    this->send_to_server_message += station;
    this->send_to_server_message += this->stop_delimiter;//+<_station>
}

void menu::logout(){
    this->send_to_server_message = this->start_delimiter + "1" + this->stop_delimiter;
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
    //before we print the message, we delete the <logged_status><_message>
    this->received_message.erase(0,1 + this->start_delimiter.length()+this->stop_delimiter.length());

    printf("the received message is : \n%s\n", (this->received_message).c_str());
}