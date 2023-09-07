# 1 "C:\\Users\\Administrator\\OneDrive\\Documents\\Arduino\\E32\\esp32_e32_web_manager_gateway\\esp32_e32_web_manager_gateway.ino"
/*
 * LoRa EByte E32
 * EByte LoRa E32 Manager for esp32.
 *
 * Created by Renzo Mischianti <renzo.mischianti@gmail.com>
 * License: CC BY-NC-ND 3.0
 *
 * https://www.mischianti.org
 *
 * E32		  ----- esp32
 * M0         ----- 19 (or GND)
 * M1         ----- 21 (or 3.3v)
 * RX         ----- TX2 (PullUP)
 * TX         ----- RX2 (PullUP)
 * AUX        ----- 18  (PullUP)
 * VCC        ----- 3.3v/5v
 * GND        ----- GND
 *
 */



# 24 "C:\\Users\\Administrator\\OneDrive\\Documents\\Arduino\\E32\\esp32_e32_web_manager_gateway\\esp32_e32_web_manager_gateway.ino" 2
# 25 "C:\\Users\\Administrator\\OneDrive\\Documents\\Arduino\\E32\\esp32_e32_web_manager_gateway\\esp32_e32_web_manager_gateway.ino" 2
# 26 "C:\\Users\\Administrator\\OneDrive\\Documents\\Arduino\\E32\\esp32_e32_web_manager_gateway\\esp32_e32_web_manager_gateway.ino" 2
# 27 "C:\\Users\\Administrator\\OneDrive\\Documents\\Arduino\\E32\\esp32_e32_web_manager_gateway\\esp32_e32_web_manager_gateway.ino" 2
# 28 "C:\\Users\\Administrator\\OneDrive\\Documents\\Arduino\\E32\\esp32_e32_web_manager_gateway\\esp32_e32_web_manager_gateway.ino" 2

# 30 "C:\\Users\\Administrator\\OneDrive\\Documents\\Arduino\\E32\\esp32_e32_web_manager_gateway\\esp32_e32_web_manager_gateway.ino" 2

# 32 "C:\\Users\\Administrator\\OneDrive\\Documents\\Arduino\\E32\\esp32_e32_web_manager_gateway\\esp32_e32_web_manager_gateway.ino" 2
# 33 "C:\\Users\\Administrator\\OneDrive\\Documents\\Arduino\\E32\\esp32_e32_web_manager_gateway\\esp32_e32_web_manager_gateway.ino" 2
# 34 "C:\\Users\\Administrator\\OneDrive\\Documents\\Arduino\\E32\\esp32_e32_web_manager_gateway\\esp32_e32_web_manager_gateway.ino" 2

# 36 "C:\\Users\\Administrator\\OneDrive\\Documents\\Arduino\\E32\\esp32_e32_web_manager_gateway\\esp32_e32_web_manager_gateway.ino" 2
# 45 "C:\\Users\\Administrator\\OneDrive\\Documents\\Arduino\\E32\\esp32_e32_web_manager_gateway\\esp32_e32_web_manager_gateway.ino"
// Uncomment to enable printing out nice debug messages.


// Define where debug output will be printed.


// Setup debug printing macros.
# 63 "C:\\Users\\Administrator\\OneDrive\\Documents\\Arduino\\E32\\esp32_e32_web_manager_gateway\\esp32_e32_web_manager_gateway.ino"
WebServer httpRestServer(8080);


WebServer httpServer(80);




WebSocketsServer webSocket = WebSocketsServer(8081);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);


// ---------- esp8266 pins --------------
//LoRa_E22 e32ttl(RX, TX, AUX, M0, M1);  // Arduino RX <-- e22 TX, Arduino TX --> e22 RX
//LoRa_E32 e32ttl(D3, D4, D5, D7, D6); // Arduino RX <-- e22 TX, Arduino TX --> e22 RX AUX M0 M1
//LoRa_E22 e22ttl(D2, D3); // Config without connect AUX and M0 M1
//LoRa_E32 e32ttl(&Serial, D3, D0, D8); // RX, TX
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(D2, D3); // Arduino RX <-- e22 TX, Arduino TX --> e22 RX
//LoRa_E22 e22ttl(&mySerial, D5, D7, D6); // AUX M0 M1
// -------------------------------------

// ---------- Arduino pins --------------
//LoRa_E22 e22ttl(4, 5, 3, 7, 6); // Arduino RX <-- e22 TX, Arduino TX --> e22 RX AUX M0 M1
//LoRa_E22 e32ttl(4, 5); // Config without connect AUX and M0 M1

//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(4, 5); // Arduino RX <-- e22 TX, Arduino TX --> e22 RX
//LoRa_E22 e22ttl(&mySerial, 3, 7, 6); // AUX M0 M1
// -------------------------------------

// ---------- esp32 pins --------------
////LoRa_E32 e32ttl(&Serial2, 15, 21, 19); //  RX AUX M0 M1
HardwareSerial* mySerial = new HardwareSerial(2); // Arduino RX2 <-- e32 TX, Arduino TX2 --> e32 RX
LoRa_E32 e32ttl(mySerial);

//LoRa_E22 e32ttl(&Serial2, 22, 4, 18, 21, 19, UART_BPS_RATE_9600); //  esp32 RX <-- e22 TX, esp32 TX --> e22 RX AUX M0 M1
// -------------------------------------

void sendCrossOriginHeader();
void printParameters(struct Configuration configuration);
void printModuleInformation(struct ModuleInformation moduleInformation);

void sendTransparentMessage();
void sendFixedMessage();
void sendBroadcastMessage();
void resetModule();
void resetMicrocontroller();
void serverRouting();
void realtimeDataCallbak();

void sendWSMessageOfMessageReceived(bool readSingleMessage);

