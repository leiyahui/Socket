
#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
void log_in(int socket_fd)
{
	char name[20];
	char passwd[20];
	int recv_len;
	char recv_line[4096];
	while(1)
	{
		printf("please input user name:");
		while(fgets(name,20,stdin)==NULL)
		{
			printf("please input user name:");
		}
		printf("please input password:");
		fgets(passwd,20,stdin);	
		if(send(socket_fd,name,strlen(name),0)==-1)		//send user name
		{
			printf("send error:%s(errno:%d)\n",strerror(errno),errno);
		}
		if(send(socket_fd,passwd,strlen(passwd),0)==-1)	//send password
		{
			printf("send error:%s(errno:%d)\n",strerror(errno),errno);
		}
		if((recv_len=recv(socket_fd,recv_line,4096,0))!=-1)
		{
			recv_line[recv_len]='\0';
			if(strcmp(recv_line,"log success!")==0)
			{
				printf("%s\n",recv_line);		//log success
				break;
			}
			else
			{
				printf("log fail\n");
			}
		}
	}
}
	
void main()
{
	int socket_fd;
	struct sockaddr_in servaddr;
	char send_line[4096],recv_line[4096];
	int recv_len;
	int pid,ppid;
	if((socket_fd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("socket fail:%s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(8000);
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	if(connect(socket_fd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1)
	{
		printf("connect error:%s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	log_in(socket_fd);
	printf("who you want to communicate\n");
	fgets(send_line,4096,stdin);
	send(socket_fd,send_line,strlen(send_line),0);
	pid=fork();
	if(pid==-1)
	{
		printf("create thread 1 failed\n");
	}
	else if(pid==0)		//receive message 
	{
			while((recv_len=recv(socket_fd,recv_line,4096,0))!=-1)
			{
				//printf("revcive error:%s(errno:%d)\n",strerror(errno),errno);
				//exit(0);

				recv_line[recv_len]='\0';
				printf("received is:%s",recv_line);
			}
	}
	else		//send message
	{
		while(fgets(send_line,4096,stdin)!=NULL)
		{
			if(send(socket_fd,send_line,strlen(send_line),0)==-1)
			{
				printf("send error:%s(errno:%d)\n",strerror(errno),errno);
				exit(0);
			}
			//	if((recv_len=recv(socket_fd,recv_line,4096,0))==-1)
			//	{
			//		printf("revcive error:%s(errno:%d)\n",strerror(errno),errno);
			//		exit(0);
			//	}	
		}
	}
	close(socket_fd);
}
	
	
	
