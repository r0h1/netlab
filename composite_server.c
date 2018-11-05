#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
void main()
{
int ssock,csock;
int a,c,i;
unsigned int len;
struct sockaddr_in server,client;
if((ssock=socket(AF_INET,SOCK_STREAM,0))==-1)
{
perror("socket: is not created");
exit(-1);
}
server.sin_family=AF_INET;
server.sin_port=htons(10000);
server.sin_addr.s_addr=INADDR_ANY;
bzero(&server.sin_zero,0);
len=sizeof(struct sockaddr_in);
if((bind(ssock,(struct sockaddr *)&server,len))==-1){
perror("bind: ");
exit(-1);
}
if((listen(ssock,5))==-1){
perror("listen: ");
exit(-1);
}
if((csock=accept(ssock,(struct sockaddr *)&client,&len))==-1){
perror("accept: ");
exit(-1);
}
while(1)
{
recv(csock,&a,sizeof(a),0);

    
    while (a!=1&&a%2 == 0)
    {
        c=2;
        send(csock,&c,sizeof(c),0);
        a = a/2;
    }
 
    
    for (i = 3; i <= a; i = i+2)
    {
        
        while (a!=1&&a%i == 0)
        {
            c=i;
            send(csock,&c,sizeof(c),0);
            a = a/i;
        }
    }
 
c=-1;
send(csock,&c,sizeof(c),0);

}
close(ssock);
}
