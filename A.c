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
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if (sock < 0) {
        printf("Sock val:%d\n",sock);
    }

    struct sockaddr_in server_add;
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(9000);
    server_add.sin_addr.s_addr = INADDR_ANY;


    int con_stats = connect(sock,(struct sockaddr *)&server_add, sizeof(server_add)); 
    if (con_stats == -1) {
        printf("unsuccessful connection establishment,%d\n",errno);
        close(sock);
    }
    
    return sock;
    
}

// char* fragment_data(char *msg, int length, int fragmentation_val){
//     int iter = 0;
//     for(; iter<length; iter++){
//         if(iter > 0 && iter <= fragmentation_val){

//         }
//         else{
//             msg[iter] = '\0';
//         }
//     }
//     //printf("%s\n",msg);
//     return (char *)msg;
// }


void send_to_B(int sock_arg){
    char arr[] = "abcdefghijklmnopqrstuvwxyz";
    int fragmentation_val = 0;
    int iter = 0;
    strcpy(obj1.msg, arr);
    obj1.dest = 2;
    obj1.src = 0;
    obj1.len = strlen(obj1.msg);
    int sret = send(sock_arg, (void *)&obj1, sizeof(obj1), 0);
    printf("No of bytes sending: %d\n",sret);
    printf("%s\t%d\t%d\t%d\n",obj1.msg, obj1.dest, obj1.len, obj1.src);
    int rret = recv(sock_arg, &fragmentation_val, sizeof(fragmentation_val), 0);
    if(fragmentation_val != obj1.len && fragmentation_val != 0) { 
        printf("Fragmentation needed %d\n",fragmentation_val);
        for (; iter<obj1.len; iter++) {
            if (iter >=0 && iter < fragmentation_val) {

            }
            else {
                obj1.msg[iter]= '\0';
            }
        }
        printf("%s\n",obj1.msg);
        obj1.len = strlen(obj1.msg);
        sret = send(sock_arg, (void *)&obj1, sizeof(obj1), 0);
        printf("No of bytes sending: %d\n",sret);
        printf("%s\t%d\t%d\t%d\n",obj1.msg, obj1.dest, obj1.len, obj1.src);
        
    }
    
    rret = recv(sock_arg, (sample_packet *)&obj1, sizeof(obj1), 0);
    printf("%s\n",obj1.msg);
    return ;
    
        
    

}







int main(){
    int client_socket = connect_socket();

   

    send_to_B(client_socket);
    //printf("%s", fragment_data("helloworld", 10, 2));

    close(client_socket);
    return 0;
}

    