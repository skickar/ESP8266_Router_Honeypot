
// NAPT example released to public domain

#if LWIP_FEATURES && !LWIP_IPV6

#define HAVE_NETDUMP 0

#ifndef STASSID
#define STASSID "Your_Wifi_Network_Name" // set the SSID (name) of the Wi-Fi network the ESP8266 will connect to for internet
#define STAPSK  "Your_Wifi_Network_Password" // set the password of the Wi-Fi network the ESP8266 will connect to for internet
#define NEWSSID  "honeypot_Wifi_Name" // set the name (SSID) of the Wi-Fi network the ESP8266 will create
#define NEWPASS  "honeypot_Wifi_Password" // set the password of the Wi-Fi network the ESP8266 will create
#endif

#include <ESP8266WiFi.h>
#include <lwip/napt.h>
#include <lwip/dns.h>
#include <dhcpserver.h>
#include <ESPCanary.h>

#define NAPT 1000
#define NAPT_PORT 10

#if HAVE_NETDUMP

#include <NetDump.h>

void dump(int netif_idx, const char* data, size_t len, int out, int success) {
  (void)success;
  Serial.print(out ? F("out ") : F(" in "));
  Serial.printf("%d ", netif_idx);

  // optional filter example: if (netDump_is_ARP(data))
  {
    netDump(Serial, data, len);
    //netDumpHex(Serial, data, len);
  }
}
#endif

String canary = "Your_Canarytoken_URL";  //grab FREE web bug/URL tokens at http://canarytokens.org
String ftp_user = "admin";    //if you replace this with "%" it will accept ANY username
String ftp_pass = "password"; //if you replace this with "%" it will accept ANY password
bool append_ip = false;       //if you are using a canary token, leave this as false
String append_char = "?";     //if you are using a canary token, this doesn't matter
                              //if you are using your own webhook,with a bunch of GET
                              //parameters then you would want this to be "&" so the IP
                              //address becomes the final GET parameter

FtpServer ftpSrv;   //set #define FTP_DEBUG in ESPCanary.h to see ftp verbose on serial


void setup() {
  Serial.begin(115200);
  Serial.printf("\n\nNAPT Range extender\n");
  Serial.printf("Heap on start: %d\n", ESP.getFreeHeap());

#if HAVE_NETDUMP
  phy_capture = dump;
#endif

  // first, connect to STA so we can get a proper local DNS server
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.printf("\nSTA: %s (dns: %s / %s)\n",
                WiFi.localIP().toString().c_str(),
                WiFi.dnsIP(0).toString().c_str(),
                WiFi.dnsIP(1).toString().c_str());

  // give DNS servers to AP side
  dhcps_set_dns(0, WiFi.dnsIP(0));
  dhcps_set_dns(1, WiFi.dnsIP(1));

  WiFi.softAPConfig(  // enable AP, with android-compatible google domain
    IPAddress(172, 217, 28, 254),
    IPAddress(172, 217, 28, 254),
    IPAddress(255, 255, 255, 0));
  WiFi.softAP(NEWSSID, NEWPASS);
  Serial.printf("AP: %s\n", WiFi.softAPIP().toString().c_str());

  Serial.printf("Heap before: %d\n", ESP.getFreeHeap());
  err_t ret = ip_napt_init(NAPT, NAPT_PORT);
  Serial.printf("ip_napt_init(%d,%d): ret=%d (OK=%d)\n", NAPT, NAPT_PORT, (int)ret, (int)ERR_OK);
  if (ret == ERR_OK) {
    ret = ip_napt_enable_no(SOFTAP_IF, 1);
    Serial.printf("ip_napt_enable_no(SOFTAP_IF): ret=%d (OK=%d)\n", (int)ret, (int)ERR_OK);
    if (ret == ERR_OK) {
      Serial.printf("WiFi Network '%s' with password '%s' is now NATed behind '%s'\n", NEWSSID, NEWPASS, STASSID);
    }
  }
  Serial.printf("Heap after napt init: %d\n", ESP.getFreeHeap());
  if (ret != ERR_OK) {
    Serial.printf("NAPT initialization failed\n");
  }

  /////FTP Setup, ensure SPIFFS is started before ftp;  /////////
    if (SPIFFS.begin()) {
        Serial.println("SPIFFS opened!");
        ftpSrv.begin(ftp_user,ftp_pass,canary,append_ip,append_char);    //username, password for ftp.  set ports in ESPCanary.h  (default 21, 50009 for PASV)
    }
}

#else

#error "NAPT not supported in this configuration"

void setup() {
  Serial.begin(115200);
  Serial.printf("\n\nNAPT not supported in this configuration\n");
}

#endif

void loop() {
  ftpSrv.handleFTP(); 
}