const char* ssid = "RESEAUXBC"; // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "Gxy2b3jcbc#z?"; // The password of the Wi-Fi network

// Config for SoftAccessPoint
const char* hostname = "e32dev01";







bool readSingleMessage = true;
bool isConnectedWebSocket = false;
bool isConnectedWebSocketAck = false;

void setup() {
 Serial.begin(9600);
 delay(500);


 WiFi.begin(ssid, password); // Connect to the network
   { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Connecting to "))))); };
   { Serial.print(ssid); }; { Serial.println(" ..."); };

   int i = 0;
   while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
     delay(1000);
     { Serial.print(++i); }; { Serial.print(' '); };
   }

   { Serial.println('\n'); };
   { Serial.println("Connection established!"); };
   { Serial.print("IP address:\t"); };
   { Serial.println(WiFi.localIP()); }; // Send the IP address of the ESP8266 to the computer
# 180 "C:\\Users\\Administrator\\OneDrive\\Documents\\Arduino\\E32\\esp32_e32_web_manager_gateway\\esp32_e32_web_manager_gateway.ino"
     if (!MDNS.begin(hostname)) { // Start the mDNS responder for esp8266.local
      { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("Error setting up mDNS responder!"))))); };
     }
     { Serial.print(hostname); };
     { Serial.println(((reinterpret_cast<const __FlashStringHelper *>((" --> mDNS responder started"))))); };
        // Add service to MDNS-SD
        MDNS.addService("http", "tcp", 80);

 // Startup all pins and UART
 e32ttl.begin();

 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("Inizializing FS..."))))); };
 if (SPIFFS.begin()){
  { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("done."))))); };
 }else{
  { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("fail."))))); };
 }


 restServerRouting();
 httpRestServer.begin();
 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("REST Server Started"))))); };

 serverRouting();
 httpServer.begin();
 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("Web Server Started"))))); };

 webSocket.begin();
 webSocket.onEvent(webSocketEvent);
 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("WS Server Started"))))); };


 pinMode(GPIO_NUM_33, 0x03);
 digitalWrite(GPIO_NUM_33, 0x1);

}


unsigned long lastRead = millis();

void loop() {
 if (isConnectedWebSocket){
  if (isConnectedWebSocketAck && e32ttl.available()>1){
   sendWSMessageOfMessageReceived(readSingleMessage);
  }
  if (lastRead+10000<millis()){
   realtimeDataCallbak();
   lastRead = millis();
  }

 }

 httpRestServer.handleClient();
 httpServer.handleClient();

 webSocket.loop();




}

void restServerRouting() {
//	httpRestServer.header("Access-Control-Allow-Headers: Authorization, Content-Type");
//
    httpRestServer.on(((reinterpret_cast<const __FlashStringHelper *>(("/")))), HTTP_GET, []() {
     { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("CALL /"))))); };
     httpRestServer.send(200, ((reinterpret_cast<const __FlashStringHelper *>(("text/html")))),
            ((reinterpret_cast<const __FlashStringHelper *>(("Welcome to the Inverter Centraline REST Web Server")))));
    });
    httpRestServer.on(((reinterpret_cast<const __FlashStringHelper *>(("/configuration")))), HTTP_GET, getConfiguration);
    httpRestServer.on(((reinterpret_cast<const __FlashStringHelper *>(("/configuration")))), HTTP_POST, postConfiguration);
    httpRestServer.on(((reinterpret_cast<const __FlashStringHelper *>(("/configuration")))), HTTP_OPTIONS, sendCrossOriginHeader);

    httpRestServer.on(((reinterpret_cast<const __FlashStringHelper *>(("/reset")))), HTTP_GET, resetMicrocontroller);

    httpRestServer.on(((reinterpret_cast<const __FlashStringHelper *>(("/resetModule")))), HTTP_GET, resetModule);
    httpRestServer.on(((reinterpret_cast<const __FlashStringHelper *>(("/moduleInfo")))), HTTP_GET, getModuleInfo);

    httpRestServer.on(((reinterpret_cast<const __FlashStringHelper *>(("/transparentMessage")))), HTTP_POST, sendTransparentMessage);
    httpRestServer.on(((reinterpret_cast<const __FlashStringHelper *>(("/transparentMessage")))), HTTP_OPTIONS, sendCrossOriginHeader);

    httpRestServer.on(((reinterpret_cast<const __FlashStringHelper *>(("/fixedMessage")))), HTTP_POST, sendFixedMessage);
    httpRestServer.on(((reinterpret_cast<const __FlashStringHelper *>(("/fixedMessage")))), HTTP_OPTIONS, sendCrossOriginHeader);

    httpRestServer.on(((reinterpret_cast<const __FlashStringHelper *>(("/broadcastMessage")))), HTTP_POST, sendBroadcastMessage);
    httpRestServer.on(((reinterpret_cast<const __FlashStringHelper *>(("/broadcastMessage")))), HTTP_OPTIONS, sendCrossOriginHeader);


}

void setCrossOrigin(){
 httpRestServer.sendHeader(((reinterpret_cast<const __FlashStringHelper *>(("Access-Control-Allow-Origin")))), ((reinterpret_cast<const __FlashStringHelper *>(("*")))));
 httpRestServer.sendHeader(((reinterpret_cast<const __FlashStringHelper *>(("Access-Control-Max-Age")))), ((reinterpret_cast<const __FlashStringHelper *>(("600")))));
 httpRestServer.sendHeader(((reinterpret_cast<const __FlashStringHelper *>(("Access-Control-Allow-Methods")))), ((reinterpret_cast<const __FlashStringHelper *>(("PUT,POST,GET,OPTIONS")))));
 httpRestServer.sendHeader(((reinterpret_cast<const __FlashStringHelper *>(("Access-Control-Allow-Headers")))), ((reinterpret_cast<const __FlashStringHelper *>(("*")))));
};
void sendCrossOriginHeader(){
 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("sendCORSHeader"))))); };

 httpRestServer.sendHeader(((reinterpret_cast<const __FlashStringHelper *>(("access-control-allow-credentials")))), ((reinterpret_cast<const __FlashStringHelper *>(("false")))));

 setCrossOrigin();

 httpRestServer.send(204);
}


