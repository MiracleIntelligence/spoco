/*
 * HTTPS Secured Client GET Request
 * Copyright (c) 2019, circuits4you.com
 * All rights reserved.
 * https://circuits4you.com
 * Connects to WiFi HotSpot. */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>


const int SPOCO_CMD_INIT = 0;
const int SPOCO_CMD_SEND = 1;
const int SPOCO_CMD_KNOCK = 2;

/* Set these to your desired credentials. */
const char *ssid = "Pymonenko";  //ENTER YOUR WIFI SETTINGS
const char *password = "kopernyk43";

int recieved_cmd = -1;

bool ready = false;

//Link to read data from https://jsonplaceholder.typicode.com/comments?postId=7
//Web/Server address to read/write from 
const char *host = "elspoco.azurewebsites.net";
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80

//SHA1 finger print of certificate use web browser to view and copy
const char fingerprint[] PROGMEM = "3A B0 B1 C2 7F 74 6F D9 0C 34 F0 D6 A9 60 CF 73 A4 22 9D E8";
//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
	delay(1000);
	Serial.begin(115200);
	WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
	delay(1000);
	WiFi.mode(WIFI_STA);        //Only Station No AP, This line hides the viewing of ESP as wifi hotspot

	WiFi.begin(ssid, password);     //Connect to your WiFi router
	Serial.println("");

	Serial.print("Connecting");
	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	//If connection successful show IP address in serial monitor
	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {

	if (Serial.available())
	{
		recieved_cmd = Serial.read();

		switch (recieved_cmd)
		{
		case SPOCO_CMD_INIT: ready = true; break;
		case SPOCO_CMD_SEND: SendData(); break;
		case SPOCO_CMD_KNOCK: Knock(); break;
		default:
			Serial.println(recieved_cmd);
			break;
		}
	}
	return;

	WiFiClientSecure httpsClient;    //Declare object of class WiFiClient

	Serial.println(host);

	Serial.printf("Using fingerprint '%s'\n", fingerprint);
	httpsClient.setFingerprint(fingerprint);
	httpsClient.setTimeout(15000); // 15 Seconds
	delay(1000);

	Serial.print("HTTPS Connecting");
	int r = 0; //retry counter
	while ((!httpsClient.connect(host, httpsPort)) && (r < 30)) {
		delay(100);
		Serial.print(".");
		r++;
	}
	if (r == 30) {
		Serial.println("Connection failed");
	}
	else {
		Serial.println("Connected to web");
	}

	String ADCData, getData, Link;
	int adcvalue = analogRead(A0);  //Read Analog value of LDR
	ADCData = String(adcvalue);   //String to interger conversion

	Serial.println("Analog data: " + ADCData);

	//GET Data
	//Link = "/comments?postId=" + ADCData;
	Link = "/api/knock?ZUMO-API-VERSION=2.0.0";

	Serial.print("requesting URL: ");
	Serial.println(host + Link);

	httpsClient.print(String("GET ") + Link + " HTTP/1.1\r\n" +
		"Host: " + host + "\r\n" +
		"Connection: close\r\n\r\n");

	Serial.println("request sent");

	while (httpsClient.connected()) {
		String line = httpsClient.readStringUntil('\n');
		if (line == "\r") {
			Serial.println("headers received");
			break;
		}
	}

	Serial.println("reply was:");
	Serial.println("==========");
	String line;
	while (httpsClient.available()) {
		line = httpsClient.readStringUntil('\n');  //Read Line by Line
		Serial.println(line); //Print response
	}
	Serial.println("==========");
	Serial.println("closing connection");

	delay(2000);  //GET Data at every 2 seconds
}


void SendData()
{
	Serial.println("DATA SENT!");
	Serial.println("==========");
}

void Knock()
{
	Serial.println("KNOCK");
	Serial.println("==========");
}
//=======================================================================