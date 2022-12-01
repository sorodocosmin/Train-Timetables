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
  MYSQL * getConnection();

  bool connect_to_databse();
  MYSQL_RES* execute_query (std::string cmd_query);
  std::string get_result_of_the_executed_query();
  std::string get_result_of_the_executed_query_2();
};
