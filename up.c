#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LIMIT 100

int main(int argc, char *argv[])
{

    while (1)
    {

        FILE *ptr, *filePointer;
        char ch, filename[MAX_LIMIT] = "\0";

        int choice, flag;
        char msg[MAX_LIMIT],buffer[MAX_LIMIT], port_no[MAX_LIMIT], arg[MAX_LIMIT] = "./us ", arg1[MAX_LIMIT] = "./ur ";


        printf("\n\n1- Enter 1 to send a message.\n2- Enter 2 to send a file.\n3- Enter 3 to receive a message.\n4- Enter 4 to receive a file.\n5- Enter 5 to show all received messages.\n6- Enter 6 to exit.\n");
        printf("Enter your choice : ");
        scanf("%d", &choice);
        fflush(stdout);

        if (choice == 1)
        {
            puts("Enter port number : ");
            scanf("%s", &port_no);

            fflush(stdin);
            while ((getchar()) != '\n'); // This will consume the '\n' char
                

            puts("Enter message :");
            fgets(msg, MAX_LIMIT, stdin);

            //creating file to insert message into it.
            //It will act as buffer for ./us

            strcat(filename, argv[1]);
            strcat(filename, "buffer.txt");  //[current_port_no]buffer.txt

            filePointer = fopen(filename, "w");

            if (filePointer == NULL)
            {
                printf("%s file failed to open.", filename);
            }
            else
            {
                fputs(msg, filePointer);
            }

            fclose(filePointer);


            //arguments for ./us
            strcat(arg,"0");      //set ./us to send message
            strcat(arg, " ");
            strcat(arg, port_no); // message receiver's port number
            strcat(arg, " ");
            strcat(arg, argv[1]); // current working port / message sender's port
            
            
            flag = system(arg); //initiate ./us process with all arguments

            if (flag != -1)
            {
                printf("message sent!\n");
            }
            else
            {
                printf("message not sent!\n");
             
            }
        }



        else if (choice== 2)
        {
            printf("\n**File size must be less than 64KB**\n\n");

            puts("Enter port number : ");
            scanf("%s", &port_no);

            fflush(stdin);
            while ((getchar()) != '\n'); // This will consume the '\n' char
                

            puts("Enter file name (with extension) to send :");
            fgets(filename, MAX_LIMIT, stdin);


            //arguments for ./us
            strcat(arg,"1");            //set ./us to send file
            strcat(arg, " ");
            strcat(arg, port_no);       // file receiver's port number
            strcat(arg, " ");
            strcat(arg, argv[1]);       // current working port / file sender's port
            strcat(arg, " ");
            strcat(arg, filename);      // attaching filename


            flag = system(arg);         //initiate ./us process with all arguments

            if (flag != -1)
            {
                printf("file sent!\n");
            }
            else
            {
                printf("file not sent!\n");
             
            }


        }




        else if (choice == 3)
        {
            printf("waiting for message to be received: \n");

            //arguments for ./ur
            strcat(arg1,"0");       //set ./ur to receive message
            strcat(arg1," ");       
            strcat(arg1, argv[1]);  //current working port / listening port

            flag = system(arg1);    //initiate ./ur process with all arguments

            if (flag != -1)
            {
                //Opening [current_port_no]messages.txt and reading message written by ./ur

                strcat(filename, argv[1]);
                strcat(filename, "messages.txt");

                ptr = fopen(filename, "r");
                if (NULL == ptr)
                {
                    printf("No message found\n");
                    continue;
                }

                while (!feof(ptr))
                    fgets(buffer, MAX_LIMIT, ptr);

                printf("message received from %s", buffer);
                fclose(ptr);
            }
            else
            {
                printf("Error in listening message!\n");
            }
        }

        else if(choice==4)
        {
            printf("waiting for file to be received: \n");

            //arguments for ./ur
            strcat(arg1,"1");       //set ./ur to receive file
            strcat(arg1," ");       
            strcat(arg1, argv[1]);  //current working port / listening port

            flag = system(arg1);    //initiate ./ur process with all arguments

            if (flag != -1)
            {
               
                printf("\nFile received\n");
                
            }
            else
            {
                printf("Error in listening message!\n");
            }

            
        }

        else if (choice == 5)
        {

            //Opening [current_port_no]messages.txt and reading all messages.

            strcat(filename, argv[1]);
            strcat(filename, "messages.txt");

            ptr = fopen(filename, "r");
            if (NULL == ptr)
            {
                printf("No messages found\n");
                continue;
            }
            printf("Showing all messages: \n");
            do
            {
                ch = fgetc(ptr);
                printf("%c", ch);

            } while (ch != EOF);

            fclose(ptr);
        }


        else if (choice == 6)
        {
            remove(strcat(argv[1], "buffer.txt"));  //Removing [current_port_no]buffer.txt
            printf("closing the program!\n");
            break;
        }

        
        else
        {
            printf("Enter a valid choice!");
        }
    }

    return 0;
}
