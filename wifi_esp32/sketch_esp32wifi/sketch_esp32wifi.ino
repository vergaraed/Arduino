#include <WiFi.h>

#define MAX_WIFI_NETWORKS 5
#define WIFI_TIMEOUT_MS   20000
void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  //WIFI_STA, WIFI_STA_AP, 
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
}
String input;
byte mac[6];                     // the MAC address of your Wifi shield


typedef struct network
{
  char SSID[32];
  char PWD[16];
  int RSSI;
  char MAC[64];
  int encryptionType;
} network_t;


network_t *Networks[MAX_WIFI_NETWORKS];

void scan()
{
  initNetworks();
  Serial.println("scan start");

  //free(Networks);

  //Networks = malloc(sizeof(network_t*));
  
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
      Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    Serial.println(" Select a Network by index or S for Scan or Q to quit.");
    for (int i = 0; i < n; ++i) {
      network_t *ntwk = (network_t*)malloc(sizeof(network_t));     
      snprintf(ntwk->SSID, 32, WiFi.SSID(i).c_str());
      ntwk->RSSI=WiFi.RSSI(i);
      snprintf(ntwk->PWD,16, "Password1");
      ntwk->encryptionType=WiFi.encryptionType(i);
      Networks[i-1] = ntwk;
      // Print SSID and RSSI for each network found
      Serial.print(i);
      Serial.print(")  SSID: ");
      Serial.print(Networks[i-1]->SSID);
      Serial.print(" RSSI(");
      Serial.print(Networks[i-1]->RSSI);
      Serial.print(") EncrType: ");
      Serial.println(Networks[i-1]->encryptionType);
      delay(10);
    }
  }
  Serial.println("Enter the index of the desired network...");
  delay(3000);
}

void loop() {

  scan();

  while (Serial.available() == 0) {delay(10); }

  String cmdread = Serial.readString();
  printf("You selected: %c/n", cmdread);
  cmdread.trim();
  if (cmdread=="Q" || cmdread=="q")
  {
    Serial.println("Quiting...");
    //exit(1);
  }
  else if (cmdread=="s" || cmdread=="S") {
    Serial.println("Rescanning...");
    printf("Rescanning...");
    return;
  } 
  else 
  {
    Serial.print("Connecting to selection:");
    Serial.print(cmdread.toInt());
    Serial.print("      SSID :  "); 
    int i = cmdread.toInt()-1;
    Serial.println(Networks[i]->SSID);
    printf("Connecting to selection: %d/n", i);

    Serial.println("Enter Password:");

    while (Serial.available() == 0) {delay(10); }
    
    String password = Serial.readString();
    strncpy(Networks[i]->PWD, password.c_str(), 16);
    connect(i);
    while (Serial.available() == 0) {delay(10); }
  }

  // Wait a bit before scanning again
  delay(1000);
  clearNetworks();
  
}

void connect(int i)
{
  Serial.println("Connecting to Wifi...");
  WiFi.begin(Networks[i]->SSID, Networks[i]->PWD);

  unsigned long conntm = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - conntm < WIFI_TIMEOUT_MS )
  {
    Serial.print(".");
    delay(100);
  }

  if (WiFi.status() != WL_CONNECTED)
    Serial.print("FUCK!!");
  else
  {
    Serial.println("Connected!");
    PrintMac();
    Serial.println(WiFi.localIP());
  }
}

void initNetworks()
{
  for (int i=0; i<MAX_WIFI_NETWORKS; i++)
    Networks[i] = (network_t*)malloc(sizeof(network_t));
}

void clearNetworks()
{
  for (int i=0; i<MAX_WIFI_NETWORKS; i++)
    free(Networks[i]);
}

void PrintMac()
{
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
}
