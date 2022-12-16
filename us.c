#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFLEN 1024
#define MAX_LIMIT 50

int main(int argc, char *argv[])
{
  if (strcmp(argv[1], "0") == 0)
  {
    FILE *ptr;
    int myremoteport = atoi(argv[2]), i;
    char ch, filename[MAX_LIMIT] = "\0", str[MAX_LIMIT], msg[MAX_LIMIT] = "\0";

    int sockfd;
    char buffer[BUFLEN];

    // opening [current_port_no]buffer.txt
    strcat(filename, argv[3]);
    strcat(filename, "buffer.txt");
    ptr = fopen(filename, "r");
    if (NULL == ptr)
    {
      return (-1);
    }

    fgets(str, MAX_LIMIT, ptr);

    fclose(ptr);

    // setting message as [current_port_no] : <message>
    strcat(msg, argv[3]);
    strcat(msg, " : ");
    strcat(msg, str);

    // Creating socket and sending message


    char *messageStr = msg;
    struct sockaddr_in receiverAddr;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      return -1;
    }

    memset(&receiverAddr, 0, sizeof(receiverAddr));
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(myremoteport);
    receiverAddr.sin_addr.s_addr = INADDR_ANY;
    sendto(sockfd, (const char *)messageStr, strlen(messageStr), 0, (const struct sockaddr *)&receiverAddr, sizeof(receiverAddr));

    close(sockfd);
  }

  else if (strcmp(argv[1], "1") == 0)
  {
    FILE *ptr;
    int myremoteport = atoi(argv[2]), i;
    char ch, filename[MAX_LIMIT] = "\0", str[MAX_LIMIT], msg[MAX_LIMIT] = "\0", vfile[1000000];

    int sockfd;
    char buffer[BUFLEN];

    // Creating socket and sending file

    struct sockaddr_in receiverAddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      return -1;
    }

    memset(&receiverAddr, 0, sizeof(receiverAddr));
    bzero(&receiverAddr, sizeof(receiverAddr));
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(myremoteport);
    receiverAddr.sin_addr.s_addr = INADDR_ANY;
    int choice = 1;
    char num = choice; // just for avoiding errors. (REMOVABLE)
    sendto(sockfd, &num, sizeof(num), 0, (const struct sockaddr *)&receiverAddr, sizeof(struct sockaddr));
    strcat(filename, argv[4]); // taking filename from ./up as argument
    sendto(sockfd, filename, strlen(filename), 0, (struct sockaddr *)&receiverAddr, sizeof(struct sockaddr));

    // reading binary data of file
    ptr = fopen(filename, "rb");
    fseek(ptr, 0, SEEK_END);
    size_t vfsize = ftell(ptr); // The size_t represent the size of objects in bytes. It is guaranteed to be big enough to contain the size of the biggest object the host system can handle.
    fseek(ptr, 0, SEEK_SET);

    if (ptr)
    {
      if (fread(vfile, 1, vfsize, ptr) <= 0)
      {
        return -1;
      }
    }
    else
    {
      return -1;
    }

    if (sendto(sockfd, vfile, vfsize, 0, (struct sockaddr *)&receiverAddr, sizeof(struct sockaddr)) < 0)
    {
      return -1;
    }

    fclose(ptr);
  }

  else
  {
    return -1;
  }

  return 0;
}
