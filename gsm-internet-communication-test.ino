#include <SoftwareSerial.h>

SoftwareSerial SerialAT(10, 11); // RX, TX


int8_t answer;
int onModulePin= 2;

char data[512];
int data_size;

char aux_str[100];
char aux;
int x = 0;


void setup(){

    pinMode(onModulePin, OUTPUT);
    Serial.begin(9600);
    SerialAT.begin(9600);   
while(1){
 if(Serial.read()=='1')break; 
  
}
    Serial.println("Starting...");
SerialAT.println("AT");
    //power_on();
    delay(3000);
    //Serial.println(sendATcommand("AT+CREG?", "+CREG: 1,5",2000));
    while (sendATcommand("AT", "OK", 2000) == 0 );
    while (sendATcommand2("AT+CREG?", "+CREG: 1,1","+CREG: 1,5", 2000) == 0 );


    sendATcommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", 2000);
    sendATcommand("AT+SAPBR=3,1,\"APN\",\"\"", "OK", 2000);
    //sendATcommand("AT+SAPBR=3,1,\"USER\",\"\"", "OK", 2000);
    //sendATcommand("AT+SAPBR=3,1,\"PWD\",\"\"", "OK", 2000);
    //sendATcommand("AT+SAPBR=1,1", "OK", 2000);
    sendATcommand("AT+SAPBR=?", "OK", 2000);
    while (sendATcommand("AT+SAPBR=1,1", "OK", 2000) == 0)
    {
        delay(5000);
    }


}
void loop(){

    // Initializes HTTP service
    answer = sendATcommand2("AT+HTTPINIT", "OK","ERROR", 10000);
    if (1)
    {
        // Sets CID parameter
        answer = sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", 5000);
        if (answer == 1)
        {
            // Sets url 
            answer = sendATcommand("AT+HTTPPARA=\"URL\",\"http://hariprasadiit.ddns.net/test/test.php\data=\"", "OK", 5000);
            if (answer == 1)
            {
                // Starts GET action
                answer = sendATcommand("AT+HTTPACTION=0", "+HTTPACTION:0,200", 10000);
                
                if (answer == 1)
                {
                    x=0;
                    do{
                        sprintf(aux_str, "AT+HTTPREAD=%d,100", x);
                        if (sendATcommand2(aux_str, "+HTTPREAD:", "ERROR", 30000) == 1)
                        {
                            data_size = 0;
                            while(SerialAT.available()==0);
                            aux = SerialAT.read();
                            do{
                                data_size *= 10;
                                data_size += (aux-0x30);
                                while(SerialAT.available()==0);
                                aux = SerialAT.read();        
                            }while(aux != 0x0D);

                            Serial.print("Data received: ");
                            Serial.println(data_size);

                            if (data_size > 0)
                            {
                                while(SerialAT.available() < data_size);
                                SerialAT.read();

                                for (int y = 0; y < data_size; y++)
                                {
                                    data[x] = SerialAT.read();
                                    x++;
                                }
                                data[x] = '\0';
                            }
                            else
                            {
                                Serial.println("Download finished");    
                            }
                        }
                        else if (answer == 2)
                        {
                            Serial.println("Error from HTTP");
                        }
                        else
                        {
                            Serial.println("Error getting the url");
                            data_size = 0;
                        }
                        
                        sendATcommand("", "+HTTPACTION:0,200", 20000);
                    }while (data_size > 0);
                    
                    Serial.print("Data received: ");
                    Serial.println(data);
                }
                else
                {
                    Serial.println("Error getting the url");
                }
            }
            else
            {
                Serial.println("Error setting the url");
            }
        }
        else
        {
            Serial.println("Error setting the CID");
        }    
    }
    else
    {
        Serial.println("Error initializating");
    }
    
    sendATcommand("AT+HTTPTERM", "OK", 5000);

    delay(5000);

}

void power_on(){

    uint8_t answer=0;

    // checks if the module is started
    answer = sendATcommand("AT", "OK", 2000);
    if (answer == 0)
    {
        // power on pulse
        digitalWrite(onModulePin,HIGH);
        delay(3000);
        digitalWrite(onModulePin,LOW);

        // waits for an answer from the module
        while(answer == 0){  
            // Send AT every two seconds and wait for the answer   
            answer = sendATcommand("AT", "OK", 2000);    
        }
    }

}


int8_t sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout){
Serial.println(ATcommand);
    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( SerialAT.available() > 0) SerialAT.read();    // Clean the input buffer

    SerialAT.println(ATcommand);    // Send the AT command 


        x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        if(SerialAT.available() != 0){    
            response[x] = SerialAT.read();
            x++;
            // check if the desired answer is in the response of the module
            if (strstr(response, expected_answer1) != NULL)    
            {
                answer = 1;
            }
        }
        // Waits for the asnwer with time out
    }
    while((answer == 0) && ((millis() - previous) < timeout));    
  Serial.println(response);
    return answer;
}

int8_t sendATcommand2(char* ATcommand, char* expected_answer1,char* expected_answer2, unsigned int timeout){
Serial.println(ATcommand);
    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( SerialAT.available() > 0) SerialAT.read();    // Clean the input buffer

    SerialAT.println(ATcommand);    // Send the AT command 


        x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        if(SerialAT.available() != 0){    
            response[x] = SerialAT.read();
            x++;
            // check if the desired answer is in the response of the module
            if (strstr(response, expected_answer1) != NULL)    
            {
                answer = 1;
            }
            if (strstr(response, expected_answer2) != NULL)    
            {
                answer = 2;
            }
        }
        // Waits for the asnwer with time out
    }
    while((answer == 0) && ((millis() - previous) < timeout));    
  Serial.println(response);
    return answer;
}
