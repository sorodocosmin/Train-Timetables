#include<string>
#include<mysql/mysql.h>
class SQL {
  private :

  MYSQL *connection;
  std::string server_IP, username, password, database_name;
    
  public :

  SQL(std::string IP, std::string usrname, std::string pswd, std::string db_name );//the constructor
  const std::string getServer_IP();
  const std::string getUsername();
  const std::string getDatabaseName();
  const std::string getPassword();

  bool connect_to_databse();

  MYSQL * getConnection();//returns the connection with the database

};
