# ESP8266_Router_Honeypot
A NAT router with an FTP honeypot using a canarytoken by @spacehuhn and @kodykinzie based on the espcanary library.

<h1>Requirements:</h1>

* Wi-Fi network with internet to connect the ESP.

* Arduino IDE with the following URL added under Preferences -> Additional board manager URL's: http://arduino.esp8266.com/stable/package_esp8266com_index.json

* ESP8266 boards added in Arduino IDE by going to Tools -> Boards -> Boards Manager, and installing ESP8266 by ESP8266 Community

* ESPcanary library installed in Arduino IDE by going to Sketch -> Include Library -> Manage Libraries and installing "ESPcanary" by Dan Hoover



<h1>Setup:</h1>


Git clone or download the ZIP file from this GitHub repo and open the ESP8266_Router_Honeypot.ino sketch in Arduino IDE.
Replace the variables below with the Wi-Fi network information:

![Alt text](https://i.imgur.com/JIVvYhe.png "Update these")

#define STASSID "Your_Wifi_Network_Name" // set the SSID (name) of the Wi-Fi network the ESP8266 will connect to for internet

#define STAPSK  "Your_Wifi_Network_Password" // set the password of the Wi-Fi network the ESP8266 will connect to for internet

#define NEWSSID  "honeypot_Wifi_Name" // set the name (SSID) of the Wi-Fi network the ESP8266 will create

#define NEWPASS  "honeypot_Wifi_Password" // set the password of the Wi-Fi network the ESP8266 will create


Next, got to Canarytokens.org and create a "Web bug" token. Copy the URL and paste it in the part of the code below, while setting the username and password for the honeypot FTP server:

![Alt text](https://i.imgur.com/UY83p5k.png "Update these too")

String canary = "Your_Canarytoken_URL";  //grab FREE web bug/URL tokens at http://canarytokens.org

String ftp_user = "admin";    //if you replace this with "%" it will accept ANY username

String ftp_pass = "password"; //if you replace this with "%" it will accept ANY password


Now, plug in your ESP8266 based device and select the correct board under Tools -> Board. Select the serial port the board is attached to under Tools -> Port.

Finally, press the green arrow to flash the software to the board.

Once it's done, open the serial monitor and set the baud rate to 115200, then press the reset button to watch the board boot. 
                              
   


