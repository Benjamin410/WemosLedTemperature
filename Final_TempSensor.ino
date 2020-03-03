#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_NeoPixel.h>

#define PIN D6
#define ONE_WIRE_BUS D2

//the Wemos WS2812B RGB shield has 1 LED connected to pin 2
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);

const char* ssid = "CTP_TEMP_SENSOR";
const char* pw = "ctp1fb";

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

float temperatureC;
char temperatureCString[6];
char temperatureFString[6];


IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

void setup()
{
  Serial.begin(115200);
  DS18B20.begin(); 
  pixels.begin(); // This initializes the NeoPixel library.
  Serial.println();

  Serial.print("Konfiguriere WiFi ... ");
  Serial.println(ssid);

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}

void getTemperature() {
 do {
 DS18B20.requestTemperatures(); 
 temperatureC = DS18B20.getTempCByIndex(0);
 dtostrf(temperatureC, 2, 2, temperatureCString);
 delay(100);
 } while (temperatureC == 85.0 || temperatureC == (-127.0));
}

//simple function which takes values for the red, green and blue led and also
//a delay
void setColor(int led, int redValue, int greenValue, int blueValue, int delayValue)
{
  pixels.setPixelColor(led, pixels.Color(redValue, greenValue, blueValue)); 
  pixels.show();
  delay(delayValue);
}


void loop() {
 delay(3000);
 getTemperature();
 Serial.printf(temperatureCString);
 Serial.println(" Grad Celsisus");

  
  int led;
  for(led=0; led <=12; led++)
  {
    if(temperatureC>28.00){
    setColor(led,200,0,0,300); //red
    }

    if(temperatureC<20.00){
    setColor(led,0,0,200,300); //blue
    }

    if(temperatureC>20.00&&temperatureC<25.00){
    setColor(led,80,80,80,300); //white
    }

    if(temperatureC>25.00&&temperatureC<28.00){
    setColor(led,180,180,0,300); //yellow
    }

}
 
}
