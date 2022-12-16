1.open terminal.
2-make sure you are in right directory (where you have extraced the file).
3-enter following commands -


    $ gcc -o ur ur.c
    $ gcc -o us us.c
    $ gcc -o up up.c

    ignore the warnings.

4- Here ./up provides interface for messaging.

5- open instances of ./up in diffrent terminals.use following command.
    
    $sudo ./up <port_number>

    (sudo password required for every instance otherwise program may misbehave on SOME SYSTEMS.)

    port_number :       
                    -is an argument for up
                    -must be a valid port number.
                    -must be unique for every instance
                    -eg. sudo ./up 10000
                         sudo ./up 20000
                         sudo ./up 30000


6-First, You have to put one terminal(system) in receiver mode (select appropriate choice code on terminal),
Then select choice code for send message and hit enter from where you wish to send message to above terminal.
(you need to know the port number of receiver's terminal).

7-For sending/receiving files, copy all binary files (up,ur,us) to another folder and open instance
  there so that you can able to see that files are transferring.
  If you try to send file to the instance that has opened in same directory then you may not able to 
  see that files are transferring because same file is overwritting there.
  
8-File must be at same directory from where file sending instance is opened.File receiving instance should be
  opened at diffrent directory.

9-You can try with readme.txt file.
  
10-File size upto 64KB can be transferred at a time.

---------------------------------------------------------------------------------------------------------------------------------------------------------
