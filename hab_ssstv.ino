/* Hamshield
SSTV Beacon for HAB purposes
Uses standard Hamshield, arduino UNO, and MP3 Player "shield"
MP3 Player is connected as follows:
VCC - 5V
RX - D11 with 1k Resistor
TX - D10
AUDIO_R = D3
AUDIO_L
GND - GND
*/

#define DDS_REFCLK_DEFAULT 9600
#include <HamShield.h>
#include <DFPlayerMini_Fast.h>

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
#define MIC_PIN 3 //PWM or not?
#define RESET_PIN A3
#define SWITCH_PIN 2

HamShield radio; // nCS, CLK, DAT

DFPlayerMini_Fast myMP3;

// Run our start up things here
void setup() { 
  // NOTE: if not using PWM out, it should be held low to avoid tx noise
  //3pinMode(MIC_PIN, OUTPUT);
  //digitalWrite(MIC_PIN, LOW);
  
  // prep the switch
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  
  // set up the reset control pin
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);
  delay(5);
  // Set up the serial port at 9600 Baud
  Serial.begin(9600);
  
  // Send a quick serial string
  Serial.println("HamShield SSTV");
  
  // Tell the HamShield to start up
  delay(200);
  radio.initialize();

  // Set the transmit power level (0-8)
  radio.setRfPower(0);
  // Configure the HamShield to operate on 438.000MHz
  radio.frequency(145650); //update
  mySerial.begin(9600);
  myMP3.begin(mySerial, true);
  Serial.println("Radio Configured.");
}

void loop() {
    radio.setRfPower(8);
    myMP3.wakeUp();
    radio.setTxSourceMic();
    myMP3.play(1); // Requires an mp3 filed named "1" with SSTV-encoded signal. More work will be done to encode bits using the programmer itself. use https://www.vr2woa.com/sstv/
    delay(10);
    radio.setModeTransmit();
    Serial.println("Tx");
    Serial.print("Transmitting... ");
    delay(35000);
    myMP3.stop();
    myMP3.sleep();

    /*
    myMP3.volume(10);
    myMP3.startDAC();

    myMP3.sendData();
    */
    
    radio.setModeReceive();
    delay(60000);


}
