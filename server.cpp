#include<stdio.h>
#include "sql.h"
int main ()
{
  SQL BD;
  if(BD.connect_to_databse() == false){//connect the server to the databse

      printf("The server couldn't connect to the databse!\n ! Try to restart the server ! \n");
      return -1;
    }
  else{
      printf("[+] The server connected to the databse successfully \nn");
    }
  return 0;
}
