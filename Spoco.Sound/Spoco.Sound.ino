#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

int sensorPin = A0; // select the input pin for the potentiometer
int ledPin = 13; // select the pin for the LED
int sensorValue = 0; // variable to store the value coming from the sensor


const int SPOCO_CMD_INIT = 0;
const int SPOCO_CMD_SEND = 1;
const int SPOCO_CMD_KNOCK = 2;

void setup()
{
	pinMode(ledPin, OUTPUT);
	Serial.begin(115200);

	// устанавливаем скорость передачи данных для последовательного порта, созданного 
	// библиотекой SoftwareSerial
	mySerial.begin(115200);
	mySerial.println(SPOCO_CMD_INIT);


}

int indexer = -1;
void loop()
{
	if (mySerial.available())
		Serial.write(mySerial.read());

	sensorValue = analogRead(sensorPin);

	if (sensorValue > 30)
	{
		Serial.println(sensorValue);
		indexer++;
	}
	else
	{
		if (indexer > -1)
		{
			mySerial.write(SPOCO_CMD_KNOCK);
			digitalWrite(ledPin, HIGH);
			delay(indexer * 200);
			digitalWrite(ledPin, LOW);
		}
		indexer = -1;
	}
	/*digitalWrite(ledPin, HIGH);
	delay(sensorValue);
	digitalWrite(ledPin, LOW);
	delay(sensorValue);*/


	//if (sensorValue > 100)
	//{
	//	//Serial.println(sensorValue, DEC);
	//	digitalWrite(ledPin, HIGH);
	//	digitalWrite(ledPin, LOW);
	//	mySerial.write(SPOCO_CMD_KNOCK);
	//}
}