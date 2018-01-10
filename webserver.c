#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define MAXSIZE     50
#define PORT 15000

void die(char *s)
{
    perror(s);
    exit(1);
}

int main() {
   int create_socket, new_socket;
   socklen_t addrlen;
   int bufsize = 1024;
   char *buffer = malloc(bufsize);
   struct sockaddr_in address;
   int shmid;
   key_t key;
   int *shm;
   int *rand_arr;
   key = 2345;


   if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) > 0){
      printf("The socket was created\n");
   }

   address.sin_family = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons(PORT);

   if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) == -1)   {
      printf("Binding error\n");
      exit(-1);
   }
   printf("binding complete\n");

   while (1) {
      if (listen(create_socket, 10) < 0) {
         perror("server: listen");
         exit(1);
 }
   printf("listening on port %d\n",PORT);
   if ((new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) < 0) {
         perror("server: accept");
         exit(1);
    }

   if (new_socket > 0){
         printf("The Client is connected...\n");
    }
   int avg=0;
   if ((shmid = shmget(key, MAXSIZE, 0666)) < 0)
        die("shmget");

   if ((shm = shmat(shmid, NULL, 0)) == (int *) -1)
        die("shmat");
   rand_arr = (int *)shm;
    //Now read what the server put in the memory.
   avg =rand_arr[0];
   recv(new_socket, buffer, bufsize, 0);
   sprintf(buffer,"%d",avg);
   write(new_socket,(const void *) buffer,strlen(buffer));
   close(new_socket);

   }
   close(create_socket);
   return 0;
}
  
