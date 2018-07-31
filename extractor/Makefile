CXXFLAGS += -O2 -Wall -g -pthread -std=c++11
all: RFSniffer



	
RFSniffer: RCSwitch.o RFSniffer.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi
	

clean:
	$(RM) *.o RFSniffer