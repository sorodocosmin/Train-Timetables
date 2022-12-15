#include "sql_functions.h"

//bool sql_functions::stop_light = false;

sql_functions::sql_functions(MYSQL * conn){
    this->connection = conn;
}

bool sql_functions::query_error(std::string cmd_query){
    int err = mysql_query(this->connection,cmd_query.c_str());//this query will return a nonZero value in case of error
  if (err!=0){
    if(mysql_errno(this->connection)!=2014){ //if the error it's not produced by the fact that 2 clients try to execute a query at the same time
      return false;
    }
  }
  /*  Error number: 2014; Symbol: CR_COMMANDS_OUT_OF_SYNC;
Message: Commands out of sync; you can't run this command now
This can happen, for example, if you are using mysql_use_result() and try to execute a new query before you have called mysql_free_result() .
*/

  while (mysql_errno(this->connection)==2014){//we wait, until it's made mysql_free() of the query of the other
    //Very unlikely to enter in this while
    mysql_query(this->connection,cmd_query.c_str());
  }

  return true;
}

MYSQL_RES* sql_functions::execute_query (){
  //send the querry to the database

  return (mysql_use_result(this->connection));
}


std::string sql_functions::get_result_of_the_query(std::string query){
    MYSQL_RES *result;	// the results
    MYSQL_ROW row;	// the results row (line by line)
    MYSQL_FIELD *field; //contains information about the table's header
    int number_fields; //numbr of columns which are 'printed' by the query
    std::string res="";
   // while(sql_functions::stop_light == true )
   // printf("\n\n\n\nI WAITED HEREEE \n\n\n\n\n\n\n");
    //this->stop_light = true ;
    if( this->query_error(query.c_str()) == false ){//if is was some error at the execution of the query
        printf("the error code is  : %d \n", mysql_errno(this->connection));
        char error[1024];
        sprintf(error,"ERROR :%s\n",mysql_error(this->connection) );
        return error;
    }
    //if the query executed just fine;
    std::string first_word_from_quey = query.substr(0,6);
    if(first_word_from_quey!="UPDATE" && first_word_from_quey!="INSERT" && first_word_from_quey!="DELETE"){
      result = this->execute_query();
      number_fields = mysql_num_fields(result);
      while ((row=mysql_fetch_row(result)) != NULL){ //row gets one by one line from the result;
        for( int i=0 ; i<number_fields; ++i ) {//
            if(i==0){
                while((field = mysql_fetch_field(result))!= NULL){//this resets once a query is again executed
                    res=res + (field->name) + "|";
                }

                res = res + "\n";
            }
            res = res + (row[i]? row[i] : "NULL" ) + "|";
        }//for
      }//while

      mysql_free_result(result);//clean up the result of the query

      //this->stop_light = false;
      if(res == ""){
        return res;
      }
      else if (res.at(0) == 'c'){
        return res;
      }
      else{
        return format_response(res,number_fields);
      }
    }
    else if(first_word_from_quey=="UPDATE"){
      return "The update of the delay was successfully made!";
    }
    else if (first_word_from_quey=="INSERT"){
      return "The insert of the train was successfully made!";
    }
    else{
      return "The train deleted successfully!";
    }
   
}

 std::string sql_functions::format_response(std::string response, int nr_col){
  std::string response_formatat = "";

  std::string aux_res = response ;
  int max_chars_in_each_coloumn[]={1,1,1,1,1,1,1,1,1,1};
  int k = 0 ;
  while(aux_res.find("|")!= std::string::npos){
    int poz = aux_res.find("|");
    int nr_chr = (aux_res.substr(0,poz)).length();
    if(aux_res.substr(0,1) != "\n")
        nr_chr++;
    if ( max_chars_in_each_coloumn[k] < nr_chr ){
      max_chars_in_each_coloumn[k] = nr_chr;
    }
    k++;
    k = k % nr_col;
    aux_res.erase(0,poz+1);
  }
  //first line of ---
  response_formatat = "+";
  for( int i = 0 ; i< nr_col ; ++i){
    std::string s(max_chars_in_each_coloumn[i],'-');
    response_formatat += s + "+";
  }
  //line of columns
  response_formatat += "\n|";
  for( int i = 0 ; i< nr_col ; ++i){
    int poz = response.find("|");
    std::string word = response.substr(0,poz); 
    std::string s(max_chars_in_each_coloumn[i] - word.length(),' ');
    response_formatat += word + s + "|";
    response.erase(0,poz+1);
  }
  response_formatat += "\n+";


    //second line of ----
  for( int i = 0 ; i< nr_col ; ++i){
    std::string s(max_chars_in_each_coloumn[i],'-');
    response_formatat += s + "+";
  }

  response_formatat += "\n|";
  k=0;
  // we insert each word  + the space
  while(response.find("|")!= std::string::npos){
    if (response.at(0)=='\n')
        response.erase(0,1);

    int poz = response.find("|");
    std::string word =response.substr(0,poz); 
    int nr_chr = word.length();
    std::string space(max_chars_in_each_coloumn[k]-nr_chr,' ');
    k++;
    response_formatat += word + space + "|";
    response.erase(0,poz+1);

    if( k == nr_col && response.find("|")!= std::string::npos)
        response_formatat += "\n|";
    k = k % nr_col;
  }
//last line of ---
  response_formatat += "\n+";
  for( int i = 0 ; i< nr_col ; ++i){
    std::string s(max_chars_in_each_coloumn[i],'-');
    response_formatat += s + "+";
  }

  return response_formatat;

}