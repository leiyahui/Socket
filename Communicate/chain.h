#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct _connection
{
	int connect_fd,
	char* name,
	_connect* next;
}conn,*connection_list;
void conn_insert(conn_list chain_list,int connect_fd,char* name);		//insert
int conn_delete(conn_list* chain_list,char* name);		//success return 1,false return 0
int conn_search(conn_list chain_list,char* name);		///if find the name retrun connect_fd,else return 0;
