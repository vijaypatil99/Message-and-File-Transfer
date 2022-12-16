#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
// #define BUFLEN 1024
// #define LISTENPORT 1234

int main(int argc, char *argv[])
{
  int sockfd, len, n;
  int LISTENPORT = atoi(argv[1]);
  long limitOnData = atoi(argv[2]);

  char buffer[limitOnData];
  struct sockaddr_in receiverAddr, senderAddr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket system call failed");
    exit(EXIT_FAILURE);
  }

  memset(buffer, 0, limitOnData);
  memset(&receiverAddr, 0, sizeof(receiverAddr));
  memset(&senderAddr, 0, sizeof(senderAddr));

  receiverAddr.sin_family = AF_INET;
  receiverAddr.sin_addr.s_addr = INADDR_ANY;
  receiverAddr.sin_port = htons(LISTENPORT);

  if (bind(sockfd, (const struct sockaddr *)&receiverAddr, sizeof(receiverAddr)) < 0)
  {
    perror("bind syscall failed");
    exit(EXIT_FAILURE);
  }
  len = sizeof(senderAddr);
  // printf("waiting for data to be received: \n");
  n = recvfrom(sockfd, (char *)buffer, limitOnData, MSG_WAITALL, (struct sockaddr *)&senderAddr, &len);
 
  char mock_buffer[12];
  memset(mock_buffer,0,12);
  int pipe = open("pipe", O_RDWR);
  read(pipe,mock_buffer,12);
  //printf("\n%s\n",tsqp);
  write(pipe, buffer, limitOnData);
  close(pipe);

  // printf("\ndata received\n");
  // printf("----------------------------------------------\n");
  return 0;
}
