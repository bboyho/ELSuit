/*******************************************************************
XBee_ELSequencer_ControllerV1.ino
Modified by Ho Yun Bobby Chan @  SparkFun Electronics May 10th, 2017

Taken from SparkFun XBee EL Sequencer Controller Example
Ho Yun Bobby Chan @ SparkFun Electronics June 20, 2014
Updated by Toni Klopfenstein @ SparkFun Electronics April, 2015
https://github.com/sparkfun/EL_Sequencer

Description:
This is a basic test of the wireless controller to control an EL Sequencer.
The wireless controller consists of an RedBoard Programmed with Arduino, XBee Shield,
XBee Series 1 transceiver, and LED Push Button. The EL Sequencer
uses an EL wire, 12V EL inverter, a 9V wall wart, 9V adapter, XBee female sockets soldered,
battery, and a XBee Series 1 transceiver. An XBee Series 2
can be used but the throughput of the Series 1 is much higher. To
reduce latency, I recommend using the XBee Series 1. The basic
configuration of the XBee module with point-to-point configuratin is
based on Digi's Example tutorial
=> http://examples.digi.com/get-started/basic-xbee-802-15-4-chat/ .
Page 5 of the tutorial shows you how to broadcast with
point-to-multipoint configuration so that multiple EL Sequencers
can be controlled.

Connect the XBee Shield to the Arduino with the switch flipped to the
software serial side labelled "DLINE".

By pushing the button, a character is sent from a remote microcontroller.
The corresponding EL Sequencer will receive the character and control
the EL component on a channel that is associated with that character.
A LED push button was used but not necessary for this to operate. A
normal momentary push button can be used.

Connect momentary LED tactile push buttons to Arduino:
     C = pin 13, the cathode (+) of the LED
     A = GND, the anode (-) of the LED
     B1 = GND
     B2 = pin 8

Using a RedBoard Programmed with Arduino, the XBee transceiver is connected to the
Software Serial pins. By pushing the button, the Arduino will
send one character through the XBee. Logic is used to control
how many characters are sent with the push button. The Arduino
will not send another character until the button is pressed again.

Note: This section of the code can be optimized. As the user is not
pressing down on the button, logic can be added so that the XBee is
not continuously sending serial characters to the receiving
EL Sequencer when idle.

Development environment specifics:
Arduino 1.6.5

This code is beerware; if you see me (or any other SparkFun employee) at the local,
and you've found our code helpful, please buy us a round!
Distributed as-is; no warranty is given.
**************************************************************************/

#include <SoftwareSerial.h>

SoftwareSerial xbee(2, 3); //Rx = 2, Tx = 3

//Declare variables and pin definitions

//Send
const int button1Pin = 8; //push button
const int ledPin1 = 13;  //LED on the push button
//variables to check for button1 state
boolean prev_button1State = false;
boolean current_button1State = false;

/*******************Setup Loop***************************/
void setup() {

  //Declare pin modes
  pinMode(button1Pin, INPUT_PULLUP); //use internal pullup resistor with LED
  pinMode (ledPin1, OUTPUT);//LED to indicate when character has been sent

  //Declare serial connections for debugging
  Serial.begin(9600);
  Serial.println("Arduino started sending bytes via XBee");

  xbee.begin(9600);
  Serial.println("EL Sequencer Controller's XBee Ready to Communicate");
}

/*******************Main Loop***************************/
void loop() {
  //initialize variables to read buttons
  int button1State;
  
  button1State = digitalRead(button1Pin);
  /***button1state
   - LOW or 0 means pressed
   - HIGH or 1 means not pressed
   ****/

  //if button is pressed, it will be pulled low
  if (button1State == LOW) {
    digitalWrite(ledPin1, HIGH); //turn push button LED ON
    current_button1State = true; // button has been pressed once

    if (prev_button1State != current_button1State) //check to see if button is still being pressed
    {
      Serial.println("Button is pressed.");
      xbee.write("A");//Tell Sequencer to change to mode by sending a character
    }
    else {
      //do nothing because finger is still on button
    }
    prev_button1State = current_button1State;
  }

  //button has not been pressed, it will be high again
  else {
    current_button1State = false;
    digitalWrite(ledPin1, LOW); // turn push button LED OFF

    prev_button1State = current_button1State;
  }

}
