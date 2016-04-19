#include<stdio.h>
#include<stdlib.h>
#include<stdlib>
void mysql_connect(MYSQL& connection_ptr);		//connect mysql connection
void execute_nonquery(MYSQL& connection_ptr,char* sql);	//execute sql and no return
MYSQL_RES* execute_query(MYSQLl& connection_ptr,char* sql);		//execute sql and return result
int data_row(MYSQL* connection_ptr,char* sql);		//return row of data that sql query