void getConfigurationJSON(Configuration configuration){

 DynamicJsonDocument doc(2048);
 JsonObject rootObj = doc.to<JsonObject>();

 JsonObject config = rootObj.createNestedObject("configuration");

 config[((reinterpret_cast<const __FlashStringHelper *>(("ADDH"))))] = configuration.ADDH;
 config[((reinterpret_cast<const __FlashStringHelper *>(("ADDL"))))] = configuration.ADDL;
 config[((reinterpret_cast<const __FlashStringHelper *>(("CHAN"))))] = configuration.CHAN;

 JsonObject option = config.createNestedObject("OPTION");

 option[((reinterpret_cast<const __FlashStringHelper *>(("fec"))))] = configuration.OPTION.fec;
 option[((reinterpret_cast<const __FlashStringHelper *>(("fixedTransmission"))))] = configuration.OPTION.fixedTransmission;
 option[((reinterpret_cast<const __FlashStringHelper *>(("ioDriveMode"))))] = configuration.OPTION.ioDriveMode;
 option[((reinterpret_cast<const __FlashStringHelper *>(("transmissionPower"))))] = configuration.OPTION.transmissionPower;
 option[((reinterpret_cast<const __FlashStringHelper *>(("wirelessWakeupTime"))))] = configuration.OPTION.wirelessWakeupTime;

 JsonObject speed = config.createNestedObject("SPED");

 speed[((reinterpret_cast<const __FlashStringHelper *>(("airDataRate"))))] = configuration.SPED.airDataRate;
 speed[((reinterpret_cast<const __FlashStringHelper *>(("uartBaudRate"))))] = configuration.SPED.uartBaudRate;
 speed[((reinterpret_cast<const __FlashStringHelper *>(("uartParity"))))] = configuration.SPED.uartParity;

 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Stream file..."))))); };
 String buf;
 serializeJson(rootObj, buf);
 httpRestServer.send(200, ((reinterpret_cast<const __FlashStringHelper *>(("application/json")))), buf);
 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("done."))))); };
}

void getConfiguration() {
 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("------------------------------ getConfiguration ------------------------------"))))); };

 setCrossOrigin();

 ResponseStructContainer c;
 c = e32ttl.getConfiguration();
 // It's important get configuration pointer before all other operation
 Configuration configuration = *(Configuration*) c.data;
 { Serial.println(c.status.getResponseDescription()); };
 { Serial.println(c.status.code); };

 printParameters(configuration);

 getConfigurationJSON(configuration);

 c.close();
}
void getModuleInfo() {
 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("------------------------------ getModuleInfo ------------------------------"))))); };

 setCrossOrigin();

 ResponseStructContainer c;
 c = e32ttl.getModuleInformation();
 // It's important get configuration pointer before all other operation
 ModuleInformation moduleInformation = *(ModuleInformation*) c.data;
 { Serial.println(c.status.getResponseDescription()); };
 { Serial.println(c.status.code); };

 printModuleInformation(moduleInformation);

 DynamicJsonDocument doc(2048);
 JsonObject rootObj = doc.to<JsonObject>();

 JsonObject config = rootObj.createNestedObject("moduleInfo");

 config[((reinterpret_cast<const __FlashStringHelper *>(("frequency"))))] = String(moduleInformation.frequency, 16);
 config[((reinterpret_cast<const __FlashStringHelper *>(("version"))))] = String(moduleInformation.version, 16);
 config[((reinterpret_cast<const __FlashStringHelper *>(("features"))))] = String(moduleInformation.features, 16);

 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Stream file..."))))); };
 String buf;
 serializeJson(rootObj, buf);
 httpRestServer.send(200, ((reinterpret_cast<const __FlashStringHelper *>(("application/json")))), buf);
 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("done."))))); };

 c.close();
}

void resetMicrocontroller() {

 digitalWrite(GPIO_NUM_33, 0x0);

}

void resetModule() {
 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("------------------------------ resetModule ------------------------------"))))); };

 setCrossOrigin();

 ResponseStatus rs;
 rs = e32ttl.resetModule();
 // It's important get configuration pointer before all other operation
 { Serial.println(rs.getResponseDescription()); };
 { Serial.println(rs.code); };

 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("==> reset END"))))); };

 DynamicJsonDocument doc(512);

 if (rs.code != E32_SUCCESS) {
     { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("fail."))))); };
     httpRestServer.send(400, ((reinterpret_cast<const __FlashStringHelper *>(("text/html")))), String(rs.getResponseDescription()));
 }else{
  JsonObject rootObj = doc.to<JsonObject>();

  JsonObject status = rootObj.createNestedObject("status");

  status[((reinterpret_cast<const __FlashStringHelper *>(("code"))))] = String(rs.code);
  status[((reinterpret_cast<const __FlashStringHelper *>(("error"))))] = rs.code!=1;
  status[((reinterpret_cast<const __FlashStringHelper *>(("description"))))] = rs.getResponseDescription();

  { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Stream file..."))))); };
  String buf;
  serializeJson(rootObj, buf);
  if (rs.code!=1){
   httpRestServer.send(500, ((reinterpret_cast<const __FlashStringHelper *>(("application/json")))), buf);
  }else{
   httpRestServer.send(200, ((reinterpret_cast<const __FlashStringHelper *>(("application/json")))), buf);
  }
  { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("done."))))); };

 }
}

