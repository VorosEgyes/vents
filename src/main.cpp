  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>

  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>



  AsyncWebServer server(80);


  const int fan1Pin = D7;
  const int fan2Pin = D6;
  const int fan3Pin = D5;

  // Global variables to handle the fan state and timer
  unsigned long turnOffTime = 0; // The time the fan should turn off
  int fanSpeeds[3] = {0, 0, 0};
  // This function updates the fan's speed and sets the timer
  
  void handleFanUpdate(int fan, int value, int hours = 0) {
    // Convert percentage to actual fan speed value
    
    value = constrain(value, 0, 100);
    int fanSpeed = (value * 255) / 100;
    fanSpeeds[fan - 1] = value;

    // If needed, uncomment the following line to invert PWM:
    // fanSpeed = 1023 - fanSpeed;

    switch (fan) {
        case 1: analogWrite(fan1Pin, fanSpeed); break;
        case 2: analogWrite(fan2Pin, fanSpeed); break;
        case 3: analogWrite(fan3Pin, fanSpeed); break;
    }

    // If a timer is set, calculate the future turn-off time
    if (hours > 0) {
        turnOffTime = millis() + hours * 3600000; // hours to milliseconds
    } else if (hours == 0) { // If the button is OFF
        turnOffTime = ULONG_MAX; // Set to maximum value, fans won't turn off automatically
    }
}


  void setup() {
      analogWriteFreq(20000); // Set PWM frequency to 25kHz
      pinMode(fan1Pin, OUTPUT);
      pinMode(fan2Pin, OUTPUT);
      pinMode(fan3Pin, OUTPUT);

      Serial.begin(115200);

      const char* ssid = ""; // TO BE FILLED
      const char* password = ""; // TO BE FILLED

      WiFi.begin(ssid, password);

      while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
      }

      Serial.println("");
      Serial.println("WiFi connected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());

      // OTA setup
      ArduinoOTA.onStart([]() {
          String type;
          if (ArduinoOTA.getCommand() == U_FLASH) {
              type = "sketch";
          } else {
              type = "filesystem";
          }
          Serial.println("Start updating " + type);
      });
      ArduinoOTA.onEnd([]() {
          Serial.println("\nEnd");
      });
      ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
          Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      });
      ArduinoOTA.onError([](ota_error_t error) {
          Serial.printf("Error[%u]: ", error);
          if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
          else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
          else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
          else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
          else if (error == OTA_END_ERROR) Serial.println("End Failed");
      });

      ArduinoOTA.begin();


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  String html = R"=====(
  <html>
  <head>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <style>
          body {
              font-family: Arial, sans-serif;
          }

          button {
              display: inline-block;
              margin: 5px;
              padding: 15px 25px;
              font-weight: bold;
              border: none;
              cursor: pointer;
              border-radius: 5px;
              box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.3);
              font-size: 16px;
          }
          .active {
              background-color: #4CAF50;
              color: white;
          }

          .fan-container {
              margin: 15px 0;
              border: 1px solid #ddd;
              padding: 10px;
              border-radius: 5px;
              box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.1);
              display: flex;
              flex-wrap: wrap;
              justify-content: space-between;
              align-items: center;
          }

          .fan-container strong {
              flex: 100%;
              margin: 10px 0;
              font-size: 18px;
          }

      </style>
  </head>
  <body>
      <div class="fan-container" data-fan="1">
          <strong>Fan 1:</strong> 
          <button data-value="0" onclick="updateFan(1, 0); setActiveButton(1, 0);">0%</button>
          <button data-value="30" onclick="updateFan(1, 30); setActiveButton(1, 30);">30%</button>
          <button data-value="50" onclick="updateFan(1, 50); setActiveButton(1, 50);">50%</button>
          <button data-value="70" onclick="updateFan(1, 70); setActiveButton(1, 70);">70%</button>
          <button data-value="90" onclick="updateFan(1, 90); setActiveButton(1, 90);">90%</button>
          <button data-value="100" onclick="updateFan(1, 100); setActiveButton(1, 100);">100%</button>
      </div>

      <div class="fan-container" data-fan="2">
          <strong>Fan 2:</strong>
          <button data-value="0" onclick="updateFan(2, 0); setActiveButton(2, 0);">0%</button>
          <button data-value="30" onclick="updateFan(2, 30); setActiveButton(2, 30);">30%</button>
          <button data-value="50" onclick="updateFan(2, 50); setActiveButton(2, 50);">50%</button>
          <button data-value="70" onclick="updateFan(2, 70); setActiveButton(2, 70);">70%</button>
          <button data-value="90" onclick="updateFan(2, 90); setActiveButton(2, 90);">90%</button>
          <button data-value="100" onclick="updateFan(2, 100); setActiveButton(2, 100);">100%</button>
          </div>

      <div class="fan-container" data-fan="3">
          <strong>Fan 3:</strong>
          <button data-value="0" onclick="updateFan(3, 0); setActiveButton(3, 0);">0%</button>
          <button data-value="30" onclick="updateFan(3, 30); setActiveButton(3, 30);">30%</button>
          <button data-value="50" onclick="updateFan(3, 50); setActiveButton(3, 50);">50%</button>
          <button data-value="70" onclick="updateFan(3, 70); setActiveButton(3, 70);">70%</button>
          <button data-value="90" onclick="updateFan(3, 90); setActiveButton(3, 90);">90%</button>
          <button data-value="100" onclick="updateFan(3, 100); setActiveButton(3, 100);">100%</button>
    
      </div>
     <div class="timer-container">
              <strong>Timer:</strong>
              <button data-value="0" onclick="setTimer(0); setActiveTimer(0);">OFF</button>
              <button data-value="1" onclick="setTimer(1); setActiveTimer(1);">1H</button>
              <button data-value="2" onclick="setTimer(2); setActiveTimer(2);">2H</button>
              <button data-value="3" onclick="setTimer(3); setActiveTimer(3);">4H</button>
              <button data-value="4" onclick="setTimer(4); setActiveTimer(4);">8H</button>
      </div>


      <script>
          document.addEventListener("DOMContentLoaded", function() {
              [1, 2, 3].forEach(fan => {
                  getCurrentFanSpeedSync(fan);
              });
              initializeActiveState();
          });

          function getCurrentFanSpeedSync(fan) {
              return fetch('/getFanSpeed?fan=' + fan)
                  .then(response => response.text())
                  .then(speed => {
                      updateFanSpeedDisplay(fan, speed);
                      return speed;
                  });
          }
          function updateFanSpeedDisplay(fan, speed) {
              setActiveButton(fan, speed);
          }
          function initializeActiveState() {
              // Assuming all fans start at 0% and the timer is off
              document.querySelector('.fan-container[data-fan="1"] button[data-value="0"]').classList.add('active');
              document.querySelector('.fan-container[data-fan="2"] button[data-value="0"]').classList.add('active');
              document.querySelector('.fan-container[data-fan="3"] button[data-value="0"]').classList.add('active');
              document.querySelector('.timer-container button[data-value="0"]').classList.add('active');
          }

          function setActiveButton(fan, value) {
              // First, remove 'active' from all buttons for this fan
              document.querySelectorAll(`.fan-container[data-fan="${fan}"] button`).forEach(btn => {
                  btn.classList.remove('active');
              });
              // Set the active button for the provided value
              document.querySelector(`.fan-container[data-fan="${fan}"] button[data-value="${value}"]`).classList.add('active');
          }

          function setActiveTimer(hours) {
              // First, remove 'active' from all timer buttons              
              document.querySelectorAll('.timer-container button').forEach(btn => {
                  btn.classList.remove('active');
              });
              // Set the active button for the provided timer value
              document.querySelector(`.timer-container button[data-value="${hours}"]`).classList.add('active');
          }



         function markActive(element, containerClass) {
          // Remove 'active' class from all buttons in the container
          document.querySelectorAll(`.${containerClass} button`).forEach(btn => {
              btn.classList.remove('active');
          });
          // Add 'active' class to the clicked button
          element.classList.add('active');
          }

          function updateFan(fan, value, hours = 0) {
              fetch('/setfan?fan=' + fan + '&value=' + value + '&hours=' + hours);
          }

          async function setTimer(hours) {
              for (let fan of [1, 2, 3]) {
                  let speed = await getCurrentFanSpeedSync(fan);
                  updateFan(fan, speed, hours);
              }
          }
          function getCurrentFanSpeedSync(fan) {
            return fetch('/getFanSpeed?fan=' + fan)
            .then(response => response.text())
            .then(speed => {
                updateFanSpeedDisplay(fan, speed);
                return speed;
          });
          }
          function getCurrentFanSpeed(fan) {
              return 0;  // Placeholder, modify based on actual implementation
          }
      </script>
  </body>
  </html>
  )=====";


      request->send(200, "text/html", html);
  });

