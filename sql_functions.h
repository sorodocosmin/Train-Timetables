#include<mysql/mysql.h>
#include<string>
#include<string.h>
class sql_functions{//based of a connection with a database, this class allows you to execute queries on that DB
    private : 
    MYSQL *connection;

    MYSQL_RES* execute_query ();//returns the result of the query as MYSQL_RES*
    bool query_error(std::string cmd_query);
    std::string format_response(std::string response,int nr_col);//formats the response as a table

    public : 

    sql_functions(MYSQL * conn);
    
    std::string get_result_of_the_query(std::string query);
    //^-returns the result of the query as a string( inside this method, the private method execute_query is called)
};
