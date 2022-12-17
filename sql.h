#include<string>
#include<mysql/mysql.h>
class SQL {
  private :

  MYSQL *connection;
  std::string const server_IP, username, password, database_name;

  std::string getServer_IP();
  std::string getUsername();
  std::string getDatabaseName();
  std::string getPassword();
    
  public :

  SQL(std::string IP, std::string usrname, std::string pswd, std::string db_name );//the constructor

  bool connect_to_databse();// makes the connection with the database
  MYSQL * getConnection();//returns the connection with the database

};
