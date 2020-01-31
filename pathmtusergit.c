//tcp server workflow: socket creation-> binding the socket to the ip and port where it is should be listening to-->listening to that addresss and port--> accept the connection-->send or receive data
//final 1
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<errno.h>
#include<arpa/inet.h>
#include<string.h>

int ssock1,sockC;

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


int connectSocketA(){
    printf("In fun");
    ssock1 = socket(AF_INET,SOCK_STREAM,0);
    printf("%d",ssock1);
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(9005);
    server_address.sin_addr.s_addr=INADDR_ANY;
    int bret=bind(ssock1,(struct sockaddr*)&server_address,sizeof(server_address) );
    printf("%d",bret);
    int lret=listen(ssock1,10);
    printf("%d",lret);
    int client_socket; 
    client_socket=accept(ssock1,NULL,NULL); 
    printf("%d",client_socket);
    return client_socket;    

}


int connectSocketC(){
    printf("In fn\n");
    sockC = socket(AF_INET,SOCK_STREAM,0);
    printf("%d",sockC);

    struct sockaddr_in server_address1;
    server_address1.sin_family=AF_INET;
    server_address1.sin_port=htons(9000);
    server_address1.sin_addr.s_addr=INADDR_ANY;

    int con_stats= connect(sockC,(struct sockaddr *)&server_address1, sizeof(server_address1)); 
    if(con_stats==-1){
        printf("unsuccessful connection establishment");
    }
    return sockC;
    
}


    

int main(){
    struct packetSending pkt,pkt1,pkt2;
    struct packetReceiving pack;
    pack.mtu=10;
    int i;
   
    int valread;
    
    
     printf("In main\n");
    
   
    
    int client_socket1=connectSocketA();
    printf("Returned to main\n");
    int client_socket2;
    

   int ret=recv(client_socket1,(struct packetSending *)&pkt,sizeof(pkt),0);
   printf("%d",ret);
   //printf("Data received:%s\t%d\t%d\n",pkt.msg,pkt.Dest,pkt.pack_l);
   
   
   if(pkt.Dest==1){
    
           printf("Data packet:%s\n",pkt.msg);
           strcpy(pack.er_msg,"Data received");
           send(client_socket1,(void*)&pack,sizeof(pack),0);

        
   }
    else if(pkt.Dest==2){
       if(pkt.pkt_len>pack.mtu){
       strcpy(pack.er_msg,"Fragmentation needed");
       send(client_socket1,(void *)&pack,sizeof(pack),0);
       int ret6=recv(client_socket1,(struct packetSending*)&pkt,sizeof(pkt),0);
       printf("%d",ret6);
       printf("%s\t%d\t%d\n",pkt.msg,pkt.Dest,pkt.pkt_len);
       }
       printf("Connecting C\n");
       client_socket2=connectSocketC();
       strcpy(pkt1.msg,"Send me your MTU");
       
       int ret=send(client_socket2,(void *)&pkt1,sizeof(pkt1),0);
       printf("%d",ret);
       printf("%s\n",pkt1.msg);

       int ret1=recv(client_socket2,(struct packetReceiving *)&pack,sizeof(pack),0);
       //printf("%d",ret);
       printf("%s:%d\n",pack.er_msg,pack.mtu);
       if(pack.mtu>=pkt.pkt_len){
           pkt.Dest--;
           int ret3=send(client_socket2,(void *)&pkt,sizeof(pkt),0);
           printf("%d",ret3);
           printf("%s\t%d\t%d\n",pkt.msg,pkt.Dest,pkt.pkt_len);


            strcpy(pack.er_msg,"Data received");
            send(client_socket1,(void*)&pack,sizeof(pack),0);

           
       }else if(pack.mtu<pkt.pkt_len){
        //    strcpy(pack.er_msg,"Fragmentation needed");
        //    int ret2=send(client_socket1,(void *)&pack,sizeof(pack),0);
        //    printf("%d",ret2);
            //pkt.Dest--;
            
            // for(i=0;i<pkt.pkt_len;i++){
            //     if(i>=0 &&i<pack.mtu){

            //     }
            //     else{
            //         pkt.msg[i]=NULL;
            //     }
            // }
            // pkt.pkt_len=strlen(pkt.msg);
            // printf("%s",pkt.msg);
            // int ret4=send(client_socket2,(void *)&pkt,sizeof(pkt),0);
            // printf("%d",ret4);
            // printf("%s\t%d\t%d\n",pkt.msg,pkt.Dest,pkt.pkt_len);
            
                
                strcpy(pack.er_msg,"Fragmentation needed");

                send(client_socket1,(void *)&pack,sizeof(pack),0);

                int ret5=recv(client_socket1,(struct packetSending *)&pkt,sizeof(pkt),0);
                printf("%d",ret5);
                //printf("fragmented data:\n %s\t%d\t%d",pkt.msg,pkt.Dest,pkt.pkt_len);


                //pkt.Dest--;
                int ret3=send(client_socket2,(void *)&pkt,sizeof(pkt),0);
                printf("%d",ret3);
                printf("%s\t%d\t%d\n",pkt.msg,pkt.Dest,pkt.pkt_len);



       }




    }
    
  
   //printf("Data in received packet\n");
   //printf("Received packet data:%s\n Packet data length: %d\n Packet Destination:%d\n",pkt.msg,pkt.pack_l,pkt.Dest);
   
   
    
    // valread=read(client_socket2,client_msg,1000);
    // printf("C's msg: %s",client_msg);
    // int sret=send(client_socket2,welcome,sizeof(welcome),0);
    // printf("%d",sret);

    // if(read(client_socket2,(struct packet *)&pkt1,sizeof(pkt1))<0){
    //     printf("Error in receivinig packet");
    // }else{
    //     printf("The received string :%s\n",pkt1.msg);
    //    // printf("Received mtu is %d\n",pkt1.mtu);
        

    // }




   
    close(ssock1); 
    //close(client_socket2);
    
    return 0;

}

 