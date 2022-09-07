#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <getopt.h>
#include <netdb.h>

#define BUFSIZE 256



int main(int argc, char **argv) {
  int portno = 10823; /* port to listen on */
  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  char buffer[BUFSIZE];
  int n;

  /* Socket Code Here */
  /*create a socket and verify*/
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0){
    perror("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");

  memset(&server_addr, '\0', sizeof(server_addr)); /*set server_addr to null*/
  server_addr.sin_family = AF_INET; /*family type*/
  server_addr.sin_port = portno; /*port number*/
  server_addr.sin_addr.s_addr = INADDR_ANY; /*internet address*/
  /*bind newly created socket to a given IP address and verify it*/
  n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0){
    perror("[-]Bind error");
    exit(1);
  }
  //printf("[+]Bind to the port number: %d\n", portno);
  
  /*make sure server is ready to listen*/
  listen(server_sock, 5);
  //printf("Listening...\n");
  
  while(1)
  {
      /*accept data package*/
      addr_size=sizeof(client_addr);
      client_sock=accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
      //printf("[+]Client connected.\n");
      
    while(1) {
      bzero(buffer,BUFSIZE);
      /*accept data and save it into buffer*/
      recv(client_sock, buffer, sizeof(buffer),0);
      
     if (strlen(buffer)==1 || strlen(buffer)>16)
     {printf("Received message error \n");
      exit(1);
     }

      //printf("Message Received from Client: %s\n",buffer);
      /*send the same message back to the client*/
      send(client_sock,buffer,strlen(buffer),0);
    }
      close(client_sock); /*close the socket*/
      printf("Client disconnected");
      
  }
  
  return(0);

}
