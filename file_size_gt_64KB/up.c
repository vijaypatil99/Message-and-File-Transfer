#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_LIMIT 1024

int pipe_fd;

int my_sleep(int x)
{
    return usleep(x * 50000);
}

void create_and_open_pipe()
{
    int temp = mkfifo("pipe", 0777);
    pipe_fd = open("pipe", O_RDWR);
}

void close_and_remove_pipe()
{
    close(pipe_fd);
    system("rm pipe");
}

void send_data(char *data, long data_size, int sending_port, int receiving_port)
{
    FILE *process;
    char us_arg[MAX_LIMIT];
    char mock_buffer[256];

    memset(us_arg, 0, MAX_LIMIT);
    memset(mock_buffer, 0, 256);

    write(pipe_fd, data, data_size);

    snprintf(us_arg, MAX_LIMIT, "./us %d %d %ld", sending_port, receiving_port, data_size);
    process = popen(us_arg, "w");
    pclose(process);

    read(pipe_fd, mock_buffer, 256);
    // printf("replay buffer: %s\n", reply_buffer);

    my_sleep(1);
}

void receive_data(char *buffer, long data_size, int listening_port)
{
    FILE *process;
    char ur_arg[MAX_LIMIT];
    char mock_buffer[12] = "./ur opened";

    memset(ur_arg, 0, MAX_LIMIT);

    write(pipe_fd, mock_buffer, strlen(mock_buffer) + 1);

    snprintf(ur_arg, MAX_LIMIT, "./ur %d %ld", listening_port, data_size);
    process = popen(ur_arg, "w");
    pclose(process);

    read(pipe_fd, buffer, data_size);
}

int main(int argc, char *argv[])
{
    if (argc==3)
    {

        FILE *filePointer;
        FILE *process;
        long limitOnData = atoi(argv[2]);
        char ch, filename[MAX_LIMIT] = "\0";
        char buffer[limitOnData];
        int listening_port = atoi(argv[1]);
        int port_no;
        int choice, i;
        long total_packets;

        printf("\nEnter 1 to send file.\n Enter 2 to receive file.\n");
        scanf("%d", &choice);
        if (choice == 1)
        {

            printf("Enter port number :");
            scanf("%d", &port_no);

            printf("Enter filename to send: ");
            scanf("%s", &filename);

            filePointer = fopen(filename, "rb");
            if (filePointer == NULL)
            {
                printf("file not opened to read.");
                return 1;
            }

            // file size
            fseek(filePointer, 0, SEEK_END);
            long file_size = ftell(filePointer);
            fseek(filePointer, 0, SEEK_SET);

            printf("\nsize of file = %ld Bytes", file_size);

            // to get total number of packets
            if ((file_size % limitOnData) != 0)
                total_packets = (file_size / limitOnData) + 1;
            else
                total_packets = (file_size / limitOnData);

            printf("\nTotal number of packets to send= %ld\n", total_packets);

            create_and_open_pipe();

            // sending file name
            send_data(filename, strlen(filename), listening_port, port_no);

            // sending file size
            char fs[10];
            memset(fs, 0, 10);
            sprintf(fs, "%ld", file_size);
            send_data(fs, strlen(fs), listening_port, port_no);

            // sending file in blocks of size 'limitOnData'
            printf("\nsending file...\n");
            for (i = 1; i <= total_packets; i++)
            {
                memset(buffer, 0, limitOnData);
                long sd = fread(buffer, 1, limitOnData, filePointer);
                send_data(buffer, sd, listening_port, port_no);
            }

            close_and_remove_pipe();
            fclose(filePointer);

            if (i == total_packets + 1)
                printf("\nFile sent\n");
            else
                printf("\nFile not sent\n");
        }

        else if (choice == 2)
        {

            char temp[MAX_LIMIT];

            memset(buffer, 0, limitOnData);
            memset(filename, 0, MAX_LIMIT);
            memset(temp, 0, MAX_LIMIT);

            create_and_open_pipe();

            receive_data(filename, MAX_LIMIT, listening_port);
            printf("%s  filename received\n", filename);

            receive_data(temp, MAX_LIMIT, listening_port);

            long file_size = atoi(temp);

            // to get total number of packets
            int flag = file_size % limitOnData;

            if (flag > 0)
                total_packets = (file_size / limitOnData) + 1;
            else
                total_packets = (file_size / limitOnData);

            printf("\nTotal number of packets to receive= %ld\n", total_packets);
            filePointer = fopen(filename, "wb");
            if (filePointer == NULL)
            {
                printf("file not opened to write.");
                return 1;
            }
            printf("\nreceiving file...\n");
            for (i = 1; i <= total_packets; i++)
            {
                memset(buffer, 0, limitOnData);

                if (i == total_packets && flag > 0)
                {
                    receive_data(buffer, limitOnData, listening_port);
                    fwrite(buffer, 1, flag, filePointer);
                }
                else
                {
                    receive_data(buffer, limitOnData, listening_port);
                    fwrite(buffer, 1, limitOnData, filePointer);
                }
            }

            fclose(filePointer);
            close_and_remove_pipe();

            if (i == total_packets + 1)
                printf("\nFile recieved\n");
            else
                printf("\nFile not recieved\n");
        }

        else
            printf("Invalide choice!");
    }
      else
            printf("\nEnter appropriate arguments.\nFormat : ./filename [port_number] [limit_on_data]\n ");

    return 0;
}