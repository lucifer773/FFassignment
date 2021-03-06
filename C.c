#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include "structures.h"


sample_packet obj1;


int connect_socket(){
    //printf("In fun\n");
    int sock1 = socket(AF_INET, SOCK_STREAM, 0);
    printf("Sock connecting to B:%d\n",sock1);
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(9001);
    server_address.sin_addr.s_addr=INADDR_ANY;
    int bret = bind(sock1, (struct sockaddr*)&server_address, sizeof(server_address) );
    if (bret < 0) {
        printf("Binding Val of B-C:%d\n",bret);
    }
    
    int lret=listen(sock1,10);
    if (lret  < 0) {
        printf("Listening val of B-C:%d\n",lret);
    }
    int client_socket; 
    client_socket = accept(sock1, NULL, NULL); 
    printf("Client_socket NUmber:%d\n",client_socket);
    return client_socket;    

}


void receive_from_B(int sock_arg){
    mtu = 5;
    int rret = recv(sock_arg, (sample_packet *)&obj1, sizeof(obj1), 0);
    if (obj1.len <= mtu){
        printf("No of bytes receving: %d\n",rret);
        printf("%s\t%d\t%d\t%d\n",obj1.msg, obj1.dest, obj1.len, obj1.src);
        //return;
    }
    else if (obj1.len > mtu) {
        int sret = send(sock_arg, &mtu, sizeof(mtu), 0);
        printf("Sending the MTU %d\n",mtu);
    }
   //if(obj1.dest == 2){
    rret = recv(sock_arg, (sample_packet *)&obj1, sizeof(obj1), 0);
    printf("No of bytes receving: %d\n",rret);
    printf("%s\t%d\t%d\t%d\n",obj1.msg, obj1.dest, obj1.len, obj1.src);
        //printf("%s\t%d\t%d\t%d\n",obj1.msg, obj1.dest, obj1.len, obj1.src);
    strcpy(obj1.msg,"Fragment reached the destination C");
    int sret = send(sock_arg, (void *)&obj1 , sizeof(obj1), 0);
        //printf("Message sent to B: %s\n",obj1.msg);
    return ;
    //}
}


int main(){
    int client_socket = connect_socket();
    
    receive_from_B(client_socket);
    close(client_socket);

    return 0;
}