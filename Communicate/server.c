
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<errno.h>
#include<string.h>
#include<database.h>
typedef char 	bool;
#define true	1
#define false	0
#define HOST		"localhost"
#define USERNAME	"root"
#define PASSWORD	"lyh12063259"
#define DATABASE	"communicate"
void log_in(int connect_fd,MYSQL* connection,conn_list chain_list)
{
	char sql[100];
	char name[20];
	char passwd[20];
	int n;
	while(1)
	{
		n=recv(connect_fd,name,0);
		name[n-1]='\0';			//rm '\n'
		n=recv(connect_fd,passwd,20,0);			//get password
		passwd[n-1]='\0';		//rm '\n'
		strcpy(sql,"select count(*) from t_user where name ='");		//create sql
		strcat(sql,name);
		strcat(sql,"' and passwd = '");
		strcat(sql,passwd);
		strcat(sql,"'");
		if(data_row(connection,sql))	//log success
		{
			printf("validation pass\n");
			if(send(connect_fd,"log success!",strlen("log success!"),0)==-1)
			{
				printf("send error:%s(errno:%d)\n",strerror(errno),errno);
				close(connect_fd);
				break;
			}
			strcpy(sql,"update t_user set islog=1 where name = '");		//update the database status
			strcat(sql,name);
			strcat(sql,"'");
			execute_nonquery(connection,sql);
			conn_insert(chain_list,connect_fd,name);			//insert the name and connect_fd to chain
			break;		//quit this loop
		}
		else
		{
			printf("validataion fail\n");	
			if(send(connect_fd,"log fail",strlen("log fail")+1,0)==-1)
			{
				printf("send error:%s(errno:%d)\n",strerror(errno),errno);
				close(connect_fd);
				break;
			}
		}
	}
}
	
void main()
{
	int socket_fd,connect_fd,opp_connect_fd;
	struct sockaddr_in seraddr;
	socklen_t client_len;
	struct sockaddr_in clientaddr;
	char send_buffer[4096],recv_buffer[4096];
	int n;			//length of receive number
	char net_str[INET_ADDRSTRLEN];
	int pid,ppid;
	char sql[100];
	MYSQL connection;
	MYSQL_ROW row;
	conn_list chain_list;		//the chain_list contain the name and connect_fd
	if((socket_fd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("create socket error:%s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}
	memset(&seraddr,0,sizeof(seraddr));
	seraddr.sin_family=AF_INET;
	seraddr.sin_addr.s_addr=htonl(INADDR_ANY);		//get ip automaticly
	seraddr.sin_port=htons(8000);
	if(bind(socket_fd,(struct sockaddr*)&seraddr,sizeof(seraddr))==-1)
	{
		printf("bind socket error: %s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	if(listen(socket_fd,10)==-1)
	{
		printf("listen error: %s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	mysql_connect(&connection);		//connect mysql
	printf("Wating for Client connecting\n");
	while(1)
	{
		client_len=sizeof(clientaddr);
		if((connect_fd=accept(socket_fd,(struct sockaddr*)&clientaddr,&client_len))==-1)
		{
			printf("connect error:%s(errno:%d)\n",strerror(errno),errno);
		}
		printf("%s::%d is connected\n",inet_ntop(AF_INET,&clientaddr.sin_addr,net_str,sizeof(net_str)),ntohs(clientaddr.sin_port));
		ppid=fork();
		if(ppid==-1)
		{
			printf("create thread failed\n");
		}
		else if(ppid==0)			//get and send message thread
		{
			pid=fork();
			if(pid==-1)
			{
				printf("create thread failed\n");
			}
			else if(pid==0)			//receive message
			{
				log_in(connect_fd,&connection);
				n=recv(connect_fd,recv_buffer,4096,0);
				recv_buffer[n-1]='\0';
				strcpy(name,rev_buffer);
				
				if(opp_connect_fd=conn_serach(chain_list,name)!=0)		//get the connect_fd who you want to connunicate
				{
					while(n=recv(connect_fd,recv_buffer,4096,0))
					{
						recv_buffer[n]='\0';
						printf("%s",recv_buffer);
						send(opp_connect_fd,recv_buffer,strlen(recv_buffer),0);		//send to the one you want to communicate	
					}
				}
				else
				{
					send(connect_fd,"it isn't on line",strlen("it isn't on line"),0);
				}	
			}
			else				//send message
			{
				while(fgets(send_buffer,4096,stdin)!=NULL)
				{
					if(send(connect_fd,send_buffer,strlen(send_buffer),0)==-1)
					{
						printf("send error:%s(errno:%d)\n",strerror(errno),errno);
						close(connect_fd);
					}
				}
			}
		}
		else		// main thread close connect_fd
		{
			
			close(connect_fd);
		}
			
	}
	close(socket_fd);
}

