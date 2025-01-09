#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>


int server();
int client();


struct sockaddr_in listener_addr;

int main(){
    
    int type;
    printf("Hello World!\n");
    printf("Choose handler type:\n");
    printf("1. Client\n");
    printf("2. Server\n");

    scanf("%i", &type);
    if(type==1){
        client();
    }
    else{
        server();
    }

    

    return 0;
}


    int server(){
    int serverfd = socket(AF_INET,SOCK_STREAM,0);

    listener_addr.sin_family = AF_INET;
    listener_addr.sin_port = htons(5555);
    listener_addr.sin_addr.s_addr = INADDR_ANY;


    if (serverfd<0){
        printf("Error creating socket.\n");
        return 1;
}

    int bindfd = bind(serverfd,(struct sockaddr *)&listener_addr, sizeof(listener_addr));
    if(bindfd<0){
        printf("Error binding socket.\n");
        return 1;
    }
    char ip_address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(listener_addr.sin_addr), ip_address, INET_ADDRSTRLEN);
    //add ip address to this print
    printf("Waiting for connection on port %d at %s...\n",ntohs(listener_addr.sin_port), ip_address);

    listen(serverfd, 0);

    int acceptfd = accept(serverfd,NULL,NULL);
    if(acceptfd<0){
        printf("Could not connect to client\n");
    }
    for(int i=0;i<3;i++){
        dup2(acceptfd,i);
    }
    printf("Successful connection");

    close(serverfd);

}
    

int client(){
    int sockfd = socket(AF_INET, SOCK_STREAM,0);
    if(sockfd<0){
        printf("Error creating a socket.\n");
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(5555),
    };
    if(inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr)<0){
        printf("Could not assign ip address");
    }
    

    connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));


}
