/*
Usage: ./codesend decimalcode [-p pin] [-l pulselength] [-c protocol]
decimalcode - As decoded by RFSniffer
protocol    - According to rc-switch definitions
pulselength - pulselength in microseconds

 'codesend' hacked from 'send' by @justy
 
 - The provided rc_switch 'send' command uses the form systemCode, unitCode, command
   which is not suitable for our purposes.  Instead, we call 
   send(code, length); // where length is always 24 and code is simply the code
   we find using the RF_sniffer.ino Arduino sketch.

(Use RF_Sniffer.ino to check that RF signals are being produced by the RPi's transmitter 
or your remote control)
*/

#include "../rc-switch/RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
     
#define DEFAULT_PIN           0
#define DEFAULT_PULSE_LENGTH  189
#define DEFAULT_PROTOCOL      1

void printUsage(char *argv[]) {
    printf("Usage: %s <code> [-p <PIN Number> (default: %i)] [-l <Pulse Length> (default: %i)] [-c <protocol> (default: %i)].\n", argv[0], DEFAULT_PIN, DEFAULT_PULSE_LENGTH, DEFAULT_PROTOCOL);
}

int main(int argc, char *argv[]) {
    
    int i;

    char * argumentPIN = NULL;
    char * argumentPulseLength = NULL;
    char * argumentProtocol = NULL;

    int c;
    while ((c = getopt(argc, argv, "p:l:")) != -1) {
        switch (c) {
            case 'p':
                argumentPIN = optarg;
                break;

            case 'l':
                argumentPulseLength = optarg;
                break;

            case 'c':
                argumentProtocol = optarg;
                break;

            case '?':
            default:
                printUsage(argv);
                break;
            }
    }

    /* Now set the values of "argc" and "argv" to the values after the
       options have been processed, above. */
    argc -= optind;

    if (argc == 0) {
        printUsage(argv);
        return EXIT_FAILURE;
    }

    argv += optind;

    // Parse the first parameter to this command as an integer
    int code = atoi(argv[0]);

    // This PIN is not the first PIN on the Raspberry Pi GPIO header!
    // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
    // for more information.
    int PIN = DEFAULT_PIN;

    if (argumentPIN != NULL) {
        PIN = atoi(argumentPIN);
    }

    // Pulse length depends on the RF outlets you are using. Use RFSniffer to see what pulse length your device uses.
    int pulseLength = DEFAULT_PULSE_LENGTH;

    if (argumentPulseLength != NULL) {
        pulseLength = atoi(argumentPulseLength);
    }

    // Protocol depends on the RF outlets you are using. Use RFSniffer to see what pulse length your device uses.
    int protocol = DEFAULT_PROTOCOL;

    if (argumentProtocol != NULL) {
        protocol = atoi(argumentProtocol);
    }

    printf("Sending Code: %i. PIN: %i. Protocol: %i. Pulse Length: %i.\n", code, PIN, protocol, pulseLength);

    RCSwitch mySwitch = RCSwitch();
    mySwitch.setProtocol(protocol, pulseLength);
    mySwitch.enableTransmit(PIN);
    
    mySwitch.send(code, 24);
    
    return 0;

}
