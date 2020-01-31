//final 2

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<limits.h>
#include<netinet/in.h>
#include<strings.h>

int sock;

struct packetSending{
    char msg[200];
    int Dest;
    int pkt_len;
};

struct packetReceiving{
    char er_msg[100];
    int mtu;
};

int connectSocket(){
    printf("Inside fn\n");
    sock= socket(AF_INET,SOCK_STREAM,0);
    printf("%d",sock);
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(9000);
    server_address.sin_addr.s_addr=INADDR_ANY;
    int bret=bind(sock,(struct sockaddr*)&server_address,sizeof(server_address));
    printf("%d",bret);
    int lret=listen(sock,10);
    printf("%d",lret);
    int client_socket;
    client_socket=accept(sock,NULL,NULL);
    printf("%d",client_socket);
    return client_socket;
  }

int main(){
    //pack.mtu=10;
    struct packetSending pkt;
    struct packetReceiving pack;
    
    printf("In main\n");
    int client_socket=connectSocket();
    // 
    printf("Returned from fn\n");
    

    recv(client_socket,(struct packetSending *)&pkt,sizeof(pkt),0);
    printf("%s\n",pkt.msg);

    //send(client_socket,(void *)&pkt,sizeof(pkt),0);
    //printf("%s\n",pkt.msg);
    strcpy(pack.er_msg,"sending MTU in packet");
    pack.mtu=10; ///it is mtu of C
    //printf("Sending packet\n");
    int sret=send(client_socket,(void*)&pack,sizeof(pack),0);
    printf("Sent MTU\n");
    int ret=recv(client_socket,(struct packetSending *)&pkt,sizeof(pkt),0);
    printf("%d",ret);
    printf("%s\t%d\t%d\t",pkt.msg,pkt.Dest,pkt.pkt_len);



    //printf("%d\n",sret);
    //printf("%s\t%d\n",pack.er_msg,pack.mtu);



    close(client_socket);
    return 0;
}