#include "MQTTHandler.h"

//******************************************
//MQTT handler constructor
MQTTHandler::MQTTHandler(char* server,
			 unsigned int port,
			 char* username,
			 char* password,
			 char* topic,
			 unsigned int messagesize) {
  _server = server;
  _port = port;
  _username = username;
  _password = password;
  _topic = topic;
  _messagesize = messagesize;

  _wificlient = WiFiClient();
  _client = PubSubClient(_wificlient);
  _client.setServer(_server, _port);

  return;
}

//******************************************
//initialize MQTT handler
//NOTE setBufferSize cannot be chained with other pubSubClient methods
//https://pubsubclient.knolleary.net/api#setBufferSize
void MQTTHandler::init() {
  _client.setBufferSize(_messagesize);
  return;
}

//******************************************
//connect to MQTT broker
bool MQTTHandler::connect(bool verbose) {

  //------------------------------------------
  //check if already connected to the MQTT server
  if (_client.connected()) {
    if (verbose) {
      Serial.print("already connected to MQTT server ");
      Serial.println(_server);
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
  //try connecting for 10 times with 1 s intervals
  for (int ii = 0; ii < 10; ii++) {

    //------------------------------------------
    //generate random client ID
    for (int i = 0; i < 10; i++) {
      _clientid[i] = _alphanum[random(51)];
    }

    //https://community.thingspeak.com/forum/esp8266-wi-fi/problem-rc-4-using-library-pubsub/
    _clientid[10] = '\0';

    if (verbose) {
      Serial.print("client ID: ");
      Serial.println(_clientid);
    }

    //------------------------------------------
    //connect
    _client.connect(_clientid, _username, _password);

    //------------------------------------------
    //check connection status
    if (verbose) {
      status(verbose);
    }

    //------------------------------------------
    //print connection info
    if (_client.connected()) {
      if (verbose) {
	Serial.print("MQTT connected to ");
	Serial.println(_server);
	Serial.println();
      }
      return true;
    }

    //------------------------------------------
    //delay between trials
    delay(1000);//ms
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
//check the client status
//this can explain why the connection may have failed
//see http://pubsubclient.knolleary.net/api.html#state for the failure code explanation
int MQTTHandler::status(bool verbose) {

  int status = _client.state();
    
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
		       bool disconnect,
		       bool verbose) {

  //------------------------------------------
  //convert message JSON to char array
  char message[measureJson(doc) + 1];
  serializeJson(doc, message, measureJson(doc) + 1);

  //------------------------------------------
  //convert topic string to char array
  //char topic[_topic.length()];
  //_topic.toCharArray(topic, _topic.length() + 1);

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
      result = _client.publish(_topic, message);
      if (verbose) {
	if (result) {
	  Serial.println("success\n");
	} else {
	  Serial.println("fail\n");
	}
      }
    }
    
    //------------------------------------------
    //disconnect before leaving
    if (disconnect) {
      _client.disconnect();
    }

  }
  
  return result;
}
