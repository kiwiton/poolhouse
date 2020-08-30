#include <DHT.h>
#include <DHT_U.h>

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
//#include <DHT.h>
/*used pins: 
  2,6,7,9,10,rx,tx
  3<= heat
  4<= dht11
  5<= button
  A0<= vent
  A1<= ldr
  A2<= lightI
  A3<= lightO
  A4<= fan
*/

#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

int heat = 3;     //pin connected to the heat source
int button= 5;    //pin connected to the light switch 
int vent = A0;    //pin connected to the ventilator
int ldr = A1;     //pin connected to the light sensor
int lightI = A2;  //pin connected to the lights inside
int lightO = A3;  //pin connected to the lights outside
//int fan = A4;   //pin connected to the fan  

/* vars for the light switch*/
int buttState = 0;
int ledInState = 0;
int prevButtState = 1;

int ledOutState = 0;
int ventState = 0;
int heatState = 0;


float temperature,humidity;      
float tem,hum;

unsigned int count = 1;        //For times count

static const PROGMEM u1_t NWKSKEY[16] = { 0xC1, 0x58, 0xDF, 0x8C, 0xB4, 0x4F, 0xC2, 0xAE, 0x27, 0x38, 0x39, 0xBC, 0x01, 0x39, 0x56, 0x62 }; 
static const u1_t PROGMEM APPSKEY[16] = { 0xFE, 0x1A, 0x9F, 0xA1, 0x6A, 0x50, 0x43, 0xFF, 0xE8, 0x29, 0x6A, 0xED, 0x4C, 0x77, 0xD1, 0x82 };
static const u4_t DEVADDR = 0x26011C69; // <-- Change this address for every node!

void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

//uint8_t mydata[7] = {0x03,0x67,0x00,0x00,0x04,0x68,0x00};
uint8_t mydata[26] = {0x03,0x67,0x00,0x00,0x04,0x68,0x00,0x05,0x65,0x00,0x00,0x06,0x00,0x00,0x07,0x01,0x00,0x08,0x01,0x00,0x09,0x01,0x00,0x0a,0x01,0x00};
                    //dht /temp          /hum           /ldr                /button         /heat          /vent          /lightI        /lightO

static osjob_t initjob,sendjob,blinkjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 10;

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 6, 7},
};
void do_send(osjob_t* j){

       dhtTem();
    /*lpp.reset();
    lpp.addTemperature(1,22.5);
    mydata = lpp ;*/

    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println("OP_TXRXPEND, not sending");
    }else 
    {
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, mydata, sizeof(mydata), 0);

        Serial.println("Packet queued");
        Serial.println(LMIC.freq);
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    Serial.println(ev);
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println("EV_SCAN_TIMEOUT");
            break;
        case EV_BEACON_FOUND:
            Serial.println("EV_BEACON_FOUND");
            break;
        case EV_BEACON_MISSED:
            Serial.println("EV_BEACON_MISSED");
            break;
        case EV_BEACON_TRACKED:
            Serial.println("EV_BEACON_TRACKED");
            break;
        case EV_JOINING:
            Serial.println("EV_JOINING");
            break;
        case EV_JOINED:
            Serial.println("EV_JOINED");
            break;
        case EV_RFU1:
            Serial.println("EV_RFU1");
            break;
        case EV_JOIN_FAILED:
            Serial.println("EV_JOIN_FAILED");
            break;
        case EV_REJOIN_FAILED:
            Serial.println("EV_REJOIN_FAILED");
            break;
        case EV_TXCOMPLETE:
            Serial.println("EV_TXCOMPLETE (includes waiting for RX windows)");
            if(LMIC.dataLen) {
                // data received in rx slot after tx
                Serial.print("Data Received: ");
                Serial.write(LMIC.frame+LMIC.dataBeg, LMIC.dataLen);
                Serial.println();
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println("EV_LOST_TSYNC");
            break;
        case EV_RESET:
            Serial.println("EV_RESET");
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println("EV_RXCOMPLETE");
            break;
        case EV_LINK_DEAD:
            Serial.println("EV_LINK_DEAD");
            break;
        case EV_LINK_ALIVE:
            Serial.println("EV_LINK_ALIVE");
            break;
         default:
            Serial.println("Unknown event");
            break;
    }
}