server.on("/getFanSpeed", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!request->hasParam("fan")) {
        request->send(400, "text/plain", "Bad Request: Missing Parameters");
        return;
    }
    
    int fan = request->getParam("fan")->value().toInt();
    request->send(200, "text/plain", String(fanSpeeds[fan - 1]));
});

  server.on("/setfan", HTTP_GET, [](AsyncWebServerRequest *request){
      if (!request->hasParam("fan") || !request->hasParam("value") || !request->hasParam("hours")) {
          request->send(400, "text/plain", "Bad Request: Missing Parameters");
          return;
      }
      
      int fan = request->getParam("fan")->value().toInt();
      int value = request->getParam("value")->value().toInt(); // Fan speed (0-100)
      int hours = request->getParam("hours")->value().toInt(); // Timer setting in hours

      handleFanUpdate(fan, value, hours);

      request->send(200, "text/plain", "OK");
  });

  server.begin();

  }

void loop() {
    ArduinoOTA.handle();

    // Check if the current time has surpassed the turn-off time
    if (turnOffTime != ULONG_MAX && millis() > turnOffTime) {
        // Time has passed, turn off fans
        handleFanUpdate(1, 0, 0);
        handleFanUpdate(2, 0, 0);
        handleFanUpdate(3, 0, 0);

        turnOffTime = ULONG_MAX; // Reset the timer
    }
}

