// pop.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <winsock2.h>
using namespace std;
#define MAXBUF  2048

#pragma comment(lib,"ws2_32.lib")
//================================================
bool InitSock();
char sendbuf[MAXBUF] = { 0 } ;
char name[] = "user lzj9413@163.com\r\n" ; // pop3 请求头
char pass[] = "pass lzj159357\r\n" ;
char ret[]  = "retr 1\r\n";
char quit[] = "quit\r\n";  //  
char recvbuf[MAXBUF] = { 0 } ;
int  num = 0;
//=================================================================
void main() 
{
	InitSock();

	struct hostent *phost = gethostbyname("pop.163.com"); 
	
	//用TCP协议
	SOCKET httpsock = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
	if (INVALID_SOCKET == httpsock)
	{
		cout<<"socket error !"<<endl;
		WSACleanup( );
	}
	
	SOCKADDR_IN sock_addr = { 0 } ;
	sock_addr.sin_family  = AF_INET ;// TCP/IP协议
	sock_addr.sin_port    = htons(110) ;//存储端口号

	//拷贝字节到dest内存起始位置
        memcpy(&sock_addr.sin_addr.S_un.S_addr , phost->h_addr_list[0] , phost->h_length );
	cout<< "POP3 server IP: " <<inet_ntoa( sock_addr.sin_addr)<<endl;

	int res = connect(httpsock , (sockaddr *)&sock_addr , sizeof(sockaddr));
	if (res == SOCKET_ERROR)
	{
		cout<<"connect error !"<<endl;
		closesocket(httpsock);
		WSACleanup( );
		return ;
	}
	recv(httpsock , recvbuf , MAXBUF , 0);
	cout<<"Client : connect successfully!\nServer : "<<recvbuf<<endl;
	memset(recvbuf , 0 ,MAXBUF);
	


	send( httpsock , name , strlen(name) , 0 ) ;  //--------------------------NAME
	recv(httpsock , recvbuf , MAXBUF , 0);
	cout<<"Client : send name \nServer : "<<recvbuf<<endl;
	memset(recvbuf , 0 ,MAXBUF);

	send( httpsock , pass , strlen(pass) , 0 ) ; //----------------------------PASSWORD
	recv(httpsock , recvbuf , MAXBUF , 0);
	cout<<"Client : send password\nServer : "<<recvbuf<<endl;
    memset(recvbuf , 0 ,MAXBUF);


	send( httpsock , "stat\r\n" , strlen("stat\r\n") , 0 ) ;
	recv(httpsock , recvbuf , MAXBUF , 0);
	cout<<"Client : send stat \nServer : "<<recvbuf<<endl;
	memset(recvbuf , 0 ,MAXBUF);
	// Sleep(20);

	send( httpsock , ret , strlen(ret) , 0 ) ;
	cout<<"Client : send retr 1 \n";
	while (1)
	{		
		num = recv(httpsock , recvbuf , MAXBUF , 0);
		cout<<"Server : "<<recvbuf<<endl;
		memset(recvbuf , 0 ,MAXBUF);
		if (num<=0) 
		{
			break;
		}
	}

	// send( httpsock , "dele 1" , strlen("dele 1") , 0 ) ;

	send( httpsock , quit , strlen(quit) , 0 ) ;
	num = recv(httpsock , recvbuf , MAXBUF , 0);
	cout<<"Client : send quit  \nServer : "<<recvbuf<<endl;
	memset(recvbuf , 0 ,MAXBUF);
	// Sleep(20);

	closesocket(httpsock);

	system("pause");
	
	WSACleanup( );
	
}

//==================================================================
bool InitSock()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		
		return false;
	}	
	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {
		WSACleanup( );
		return false;
	}
	return true;		
}

