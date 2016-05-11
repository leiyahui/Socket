#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<netdb.h>
void main()
{
	int socket_fd;
	struct sockaddr_in servaddr;
	char send_line[4096],recv_line[4096];
	int recv_len;
	int pid,ppid;
	char szWeb[]="www.baidu.com";
    struct hostent* phost;
    FILE* fp=fopen("test.txt","w");
	if((socket_fd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("socket fail:%s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
    phost=gethostbyname("www.baidu.com");
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(80);
	servaddr.sin_addr=*((struct in_addr*)phost->h_addr);
	if(connect(socket_fd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1)
	{
		printf("connect error:%s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	  sprintf(send_line, "GET /index.html HTTP/1.1\r\nHost:%s\r\nConnection: Keep-Alive\r\n\r\n", szWeb);  
		if(send(socket_fd,send_line,strlen(send_line),0)==-1)
		{
            printf("send error:%s(errno:%d)\n",strerror(errno),errno);
			exit(0);
        }
	while((recv_len=recv(socket_fd,recv_line,4096,0))!=-1)
	{			
	  	recv_line[recv_len]='\0';
         fputs(recv_line,fp);
	}
	close(socket_fd);
   fclose(fp);
   return ;
}
	
	
	
	
	
