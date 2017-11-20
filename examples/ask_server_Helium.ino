

#include <SPI.h>
#include <Ethernet2.h>

#include "TimerOne.h"
#include <MsTimer2.h>

#include "Event_list.h"


byte mac[] = {
  0x90, 0xA2, 0xDA, 0x10, 0xF8, 0xDD
};
IPAddress ip(10, 214, 1, 84);
IPAddress myDns(10,214,1, 1);
IPAddress gateway(10, 214, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress server(10,214,1,81);
IPAddress HeliumDepth(10,214,1,78);
EthernetClient client_OpticReader;
EthernetClient client_HeliumDepth;
Event_list events;

const long ask_delay_OpticReader=1000000;
const long ask_delay_HeliumDepth = 5000000;
char buf[100];
char buf_HeliumDepth[100];
char buf_read[100];
char buf_count[100];
int i=0, j=0, k=0;
int i_lim = 0;
bool receiving = false;
long level=0;
uint32_t count =0;
uint32_t previous_count =0;
const int Remy_effect = 0;
const int thresh_low_1 = 15+Remy_effect;
const int thresh_low_2 = 19+Remy_effect;
const int level_threshold = 550;
int thresh_low = thresh_low_2;
const int thresh_high = 23+Remy_effect;
const int PIN_1 = 8;
const int PIN_2 = 9;
const int i_offset = 16;
int current_state = 0;
unsigned long t=micros();
unsigned long last_t=micros();
const unsigned long Delay = 1000000;
bool waiting_connection_OpticReader=false;
bool waiting_connection_HeliumDepth=false;
bool receiving_HeliumDepth=false;
bool level_threshold_passed=false;
bool waiting_HeliumDepth = false;

void wait_HeliumDepth(){
  waiting_HeliumDepth = false;
}

void connect_OpticReader(){
  waiting_connection_OpticReader=false;
  Serial.println("Establishing a connexion with optic reader...");
  if (client_OpticReader.connect(server, 80)) {
      Serial.println("Make a HTTP request ... ");

      client_OpticReader.println("GET http://10.214.1.81/");
      client_OpticReader.println();

      //Serial.println("ok");

    } 
  else {
      // if you didn't get a connection to the server:
      Serial.println("connection failed");
      waiting_HeliumDepth= true;
      events.add(&wait_HeliumDepth, micros(), 0);
      //waiting_connection_OpticReader=true;
  }
    
}

void connect_HeliumDepth(){
  waiting_connection_HeliumDepth=false;
  Serial.println("Establishing Connexion to Helium Depth...");
  if (client_HeliumDepth.connect(HeliumDepth, 5000)) {
    delay(50);
    Serial.print(client_HeliumDepth.write("G\r\n", 3));
    Serial.println(" bytes were written to client_HeliumDepth");
    //client_HeliumDepth.println(); 
    client_HeliumDepth.flush();
  }
  else {
    Serial.println("Connexion failed Helium Depth");
    waiting_connection_HeliumDepth=true;
    events.add(&connect_HeliumDepth, micros(), ask_delay_HeliumDepth);
  }
}

void receiving_new_count(){
  i=0;
  for(int j=i_offset;j<i_lim;j++){
    buf_count[j-16]=buf[j];
    //Serial.print(buf[j]);
  }
  count = atol(buf_count);
  int dif = count-previous_count;
  //Serial.print("new count is ");
  //Serial.println(count);
  Serial.print("The time passed between two measures is ");
  Serial.print(float((micros()-last_t)/float(1000000)));
  Serial.println(" s");
  last_t = micros();
  if(level_threshold_passed){    
    digitalWrite(PIN_1, HIGH);
    digitalWrite(PIN_2, HIGH);
    Serial.println("threshold passed !");
  }
  else{
    if(current_state==0){
        thresh_low=thresh_low_2;
    }
    else{
        thresh_low=thresh_low_1;
    }
    if(dif<thresh_low){
      digitalWrite(PIN_1, LOW);
      digitalWrite(PIN_2, HIGH);
      Serial.println("LOW");
      current_state=0;
    }
    if(dif>thresh_high){
      digitalWrite(PIN_1, HIGH);
      digitalWrite(PIN_2, LOW);
      Serial.println("HIGH");
      current_state=2;
    }
    if((dif<=thresh_high)&&(dif>=thresh_low)){
      digitalWrite(PIN_1, HIGH);
      digitalWrite(PIN_2, HIGH);
      Serial.println("MIDDLE");
      current_state=1;
    }
  }
  previous_count = count;
  //Serial.println(dif);
  //Serial.println("disconnecting.");
  client_OpticReader.stop();
//  if (!waiting_connection_OpticReader){
//    Serial.println("hey");
//    events.add(&connect_OpticReader, micros(), 1000000);
//    waiting_connection_OpticReader=true;
//  }
}

void receiving_new_char_OpticReader(){
  char c = client_OpticReader.read();
  //Serial.println(c);
  buf[i]=c;
  if(c==';'){
    i_lim=i;
  }
  i++;
}

void receiving_new_char_HeliumDepth(){
  char c = client_HeliumDepth.read();
  //sprintf(buf_read, "char number %d : %c", j, c);
  //Serial.println(buf_read);
  //Serial.print(c);
  if (c=='B' or c=='A'){
    receiving_HeliumDepth=true;
    //Serial.println("Receiving !");
    j=0;
  }
  if (receiving_HeliumDepth) {
    //Serial.println("Receiving");
    buf_HeliumDepth[j]=c;
    j++;
    //Serial.print("buf_HeliumDepth is now ");
    //Serial.println(buf_HeliumDepth);
  }
  if (c=='\n'){
    //Serial.println("end of line !");
    if (receiving_HeliumDepth){
      receiving_HeliumDepth=false;
      //Serial.print("buf_HeliumDepth is finally ");
      //Serial.println(buf_HeliumDepth);
      //Serial.print("his length is finally ");
      //Serial.println(j);
      Serial.print("the level is ");
      level=String(buf_HeliumDepth).substring(2,6).toInt();
      Serial.println(level);
      if(level>level_threshold){
        level_threshold_passed=true;
      }
      else{
        level_threshold_passed=false;
      }
      j=0;
      client_HeliumDepth.stop();
    }
  if(!waiting_connection_HeliumDepth){
      Serial.println("relaunching");
      waiting_connection_HeliumDepth = true;
      events.add(&connect_HeliumDepth, t, ask_delay_HeliumDepth);
      
    }
  }
}



void setup() {
  Serial.begin(9600);  

  Serial.print("Setup LAN ... ");
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Ethernet.begin(mac, ip);
  Serial.println("ok");
  pinMode(PIN_1, OUTPUT);
  pinMode(PIN_2, OUTPUT);
  delay(1000);
  connect_HeliumDepth();


  //connect_OpticReader();
    
}


void loop(){
  

  t=micros();
  if (client_OpticReader.available()) {
    
    events.add(&receiving_new_char_OpticReader, t, 0);
  }

  // if the server's disconnected, stop the client:
  if (!client_OpticReader.connected()) {
    //Serial.println("yolo");
    if(!waiting_connection_OpticReader){
      events.add(&receiving_new_count, t,0);
      waiting_connection_OpticReader = true;
      events.add(&connect_OpticReader, t, ask_delay_OpticReader);}
  }  
  //t=micros();
//  if ((t-last_t)>Delay){
//    last_t=t;
//    k++;
//    Serial.print("t= ");
//    Serial.println(t);
//    Serial.print("The length is now ");
//    Serial.println(events.get_length());
//  }
  //t=micros();
  //Serial.println("doing a small check man");
  if (client_HeliumDepth.available()){
    events.add(&receiving_new_char_HeliumDepth, t, 0);
  }
  events.check(t);

  int to_read = client_HeliumDepth.available();

  if (to_read) {
    events.add(&receiving_new_char_HeliumDepth, micros(), 0);
    //int jmax = j+to_read;
    //Serial.print(to_read);
    //Serial.println(" bytes to read");
    //Serial.print(jmax);
    //Serial.println(" is the limit index");
//    char c = client_HeliumDepth.read();
//    sprintf(buf_read, "char number %d : %c", j, c);
//    Serial.println(buf_read);
//    //Serial.print(c);
//    if (c=='B' or c=='A'){
//      receiving_HeliumDepth=true;
//      Serial.println("Receiving !");
//      j=0;
//    }
//    if (receiving_HeliumDepth) {
//      Serial.println("Receiving");
//      buf_HeliumDepth[j]=c;
//      j++;
//      Serial.print("buf_HeliumDepth is now ");
//      Serial.println(buf_HeliumDepth);
//    }
//    
//    //Serial.print("buf_HeliumDepth is now ");
//    //Serial.println(buf_HeliumDepth);
//    //if(c==';'){
//    //  i_lim=i;
//    //}
//    //j++;
//    if (c=='\n'){
//      Serial.println("end of line !");
//      if (receiving_HeliumDepth){
//        receiving_HeliumDepth=false;
//        Serial.print("buf_HeliumDepth is finally ");
//        Serial.println(buf_HeliumDepth);
//        Serial.print("his length is finally ");
//        Serial.println(j);
//        Serial.print("the level is finally ");
//        level=String(buf_HeliumDepth).substring(2,6).toInt();
//        Serial.println(level);
//        j=0;
//        client_HeliumDepth.stop();
//      }
//
//    }
//    if (c=='\r'){
//      Serial.println("carriage return !");
//      Serial.println("Disconnecting Helium Depth...");
//      client_HeliumDepth.stop();
//      Serial.println(buf_HeliumDepth);
//      delay(ask_delay_HeliumDepth);
//      connect_HeliumDepth();
//      j=0;
//    }
  }
  
  if (!client_HeliumDepth.connected()&&(!waiting_connection_HeliumDepth)&&(!waiting_HeliumDepth)) {
    client_HeliumDepth.stop();
    j=0;
    events.add(&connect_HeliumDepth, micros(), ask_delay_HeliumDepth);
    waiting_connection_HeliumDepth = true;
//    j=0;
//    Serial.println(buf_HeliumDepth);
//    Serial.println("Disconnecting Helium Depth...");
//    client_HeliumDepth.stop();
//    delay(1000);
//    //delay_async(ask_delay_HeliumDepth, &connect_HeliumDepth);
//    connect_HeliumDepth();
  }

}