void postConfiguration() {
 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("postConfigFile"))))); };

 setCrossOrigin();

    String postBody = httpRestServer.arg("plain");
    { Serial.println(postBody); };

 DynamicJsonDocument doc(2048);
 DeserializationError error = deserializeJson(doc, postBody);
 if (error) {
  // if the file didn't open, print an error:
  { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Error parsing JSON "))))); };
  { Serial.println(error.c_str()); };

  String msg = error.c_str();

  httpRestServer.send(400, ((reinterpret_cast<const __FlashStringHelper *>(("text/html")))), "Error in parsin json body! <br>"+msg);

 }else{
  JsonObject postObj = doc.as<JsonObject>();

  { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("HTTP Method: "))))); };
  { Serial.println(httpRestServer.method()); };

        if (httpRestServer.method() == HTTP_POST) {
            if (postObj.containsKey("ADDH") && postObj.containsKey("ADDL") && postObj.containsKey("CHAN") && postObj.containsKey("OPTION")&& postObj.containsKey("SPED") ) {

             { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("==> Set config file..."))))); };

//            	JsonObject config = postObj[F("configuration")];

             { Serial.print("ADDH"); };
             { Serial.println((byte)postObj[((reinterpret_cast<const __FlashStringHelper *>(("ADDH"))))]); };


             Configuration configuration;
             configuration.ADDH = (byte)postObj[((reinterpret_cast<const __FlashStringHelper *>(("ADDH"))))];
             configuration.ADDL = (byte)postObj[((reinterpret_cast<const __FlashStringHelper *>(("ADDL"))))];
             configuration.CHAN = (byte)postObj[((reinterpret_cast<const __FlashStringHelper *>(("CHAN"))))];

    JsonObject option = postObj[((reinterpret_cast<const __FlashStringHelper *>(("OPTION"))))];

             { Serial.print("fec"); };
             { Serial.println((FORWARD_ERROR_CORRECTION_SWITCH)(byte)option[((reinterpret_cast<const __FlashStringHelper *>(("fec"))))]); };

//				byte optionByte = 0b00000000;
//				bitWrite(bitRead((byte)option[F("fec")], 0))

    configuration.OPTION.fec = (FORWARD_ERROR_CORRECTION_SWITCH)(byte)option[((reinterpret_cast<const __FlashStringHelper *>(("fec"))))];
    configuration.OPTION.fixedTransmission = (FIDEX_TRANSMISSION)(byte)option[((reinterpret_cast<const __FlashStringHelper *>(("fixedTransmission"))))];
    configuration.OPTION.ioDriveMode = (IO_DRIVE_MODE)(byte)option[((reinterpret_cast<const __FlashStringHelper *>(("ioDriveMode"))))];
    configuration.OPTION.transmissionPower = (TRANSMISSION_POWER)(byte)option[((reinterpret_cast<const __FlashStringHelper *>(("transmissionPower"))))];
    configuration.OPTION.wirelessWakeupTime = (WIRELESS_WAKE_UP_TIME)(byte)option[((reinterpret_cast<const __FlashStringHelper *>(("wirelessWakeupTime"))))];

    JsonObject speed = postObj[((reinterpret_cast<const __FlashStringHelper *>(("SPED"))))];

    configuration.SPED.airDataRate = (AIR_DATA_RATE)(byte)speed[((reinterpret_cast<const __FlashStringHelper *>(("airDataRate"))))];
    configuration.SPED.uartBaudRate = (UART_BPS_TYPE)(byte)speed[((reinterpret_cast<const __FlashStringHelper *>(("uartBaudRate"))))];
    configuration.SPED.uartParity = (E32_UART_PARITY)(byte)speed[((reinterpret_cast<const __FlashStringHelper *>(("uartParity"))))];




//            	configuration.ADDH = atoi(config[F("ADDH")]);
//            	configuration.ADDL = atoi(config[F("ADDL")]);
//            	configuration.CHAN = atoi(config[F("CHAN")]);
//
//				JsonObject option = config[F("OPTION")];
//
//				configuration.OPTION.fec = atoi(option[F("fec")]);
//				configuration.OPTION.fixedTransmission = atoi(option[F("fixedTransmission")]);
//				configuration.OPTION.ioDriveMode = atoi(option[F("ioDriveMode")]);
//				configuration.OPTION.transmissionPower = atoi(option[F("transmissionPower")]);
//				configuration.OPTION.wirelessWakeupTime = atoi(option[F("wirelessWakeupTime")]);
//
//				JsonObject speed = config[F("SPED")];
//
//				configuration.SPED.airDataRate = atoi(speed[F("airDataRate")]);
//				configuration.SPED.uartBaudRate = atoi(speed[F("uartBaudRate")]);
//				configuration.SPED.uartParity = atoi(speed[F("uartParity")]);




    printParameters(configuration);

    ResponseStatus rs = e32ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
    { Serial.println(rs.getResponseDescription()); };
    { Serial.println(rs.code); };
    printParameters(configuration);
             { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("==> Set config file END"))))); };


             if (rs.code != E32_SUCCESS) {
                 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("fail."))))); };
                 httpRestServer.send(400, ((reinterpret_cast<const __FlashStringHelper *>(("text/html")))), String(rs.getResponseDescription()));
             }else{


//            		DEBUG_PRINT(F("Stream file..."));
//            		String buf;
//            		serializeJson(rootObj, buf);
//            		httpRestServer.send(200, F("application/json"), buf);
//            		DEBUG_PRINTLN(F("done."));
              ResponseStructContainer c;
              c = e32ttl.getConfiguration();
              // It's important get configuration pointer before all other operation
              Configuration configurationRetrieve = *(Configuration*) c.data;
              { Serial.println(c.status.getResponseDescription()); };
              { Serial.println(c.status.code); };

              printParameters(configuration);

              if (
                configuration.ADDH==configurationRetrieve.ADDH &&
                configuration.ADDL==configurationRetrieve.ADDL &&
                configuration.CHAN==configurationRetrieve.CHAN ){

                  getConfigurationJSON(configurationRetrieve);

                  delay(1000);

                  resetMicrocontroller();
     }else{
      httpRestServer.send(500, ((reinterpret_cast<const __FlashStringHelper *>(("text/html")))), ((reinterpret_cast<const __FlashStringHelper *>(("Problem on store data on device, try to reset!")))));
     }

              c.close();
             }
            }
            else {
             { Serial.println("Configuration not present"); }
             httpRestServer.send(204, ((reinterpret_cast<const __FlashStringHelper *>(("text/html")))), ((reinterpret_cast<const __FlashStringHelper *>(("No data found, or incorrect!")))));
            }
        }
    }
}

