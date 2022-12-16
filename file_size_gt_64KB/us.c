#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

// #define REMOTEPORT 1234
//#define BUFLEN 1024*16

int main(int argc, char *argv[])
{
  int present_port = atoi(argv[1]);
  int remote_port = atoi(argv[2]);
  int BUFLEN = atoi(argv[3]);

  int sockfd;
  char buffer[BUFLEN];
  memset(buffer,0,BUFLEN);

  struct sockaddr_in receiverAddr;

  int pipe = open("pipe", O_RDWR);
  read(pipe, buffer, BUFLEN);

  char *mock_buffer = "pipe opened and read successfully\n";
  write(pipe, mock_buffer, strlen(mock_buffer) + 1);
  close(pipe);


  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  memset(&receiverAddr, 0, sizeof(receiverAddr));
  receiverAddr.sin_family = AF_INET;
  receiverAddr.sin_port = htons(remote_port);
  receiverAddr.sin_addr.s_addr = INADDR_ANY;
  sendto(sockfd, (const char *)buffer, BUFLEN, 0, (const struct sockaddr *)&receiverAddr, sizeof(receiverAddr));
  
  close(sockfd);
  return 0;
}
