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


/* Main ========================================================= */
int  main(int argc, char **argv) 
{
  unsigned short portno = 10823;
  char *message = "Hello Summer!!";
  char *hostname = "localhost";
   int sock; /*variable for socket handle*/
  struct hostent* Hostinfo;   /* holds info about a machine */
  struct sockaddr_in addr;  /*structure for address*/
  char buffer[BUFSIZE];
  long Hostaddress;
  int n=1;
 
  /* Socket Code Here */
  /*create a socket and verify it*/
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    perror("Socket error");
    exit(1);
  }
  //printf("TCP server socket created.\n");
  
    /* get IP address */
    Hostinfo=gethostbyname(hostname);
    /* copy address*/
    memcpy(&Hostaddress,Hostinfo->h_addr,Hostinfo->h_length);

  memset(&addr, '\0', sizeof(addr));/*set struct addr to null*/
  addr.sin_family = AF_INET; /*internet protocol*/
  addr.sin_port = portno; /*address port number*/
  addr.sin_addr.s_addr = Hostaddress;/* internet address*/

  connect(sock, (struct sockaddr*)&addr, sizeof(addr));
  //printf("Connected to the server.\n");
 while(1)
 {
   bzero(buffer,BUFSIZE);
   if (n>1) 
   { 
    //printf("Please type in Message sending to Server: ");
    fgets(buffer,sizeof(buffer),stdin);
     
     if (strlen(buffer)==1 || strlen(buffer)>16)
     {printf("Input message error \n");
      exit(1);
     }
   }
   else
   {
    //printf("Message sending to Server For the First Time: \n"); 
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
