#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <inttypes.h>
#include <unistd.h>


RCSwitch mySwitch;



int main(int argc, char *argv[]) {

     // This pin is not the first pin on the RPi GPIO header!
     // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     // for more information.
     int PIN = 1;
	  FILE *fp;
     if(wiringPiSetup() == -1)
       return 0;

     mySwitch = RCSwitch();
     mySwitch.enableReceive(PIN);  // Receiver on inerrupt 0 => that is pin #2


     while(1) {
	usleep(10000);
      if (mySwitch.available()) {

        int value = mySwitch.getReceivedValue();

        if (value == 0) {
             printf("Unknown encoding");
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
				fflush(stdout);
				fclose(fp);




             printf("Received %i\n", mySwitch.getReceivedValue() );
             //Show pulse(Depends on your RF outlet device. You may need to change the pulse on codesend.cpp)
	       printf("Received pulse %i\n", mySwitch.getReceivedDelay() );
        }

        mySwitch.resetAvailable();

      }


  }

  exit(0);


}
