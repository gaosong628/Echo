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

#define USAGE                                                        \
  "usage:\n"                                                         \
  "  echoserver [options]\n"                                         \
  "options:\n"                                                       \
  "  -p                  Port (Default: 10823)\n"                    \
  "  -m                  Maximum pending connections (default: 5)\n" \
  "  -h                  Show this help message\n"

/* OPTIONS DESCRIPTOR ====================================================== */
static struct option gLongOptions[] = {
    {"help", no_argument, NULL, 'h'},
    {"port", required_argument, NULL, 'p'},
    {"maxnpending", required_argument, NULL, 'm'},
    {NULL, 0, NULL, 0}};

int main(int argc, char **argv) {
  int maxnpending = 5;
  int option_char;
  int portno = 10823; /* port to listen on */
  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  char buffer[BUFSIZE];
  int n;

  // Parse and set command line arguments
  while ((option_char =
              getopt_long(argc, argv, "hx:m:p:", gLongOptions, NULL)) != -1) {
    switch (option_char) {
      case 'm':  // server
        maxnpending = atoi(optarg);
        break;
      case 'p':  // listen-port
        portno = atoi(optarg);
        break;
      case 'h':  // help
        fprintf(stdout, "%s ", USAGE);
        exit(0);
        break;
      default:
        fprintf(stderr, "%s ", USAGE);
        exit(1);
    }
  }

  setbuf(stdout, NULL);  // disable buffering

  if ((portno < 1025) || (portno > 65535)) {
    fprintf(stderr, "%s @ %d: invalid port number (%d)\n", __FILE__, __LINE__,
            portno);
    exit(1);
  }
  if (maxnpending < 1) {
    fprintf(stderr, "%s @ %d: invalid pending count (%d)\n", __FILE__, __LINE__,
            maxnpending);
    exit(1);
  }

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
  printf("[+]Bind to the port number: %d\n", portno);
  
  /*make sure server is ready to listen*/
  listen(server_sock, 5);
  printf("Listening...\n");
  
  while(1)
  {
      /*accept data package*/
      addr_size=sizeof(client_addr);
      client_sock=accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
      printf("[+]Client connected.\n");
      
    while(1) {
      bzero(buffer,BUFSIZE);
      /*accept data and save it into buffer*/
      recv(client_sock, buffer, sizeof(buffer),0);
      
     if (strlen(buffer)==1 || strlen(buffer)>16)
     {printf("Received message error \n");
      exit(1);
     }

      printf("Message Received from Client: %s\n",buffer);
      /*send the same message back to the client*/
      send(client_sock,buffer,strlen(buffer),0);
    }
      close(client_sock); /*close the socket*/
      printf("Client disconnected");
      
  }
  
  return(0);

}
