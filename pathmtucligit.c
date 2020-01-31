//tcp client workflow : socket create--> connect--> recv-->close the socket

//final 3
#include<stdio.h>
#include<stdlib.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<limits.h>
#include<netinet/in.h> 
#include<string.h>

struct packetSending{
    char msg[200];
    //int mtu;
    int Dest;
    int pkt_len;
};

struct packetReceiving{
    char er_msg[100];
    int mtu;
};

int connectSocket(){
    int sock= socket(AF_INET,SOCK_STREAM,0); 
    //printf("%d",sock);

    struct sockaddr_in server_address;
    server_address.sin_family =AF_INET;
    server_address.sin_port = htons(9005);
    server_address.sin_addr.s_addr = INADDR_ANY; 
    

    int con_stats= connect(sock,(struct sockaddr *)&server_address, sizeof(server_address)); 
    if(con_stats==-1){
        printf("unsuccessful connection establishment");
    }
    return sock;
}


int main(){
    struct packetSending pkt;
    
    struct packetReceiving pack,pack1;
    int valread;
    int i;
    //char client_welcome[1000]="Hello from client.Send me the MTU";
    //char server_msg[1000]={0};
    //int pmtu=INT_MIN;
    //int mtu1[2];
    //int i;
    //int mtuC=15;
    
    //char buff[1024];
    
    char arr[200]="abcdefghijklmnopqrstuvwxyz";
    int sock=connectSocket();
    
    //int s=send(sock,client_welcome,sizeof(client_welcome),0);
   // printf("%d sent....:)\n", s);
    /*if(read(sock,(struct packet *)&pkt,sizeof(pkt))<0){
        printf("Error in receivinig packet");
    }else{
        printf("The received string :%s\n",pkt.msg);
        printf("MTU of server is %d\n",pkt.mtu);
        

    }*/

    //printf("Enter data in to the packet:\n");
   // scanf("%[^\n]s",pkt.msg);
    strcpy(pkt.msg,arr);
    pkt.Dest=2;
    pkt.pkt_len=strlen(pkt.msg);
    printf("%s\t %d\t %d\n",pkt.msg,pkt.Dest,pkt.pkt_len);
    int s=send(sock,(void *)&pkt,sizeof(pkt),0);
    printf("%d\n",s);
    int j=pkt.Dest;
    //while(j<0){
    recv(sock,(struct packetReceiving *)&pack,sizeof(pack),0);
    
    if(strcmp(pack.er_msg,"Data received")==0){
        printf("%s\n",pack.er_msg);
    }
    else if(strcmp(pack.er_msg,"Fragmentation needed")==0){
        printf("%s: %d\n",pack.er_msg,pack.mtu);
        for(i=0;i<pkt.pkt_len;i++){
            if(i>=0 &&i<pack.mtu){

            }
            else{
                pkt.msg[i]=NULL;
            }
        }
        
        pkt.pkt_len=strlen(pkt.msg);
        printf("%s\t%d\n",pkt.msg,pkt.pkt_len);
        int sret=send(sock,(struct packetSending*)&pkt,sizeof(pkt),0);
        printf("%d",sret);

    }
   // j--;
//}

    recv(sock,(struct packetReceiving *)&pack1,sizeof(pack1),0);
    if(strcmp(pack1.er_msg,"Data received")==0){
        printf("%s\n",pack1.er_msg);
    }
    else if(strcmp(pack1.er_msg,"Fragmentation needed")==0){
        printf("%s: %d\n",pack1.er_msg,pack1.mtu);
        for(i=0;i<pkt.pkt_len;i++){
            if(i>=0 &&i<pack1.mtu){

            }
            else{
                pkt.msg[i]=NULL;
            }
        }
        pkt.Dest--;
        pkt.pkt_len=strlen(pkt.msg);
        printf("%s\t%d\n",pkt.msg,pkt.pkt_len);
        int sret=send(sock,(void *)&pkt,sizeof(pkt),0);
        printf("%d",sret);

    }
    

    

    



     
    
    
    
    
   
  
    close(sock);




    return 0;
}