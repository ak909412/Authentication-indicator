Authentication Indicator 

Introduction 

The Authentication Indicator project aims to provide a visual indication to users regarding the acceptance or denial of their access to a meeting room. This project utilizes the NodeMCU ESP8266 module and LED strips to display the access status. It involves a combination of hardware and software components, including the Arduino IDE for programming the ESP8266 and a React Native application for user interaction. 

Hardware Components 

NodeMCU ESP8266 Module 

LED Strips 

Software Components 

Arduino IDE 

React Native 

Process 

User Access Attempt: 

The user attempts to access a meeting room by providing their credentials. 

Database Authentication: 

The database processes the user's credentials to determine whether access is allowed or denied. 

Application Action: 

The React Native application communicates with the server based on the database response. 

If access is allowed, the application sends a command to the server. 

If access is denied, the application sends a different command. 

Server Communication: 

The application posts the appropriate command to either an MQTT (Message Queuing Telemetry Transport) server or ThingSpeak server, depending on the response from the database. 

ESP8266 Interaction: 

The NodeMCU ESP8266 module fetches the command from the server, which was sent by the React Native application. 

LED Indicator: 

The ESP8266 controls the LED strips to visually indicate the access status to the user. 

If access is accepted, the LED strips may display a green light. 

If access is denied, the LED strips may display a red light or a different visual pattern to signify denial. 

Conclusion 

The Authentication Indicator project provides a clear and user-friendly way to inform individuals about the acceptance or denial of access to a meeting room. By combining hardware components like the NodeMCU ESP8266 and LED strips with software components like the Arduino IDE and React Native, this project offers an effective solution for access control and indication. 

 
 

 
