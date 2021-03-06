

#include <ESP8266WiFi.h>

//------------------------------------------------------------------------------------
  // Authentication Variables
  char*       ssid;              
  char*       password;         

//------------------------------------------------------------------------------------

  
// WiFi settings
#define     MAXSC     6  

IPAddress APlocal_IP(192, 168, 4, 1);
IPAddress APgateway(192, 168, 4, 1);
IPAddress APsubnet(255, 255, 255, 0);

unsigned int TCPPort = 2390;

WiFiServer  TCP_SERVER(TCPPort);      
WiFiClient  TCP_Client[MAXSC];       

// Some Variables
char result[10];


void setup(){
  
  // Setting the serial port
  Serial.begin(115200);          
    
  // setting up a Wifi AccessPoint
  SetWifi("Iot Project","");
}

 
void loop(){
  
  HandleClients(); 

}

 
void SetWifi(char* Name, char* Password){
  // Stop any previous WIFI
  WiFi.disconnect();

  // Setting The Wifi Mode
  WiFi.mode(WIFI_AP_STA);
  Serial.println("WIFI Mode : AccessPoint");
  
  // Setting the AccessPoint name & password
  ssid      = Name;
  password  = Password;
   
  // Starting the access point
  WiFi.softAPConfig(APlocal_IP, APgateway, APsubnet);                 
  WiFi.softAP(ssid, password, 1, 0, MAXSC);                 
  Serial.println("WIFI < " + String(ssid) + " > ... Started");
   
  // wait a bit
  delay(50);
   
  // getting server IP
  IPAddress IP = WiFi.softAPIP();
  
  // printing the server IP address
  Serial.print("AccessPoint IP : ");
  Serial.println(IP);

  // starting server
  TCP_SERVER.begin();                                                 
  
  Serial.println("Server Started");
}


void HandleClients(){
unsigned long tNow;
       
  if(TCP_SERVER.hasClient()){
    WiFiClient TCP_Client = TCP_SERVER.available();
    TCP_Client.setNoDelay(1);                                          
    while(1){
      //---------------------------------------------------------------
      // If clients are connected
      //---------------------------------------------------------------
      if(TCP_Client.available()){
        // read the message
        String Message = TCP_Client.readStringUntil('\r');
          
        // print the message on the screen
        Serial.print("Received packet of size ");
        Serial.println(sizeof(Message));
          
        // print who sent it
        Serial.print("From ");
        Serial.print(TCP_Client.remoteIP());
        Serial.print(", port ");
        Serial.println(TCP_Client.remotePort());

        // content
        Serial.print("Content: ");
        Serial.println(Message);

        // generate a response - current run-time -> to identify the speed of the response
        tNow=millis();
        dtostrf(tNow, 8, 0, result);
          
        // reply to the client with a message     
        TCP_Client.println(result);                 
        TCP_Client.flush();
      }
       
      //---------------------------------------------------------------
      // If clients are disconnected                                         
      //---------------------------------------------------------------     
      if(!TCP_Client || !TCP_Client.connected()){
        // Here We Turn Off The LED To Indicated The Its Disconnectted
        digitalWrite(LED0, LOW);
        break;
      }
    }   
  }
  else{
    Serial.print("No Client are available");
  }
}


