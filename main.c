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


int server(int sockfd){
    int serverfd = socket(AF_INET, SOCK_STREAM,0);
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
    //add ip address to this print
    printf("Waiting for connection on port %d...\n",listener_addr.sin_port);

    listen(serverfd, 0);

    int acceptfd = accept(serverfd,NULL,NULL);
    for(int i=0;i<3;i++){
        dup2(acceptfd,i);
    }


}
    

int client(){
    int sockfd = socket(AF_INET, SOCK_STREAM,0);

    connect(sockfd, (struct sockaddr *) &listener_addr, sizeof(listener_addr));

}
