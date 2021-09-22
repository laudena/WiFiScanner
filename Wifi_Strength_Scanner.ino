#include <SPI.h>
#include <WiFiClientSecure.h>
#include <ssl_client.h>
#include <HTTPClient.h>
#include <WiFiManager.h> 
#include <DNSServer.h>
#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>
#include <ezButton.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
String NETWORK_OF_INTEREST = "wifinetwork";
ezButton buttonUp(4);
ezButton buttonDown(2);
ezButton buttonSend(15);
int locationTagNum = 1;
int data_version = 0;
int timeOutForSubmission = 15000;
int timeLastSubmit = 0;
String thingSpeakApiKey = "PUT-YOUR-KEY-HERE";
String uri = "thingspeak.com/update?api_key=" + thingSpeakApiKey;  
const char* root_ca = \
                      "-----BEGIN CERTIFICATE-----\n" \
"MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
"d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n" \
"ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL\n" \
"MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n" \
"LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\n" \
"RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm\n" \
"+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW\n" \
"PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM\n" \
"xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB\n" \
"Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3\n" \
"hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg\n" \
"EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF\n" \
"MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA\n" \
"FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec\n" \
"nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z\n" \
"eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF\n" \
"hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2\n" \
"Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe\n" \
"vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep\n" \
"+OkuE6N36B9K\n" \
"-----END CERTIFICATE-----\n";


void setup() {
  Serial.begin(9600);
  Serial.print("-----------Starting------------");
  lcd.init(); // initialize the lcd 
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Waiting for WiFi...  ");
  
  //If the device can't connect to the internet, the user can connect to "WiFiScanner"
  WiFiManager wifiManager;
  wifiManager.autoConnect("WiFiScanner");
  Serial.println("Connected to WiFi.  ");
  lcd.setCursor(1,0);lcd.print("Connected.         ");

  buttonUp.setDebounceTime(10);
  buttonDown.setDebounceTime(10);
  buttonSend.setDebounceTime(10);
  
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
}
void loop() {
  buttonUp.loop();
  buttonDown.loop();
  buttonSend.loop();


  if(buttonUp.isPressed()){
    if (locationTagNum < 50)
      locationTagNum++;

  }
  if(buttonDown.isPressed()){
    if (locationTagNum > 1)
      locationTagNum--;
  }
  if(buttonSend.isPressed()){
    buttonSend.loop();
    if (millis() > timeOutForSubmission + timeLastSubmit)
      scan();
    else{
      lcd.setCursor(1,0);lcd.print("Too early to send.    ");
    }
    
  }
  String timeOutTxt = "OK";   
  if (millis() < timeOutForSubmission + timeLastSubmit){
    timeOutTxt = String((int)((timeOutForSubmission + timeLastSubmit - millis())/1000))+ " ";
  }
  lcd.setCursor(1,1);lcd.print("Location #" + String(locationTagNum) + " " + timeOutTxt);
  delay(5);  
  
}
void scan() {
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("scan start");
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Scanning...");
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("Scan complete.");
  lcd.setCursor(1,0);lcd.print("Scan done.");
  if (n == 0) {
      Serial.println("No networks found");
      lcd.setCursor(1,0);lcd.print("No networks!");
      delay(1000);
  } else {
    Serial.print(n);
    Serial.println(" Networks found");
    connectToWifi();
    for (int i = 0; i < n; ++i) {
      // Submit SSID and RSSI data for each network found
      sendData(WiFi.SSID(i),  WiFi.RSSI(i));
      
      // Print and Display SSID and RSSI for each network found
      char buffer[16];
      char subbuff[7];
      memcpy( subbuff, &WiFi.SSID(i)[0], 5 );
      subbuff[6] = '\0';
      sprintf(buffer, "%d:%6s (%d)", i+1, subbuff, WiFi.RSSI(i));
      lcd.setCursor(0,1);lcd.print(buffer);
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      
      delay(200);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(100);
}
void sendData(String ssid, int rssi)
{
  
  //////////////////////////////
  // Record only one network. 
  // If there's no limit on data submission interval, remove these lines and send all wifi networks data 
  // (thingspeak.com accepts only one message every 15 seconds, so I scan a single network)
  if (ssid != NETWORK_OF_INTEREST) 
    return;
  //////////////////////////////
  Serial.print("sending data: ");
  Serial.print(ssid);
  Serial.print(", ");
  Serial.println(rssi);


  Serial.print("Connecting to ");
  //ssid srri location version
  String request_uri = "https://" + uri + "&field1="+ssid+"&field2=" + String(rssi) + "&field3=" + String(locationTagNum) + "&field4=" + String(data_version);
  Serial.println(request_uri);

  HTTPClient client;
  client.begin(request_uri, root_ca);
  int httpCode = client.GET();                                                  //Make the request

  if (httpCode > 0) { //Check for the returning code
    String payload = client.getString();
    Serial.println(httpCode);
    Serial.println(payload);
  }

  else {
    Serial.println("Error on HTTP request ");
  }

  Serial.println("Closing connection.");
  client.end(); //Free the resources
  timeLastSubmit = millis();
  lcd.setCursor(1,0);lcd.print("Ready.        ");
  
}
void connectToWifi(){
  WiFiManager wifiManager;
  wifiManager.autoConnect("WiFiScanner");
  Serial.println("Connected to wifi");
  lcd.setCursor(1,0);lcd.print("Connected.      ");
}
