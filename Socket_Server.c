#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<errno.h>
void print_ip(unsigned char* start,int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		if(i!=len-1)
		printf("%u.",start[i]);
	}
}
void main()
{
	int socket_fd,connect_fd;
	struct sockaddr_in seraddr;
	socklen_t client_len;
	struct sockaddr_in clientaddr;
	char send_buffer[4096],recv_buffer[4096];
	int n;			//length of receive number
	char net_str[INET_ADDRSTRLEN];
	int pid,ppid;
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
	}
	if(listen(socket_fd,10)==-1)
	{
		printf("listen error: %s(errno:%d)\n",strerror(errno),errno);
	}
	printf("Wating for Client connecting\n");
	while(1)
	{
		client_len=sizeof(clientaddr);
		if((connect_fd=accept(socket_fd,(struct sockaddr*)&clientaddr,&client_len))==-1)
		{
			printf("connect error:%s(errno:%d)\n",strerror(errno),errno);
		}
		printf("%s::%d is connected\n",inet_ntop(AF_INET,&clientaddr.sin_addr,net_str,sizeof(net_str)),ntohs(clientaddr.sin_port));
	//	while(1)
	//	{
	//		printf("client ip address is:%s\nclient port is:%d\n",inet_ntop(AF_INET,&clientaddr.sin_addr,net_str,sizeof(net_str)),ntohs(clientaddr.sin_port));
	//		n=recv(connect_fd,buffer,4096,0);
	//		if(send(connect_fd,"get your message\n",strlen("get your message\n")+1,0)==-1)
	//		{
	//			printf("send error:%s(errno:%d)\n",strerror(errno),errno);
	//			close(connect_fd);
	//		}
	//		buffer[n]='\0';
	//		printf("recive message is:%s\n",buffer);
	//	}
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
			else if(pid==0)			//send message
			{
				while(n=recv(connect_fd,recv_buffer,4096,0))
				{
					printf("%s::%d    ",inet_ntop(AF_INET,&clientaddr.sin_addr,net_str,sizeof(net_str)),ntohs(clientaddr.sin_port));
					recv_buffer[n]='\0';
					printf("%s",recv_buffer);
				}
			}
			else				//receive message
			{
				while(fgets(send_buffer,4096,stdin)!=NULL)
				{
					if(send(connect_fd,send_buffer,strlen(send_buffer)+1,0)==-1)
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

