////////////////////////////////////////////////////////////
// NODE 1 (Float)
// NODE 2 (TMP/HUM)
// This is a float sensor with Tmp/Hum sensor that when tripped will send 
// information to a second and third node to activate a piezo 
// speaker alarm, as well as to the controller.
//
// Sensor pin in D3 (float sensor)
// Sensor pin in D4 (hum/tmp sensor)
// Alarm Nodes ID's are 1 and 2
//
//////////////////////////////////////////////////////////////


#include <MySensor.h>            // include MySensor library
#include <SPI.h>                 // incldue SPI library

unsigned long SLEEP_TIME = 10000; // 30 second sleep

// Float sensor
#define DIGITAL_INPUT_SENSOR 3   // pin number of float sensor
#define S_WATER_LEAK 1           // childSensor ID of float (static)

// Piezo sensor
#define PIEZO_RADIO_ID 1         // ID of recieving node (static)

MySensor gw;    // start gw library
MyMessage msgFloat(PIEZO_RADIO_ID, V_TRIPPED);   // send message to radio 1
MyMessage msg(S_WATER_LEAK, V_TRIPPED); // send message to controller
//
//
// runs once
void setup() {
/* Gateway begin                   
 * incomingMessageCallback = callback function for incoming messages from other nodes or controller and request response. Default is NULL 
 * nodeID = The unique ID (1-254) for this sensor. Default is AUTO(255) which means sensor tries to fetch an ID from controller 
 * repeaterMode = Activate repeater mode. This node will forward messages to other nodes in the radio network. Default is FALSE
 * parentNodeID = Use this to force node to always communicate with a certain parent node. Default is AUTO, which means node automatically tries to find parent.
 */
//  gw.begin(NULL, 1, false, 2);
gw.begin();
//Send sketch Version Information to the Gateway
  gw.sendSketchInfo("Float", "1.0");
// Set pin for Float sensor
  pinMode(DIGITAL_INPUT_SENSOR, INPUT);
// Present sensors to controller (childSensorID, sensorType, description, ack)
  gw.present(S_WATER_LEAK, V_TRIPPED);
}
//
//
//
// loops over and over
void loop() {
// Setting variable to sensor status
  boolean tripped = digitalRead(DIGITAL_INPUT_SENSOR);
// send message to node 1 (piezzo buzzers)
  gw.send(msgFloat.setDestination(1).set(tripped?"1":"0"));
// send message to node 2 (piezzo buzzer)
  gw.send(msgFloat.setDestination(2).set(tripped?"1":"0"));
  // send message to controller
  gw.send(msg.set(tripped?"1":"0"));
// debuging to Serial
  Serial.print("The float is ");
  Serial.println(tripped);

//sleep a while
  gw.sleep(SLEEP_TIME);
}
