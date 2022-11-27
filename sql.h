#include<string>
#include<mysql/mysql.h>
class SQL {
  private :
  MYSQL *connection;
  const std::string server_IP, username, password, database_name;
  public :
  SQL();
  const std::string getServer_IP();
  const std::string getUsername();
  const std::string getDatabaseName();
  const std::string getPassword();

  bool connect_to_databse();
  void add_a_train();

};