#include <pgmspace.h>
 
#define SECRET
#define THINGNAME "QTBin"                         

const char WIFI_SSID[] = "Verizon_J9SPGP";            
const char WIFI_PASSWORD[] = "cab3-azure-plot";           

const char AWS_IOT_ENDPOINT[] = "";

 
// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----END CERTIFICATE-----
 
 
)KEY";
 
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
)KEY";
