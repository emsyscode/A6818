/**********************************************************************/
/*This code is not clean and OF COURSE will be possible do it better! */
/*this is only a group of functions to be used as independent code,   */
/*and by this way will be possible to any person can do the changes   */
/*of code and see what happen.                                        */
/*The code don't depend of any external library or extenal functions  */
/*complicated.                                                        */
/*I'm let the values to sent as binnary, this allow swap bit by bit   */
/* to is possible test segment by segment without convert to HEX      */
/**********************************************************************/
/*   This code show how is possible work with the driver A6818        */
/*                                                                    */
/**********************************************************************/
#include <Arduino.h>
#include <String.h>

#define VFD_in 7  // This is the pin number 7 on Arduino UNO
#define VFD_clk 8 // This is the pin number 8 on Arduino UNO
#define VFD_stb1 9 // This is the pin number 9 on Arduino UNO
#define VFD_Blanking 10// This is the pin number 10 on Arduino UNO

int ledPin = 13; // LED connected to digital pin 13
// don't define the other pins, but is A0 to input analog signal, pins D2 & D3 is trigger input,

//ATT: On the Uno and other ATMEGA based boards, unsigned ints (unsigned integers) are the same as ints in that they store a 2 byte value.
//Long variables are extended size variables for number storage, and store 32 bits (4 bytes), from -2,147,483,648 to 2,147,483,647.
unsigned char word0 = 0x00;
unsigned char word1 = 0x00;
unsigned char word2 = 0x00;
unsigned char word3 = 0x00;
unsigned char word4 = 0x00;
unsigned char word5 = 0x00;
unsigned char word6 = 0x00;
unsigned char word7 = 0x00;
bool flag = true;
//*************************************************//
void send_char(unsigned char a) {
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  data = a;
  // the validation of data happen when clk go from LOW to HIGH.
  digitalWrite(VFD_stb1, LOW); // When strobe is low, all output is enable. If high, all output will be set to low.
  
  delayMicroseconds(5);
  digitalWrite(VFD_clk, LOW); // need invert the signal to allow 8 bits is is low only send 7 bits
  delayMicroseconds(5);
  for (mask = 0b10000000; mask > 0; mask >>= 1) { //iterate through bit mask
    digitalWrite(VFD_clk, LOW); // need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
    if (data & mask) { // if bitwise AND resolves to true
      digitalWrite(VFD_in, HIGH);
      //Serial.print(1);
    }
    else { //if bitwise and resolves to false
      digitalWrite(VFD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(VFD_clk, HIGH); // need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
    //
    digitalWrite(VFD_stb1, HIGH); // When strobe is low, all output is enable. If high, all output will be set to low.
    
    delayMicroseconds(5);
  }
}
void send_char_without(unsigned char a){
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  data = a;
  for (mask = 0b00000001; mask > 0; mask <<= 1) { //iterate through bit mask
    digitalWrite(VFD_clk, LOW);
    delayMicroseconds(5);
    if (data & mask) { // if bitwise AND resolves to true
      digitalWrite(VFD_in, HIGH);
      //Serial.print(1);
    }
    else { //if bitwise and resolves to false
      digitalWrite(VFD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(VFD_clk, HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
  }
  //Serial.println();
}
void allOn_A6818() {
  delayMicroseconds(5);

  send_char_without(0B11111111);  send_char_without(0B11111111); //   1:8   -   9:16//    byte 0 & 1
  send_char_without(0B11111111);  send_char_without(0B11111111); //  17:24  -  25:32//    Byte 2 & 3

  digitalWrite(VFD_stb1, HIGH); //
  delay(1);
  digitalWrite(VFD_stb1, LOW); //
  delay(1);
}
void animation_VFD(byte word0,byte word1,byte word2,byte word3) {
  digitalWrite(VFD_stb1, HIGH); //
  
  delayMicroseconds(5);

  send_char_without(word0);  send_char_without(word1); //   1:8   -   9:16//    byte 0 & 1
  send_char_without(word2);  send_char_without(word3); //  17:24  -  25:32//    Byte 2 & 3
 
  delayMicroseconds(5);
              digitalWrite(VFD_stb1, HIGH); //
              delay(1);
              digitalWrite(VFD_stb1, LOW); //
              delay(1);
              //
              // digitalWrite(VFD_Blanking, LOW);
              // delay(1000);
              // digitalWrite(VFD_Blanking, HIGH); 
   //
  delayMicroseconds(5);
}
void allOff_A6818() { 
  digitalWrite(VFD_stb1, HIGH); //

  delayMicroseconds(5);
  
  send_char_without(0B00000000);  send_char_without(0B00000000); //   1:8   -   9:16//
  send_char_without(0B00000000);  send_char_without(0B00000000); //  17:24  -  25:32//

  digitalWrite(VFD_stb1, LOW); //
}
void animeSeg1(){
    //--------------123456789abcdefghi..............----123456789abcdefghi..............----
  long arr[120] ={0b00010000010000000000000000000000, 0b00001000000000000000000000000000,//0 
                  0b00000100000000100000000000000000, 0b00000010000001000000000000000000,//2
                  0b00000000100100000000000000000000, 0b10000000000000000000000000000000,//4
                  0b01000000000000000000000000000000, 0b00100000000000000000000000000000,//6
                  0b00000000001000000000000000000000, 0b00000000000000010000000000000000,//8
                  0b00000000000000001000000000000000, 0b00000000000000000100000000000000,//10
                  0b00000000000000000000000000000000, 0b00000000000000000000000000000000,//12
                  0b00000000000000000000000000000000, 0b00000000000000000000000000000000,//14
                  0b00000000000000000000000000000000, 0b00000000000000000000000000000000,//16
                  0b00000000000000000000000000000000, 0b00000000000000000000000000000000,//18      
                  //
  }; 
  allOff_A6818();
  delay(600);
  word7= 0x0000;
  word6= 0x0000;
  word5= 0x0000;
  word4= 0x0000;       
  for(uint8_t s = 0; s < 5; s++){
        word3 =  (arr[s] & 0xff000000UL) >> 24;  //Because the 3 bits are assigned to GRID's
        word2 =  (arr[s] & 0x00ff0000UL) >> 16;
        word1 =  (arr[s] & 0x0000ff00UL) >>  8;
        word0 =  (arr[s] & 0x000000ffUL) >>  0;
        //animation_clock(word0,word1,word2,word3);
        word4 = (word4 | word0);
        word5 = (word5 | word1);
        word6 = (word6 | word2);
        word7 = (word7 | word3);
        animation_VFD(word4,word5,word6,word7);
        delay(750);
  }
}

void sendMove32bits(){
    unsigned long lng32 =  0b00000000000000000000000000000001; // Size of long 4294967295;
    unsigned int cnt = 0x00;
      for (int i = 0; i<32; i++){     
                word3 = (lng32 & 0xff000000UL) >> 24;  //Because the 3 bits are assigned to GRID's
                word2 = (lng32 & 0x00ff0000UL) >> 16;
                word1 = (lng32 & 0x0000ff00UL) >>  8;
                word0 = (lng32 & 0x000000ffUL) >>  0;
              send_char_without(word0);  send_char_without(word1); //
              send_char_without(word2);  send_char_without(word3); //
              //
              digitalWrite(VFD_stb1, HIGH); //
              delay(1);
              digitalWrite(VFD_stb1, LOW); //
              delay(1);
              //
              delay(500);
              lng32 = (lng32 << 1);
            }  
}
void send32seg(){
  //-------------------------123456789abcdefghi..............----------------------------
    unsigned long lng32 =  0b00011110101101100000000000000000; // Size of long 4294967295;
                word3 = (lng32 & 0xff000000UL) >> 24;  //Because the 3 bits are assigned to GRID's
                word2 = (lng32 & 0x00ff0000UL) >> 16;
                word1 = (lng32 & 0x0000ff00UL) >>  8;
                word0 = (lng32 & 0x000000ffUL) >>  0;
              send_char_without(word0);  send_char_without(word1); 
              send_char_without(word2);  send_char_without(word3);
              //
              digitalWrite(VFD_stb1, HIGH);
              delay(1);
              digitalWrite(VFD_stb1, LOW);
              delay(1);
}
void setup() {
  pinMode(VFD_clk, OUTPUT);
  pinMode(VFD_in, OUTPUT);
  pinMode(VFD_stb1, OUTPUT);
  pinMode(VFD_Blanking, OUTPUT);
 
  pinMode(ledPin, OUTPUT); // sets the digital pin as output
  Serial.begin(115200); // only to debug

}
void loop() {
  digitalWrite(VFD_Blanking, LOW);  //This let the VFD present the data.
  allOn_A6818();
  delay(2000);
    for (unsigned int v=0; v < 5; v++){  //Used like dinamic VFD control.
          allOn_A6818();
          delay(500); // waits for two second
          allOff_A6818();  //
          //digitalWrite(VFD_Blanking, HIGH); //This let the VFD blank
          delay(200); // waits for two second
          //digitalWrite(VFD_Blanking, LOW);  //This allow the VFD working
    }
    //
    for (unsigned int v=0; v < 2; v++){  
        sendMove32bits();
        delay(500);
    }
         send32seg();
         delay(1000);
         animeSeg1();
         delay(500);
}
