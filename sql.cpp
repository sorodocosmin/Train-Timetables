#include"sql.h"
#include<cstring>


SQL::SQL(): server_IP("127.0.0.1"), username ("root"), password("password"), database_name("trains_database"){//the constructor
}


const std::string SQL::getServer_IP(){
  return this->server_IP;
}

const std::string SQL::getUsername(){
  return this->username;
}

const std::string SQL::getDatabaseName(){
  return this->database_name;
}

const std::string SQL::getPassword(){
  return this->password;
}



bool SQL::connect_to_databse(){
    this->connection = mysql_init(NULL);//create a new MYSQL connection
    if (mysql_real_connect(connection,
        this->getServer_IP().c_str(),
        this->getUsername().c_str(),
        this->getPassword().c_str(),
        this->getDatabaseName().c_str(),
        0, NULL, 0) == NULL) //if the connection couldn't have been made
        {
          printf("%s\n", mysql_error(connection));//why the connection couldn't have been made
          mysql_close(connection);//we close the connection
          return false;
        }
    return true;
}


MYSQL_RES* SQL::execute_query (std::string cmd_query){
  //send the querry to the database
  int error_code = mysql_query(this->connection,cmd_query.c_str());//this query will return a nonZero value in case of error
  if (error_code!=0){
    if(mysql_errno(this->connection)!=2014){ //if the error it's not produced by the fact that 2 clients try to execute a query at the same time
      printf("the error code is  : %d \n", mysql_errno(this->connection));
      printf("MySQL query error : %s\n",mysql_error(connection) );
      exit(-1);
    }
  }
  /*  Error number: 2014; Symbol: CR_COMMANDS_OUT_OF_SYNC;
Message: Commands out of sync; you can't run this command now
This can happen, for example, if you are using mysql_use_result() and try to execute a new query before you have called mysql_free_result() .
*/

  while (mysql_errno(this->connection)==2014)//we wait, until it's made mysql_free() of the query of the other
  //Very unlikely to enter in this while
  //
    mysql_query(this->connection,cmd_query.c_str());
  printf("1 FREE");
  return (mysql_use_result(this->connection));
}


std::string SQL::get_result_of_the_executed_query(){
  MYSQL_RES *result;	// the results
  MYSQL_ROW row;	// the results row (line by line)
  MYSQL_FIELD *field; //contains information about the table's header
  int number_fields; //numbr of columns which are 'printed' by the query
  std::string res="";
  result = this->execute_query("SELECT * FROM arrivals_departures;");
  number_fields = mysql_num_fields(result);
  while ((row=mysql_fetch_row(result)) != NULL){ //row gets one by one line from the result;
      for( int i=0 ; i<number_fields; ++i ) {//
          if(i==0){
            while((field = mysql_fetch_field(result))!= NULL){//this resets once a query is again executed
                res= res + (field->name) + "\t";
             }

             res = res + "\n";
          }
        res = res + (row[i]? row[i] : "NULL" ) + "\t";
      }//for
  }//while

  mysql_free_result(result);//clean up the result of the query

  return res;
}

std::string SQL::get_result_of_the_executed_query_2(){
  MYSQL_RES *result;	// the results
  MYSQL_ROW row;	// the results row (line by line)
  MYSQL_FIELD *field; //contains information about the table's header
  int number_fields; //numbr of columns which are 'printed' by the query
  std::string res="";
  result = this->execute_query("SELECT * FROM arrivals_departures;");
  number_fields = mysql_num_fields(result);
  while ((row=mysql_fetch_row(result)) != NULL){ //row gets one by one line from the result;
      for( int i=0 ; i<number_fields; ++i ) {//
          if(i==0){
            while((field = mysql_fetch_field(result))!= NULL){//this resets once a query is again executed
                res= res + (field->name) + "\t";
             }

             res = res + "\n";
          }
        res = res + (row[i]? row[i] : "NULL" ) + "\t";
      }//for
  }//while

  mysql_free_result(result);//clean up the result of the query

  return res;
}
