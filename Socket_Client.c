#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
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
	
	
	
	
	
