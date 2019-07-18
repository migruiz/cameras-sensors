/*
  RFSniffer

  Usage: ./RFSniffer [<pulseLength>]
  [] = optional

  Hacked from http://code.google.com/p/rc-switch/
  by @justy to provide a handy RF code sniffer
*/

#include "../rc-switch/RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <chrono>
#include <inttypes.h>
     
     
RCSwitch mySwitch;
 


int main(int argc, char *argv[]) {
  
     // This pin is not the first pin on the RPi GPIO header!
     // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     // for more information.
     int PIN = 1;
     FILE *fp;
     
     if(wiringPiSetup() == -1) {
       printf("wiringPiSetup failed, exiting...");
       return 0;
     }

     int pulseLength = 0;
     if (argv[1] != NULL) pulseLength = atoi(argv[1]);

     mySwitch = RCSwitch();
     if (pulseLength != 0) mySwitch.setPulseLength(pulseLength);
     mySwitch.enableReceive(PIN);  // Receiver on interrupt 0 => that is pin #2
     
    
     while(1) {
  
      if (mySwitch.available()) {
    
        int value = mySwitch.getReceivedValue();
    
        if (value == 0) {
          printf("Unknown encoding\n");
        } else {    
   


        using namespace std::chrono;
				milliseconds milis = duration_cast< milliseconds >(
					system_clock::now().time_since_epoch()
					);
				char buffer[64]; // The filename buffer.
								 // Put "file" then k then ".txt" in to filename.
				if (argc == 2) {
					snprintf(buffer, sizeof(char) * 64, "%s%" PRId64 ".csv", argv[1], milis);
				}
				else {
					snprintf(buffer, sizeof(char) * 64, "%" PRId64 ".csv", milis);
				}

				fp = fopen(buffer, "w");
				fprintf(fp, "%i", mySwitch.getReceivedValue());
				fflush(fp);
				fclose(fp);






          printf("Received %i\n", mySwitch.getReceivedValue() );
        }
    
        fflush(stdout);
        mySwitch.resetAvailable();
      }
      usleep(100); 
  
  }

  exit(0);


}

