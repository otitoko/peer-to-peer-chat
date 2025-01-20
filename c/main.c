#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>


int server();
int client();


struct sockaddr_in listener_addr;

int main(){

    char input[3];
    int type;

    printf("Hello World!\n");
    printf("Choose handler type:\n");
    printf("1. Client\n");
    printf("2. Server\n");

    fgets(input, sizeof(input),stdin);

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    type = atoi(input);
    
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

    printf("Waiting for connection on port %d at %s...\n",ntohs(listener_addr.sin_port), ip_address);

    listen(serverfd, 5);

    int acceptfd = accept(serverfd,NULL,NULL);
    if(acceptfd<0){
        printf("Could not connect to client\n");
    }
    else{
        printf("Connected to anon\n");
    }
    /*
    for(int i=0;i<3;i++){
        dup2(acceptfd,i);
    }
    */


    while(1){
        char buf[1024];

        memset(buf,0,sizeof(buf));
        ssize_t bytes = recv(acceptfd, buf, sizeof(buf)-1,0);


        if(bytes<0){
            printf("Error recieving message");
            return 1;
        }
        else if(bytes == 0){
            printf("client dc");
            return 1;
        }
        else{
            printf("\nanon:");
            buf[bytes] = '\0';
            printf("%s",buf);
            fflush(stdout);
        }
    }

    close(acceptfd);
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

    if(inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr)<=0){
        printf("Could not assign ip address");
    }

    int connection_status = connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if(connection_status < 0){
        printf("Could not connect to server");
        return 1;
    }

    while(1){
        char *buf = malloc(32);

        char ch;
        int index = 0;


        while((ch = getchar()) != '\n' && ch != EOF){
            if(index >= sizeof(buf)){
                buf=realloc(buf,sizeof(buf)+(32*sizeof(char)));
            }
            buf[index] = ch;
            index++;
        }

        //input = fgets(buf,sizeof(buf),stdin);

        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        }
        printf("%s", buf);

        if(strlen(buf)>0){
            ssize_t bytes = send(sockfd, buf, strlen(buf),0);
            if(bytes<=0){
                printf("\nmsg didnt send\n");
                return 1;
            }
            fflush(stdout);
        }


        printf("\nMessage: %s\n",buf);
    }

    close(sockfd);

}
