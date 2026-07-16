// RC_Car_TCP_Server.ino

//in this example, esp8266 is an Access Point, other wifi device can connect to it 
#include "esp8266.h"
#include "SoftwareSerial.h"

#define	ssid		"esp8266"
#define	password	"12345678"
#define	serverPort	8081		// esp8266 will open it's 8081 port , and other can connect 

Esp8266 wifi;
SoftwareSerial mySerial(A0, A1); // Arduino RX, TX	

// Motor A connections
int en1_A = 2;
int in1_A = 4;
int in2_A = 5;
// Motor B connections
int en2_B = 3;
int in3_B = 6;
int in4_B = 7;
// Motor C connections
int en1_C = 8;
int in1_C = 10;
int in2_C = 11;
// Motor D connections
int en2_D = 9;
int in3_D = 12;
int in4_D = 13;

void forward() {
    wifi.debugPrintln("RC Car forward");
    // Ustaw odpowiednie piny do ruchu naprzod
    digitalWrite(in1_A, HIGH);
    digitalWrite(in2_A, LOW);
    digitalWrite(in3_B, HIGH);
    digitalWrite(in4_B, LOW);

    digitalWrite(in1_C, HIGH);
    digitalWrite(in2_C, LOW);
    digitalWrite(in3_D, HIGH);
    digitalWrite(in4_D, LOW);
}

void backward() {
    wifi.debugPrintln("RC Car backward");
    // Ustaw odpowiednie piny do ruchu wstecz
    digitalWrite(in1_A, LOW);
    digitalWrite(in2_A, HIGH);
    digitalWrite(in3_B, LOW);
    digitalWrite(in4_B, HIGH);

    digitalWrite(in1_C, LOW);
    digitalWrite(in2_C, HIGH);
    digitalWrite(in3_D, LOW);
    digitalWrite(in4_D, HIGH);
}

void left() {
    wifi.debugPrintln("RC Car left");
    // Ustaw odpowiednie piny do skretu w lewo
    digitalWrite(in1_A, HIGH);
    digitalWrite(in2_A, LOW);
    digitalWrite(in3_B, HIGH);
    digitalWrite(in4_B, LOW);

    digitalWrite(in1_C, LOW);
    digitalWrite(in2_C, HIGH);
    digitalWrite(in3_D, LOW);
    digitalWrite(in4_D, HIGH);
}

void right() {
    wifi.debugPrintln("RC Car right");
    // Ustaw odpowiednie piny do skretu w prawo
    digitalWrite(in1_A, LOW);
    digitalWrite(in2_A, HIGH);
    digitalWrite(in3_B, LOW);
    digitalWrite(in4_B, HIGH);

    digitalWrite(in1_C, HIGH);
    digitalWrite(in2_C, LOW);
    digitalWrite(in3_D, HIGH);
    digitalWrite(in4_D, LOW);
}

void stop() {
    wifi.debugPrintln("RC Car stop");
    // Zatrzymuje silniki
    digitalWrite(in1_A, LOW);
    digitalWrite(in2_A, LOW);
    digitalWrite(in3_B, LOW);
    digitalWrite(in4_B, LOW);

    digitalWrite(in1_C, LOW);
    digitalWrite(in2_C, LOW);
    digitalWrite(in3_D, LOW);
    digitalWrite(in4_D, LOW);
}

void setup() {

	delay(2000);				              // it will be better to delay 2s to wait esp8266 module OK
	
  Serial.begin(115200);
	mySerial.begin(115200);

	wifi.begin(&Serial, &mySerial);   //Serial is used to communicate with esp8266 module, mySerial is used to print debug message

  wifi.debugPrintln("system booting...");

  if (wifi.enableAP(ssid, password)) {
    wifi.debugPrintln("Access Point enabled successfully!");
  } else {
    wifi.debugPrintln("Failed to enable Access Point!");
  }

  if (wifi.setMultiConnect()) {
		wifi.debugPrintln("Multi connect!");
	}	

  if (wifi.openTCPServer(serverPort, 180)) {
    wifi.debugPrintln("TCP Server opened on port " + String(serverPort));
  } else {
    wifi.debugPrintln("Failed to open TCP Server!");
  }

  wifi.debugPrintln("Server IP:" + wifi.getIP() + " Port:" + String(serverPort));

  // MOTOR SETINGS
  // Set all the motor control pins to outputs
	pinMode(en1_A, OUTPUT);
	pinMode(en2_B, OUTPUT);
	pinMode(in1_A, OUTPUT);
	pinMode(in2_A, OUTPUT);
	pinMode(in3_B, OUTPUT);
	pinMode(in4_B, OUTPUT);

  pinMode(en1_C, OUTPUT);
	pinMode(en2_D, OUTPUT);
	pinMode(in1_C, OUTPUT);
	pinMode(in2_C, OUTPUT);
	pinMode(in3_D, OUTPUT);
	pinMode(in4_D, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1_A, LOW);
	digitalWrite(in2_A, LOW);
	digitalWrite(in3_B, LOW);
	digitalWrite(in4_B, LOW);

	digitalWrite(in1_C, LOW);
	digitalWrite(in2_C, LOW);
	digitalWrite(in3_D, LOW);
	digitalWrite(in4_D, LOW);

  analogWrite(en1_A, 255);
	analogWrite(en2_B, 255);
  analogWrite(en1_C, 255);
  analogWrite(en2_D, 255);
	
}

void loop() {
    int state = wifi.getState();
    switch (state) {
        case WIFI_NEW_MESSAGE:
            wifi.debugPrintln(String(wifi.getWorkingID()) + ":" + wifi.getMessage()); // debug
	          //wifi.sendMessage(wifi.getWorkingID(), wifi.getMessage());	// send the message back

            if (wifi.getMessage() == "F") {
                forward(); // Jazda do przodu
                delay(200);
                stop();
            } else if (wifi.getMessage() == "B") {
                backward(); // Jazda do tyłu
                delay(200);
                stop();
            } else if (wifi.getMessage() == "L") {
                left(); // Skręt w lewo
                delay(200);
                stop();
            } else if (wifi.getMessage() == "R") {
                right(); // Skręt w prawo
                delay(200);
                stop();
            } else if (wifi.getMessage() == "S") {
                stop(); // Zatrzymanie
            } else {
                wifi.debugPrintln("Unknow command: " + wifi.getMessage());
            }     

	          wifi.setState(WIFI_IDLE);
	          break;

        case WIFI_CLOSED:
            wifi.debugPrintln(String(wifi.getFailConnectID()) + ":connect closed!");
            wifi.setState(WIFI_IDLE);
            break;

        case WIFI_IDLE:
        {
            int state = wifi.checkMessage();
            wifi.setState(state);
            break;
        }

        case WIFI_CLIENT_ON:
            wifi.sendMessage(wifi.getWorkingID(), "hello, this is ESP8266 RC_CAR.");
            wifi.setState(WIFI_IDLE);
            break;
    }
}

