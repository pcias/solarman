// Copyright (C) 2024 (PCS) Przemysław Ciaś
//
#include <LiquidCrystal.h>
#include <Process.h>
#include <ArduinoJson.h>
#include <avr/wdt.h>
// #include <Console.h>

#define LOGFREQ 600000
#define ACTFREQ 10000

long int lastmilis = -999999999;

// #define DOMUS_R2_URL "https://domus_r2-c9-pcias.c9.io"
// #define DOMUS_R2_URL "http://pcsbotaniczna.no-ip.org:3000"
// #define DOMUS_R2_URL "http://95.85.2.233:8080"
#define SOLARPROXY_URL "http://192.168.1.20:3000"
#define SENSOR_ID "YUN_LCD_Controller"

String realtime = "Wait for";
String today = "hikopotam.net";
String btc = "......";

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // konfigurowanie linii do których został dołączony LCD

const int Led1 = 13; // przypisanie aliasów do pinów portów
const int Led2 = 12;
const int Led3 = 11;
const int Led4 = 10;
const int SW1 = 3;
const int SW2 = 2;
const int SW3 = 1;
const int SW4 = 0;
const int Buzzer = A5;

void setup()
{
    Bridge.begin();
    // Console.begin();

    // while (!Console) {
    //     ; // wait for Console port to connect.
    //  }

    lcd.begin(16, 2);
    // lcd.autoscroll();
    lcd.setCursor(0, 0);
    lcd.print("hikopotam.net");

    pinMode(Led1, OUTPUT); // konfigurowanie I/O, do których są
    // dołączone diody LED
    pinMode(Led2, OUTPUT);
    pinMode(Led3, OUTPUT);
    pinMode(Led4, OUTPUT);
    pinMode(Buzzer, OUTPUT); // konfigurowanie I/O, do której jest
    // dołączony brzęczyk piezzo
    pinMode(SW1, INPUT); // konfigurowanie I/O, do których są
    // dołączone przyciski
    pinMode(SW2, INPUT);
    pinMode(SW3, INPUT);
    pinMode(SW4, INPUT);

    digitalWrite(Led1, HIGH);
    digitalWrite(Led2, HIGH);
    digitalWrite(Led3, HIGH);
    digitalWrite(Led4, HIGH);

    digitalWrite(Buzzer, HIGH);

    wdt_enable(WDTO_8S);

    // attachInterrupt(0, markGateToOpen, FALLING);  //pin 2 = SW2
}

String linecom;

void loop()
{
    // send commands up/temp
    long int m;
    m = millis();
    wdt_reset();

    if (m - lastmilis > ACTFREQ)
    {

        lastmilis = m;
        getRealtime();
        getToday();
        getBTC();

        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print(realtime);

        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(today + "  " + btc);
    }

    if (!(m % 100))
    {
        //    if(digitalRead(SW1) == LOW) {
        //      manTemp>MINTEMP? manTemp-- : 10;
        //      //delay(50);
        //      tempSource = MANTEMP;
        //      //sendTempSource();
        //      dispTemp();
        //    }
        //    if(digitalRead(SW2) == LOW) {
        //      manTemp<MAXTEMP? manTemp++ : 30;
        //      tempSource = MANTEMP;
        //      //sendTempSource();
        //      dispTemp();
        //    }
        if (digitalRead(SW4) == LOW)
        {
            //      displayStr = "Will be away...";
            //      dispStr();
            //      delay(100);
        }
    }
}

void getRealtime()
{
    Process p;
    // digitalWrite(Led2, LOW);
    p.begin("curl");
    // p.addParameter("-u");
    // p.addParameter(String(USERNAME)+":"+PASSWORD);
    p.addParameter("-k");
    p.addParameter(String(SOLARPROXY_URL) + "/realtime");
    p.run();
    // Print command output on the Serial.
    // A process output can be read with the stream methods
    realtime = "";
    int i = 0;
    while (p.available() > 0 && i < 32)
    {
        char c = p.read();
        if (c != '\"' && c != '\\')
            realtime += c;
        i++;
    }
    // digitalWrite(Led2, HIGH);
}

void getToday()
{
    Process p;
    // digitalWrite(Led2, LOW);
    p.begin("curl");
    // p.addParameter("-u");
    // p.addParameter(String(USERNAME)+":"+PASSWORD);
    p.addParameter("-k");
    p.addParameter(String(SOLARPROXY_URL) + "/today");
    p.run();
    // Print command output on the Serial.
    // A process output can be read with the stream methods
    today = "";
    int i = 0;
    while (p.available() > 0 && i < 32)
    {
        char c = p.read();
        if (c != '\"' && c != '\\')
            today += c;
        i++;
    }
    // digitalWrite(Led2, HIGH);
}

void getBTC()
{
    StaticJsonDocument<512> doc;
    String json = "";

    Process p;
    // digitalWrite(Led2, LOW);
    p.begin("curl");
    // p.addParameter("-u");
    // p.addParameter(String(USERNAME)+":"+PASSWORD);
    p.addParameter("-k");
    p.addParameter("https://api.coincap.io/v2/assets/bitcoin");
    p.run();
    // Print command output on the Serial.
    // A process output can be read with the stream methods
    json = "";
    int i = 0;
    while (p.available() > 0 && i < 512)
    {
        char c = p.read();

        json += c;
        i++;
    }

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, json);

    if (error)
    {
        // Console.print(F("deserializeJson() failed: "));
        // Console.println(error.f_str());
        // today=error.f_str();
        today = json.substring(0, 10);
        return;
    }

    JsonObject data = doc["data"];
    // const char* data_priceUsd = data["priceUsd"]; // "6931.5058555666618359"
    const char *result = data["priceUsd"]; // "6931.5058555666618359"
    btc = result;
}