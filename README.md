# message_broker_implementation
Code challenge

This project aims to create 2 random number generators (which generate random numbers with an interval of 100 ms). This random no generation is treated as an event which is subscribed by an MQTT broker. A webserver program that can respond to http get requests from browser calculates the average of random nos generated in the last 5 seconds and displays it on the browser


Steps to run the project:

 1> Setting up MQTT Mosquitto broker in Ubuntu Linux

   	1. Login to Ubuntu Linux with root privileges ( sudo )

	  2. Add the mosquitto repository by given below commands

		   $sudo apt-add-repository ppa:mosquitto-dev/mosquitto-ppa
		   $sudo apt-get update
		   $sudo apt-get install mosquitto
		   $sudo apt-get install libmosquitto-dev
		   $sudo apt-get install mosquitto-clients
		   $sudo service mosquitto status 

	  3. Testing

		   Open a terminal and issue the given below command to subscribe the topic “mqtt”
			   $mosquitto_sub -h localhost -t "mqtt" -v
		   Open another terminal and issue the given below command to publish message to the topic “mqtt”
			   $mosquitto_pub -h localhost -t "mqtt" -m "Hello MQTT"   
		   Now the message “Hello MQTT” will be displayed in the first terminal where the topic “mqtt” is subscribed.
   
 2> Download the souce code into a folder in a linux machine
 
 3> Run the program by issuing the below command
   
       ./run.sh
	 
 3>  Issue a GET request from the browser as shown below
   
        http://localhost:15000