void sendTransparentMessage() {
 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("sendTransparentMessage"))))); };

 setCrossOrigin();

    String postBody = httpRestServer.arg("plain");
    { Serial.println(postBody); };

 DynamicJsonDocument doc(2048);
 DeserializationError error = deserializeJson(doc, postBody);
 if (error) {
  // if the file didn't open, print an error:
  { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Error parsing JSON "))))); };
  { Serial.println(error.c_str()); };

  String msg = error.c_str();

  httpRestServer.send(400, ((reinterpret_cast<const __FlashStringHelper *>(("text/html")))), "Error in parsin json body! <br>"+msg);

 }else{
  JsonObject postObj = doc.as<JsonObject>();

  { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("HTTP Method: "))))); };
  { Serial.println(httpRestServer.method()); };

        if (httpRestServer.method() == HTTP_POST) {
            if (postObj.containsKey("message") ) {

             { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("==> readMessage..."))))); };

//            	JsonObject config = postObj[F("configuration")];

             String message = postObj[((reinterpret_cast<const __FlashStringHelper *>(("message"))))];

    ResponseStatus rs = e32ttl.sendMessage(message+'\0');
    { Serial.println(rs.getResponseDescription()); };
    { Serial.println(rs.code); };
             { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("==> Send message END"))))); };


             if (rs.code != E32_SUCCESS) {
                 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("fail."))))); };
                 httpRestServer.send(400, ((reinterpret_cast<const __FlashStringHelper *>(("text/html")))), String(rs.getResponseDescription()));
             }else{
              JsonObject rootObj = doc.to<JsonObject>();

              JsonObject status = rootObj.createNestedObject("status");

              status[((reinterpret_cast<const __FlashStringHelper *>(("code"))))] = String(rs.code);
              status[((reinterpret_cast<const __FlashStringHelper *>(("error"))))] = rs.code!=1;
              status[((reinterpret_cast<const __FlashStringHelper *>(("description"))))] = rs.getResponseDescription();

              { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Stream file..."))))); };
              String buf;
              serializeJson(rootObj, buf);
              if (rs.code!=1){
               httpRestServer.send(500, ((reinterpret_cast<const __FlashStringHelper *>(("application/json")))), buf);
              }else{
               httpRestServer.send(200, ((reinterpret_cast<const __FlashStringHelper *>(("application/json")))), buf);
              }
              { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("done."))))); };

             }
            }
            else {
             { Serial.println("Configuration not present"); }
             httpRestServer.send(204, ((reinterpret_cast<const __FlashStringHelper *>(("text/html")))), ((reinterpret_cast<const __FlashStringHelper *>(("No data found, or incorrect!")))));
            }
        }
    }
}

void sendFixedMessage() {
 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("sendFixedMessage"))))); };

 setCrossOrigin();

    String postBody = httpRestServer.arg("plain");
    { Serial.println(postBody); };

 DynamicJsonDocument doc(2048);
 DeserializationError error = deserializeJson(doc, postBody);
 if (error) {
  // if the file didn't open, print an error:
  { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Error parsing JSON "))))); };
  { Serial.println(error.c_str()); };

  String msg = error.c_str();

  httpRestServer.send(400, ((reinterpret_cast<const __FlashStringHelper *>(("text/html")))), "Error in parsin json body! <br>"+msg);

 }else{
  JsonObject postObj = doc.as<JsonObject>();

  { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("HTTP Method: "))))); };
  { Serial.println(httpRestServer.method()); };

        if (httpRestServer.method() == HTTP_POST) {
            if (postObj.containsKey("message") && postObj.containsKey("CHAN") && postObj.containsKey("ADDL") && postObj.containsKey("ADDH")) {

             { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("==> readMessage..."))))); };

//            	JsonObject config = postObj[F("configuration")];

             String message = postObj[((reinterpret_cast<const __FlashStringHelper *>(("message"))))];
             byte CHAN = (byte)postObj[((reinterpret_cast<const __FlashStringHelper *>(("CHAN"))))];
             byte ADDL = (byte)postObj[((reinterpret_cast<const __FlashStringHelper *>(("ADDL"))))];
             byte ADDH = (byte)postObj[((reinterpret_cast<const __FlashStringHelper *>(("ADDH"))))];

    ResponseStatus rs = e32ttl.sendFixedMessage(ADDH, ADDL, CHAN, message+'\0');
    { Serial.println(rs.getResponseDescription()); };
    { Serial.println(rs.code); };
             { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("==> Send message END"))))); };


             if (rs.code != E32_SUCCESS) {
                 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("fail."))))); };
                 httpRestServer.send(400, ((reinterpret_cast<const __FlashStringHelper *>(("text/html")))), String(rs.getResponseDescription()));
             }else{
              JsonObject rootObj = doc.to<JsonObject>();

              JsonObject status = rootObj.createNestedObject("status");

              status[((reinterpret_cast<const __FlashStringHelper *>(("code"))))] = String(rs.code);
              status[((reinterpret_cast<const __FlashStringHelper *>(("error"))))] = rs.code!=1;
              status[((reinterpret_cast<const __FlashStringHelper *>(("description"))))] = rs.getResponseDescription();

              { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Stream file..."))))); };
              String buf;
              serializeJson(rootObj, buf);
              if (rs.code!=1){
               httpRestServer.send(500, ((reinterpret_cast<const __FlashStringHelper *>(("application/json")))), buf);
              }else{
               httpRestServer.send(200, ((reinterpret_cast<const __FlashStringHelper *>(("application/json")))), buf);
              }
              { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("done."))))); };

             }
            }
            else {
             { Serial.println("Configuration not present"); }
             httpRestServer.send(204, ((reinterpret_cast<const __FlashStringHelper *>(("text/html")))), ((reinterpret_cast<const __FlashStringHelper *>(("No data found, or incorrect!")))));
            }
        }
    }
}

