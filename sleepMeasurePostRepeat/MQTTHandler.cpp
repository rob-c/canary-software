#include "MQTTHandler.h"

//******************************************
//MQTT handler constructor
MQTTHandler::MQTTHandler(PubSubClient* mqttclient,
			 char* server,
			 unsigned int port,
			 bool tls,
			 char* username,
			 char* password,
			 char* topic,
			 unsigned int messagesize,
			 const char* cacert) {

  //------------------------------------------
  //MQTT handler setup
  _mqttclient = mqttclient;
  _server = server;
  _port = port;
  _tls = tls;
  _username = username;
  _password = password;
  _topic = topic;
  _messagesize = messagesize;

  //------------------------------------------
  //TLS setup
  if (_tls) {
    _wificlientsecure = WiFiClientSecure();
#ifdef ESP8266
    X509List x509cacert(cacert);
    _wificlientsecure.setTrustAnchors(&x509cacert);
#else
    _wificlientsecure.setCACert(cacert);
#endif //ESP8266
    _mqttclient->setClient(_wificlientsecure);
  }

  //------------------------------------------
  //no TLS setup
  else {
    _wificlient = WiFiClient();
    _mqttclient->setClient(_wificlient);
  }

  //------------------------------------------
  //MQTT client setup
  _mqttclient->setServer(_server, _port);
  _mqttclient->setBufferSize(_messagesize);
  
  return;
}

//******************************************
//connect to MQTT broker
bool MQTTHandler::connect(bool verbose) {

  //------------------------------------------
  //check if already connected to the MQTT server
  if (_mqttclient->connected()) {
    if (verbose) {
      Serial.print("\nalready connected to MQTT server ");
      Serial.println(_server);
      Serial.println();
    }
    return true;
  }

  //------------------------------------------
  //print status
  if (verbose) {
    Serial.print("\nconnecting to MQTT server ");
    Serial.print(_server);
    Serial.println("...");
  }

  //------------------------------------------
  //try connecting for 4 times with 5 s intervals
  for (int ii = 0; ii < 4; ii++) {

    //------------------------------------------
    //generate random client ID
    for (int i = 0; i < 10; i++) {
      _clientid[i] = _alphanum[random(51)];
    }
    _clientid[10] = '\0';

    if (verbose) {
      Serial.print("client ID: ");
      Serial.println(_clientid);
    }

    //------------------------------------------
    //connect
    _mqttclient->connect(_clientid, _username, _password);

    //------------------------------------------
    //check connection status
    if (verbose) {
      status(verbose);
    }

    //------------------------------------------
    //print connection info
    if (_mqttclient->connected()) {
      if (verbose) {
	Serial.print("MQTT connected to ");
	Serial.println(_server);
	Serial.println();
      }
      return true;
    }

    //------------------------------------------
    //delay between trials
    delay(5000);//ms
  }//END connecting loop

  //------------------------------------------
  //print connection error
  if (verbose) {
    Serial.print("could not connect to ");
    Serial.println(_server);
    Serial.println();
  }
  
  return false;
}

//******************************************
//check client status
//http://pubsubclient.knolleary.net/api.html#state
int MQTTHandler::status(bool verbose) {

  int status = _mqttclient->state();
    
  if (verbose) {
    Serial.print("status: ");
    switch (status) {
    case -4:
      Serial.println("timeout");
      break;
    case -3:
      Serial.println("lost");
      break;
    case -2:
      Serial.println("failed");
      break;
    case -1:
      Serial.println("disconnected");
      break;
    case 0:
      Serial.println("connected");
      break;
    case 1:
      Serial.println("bad protocol");
      break;
    case 2:
      Serial.println("bad client ID");
      break;
    case 3:
      Serial.println("unavailable");
      break;
    case 4:
      Serial.println("bad credentials");
      break;
    case 5:
      Serial.println("unauthorized");
      break;
    default:
      Serial.println("unknown");
      break;
    }
  }
  
  return status;
}

//******************************************
//post values from enbled sensors
bool MQTTHandler::post(JsonDocument &doc,
		       bool post,
		       bool verbose) {

  //------------------------------------------
  //convert message JSON to char array
  char message[measureJson(doc) + 1];
  serializeJson(doc, message, measureJson(doc) + 1);

  //------------------------------------------
  //print info
  if (verbose) {
    Serial.println();
    Serial.println("JSON: ");
    serializeJsonPretty(doc, Serial);
    Serial.println();
    Serial.print("message: ");
    Serial.println(message);
    Serial.print("message length: ");
    Serial.println(measureJson(doc));
    Serial.print("topic: ");
    Serial.println(_topic);
    Serial.print("server: ");
    Serial.println(_server);
    Serial.print("port: ");
    Serial.println(_port);
    Serial.printf("TLS: %s\n", _tls? "yes":"no");
    Serial.print("user: ");
    Serial.println(_username);
    Serial.print("auth: ");
    Serial.println(_password);
  }
  
  //------------------------------------------
  //post data
  bool result=false;
  if (post) {
      
    //------------------------------------------
    //connect to the MQTT broker
    if (connect(verbose)) {

      //------------------------------------------
      //publish
      result = _mqttclient->publish(_topic, message);
      if (verbose) {
	if (result) {
	  Serial.println("success\n");
	} else {
	  Serial.println("fail\n");
	}
      }
    }
  }
  
  return result;
}
