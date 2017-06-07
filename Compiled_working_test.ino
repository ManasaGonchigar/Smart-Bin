#include <XBee.h>
#define ARRAYSIZE	1024

int encrypt(String text, int keyln, char *key, int *ciphercode) ;
char *key = "98765abcd";
int keyln = 9;
int reqid = 2682;
char *clientid = "test01";
String timestamp = "";
String remarks1 = "";
int onModulePin = 2;
String str = "";
String Split_str[7];
int data_analog[64];
int addrss[64];
int No=6;


int encrypt(int textln, char *text, int keyln, char *key, int *ciphercode);
void acquire_data();

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();
ZBRxIoSampleResponse ioSample = ZBRxIoSampleResponse();
char ch;

void setup()
{
  pinMode(onModulePin, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(115200);
  Serial2.begin(9600);
  Serial3.begin(9600);
  xbee.setSerial(Serial3);
//initialisegprs();
}
void loop()
{
  int textln;
  int i;
  int x = 0;
  int answer = 0;
  String output;
  int cipher[ARRAYSIZE];

  for (int p = 1; p <= 64; p++)
  {
    data_analog[p] = 0;
    if (p <= 8)
      Split_str[p] = "";
  }

  //Time data : Start
    String str = "";
  while (Serial2.available())
    Serial.write("Cleaning input buffer");
  Serial2.write(No);
  Serial.println(No);
  delay(250);
  if (Serial2.available())
  {
    while (Serial2.available())
    {
      ch = Serial2.read();
      if (ch == '\n');
      else
        str = str + ch;
    }
  }
  //Time data : End
  remarks1="";
    remarks1 = remarks1 + reqid;
    remarks1 = remarks1 + ",";
    remarks1 = remarks1 + 7;
   

  acquire_data();//Get xbee data into dataanalog[]
   String pqr="";
  //Splits XBee data into 8 parts and stores in 8 strings :Start
  for (int p = 1; p <= 64; p++)
  {
    pqr=pqr+data_analog[p]+",";
    if (p <= 8)
      Split_str[1] = Split_str[1] + data_analog[p] + ",";
    else if (p <= 16)
      Split_str[2] = Split_str[2] + data_analog[p] + ",";
    else if (p <= 24)
      Split_str[3] = Split_str[3] + data_analog[p] + ",";
    else if (p <= 32)
      Split_str[4] = Split_str[4] + data_analog[p] + ",";
    else if (p <= 40)
      Split_str[5] = Split_str[5] + data_analog[p] + ",";
    else if (p <= 48)
      Split_str[6] = Split_str[6] + data_analog[p] + ",";
    else if (p <= 56)
      Split_str[7] = Split_str[7] + data_analog[p] + ",";
    else if (p <= 64)
      Split_str[8] = Split_str[8] + data_analog[p] + ",";

  }
 Serial.println("________________________________________________________________________________________________________________________________");
  Serial.println(Split_str[7]);
  
  //Splits XBee data into 8 parts and stores in 8 strings :End

  
  
     Serial.println(Split_str[7]);
    String text = "#test01#" + str;
    text = reqid + text;
    text = text + "#";
    //Serial.println(Split_str[7]);
    text = text + Split_str[7];
    text = text + "#";
    text = text + remarks1;
    reqid++;
    textln = text.length();
    
    Serial.println(str);
    Serial.println(remarks1);
    Serial.println(text);
    Serial.println("________________________________________________________________________________________________________________________________");
/*
    encrypt(text, keyln, key, cipher);

    output = "data=";
    for (i = 0; i < textln; i ++)// Print the entire encrypted data into a string separated by space
    {
      output = output + cipher[i] + ",";
    }
    output = "http://sensors.bennyraphael.in/cgi-bin/update.cgi?" + output; //Adding the URL to string
    output = "AT+HTTPPARA=\"URL\",\"" + output;
    output = output + "\"";
    answer = sendATcommand2("AT+HTTPINIT", "OK", "ERROR", 1000);
    Serial.print("\n Answer ="); Serial.print(answer); Serial.println("\n");
    if (answer == 1)
    {
      answer = sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", 5000);
      Serial.print("\n Answer ="); Serial.print(answer); Serial.println("\n");
      if (answer == 1)
      {
        while ( Serial1.available() > 0) Serial.println(Serial1.read());
        Serial.println(output);
        Serial1.println(output); Serial.println("\n");
        char response[100];
        unsigned long previous;

        memset(response, '\0', 100);    // Initialize the string
        delay(100);
        do {
          if (Serial1.available() != 0)
          {
            response[x] = Serial1.read();
            x++;
            // check if the desired answer is in the response of the module
            if (strstr(response, "OK") != NULL)
            {
              answer = 1;
            }
          }
          // Waits for the asnwer with time out
        }
        while ((answer == 0) && ((millis() - previous) < 5000));
        Serial.println(response);
        Serial.print("\n Answer ="); Serial.print(answer); Serial.println("\n");
        delay(5000);
        memset(response, '\0', 100);
        answer = sendATcommand("AT+HTTPACTION=0", "+HTTPACTION:0,200", 5000);
        Serial.print("\n Answer ="); Serial.print(answer);
        answer = 0;
        while (answer == 0)
        {
          answer = sendATcommand("AT+HTTPTERM", "OK", 5000);
          Serial.print("\n Answer ="); Serial.print(answer);
          Serial.println("\n");
        }


      }
    }*/
    text = "";
    remarks1 = "";
  


}
int encrypt(String text, int keyln, char *key, int *ciphercode)
{

  int textln;
  int i ;
  int ind1 ;
  int prev ;
  int val2 ;
  int ch1 ;
  int ch2 ;

  textln = text.length();

  prev = 0;
  for (i = 0; i < textln; i++) {
    ch1 = text.charAt(i);
    ind1 = i % keyln;
    ch2 = key[ind1];
    val2 = prev % 16;
    ciphercode[i] = i + ch1 + ch2 + val2;
    prev = (prev + ch1) % 1024;
  }
  return 1;
}

void initialisegprs()
{
  Serial.println("Initialising the GSM Module");
  power_on();
  //delay(3000);
  while (sendATcommand2("AT+CREG?", "+CREG: 1,1", "+CREG: 1,5", 2000) == 0 );
  //delay(2000);
  sendATcommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", 2000);
  sendATcommand("AT+SAPBR=3,1,\"APN\",\"bsnlnet\"", "OK", 2000);
  while ( sendATcommand("AT+SAPBR=1,1", "OK", 3000) == 0);
  sendATcommand("AT+SAPBR=2,1", "OK", 2000);


}
void power_on()
{

  uint8_t answer = 0;

  // checks if the module is started
  answer = sendATcommand("AT", "OK", 2000);
  Serial.println("AT");
  if (answer == 0)
  {
    // power on pulse
    digitalWrite(onModulePin, HIGH);
    delay(3000);
    digitalWrite(onModulePin, LOW);

    // waits for an answer from the module
    while (answer == 0) {
      // Send AT every two seconds and wait for the answer
      answer = sendATcommand("AT", "OK", 2000);
      Serial.println("AT");
      Serial.println(answer);
    }
  }
}
int8_t sendATcommand(char * ATcommand, char * expected_answer1, unsigned int timeout)
{


  uint8_t x = 0,  answer = 0;
  char responsea[100];
  unsigned long previous;
  memset(responsea, '\0', 100);    // Initialize the string
  delay(100);
  while ( Serial1.available() > 0) Serial1.read();   // Clean the input buffer
  Serial1.println(ATcommand);    // Send the AT command
  x = 0;
  previous = millis();
  // this loop waits for the answer
  do {
    if (Serial1.available() != 0)
    {

      responsea[x] = Serial1.read();
      x++;
      // check if the desired answer is in the response of the module
      if (strstr(responsea, expected_answer1) != NULL)
      {
        answer = 1;
      }
    }
    // Waits for the asnwer with time out
  }
  while ((answer == 0) && ((millis() - previous) < timeout));
  Serial.println(responsea);
  //Serial.println("Flow of control :Left sendATcommand");
  return answer;
}
int8_t sendATcommand2(char * ATcommand, char * expected_answer1, char * expected_answer2, unsigned int timeout)
{
  //Serial.println("Flow of control :Entered sendATcommand2");
  uint8_t x = 0,  answer = 0;
  char responseb[100];
  unsigned long previous;

  memset(responseb, '\0', 100);    // Initialize the string

  delay(100);

  while ( Serial1.available() > 0) Serial1.read();   // Clean the input buffer

  Serial1.println(ATcommand);    // Send the AT command


  x = 0;
  previous = millis();

  // this loop waits for the answer
  do {
    if (Serial1.available() != 0) {
      responseb[x] = Serial1.read();
      x++;
      // check if the desired answer is in the response of the module
      if (strstr(responseb, expected_answer1) != NULL)
      {
        answer = 1;
      }
      if (strstr(responseb, expected_answer2) != NULL)
      {
        answer = 2;
      }
    }
    // Waits for the asnwer with time out
  }
  while ((answer == 0) && ((millis() - previous) < timeout));
  Serial.println(responseb);
  //  Serial.println("Flow of control :Left sendATcommand 2");
  return answer;
}

//Handles XBEE communication
void acquire_data()
{
  Serial.println("Flow of control :Entered Acquiredata");
  int xtimeout = 5000;
  int start = millis();
  do
  {
    // delay(5000);
    xbee.readPacket();
    if (xbee.getResponse().isAvailable()) //Check whether there is an xbee response
    {
      if (xbee.getResponse().getApiId() == ZB_IO_SAMPLE_RESPONSE) //Check whether response is an IO sample
      {
        xbee.getResponse().getZBRxIoSampleResponse(ioSample);
        uint16_t deviceID = ioSample.getRemoteAddress16();//Getting the 16 bit address of the transmitting module
        int flag = 0;
        for (int p = 1; p <= 64; p++)
        {
          //if(deviceID == addrss[p]) //Mapping the address to the correct module
          //{
          flag = 1;
          if (ioSample.containsAnalog())
            data_analog[p] = ioSample.getAnalog(1);
          else
            Serial.println("Error: No analog data");
          //}
        }
        if (flag == 0)
          Serial.println("Error: Device not recognised/found");
      }
      else
        Serial.println("Error: Not an IO response");
    }
    //else
    //Serial.println("No XBee response");
  } while ((millis() - start) <= xtimeout);
  Serial.println("Flow of control :Left Acquiredata");
}
