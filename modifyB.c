
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int ssock1, sockC;
int mtuB = 10;
int mtuC;

typedef struct {
    char msg[200];
    int dest;
    int pkt_len;
}packetMsg;

typedef struct {
    char er_msg[100];
    int mtu;
}packetErr;

 packetMsg pktB;
 packetErr packB;

int connect_socketA(){
    //printf("In fun");
    ssock1 = socket(AF_INET, SOCK_STREAM, 0);
    //printf("%d",ssock1);
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(9005);
    server_address.sin_addr.s_addr=INADDR_ANY;
    int bret = bind(ssock1, (struct sockaddr*)&server_address, sizeof(server_address) );
    if (bret < 0) {
    printf("%d",bret);
    }
    
    int lret=listen(ssock1,10);
    if (lret  < 0) {
    printf("%d",lret);
    }
    int client_socket; 
    client_socket = accept(ssock1, NULL, NULL); 
    //printf("%d",client_socket);
    return client_socket;    

}


int connect_socketC(){
   // printf("In fn\n");
    sockC = socket(AF_INET, SOCK_STREAM, 0);
    //printf("%d",sockC);
    struct sockaddr_in server_address1;
    server_address1.sin_family = AF_INET;
    server_address1.sin_port = htons(9000);
    server_address1.sin_addr.s_addr = INADDR_ANY;
    int con_stats = connect(sockC,(struct sockaddr *)&server_address1, sizeof(server_address1)); 
    if (con_stats == -1) {
        printf("unsuccessful connection establishment");
    }
    recv(sockC, (void *)&packB, sizeof(packB),0);
     mtuC = packB.mtu;
    //printf("%d\n",mtuC);
    return sockC;
    
}

void destination_B(int c_sock){
    printf("Data packet:%s\n",pktB.msg);
    strcpy(packB.er_msg,"Fragment received");
    send(c_sock,(void*)&packB,sizeof(packB),0);
}

void forward_to_nexthop(int c_sock1){
    if (pktB.pkt_len > mtuB) {
       strcpy(packB.er_msg,"Fragmentation needed");
       packB.mtu = mtuB;
       send(c_sock1,(void *)&packB,sizeof(packB),0);
       int ret6 = recv(c_sock1,(packetMsg*)&pktB,sizeof(pktB),0);
       //printf("%d",ret6);
       printf("%s\t%d\t%d\n",pktB.msg,pktB.dest,pktB.pkt_len);
       }
       printf("Connecting C\n");
       int c_sock2 = connect_socketC();
       pktB.dest--;
       if (mtuC >= pktB.pkt_len) {
           
           int ret3 = send(c_sock2,(void *)&pktB,sizeof(pktB),0);
           //printf("%d",ret3);
           printf("%s\t%d\t%d\n",pktB.msg,pktB.dest,pktB.pkt_len);


            //strcpy(packB.er_msg,"Fragment received");
            //send(c_sock1,(void*)&packB,sizeof(packB),0);

           
       }else if (mtuC < pktB.pkt_len) {
                strcpy(packB.er_msg,"Fragmentation needed");
                packB.mtu = mtuC;

                send(c_sock1,(void *)&packB,sizeof(packB),0);

                int ret5 = recv(c_sock1,(packetMsg *)&pktB,sizeof(pktB),0);
                //printf("%d",ret5);
                //printf("fragmented data:\n %s\t%d\t%d",pkt.msg,pkt.dest,pkt.pkt_len);


                //pktB.dest--;
                int ret3 = send(c_sock2,(void *)&pktB,sizeof(pktB),0);
                //printf("%d",ret3);
                printf("%s\t%d\t%d\n",pktB.msg,pktB.dest,pktB.pkt_len);

                strcpy(packB.er_msg,"Fragment received");
                send(c_sock1,(void*)&packB,sizeof(packB),0);

    }

}
    
int main(){
    int client_socket1 = connect_socketA();
    int client_socket2;
    int ret = recv(client_socket1,(packetMsg *)&pktB,sizeof(pktB),0);
    if (pktB.dest == 1) {
    
           destination_B(client_socket1);        
    }
    else if (pktB.dest == 2) {
       
            forward_to_nexthop(client_socket1);
  

    }

   
    close(ssock1); 
    close(sockC);
    return 0;

}

 