
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>

typedef struct {
    char msg[200];
    int dest;
    int pkt_len;
}packetMsg;

typedef struct {
    char er_msg[100];
    int mtu;
}packetErr;

packetMsg pktA;    
packetErr packA[2];

int connect_socket(){
    int sock = socket(AF_INET, SOCK_STREAM, 0); 
    //printf("%d",sock);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9005);
    server_address.sin_addr.s_addr = INADDR_ANY; 
    int con_stats = connect(sock,(struct sockaddr *)&server_address, sizeof(server_address)); 
    if (con_stats == -1) {
        printf("unsuccessful connection establishment");
    }
    return sock;
}

void transfer_data(){
    int i;
    int count=0;
    char arr[200] = "abcdefghijklmnopqrstuvwxyz";
    int sock = connect_socket();
    strcpy(pktA.msg,arr);
    pktA.dest = 2;
    pktA.pkt_len = strlen(pktA.msg);
    printf("%s\t %d\t %d\n",pktA.msg,pktA.dest,pktA.pkt_len);
    int s = send(sock,(void *)&pktA,sizeof(pktA),0);
    
    //printf("%d\n",s);
    while(count<=1){
    recv(sock,(packetErr*)&packA[count],sizeof(packA[count]),0);
    
    if (strcmp(packA[count].er_msg,"Fragment received") == 0) {
        printf("%s\n",packA[count].er_msg);
    }
    else if (strcmp(packA[count].er_msg,"Fragmentation needed") == 0) {
        printf("%s: %d\n",packA[count].er_msg,packA[count].mtu);
        for (i=0; i<pktA.pkt_len; i++) {
            if (i>=0 && i<packA[count].mtu) {

            }
            else {
                pktA.msg[i] = NULL;
            }
        }
        count++;
        if(count==1){
            pktA.dest--;
        }
        pktA.pkt_len = strlen(pktA.msg);
        printf("%s\t%d\n",pktA.msg,pktA.pkt_len);
        int sret = send(sock,(packetMsg*)&pktA,sizeof(pktA),0);
        
        
        //printf("%d",sret);

    }
} 
    
}


int main(){
    transfer_data();
    return 0;
}