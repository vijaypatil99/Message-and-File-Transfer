#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_LIMIT 100
#define BUFLEN 1024

#define mlen 100000

int main(int argc, char *argv[])
{
  if (strcmp(argv[1], "0") == 0)
  {
    int listenport = atoi(argv[2]);

    FILE *filePointer;
    char filename[MAX_LIMIT] = "\0";

    int sockfd, len, n;
    char buffer[BUFLEN];

    // Binding
    
    struct sockaddr_in receiverAddr, senderAddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      return -1;
    }

    memset(&receiverAddr, 0, sizeof(receiverAddr));
    memset(&senderAddr, 0, sizeof(senderAddr));
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_addr.s_addr = INADDR_ANY;
    receiverAddr.sin_port = htons(listenport);

    if (bind(sockfd, (const struct sockaddr *)&receiverAddr, sizeof(receiverAddr)) < 0)
    {
      return -1;
    }

    len = sizeof(senderAddr);

    n = recvfrom(sockfd, (char *)buffer, BUFLEN, MSG_WAITALL, (struct sockaddr *)&senderAddr, &len);
    buffer[n] = '\0';

    // Opening [current_port_no]messages.txt
    strcat(filename, argv[2]);
    strcat(filename, "messages.txt");
    filePointer = fopen(filename, "a");

    if (filePointer == NULL)
    {
      return -1;
    }
    else
    {
      time_t t;
      time(&t);
      if (strlen(buffer) > 0)
      {

        fputs(ctime(&t), filePointer); // atttaching date and time
        fputs(buffer, filePointer);    // with message
      }

      fclose(filePointer);
    }
  }

  else if (strcmp(argv[1], "1") == 0)
  {

    int listenport = atoi(argv[2]);
    int vfile[mlen];

    FILE *filePointer;
    char filename[MAX_LIMIT];

    for (int i = 0; i <= MAX_LIMIT; i++)
    {
      filename[i] = '\0';
    } // new

    // Binding
    
    int sockfd, len, n;
    char buffer[BUFLEN];
    struct sockaddr_in receiverAddr, senderAddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      return -1;
    }

    bzero(&receiverAddr, sizeof(receiverAddr));
    memset(&receiverAddr, 0, sizeof(receiverAddr));
    memset(&senderAddr, 0, sizeof(senderAddr));
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_addr.s_addr = INADDR_ANY;
    receiverAddr.sin_port = htons(listenport);

    if (bind(sockfd, (const struct sockaddr *)&receiverAddr, sizeof(receiverAddr)) < 0)
    {
      return -1;
    }

    int choice = 1;
    len = sizeof(senderAddr);
    char num;
    recvfrom(sockfd, &num, sizeof(num), 0, (struct sockaddr *)&senderAddr, &len);
    choice = num;
    recvfrom(sockfd, filename, BUFLEN, 0, (struct sockaddr *)&senderAddr, &len);
    size_t vfsize;
    vfsize = recvfrom(sockfd, vfile, mlen, 0, (struct sockaddr *)&senderAddr, &len);

    filePointer = fopen(filename, "wb");

    if (filePointer)
    {
      fwrite(vfile, vfsize, 1, filePointer);
    }
    else
    {
      return -1;
    }

    fclose(filePointer);
  }
  else
  {
    return -1;
  }

  return 0;
}
