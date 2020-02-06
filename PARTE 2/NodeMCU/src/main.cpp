//Ejemplo MQTT-WebSocket & ESP8266 y la tarjeta de entrenamiento 

//--------------Librerias---------------------------------------- 

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
//-------------------VARIABLES GLOBALES--------------------------
int contconexion = 0;

const char* ssid = "Nombre de la red";
const char* password = "Contraseña";
const char* mqtt_server = "Ip server Mqtt-Aws";
const int mqttPort = 1883;
 
WiFiClient espClient;
PubSubClient client(espClient);

char Datos;
String Cadena="";

char POT1[50];
char POT2[50];
char valueStr[15];

//---------------------------  CALLBACK ------------------------------------------------

void callback(char* topic, byte* payload, unsigned int length) {
 char PAYLOAD[5] = "    ";
  
  Serial.print("Mensaje recibido: [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

//------------------------------------------------------------------------------

void reconnect() {
  // Loop hasta que nos reconectemos
    while (!client.connected()) 
    {
        Serial.print("Conectando MQTT...");
              
        if (client.connect("IoT-ESP8266-A")) 
        {
          Serial.println("Conectado");
          client.subscribe("inTopic");
        } 
        
        else 
        {
          Serial.print("Error de conexion");
          Serial.print(client.state());
          delay(2000);
        }
    }
}

//--------------------------------------------------------------------------------------

void setup() {

  Serial.begin(115200); //Start Serial
  
  // Conexión WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and contconexion <50) 
  { //Cuenta hasta 50 si no se puede conectar lo cancela
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
      if (contconexion <50) 
    {       
        Serial.println("");
        Serial.println("WiFi conectado");
        Serial.println(WiFi.localIP());
    }
  
      else 
      { 
          Serial.println("");
          Serial.println("Error de conexion");
      }
  
    
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);
  
  String Pot1 = "IoT/Pot1"; 
  Pot1.toCharArray(POT1, 50);

  String Pot2 = "IoT/Pot2"; 
  Pot2.toCharArray(POT2, 50);


}
 
void loop() {

  if (!client.connected()) 
    {
      reconnect();
    }
  
  client.loop();

  // ----------------------------------------
     if (Serial.available() > 0) 
     {
        Datos = Serial.read();
        Cadena += Datos;
    
        if (Datos == '}')
        {
            byte desde = Cadena.indexOf('A') + 2;
            byte hasta = Cadena.indexOf(';');
            String Poten1 = Cadena.substring(desde, hasta);
            desde = hasta;
            desde = Cadena.indexOf('B') + 2;
            hasta = Cadena.indexOf(';', desde);
            String Poten2 = Cadena.substring(desde, hasta);      
            Serial.println("{A=" + Poten1 + ";B=" + Poten2 + ";}");
           
            Poten1.toCharArray(valueStr, 15);
            Serial.print("[" +  String(POT1) + "] " + Poten1);
            client.publish(POT1, valueStr);
        
            Poten2.toCharArray(valueStr, 15);
            Serial.print("[" +  String(POT2) + "] " + Poten2);
            client.publish(POT2, valueStr);
            Serial.println();
            Cadena = "";
        }
    
    }


  // ----------------------------------------

}