void sendBroadcastMessage() {
 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("sendBroadcastMessage"))))); };

 setCrossOrigin();

    String postBody = httpRestServer.arg("plain");
    { Serial.println(postBody); };

 DynamicJsonDocument doc(2048);
 DeserializationError error = deserializeJson(doc, postBody);
 if (error) {
  // if the file didn't open, print an error:
  { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Error parsing JSON "))))); };
  { Serial.println(error.c_str()); };

  String msg = error.c_str();

  httpRestServer.send(400, ((reinterpret_cast<const __FlashStringHelper *>(("text/html")))), "Error in parsin json body! <br>"+msg);

 }else{
  JsonObject postObj = doc.as<JsonObject>();

  { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("HTTP Method: "))))); };
  { Serial.println(httpRestServer.method()); };

        if (httpRestServer.method() == HTTP_POST) {
            if (postObj.containsKey("message") && postObj.containsKey("CHAN")) {

             { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("==> readMessage..."))))); };

//            	JsonObject config = postObj[F("configuration")];

             String message = postObj[((reinterpret_cast<const __FlashStringHelper *>(("message"))))];
             byte CHAN = (byte)postObj[((reinterpret_cast<const __FlashStringHelper *>(("CHAN"))))];

    ResponseStatus rs = e32ttl.sendBroadcastFixedMessage(CHAN, message+'\0');
    { Serial.println(rs.getResponseDescription()); };
    { Serial.println(rs.code); };
             { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("==> Send message END"))))); };


             if (rs.code != E32_SUCCESS) {
                 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("fail."))))); };
                 httpRestServer.send(400, ((reinterpret_cast<const __FlashStringHelper *>(("text/html")))), String(rs.getResponseDescription()));
             }else{
              JsonObject rootObj = doc.to<JsonObject>();

              JsonObject status = rootObj.createNestedObject("status");

              status[((reinterpret_cast<const __FlashStringHelper *>(("code"))))] = String(rs.code);
              status[((reinterpret_cast<const __FlashStringHelper *>(("error"))))] = rs.code!=1;
              status[((reinterpret_cast<const __FlashStringHelper *>(("description"))))] = rs.getResponseDescription();

              { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Stream file..."))))); };
              String buf;
              serializeJson(rootObj, buf);
              if (rs.code!=1){
               httpRestServer.send(500, ((reinterpret_cast<const __FlashStringHelper *>(("application/json")))), buf);
              }else{
               httpRestServer.send(200, ((reinterpret_cast<const __FlashStringHelper *>(("application/json")))), buf);
              }
              { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("done."))))); };

             }
            }
            else {
             { Serial.println("Configuration not present"); }
             httpRestServer.send(204, ((reinterpret_cast<const __FlashStringHelper *>(("text/html")))), ((reinterpret_cast<const __FlashStringHelper *>(("No data found, or incorrect!")))));
            }
        }
    }
}

void printParameters(struct Configuration configuration) {
 { Serial.println("----------------------------------------"); };

 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("HEAD BIN: "))))); }; { Serial.print(configuration.HEAD, 2); };{ Serial.print(" "); };{ Serial.print(configuration.HEAD, 10); };{ Serial.print(" "); };{ Serial.println(configuration.HEAD, 16); };
 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>((" "))))); };
 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("AddH BIN: "))))); }; { Serial.println(configuration.ADDH, 10); };
 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("AddL BIN: "))))); }; { Serial.println(configuration.ADDL, 10); };
 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Chan BIN: "))))); }; { Serial.print(configuration.CHAN, 10); }; { Serial.print(" -> "); }; { Serial.println(configuration.getChannelDescription()); };
 { Serial.println(((reinterpret_cast<const __FlashStringHelper *>((" "))))); };
 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("SpeedParityBit BIN    : "))))); }; { Serial.print(configuration.SPED.uartParity, 2); };{ Serial.print(" -> "); }; { Serial.println(configuration.SPED.getUARTParityDescription()); };
 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("SpeedUARTDataRate BIN : "))))); }; { Serial.print(configuration.SPED.uartBaudRate, 2); };{ Serial.print(" -> "); }; { Serial.println(configuration.SPED.getUARTBaudRate()); };
 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("SpeedAirDataRate BIN  : "))))); }; { Serial.print(configuration.SPED.airDataRate, 2); };{ Serial.print(" -> "); }; { Serial.println(configuration.SPED.getAirDataRate()); };

 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("OptionTrans BIN       : "))))); }; { Serial.print(configuration.OPTION.fixedTransmission, 2); };{ Serial.print(" -> "); }; { Serial.println(configuration.OPTION.getFixedTransmissionDescription()); };
 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("OptionPullup BIN      : "))))); }; { Serial.print(configuration.OPTION.ioDriveMode, 2); };{ Serial.print(" -> "); }; { Serial.println(configuration.OPTION.getIODroveModeDescription()); };
 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("OptionWakeup BIN      : "))))); }; { Serial.print(configuration.OPTION.wirelessWakeupTime, 2); };{ Serial.print(" -> "); }; { Serial.println(configuration.OPTION.getWirelessWakeUPTimeDescription()); };
 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("OptionFEC BIN         : "))))); }; { Serial.print(configuration.OPTION.fec, 2); };{ Serial.print(" -> "); }; { Serial.println(configuration.OPTION.getFECDescription()); };
 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("OptionPower BIN       : "))))); }; { Serial.print(configuration.OPTION.transmissionPower, 2); };{ Serial.print(" -> "); }; { Serial.println(configuration.OPTION.getTransmissionPowerDescription()); };

 { Serial.println("----------------------------------------"); };

}
void printModuleInformation(struct ModuleInformation moduleInformation) {
 { Serial.println("----------------------------------------"); };
 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("HEAD BIN: "))))); }; { Serial.print(moduleInformation.HEAD, 2); };{ Serial.print(" "); };{ Serial.print(moduleInformation.HEAD, 10); };{ Serial.print(" "); };{ Serial.println(moduleInformation.HEAD, 16); };

 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Frequency.: "))))); }; { Serial.println(moduleInformation.frequency, 16); };
 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Version  : "))))); }; { Serial.println(moduleInformation.version, 16); };
 { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Features : "))))); }; { Serial.println(moduleInformation.features, 16); };
 { Serial.println("----------------------------------------"); };
}

