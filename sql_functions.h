#include<mysql/mysql.h>
#include<string>
#include<string.h>
class sql_functions{//based of a connection with a database, this class allows you to execute queries on that DB
    private : 
    MYSQL *connection;
   // static bool stop_light ; // it will receive true when the sgbd executes a query for a client


    MYSQL_RES* execute_query ();//returns the result of the query as MYSQL_RES*
    bool query_error(std::string cmd_query);
    std::string format_response(std::string response,int nr_col);
    public : 

    sql_functions(MYSQL * conn);
    
    std::string get_result_of_the_query(std::string query);//this functions will be called after the execute_query() one; it returns the result of the query as a string
/*
nume_col1|nume_col2|nume_col3|
abcdsdf|fdsf|a64|
bunaziuaa|numelemeu este|cossssmin

-->

+----------+---------------+-----------+
|nume_col1 |nume_col2      |nume_col3S |
+----------+---------------+-----------+
|abcdsdf   |fdsf           |a64        |
|bunaziuaa |numelemeu este |cossssmin  |
*/
};
