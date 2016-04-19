#include<database.h>
void mysql_connect(MYSQL* connection_ptr,char* host,char* username,char* passwd,char* database)
{
	
	mysql_init(connection_ptr);
	if(mysql_real_connect(connection_ptr,host,username,passwd,database,0,NULL,CLIENT_FOUND_ROWS)==NULL)
	{
		printf("connect error\n");
		exit(0);
	}
}
void execute_nonquery(MYSQL* connection_ptr,char* sql)
{
	if(mysql_query(connection_ptr,sql)!=0)
	{
		printf("query error\n");
		exit(0);
	}
}
MYSQL_RES* execute_query(MYSQL* connection_ptr,char* sql)
{
	MYSQL_RES* res_ptr;
	if(mysql_query(connection_ptr,sql)!=0)
	{
		printf("query error\n");
		exit(0);
	}
	if((res_ptr=mysql_store_result(connection_ptr))==0)
	{
		printf("store error\n");
		exit(0);
	}
	return res_ptr;
}
int data_row(MYSQL* connection_ptr,char* sql)
{
	int row;
	MYSQL_RES* res_ptr;
	res_ptr=execute_query(connection_Ptr,sql);
	row=mysql_num_rows(res_ptr);
	return row;
}	