void sendWSMessageOfMessageReceived(bool readSingleMessage){
 DynamicJsonDocument docws(512);
 JsonObject objws = docws.to<JsonObject>();

  ResponseContainer rs;
  if (readSingleMessage){
   { Serial.println("READ MESSAGE UNTIL!"); };
   rs = e32ttl.receiveMessageUntil();
  }else{
   { Serial.println("READ MESSAGE!"); };
   rs = e32ttl.receiveMessage();
  }


  objws[((reinterpret_cast<const __FlashStringHelper *>(("type"))))] = "message";
  objws[((reinterpret_cast<const __FlashStringHelper *>(("code"))))] = rs.status.code;
  objws[((reinterpret_cast<const __FlashStringHelper *>(("description"))))] = rs.status.getResponseDescription();

  { Serial.println(rs.status.getResponseDescription()); };

  if (rs.status.code==1){
   String message = rs.data;

   objws[((reinterpret_cast<const __FlashStringHelper *>(("message"))))] = message;
   { Serial.println(message); };

   objws[((reinterpret_cast<const __FlashStringHelper *>(("error"))))] = false;
  }else{
   objws[((reinterpret_cast<const __FlashStringHelper *>(("error"))))] = true;
  }

 String buf;
 serializeJson(objws, buf);

 webSocket.broadcastTXT(buf);

}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
 DynamicJsonDocument docwsT(512);
 DeserializationError error;

    switch(type) {
        case WStype_DISCONNECTED:
   webSocket.sendTXT(num, "{\"connection\": false}");

            { Serial.print(((reinterpret_cast<const __FlashStringHelper *>((" Disconnected "))))); };
            { Serial.println(num, 10); };

            isConnectedWebSocket = false;
            isConnectedWebSocketAck = false;
//            DEBUG_PRINTF_AI(F("[%u] Disconnected!\n"), num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
//                DEBUG_PRINTF_AI(F("[%u] Connected from %d.%d.%d.%d url: %s\n"), num, ip[0], ip[1], ip[2], ip[3], payload);

                { Serial.print(num); };
                { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Connected from: "))))); };
                { Serial.print(ip.toString()); };
                { Serial.print(((reinterpret_cast<const __FlashStringHelper *>((" "))))); };
                { Serial.println((char*)payload); };

    // send message to client
    webSocket.sendTXT(num, "{\"type\":\"connection\", \"connection\": true, \"simpleMessage\": "+String(readSingleMessage?"true":"false")+"}");

    isConnectedWebSocket = true;

            }
            break;
        case WStype_TEXT:
         { Serial.print("NUM -> "); };{ Serial.print(num); };
         { Serial.print("payload -> "); };{ Serial.println((char*)payload); };

         error = deserializeJson(docwsT, (char*)payload);

         if (error) {
          // if the file didn't open, print an error:
          { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Error parsing JSON "))))); };
          webSocket.broadcastTXT("Error on WS");
         }else{
          JsonObject postObjws = docwsT.as<JsonObject>();

          bool startReceiveDevMsg = postObjws[((reinterpret_cast<const __FlashStringHelper *>(("startReceiveDevMsg"))))];
          if (startReceiveDevMsg==true){
//					readSingleMessage = postObjws[F("singleMessage")];
     isConnectedWebSocketAck = true;
     { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Start listening messages SM -> "))))); };
     { Serial.println(isConnectedWebSocketAck); };

           webSocket.broadcastTXT("{\"type\": \"device_msg\", \"receiving\": true}");
          }else if (startReceiveDevMsg==false){
     isConnectedWebSocketAck = false;
     { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Start listening messages SM -> "))))); };
     { Serial.println(isConnectedWebSocketAck); };

     webSocket.broadcastTXT("{\"type\": \"device_msg\", \"receiving\": false}");
          }

    bool singleMessage = postObjws[((reinterpret_cast<const __FlashStringHelper *>(("singleMessage"))))];
    { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Single message -> "))))); };
    { Serial.println(singleMessage); };
          if (singleMessage){
     readSingleMessage = singleMessage;
          }
         }

//        	DEBUG_PRINTF_AI(F("[%u] get Text: %s\n"), num, payload);

            // send message to client
            // webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
        case WStype_BIN:
////        	DEBUG_PRINTF_AI(F("[%u] get binary length: %u\n"), num, length);
//            hexdump(payload, length);
//
//            // send message to client
//            // webSocket.sendBIN(num, payload, length);
//            break;
        case WStype_ERROR:
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
        case WStype_PING:
        case WStype_PONG:

//        	DEBUG_PRINTF_AI(F("[%u] get binary length: %u\n"), num, length);
         { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("WS : "))))); }
         { Serial.print(type); }
         { Serial.print(((reinterpret_cast<const __FlashStringHelper *>((" - "))))); }
   { Serial.println((char*)payload); };

            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
    }

}

