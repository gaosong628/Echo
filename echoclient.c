#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>

// A buffer large enough to contain the longest allowed string 
#define BUFSIZE 256

#define USAGE                                                          \
  "usage:\n"                                                           \
  "  echoclient [options]\n"                                           \
  "options:\n"                                                         \
  "  -s                  Server (Default: localhost)\n"                \
  "  -p                  Port (Default: 10823)\n"                      \
  "  -m                  Message to send to server (Default: \"Hello " \
  "Summer.\")\n"                                                       \
  "  -h                  Show this help message\n"

/* OPTIONS DESCRIPTOR ====================================================== */
static struct option gLongOptions[] = {
    {"server", required_argument, NULL, 's'},
    {"port", required_argument, NULL, 'p'},
    {"message", required_argument, NULL, 'm'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}};

/* Main ========================================================= */
int  main(int argc, char **argv) 
{
  unsigned short portno = 10823;
  int option_char = 0;
  char *message = "Hello Summer!!";
  char *hostname = "localhost";
   int sock; /*variable for socket handle*/
  struct hostent* Hostinfo;   /* holds info about a machine */
  struct sockaddr_in addr;  /*structure for address*/
  char buffer[BUFSIZE];
  long Hostaddress;
  int n=1;
 

  // Parse and set command line arguments
  while ((option_char =
              getopt_long(argc, argv, "p:s:m:hx", gLongOptions, NULL)) != -1) {
    switch (option_char) {
      default:
        fprintf(stderr, "%s", USAGE);
        exit(1);
      case 's':  // server
        hostname = optarg;
        break;
      case 'p':  // listen-port
        portno = atoi(optarg);
        break;
      case 'h':  // help
        fprintf(stdout, "%s", USAGE);
        exit(0);
        break;
      case 'm':  // message
        message = optarg;
        break;
    }
  }

  setbuf(stdout, NULL);  // disable buffering

  if ((portno < 1025) || (portno > 65535)) {
    fprintf(stderr, "%s @ %d: invalid port number (%d)\n", __FILE__, __LINE__,
            portno);
    exit(1);
  }

  if (NULL == message) {
    fprintf(stderr, "%s @ %d: invalid message\n", __FILE__, __LINE__);
    exit(1);
  }

  if (NULL == hostname) {
    fprintf(stderr, "%s @ %d: invalid host name\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Socket Code Here */
  /*create a socket and verify it*/
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    perror("Socket error");
    exit(1);
  }
  printf("TCP server socket created.\n");
  
    /* get IP address */
    Hostinfo=gethostbyname(hostname);
    /* copy address*/
    memcpy(&Hostaddress,Hostinfo->h_addr,Hostinfo->h_length);

  memset(&addr, '\0', sizeof(addr));/*set struct addr to null*/
  addr.sin_family = AF_INET; /*internet protocol*/
  addr.sin_port = portno; /*address port number*/
  addr.sin_addr.s_addr = Hostaddress;/* internet address*/

  connect(sock, (struct sockaddr*)&addr, sizeof(addr));
  printf("Connected to the server.\n");
 while(1)
 {
   bzero(buffer,BUFSIZE);
   if (n>1) 
   { 
    printf("Please type in Message sending to Server: ");
    fgets(buffer,sizeof(buffer),stdin);
     
     if (strlen(buffer)==1 || strlen(buffer)>16)
     {printf("Input message error \n");
      exit(1);
     }
   }
   else
   {
    printf("Message sending to Server For the First Time: \n"); 
    strcpy(buffer,message);
   }
    n=n+1;
   /*send message to the server*/
    send(sock, buffer, sizeof(buffer),0);
   /*clear the content in buffer*/
    bzero(buffer,BUFSIZE);
   /*receive message from the server*/
    recv(sock,buffer,sizeof(buffer),0);
    printf("Message Reveived from server: %s\n",buffer);
        
    }
    close(sock); /*close the socket*/
    printf("Disconnected from the server.\n");

  return 0;
  
}
