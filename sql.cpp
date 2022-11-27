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
          fprintf(stderr, "%s\n", mysql_error(connection));//why the connection couldn't have been made
          mysql_close(connection);//we close the connection
          return false;
        }
    return true;
}