String getContentType(String filename){
  if(filename.endsWith(((reinterpret_cast<const __FlashStringHelper *>((".htm")))))) return ((reinterpret_cast<const __FlashStringHelper *>(("text/html"))));
  else if(filename.endsWith(((reinterpret_cast<const __FlashStringHelper *>((".html")))))) return ((reinterpret_cast<const __FlashStringHelper *>(("text/html"))));
  else if(filename.endsWith(((reinterpret_cast<const __FlashStringHelper *>((".css")))))) return ((reinterpret_cast<const __FlashStringHelper *>(("text/css"))));
  else if(filename.endsWith(((reinterpret_cast<const __FlashStringHelper *>((".js")))))) return ((reinterpret_cast<const __FlashStringHelper *>(("application/javascript"))));
  else if(filename.endsWith(((reinterpret_cast<const __FlashStringHelper *>((".json")))))) return ((reinterpret_cast<const __FlashStringHelper *>(("application/json"))));
  else if(filename.endsWith(((reinterpret_cast<const __FlashStringHelper *>((".png")))))) return ((reinterpret_cast<const __FlashStringHelper *>(("image/png"))));
  else if(filename.endsWith(((reinterpret_cast<const __FlashStringHelper *>((".gif")))))) return ((reinterpret_cast<const __FlashStringHelper *>(("image/gif"))));
  else if(filename.endsWith(((reinterpret_cast<const __FlashStringHelper *>((".jpg")))))) return ((reinterpret_cast<const __FlashStringHelper *>(("image/jpeg"))));
  else if(filename.endsWith(((reinterpret_cast<const __FlashStringHelper *>((".ico")))))) return ((reinterpret_cast<const __FlashStringHelper *>(("image/x-icon"))));
  else if(filename.endsWith(((reinterpret_cast<const __FlashStringHelper *>((".xml")))))) return ((reinterpret_cast<const __FlashStringHelper *>(("text/xml"))));
  else if(filename.endsWith(((reinterpret_cast<const __FlashStringHelper *>((".pdf")))))) return ((reinterpret_cast<const __FlashStringHelper *>(("application/x-pdf"))));
  else if(filename.endsWith(((reinterpret_cast<const __FlashStringHelper *>((".zip")))))) return ((reinterpret_cast<const __FlashStringHelper *>(("application/x-zip"))));
  else if(filename.endsWith(((reinterpret_cast<const __FlashStringHelper *>((".gz")))))) return ((reinterpret_cast<const __FlashStringHelper *>(("application/x-gzip"))));
  return ((reinterpret_cast<const __FlashStringHelper *>(("text/plain"))));
}

bool handleFileRead(String path){ // send the right file to the client (if it exists)
  { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("handleFileRead: "))))); };
  { Serial.println(path); };

  // Capite portal
 if (
   path.endsWith("/generate_204") || //Android captive portal. Maybe not needed. Might be handled by notFound handler.
   path.endsWith("/fwlink") //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
 ){
  path = "/";
 }

  if(path.endsWith("/")) path += ((reinterpret_cast<const __FlashStringHelper *>(("index.html")))); // If a folder is requested, send the index file
  String contentType = getContentType(path); // Get the MIME type
  String pathWithGz = path + ((reinterpret_cast<const __FlashStringHelper *>((".gz"))));

  { Serial.print(((reinterpret_cast<const __FlashStringHelper *>(("Path exist : "))))); };
  { Serial.println(path); };

  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){ // If the file exists, either as a compressed archive, or normal
    if(SPIFFS.exists(pathWithGz)) // If there's a compressed version available
      path += ((reinterpret_cast<const __FlashStringHelper *>((".gz")))); // Use the compressed version
    File file = SPIFFS.open(path, "r"); // Open the file
    size_t sent = httpServer.streamFile(file, contentType); // Send it to the client
    file.close(); // Close the file again
    { Serial.println(String(((reinterpret_cast<const __FlashStringHelper *>(("\tSent file: "))))) + path + String(((reinterpret_cast<const __FlashStringHelper *>((" of size "))))) + sent); };
    return true;
  }
  { Serial.println(String(((reinterpret_cast<const __FlashStringHelper *>(("\tFile Not Found: "))))) + path); };
  return false; // If the file doesn't exist, return false
}

void serverRouting() {
   httpServer.onNotFound([]() { // If the client requests any URI
    { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("On not found"))))); };
     if (!handleFileRead(httpServer.uri())){ // send it if it exists
      { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("Not found"))))); };
      httpServer.send(404, ((reinterpret_cast<const __FlashStringHelper *>(("text/plain")))), ((reinterpret_cast<const __FlashStringHelper *>(("404: Not Found"))))); // otherwise, respond with a 404 (Not Found) error
     }
   });

   { Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("Set cache!"))))); };

   // httpServer.serveStatic("/settings.json", SPIFFS, "/settings.json","no-cache, no-store, must-revalidate");
   // httpServer.serveStatic("/", SPIFFS, "/","max-age=31536000");
}
void sendSimpleWSMessage(String type, String val){
 DynamicJsonDocument docws(512);
 JsonObject objws = docws.to<JsonObject>();

 objws["type"] = type;

 objws["value"] = val;

 String buf;
 serializeJson(objws, buf);

 webSocket.broadcastTXT(buf);
}

void realtimeDataCallbak() {
 String type = ((reinterpret_cast<const __FlashStringHelper *>(("wifi_rt"))));
 sendSimpleWSMessage(type, String(WiFi.RSSI()));
}
