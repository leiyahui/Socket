#include<chain.h>
void conn_insert(conn_list chain_list,int connect_fd,char* name)
{
	conn_list temp_list;
	temp_list=chain_list;
	while(temp_list!=NULL)
	{
		temp_list=temp_list->next;
	}
	if((temp_list=(connection_list)malloc(sizeof(conn)))==NULL)
	{
		printf("malloc error\n");
		exit(0);
	}
	temp_list->connect_fd=connect_fd;
	strcpy(temp_list->name,name);
	temp_list->next=NULL;
}
int conn_delete(conn_list* chain_list,char* name)		//success return 1,false return 0
{
	conn_list front_node,rear_node;
	rear_node=*chain_list;
	if(strcmp((*chain_list)->name,name)
	{
		*chain_list=(*chain_list)->next;
		free(rear_node);
		return 1;
	}
	while(rear_node!=NULL)
	{
		if(strcmp(rear_node->name,name)==0)
		{
			front_node->next=rear_node->next;
			free(rear_node);
			return 1;		//delete success
		}
		front_node=rear_node;
		rear_node=rear_node->next;
	}
	return 0;		//can't find the name
}
int conn_search(conn_list chain_list,char* name)		//if find return connect_fd,else return 0
{
	conn_list temp_list;
	temp_list=chain_list;
	while(temp_list!=NULL)
	{
		if(strcmp(temp_list->name,name)==0)
		{
			return temp_list->conect_fd;
		}
		temp_list=temp_list->next;
	}
	return 0;
}
		
