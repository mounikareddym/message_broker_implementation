/*
 * mosquitto-sub.c
 * Consumes messages from an MQTT topic indefinitely
 */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include <string.h>
#include <time.h>
//shared memory
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// Server connection parameters
#define MQTT_HOSTNAME "localhost"
#define MQTT_PORT 1883
#define MQTT_USERNAME "admin"
#define MQTT_PASSWORD "admin"
#define MQTT_TOPIC1 "randomno1"
#define MQTT_TOPIC2 "randomno2"
/*
 * my_message_callback.
 * Called whenever a new message arrives
 */
#define MAXSIZE     50
int shmid;
int *shm;
void die(char *s)
{
    perror(s);
    exit(1);
}

void my_message_callback(struct mosquitto *mosq, void *obj,
    const struct mosquitto_message *message)
{

   static int i=0;
   char buf[255];
   int randno,j,sum=0,avg=0,arr[MAXSIZE];
   int *rand_arr;
   sprintf(buf,"%s",(char *)message->payload); //read random nos rom generators
   sscanf(buf, "%d", &arr[i]);
   i=(i++)%50;
   for(j=0;j<50;j++)
      sum+=arr[j];
   avg=sum/50;
   
   //store avg in shared memory for other process(webserver ) to read

    rand_arr = (int *)shm;
    rand_arr[i]= avg;

}

/*
 * Start here
 */
int main (int argc, char **argv)
{
  struct mosquitto *mosq = NULL;
  key_t key=2345;
  // Initialize the Mosquitto library
  mosquitto_lib_init();

  // Create a new Mosquito runtime instance with a random client ID,
  //  and no application-specific callback data.
  mosq = mosquitto_new (NULL, true, NULL);
  if (!mosq)
  {
    fprintf (stderr, "Can't init Mosquitto library\n");
    exit (-1);
  }

  // Set up username and password
  mosquitto_username_pw_set (mosq, MQTT_USERNAME, MQTT_PASSWORD);

  // Establish a connection to the MQTT server. Do not use a keep-alive ping
  int ret = mosquitto_connect (mosq, MQTT_HOSTNAME, MQTT_PORT, 0);
  if (ret)
  {
    fprintf (stderr, "Can't connect to Mosquitto server\n");
    exit (-1);
  }

  // Subscribe to the specified topic. Multiple topics can be
  //  subscribed, but only one is used in this simple example.
  //  Note that we don't specify what to do with the received
  //  messages at this point
  if ((shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666)) < 0)
        die("shmget");
  if ((shm = shmat(shmid, NULL, 0)) == (int *) -1)
        die("shmat");
  printf("shared mem attached\n");

  int ret1=0;
  int ret2=0;
  ret1 = mosquitto_subscribe(mosq, NULL, MQTT_TOPIC1, 0);
  if (ret1)
  {
    fprintf (stderr, "topic 1 Can't publish to Mosquitto server\n");
    exit (-1);
  }
  ret2 = mosquitto_subscribe(mosq, NULL, MQTT_TOPIC2, 0);
  if (ret2)
  {
    fprintf (stderr, " topic 2 Can't publish to Mosquitto server\n");
    exit (-1);
  }
  // Specify the function to call when a new message is received
  mosquitto_message_callback_set (mosq, my_message_callback);

  // Wait for new messages
  mosquitto_loop_forever (mosq, -1, 1);

  // Tidy up. In this simple example, this point is never reached. We can
  //  force the mosquitto_loop_forever call to exit by disconnecting
  //  the session in the message-handling callback if required.
  mosquitto_destroy (mosq);
  mosquitto_lib_cleanup();
  shmctl(shmid, IPC_RMID, NULL);
  return 0;
}
