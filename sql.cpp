#include"sql.h"
#include<cstring>


SQL::SQL(std::string IP, std::string usrname, std::string pswd, std::string db_name ):
server_IP(IP), username (usrname), password(pswd), database_name(db_name){//the constructor

}

std::string SQL::getServer_IP(){
  return this->server_IP;
}

std::string SQL::getUsername(){
  return this->username;
}

std::string SQL::getDatabaseName(){
  return this->database_name;
}

std::string SQL::getPassword(){
  return this->password;
}

MYSQL * SQL::getConnection(){
  return this->connection;
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