void dhtTem()
{
  int16_t tem_LPP;  //var used for temp
  int16_t lux_LPP;  //var used for light intensity


  temperature = dht.readTemperature();    //Read Tmperature data
  tem = dht.readTemperature()*1.0;      
  humidity = dht.readHumidity();      //Read humidity data
  hum = dht.readHumidity() *1.0; 
  
  Serial.print("###########    ");
  Serial.print("COUNT=");
  Serial.print(count);
  Serial.println("    ###########");            
  Serial.println(F("The temperature and humidity:"));
  Serial.print("[");
  Serial.print(dht.readTemperature());//Serial.print(tem);
  Serial.print("℃");
  Serial.print(",");
  Serial.print(dht.readHumidity());//Serial.print(hum);
  Serial.print("%");
  Serial.print("]");
  Serial.println("");

  //if the bmp reads a temperature of 28 or higher a led will light up and the ventilator starts spinning
  if(dht.readTemperature() > 27){
    analogWrite(vent, 255);         //turn the vent on
    //analogWrite(fan, 255);         //turn the fan on
    digitalWrite( heat, LOW);       //turn the heat off
    heatState = 0;
    ventState= 1;
    Serial.println("heat off");       
    Serial.println("vent on");  
      
  }else{
    analogWrite(vent, 0);           //turn the vent off
    //analogWrite(fan, 0);           //turn the fan off
    digitalWrite( heat ,HIGH);      //turn the heat on
    heatState = 1;
    ventState = 0;
    Serial.println("vent off");
    Serial.println("heat on");

  }

  /*code for the ldr to automaticly control the outside lights*/
  if (analogRead(ldr) < 350){
    analogWrite(lightO, 255);     //turn the light outside on
    ledOutState = 1;
    Serial.println("lightO = on, it's dark ");   

  }else{
    analogWrite(lightO, 0);      //turn the light outside off
    ledOutState = 0;
    Serial.println("lightO = off");

  }

  Serial.println(analogRead(ldr));

  /* code for button to control the light inside*/

  if (digitalRead(button) == LOW && prevButtState == HIGH)
  {
    if(ledInState == 1){
      ledInState = 0;
      buttState = 0;

    }else{
      ledInState = 1;
      buttState = 1;

    }
  }
  digitalWrite(lightI, ledInState);

 /*dit is het stukje voor de dht data door te sturen */ 
  count++;
  tem_LPP = tem * 10; 
  lux_LPP = analogRead(ldr);

  mydata[2] = tem_LPP>>8;
  mydata[3] = tem_LPP;
  mydata[6] = hum * 2;

  mydata[9] = lux_LPP>>8;
  mydata[10] = lux_LPP;

  mydata[13] = buttState;

  mydata[16] = heatState;

  mydata[19] = ventState;

  mydata[22] = ledInState;

  mydata[25] = ledOutState;
  
}

void setup() {

  pinMode(vent, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(ldr, INPUT);
  pinMode(heat, OUTPUT);
  pinMode(lightI, OUTPUT);
  pinMode(lightO, OUTPUT);
  //pinMode(fan, OUTPUT);

    Serial.begin(9600);
    dht.begin();

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(false);

    
    if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(("Failed to read from DHT sensor!"));
    return;
    }

    
    while(!Serial);
    Serial.println("Starting");
    #ifdef VCC_ENABLE
    // For Pinoccio Scout boards
    pinMode(VCC_ENABLE, OUTPUT);

    
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
    #endif

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();
    //LMIC_setClockError(MAX_CLOCK_ERROR * 1/100);
    // Set static session parameters. Instead of dynamically establishing a session
    // by joining the network, precomputed session parameters are be provided.
    #ifdef PROGMEM
    // On AVR, these values are stored in flash and only copied to RAM
    // once. Copy them to a temporary buffer here, LMIC_setSession will
    // copy them into a buffer of its own again.
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
    #else
    // If not running an AVR with PROGMEM, just use the arrays directly 
    LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
    #endif
    
    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF7;
    
    // Set data rate and transmit power (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF7,14);

    // Start job
    do_send(&sendjob);
}

void loop() {
    os_runloop_once();
}
