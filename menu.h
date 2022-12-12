#include<string>

#include <unistd.h>
class menu {
    private : 
        std::string welcome_message, regular_menu_message, administrator_menu_message, bye_message;
        std::string send_to_server_message;
        std::string received_message;

        std::string const start_delimiter="<", stop_delimiter=">";
        //when sending the message to the server, the logged_status, option,...etc will be put between these delimiters

        bool logged_status;//this value will change to true if the users will login
        bool quit_status;//it will change to true, if the users chooses the Exit option

        std::string read_the_extra_info_from_the_user(std::string error_message,std::string err_message_nr_of_chars_is_bigger, int max_nr_of_chars_which_can_be_read);
        std::string read_the_delay();
        void login_option();
        void quit_option();
        void trains_from_station_X_to_station_Y_option();
        void trains_which_leave_in_the_next_hour_option();
        void trains_which_arrive_in_the_next_hour_option();

        //the below methods can't be called if the user isn't an Administrator
        //void add_train_option();
        void add_delay_option();
        //void add_new_account();
        //void change_username();
        //void change_password();
        void logout();
    public :

    menu();//it initializes the messages

    std::string get_welcome_message();
    std::string get_regular_menu_message();
    std::string get_administrator_menu_message();
    std::string get_bye_message();
    std::string get_the_message_which_will_be_send_to_the_server();

    bool get_quit_status();
    bool get_logged_status();

    void set_received_message(std::string rcv_msg);
    void set_logged_status();
    void print_received_message();
    
    void clear_message_send();
    void handle_client_option(std::string option);
    //^- depending on which option the client has chosen, it call some private methods
};