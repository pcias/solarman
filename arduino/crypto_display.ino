#include <LiquidCrystal.h>
#include <Process.h>
#include <ArduinoJson.h>
#include <avr/wdt.h>
// #include <Console.h>

#define LOGFREQ 600000
#define ACTFREQ 10000

long int lastmilis = -999999999;

String realtime = "Wait for";
String today = "hikopotam.net";
String btc;
String eth;
String sol;

long int counter = 0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // konfigurowanie linii do których został dołączony LCD

void setup()
{
    Bridge.begin();

    lcd.begin(16, 2);
    // lcd.noAutoscroll();
    lcd.setCursor(0, 0);
    lcd.print("hikopotam.net BTC");
    wdt_enable(WDTO_8S);
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
        btc = get_asset("bitcoin");
        eth = get_asset("ethereum");
        // sol = get_asset("solana"); //solana not available in coincap for the moment

        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print(btc);

        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(eth);
    }
}

String get_asset(String asset)
{
    StaticJsonDocument<500> doc;
    String json = "";
    String result = "";

    String ret = "";

    Process p;

    p.begin("curl");
    // p.addParameter("-u");
    // p.addParameter(String(USERNAME)+":"+PASSWORD);
    p.addParameter("-k");
    p.addParameter("https://api.coincap.io/v2/assets/" + asset);
    p.run();
    // Print command output on the Serial.
    // A process output can be read with the stream methods
    json = "";
    int i = 0;
    while (p.available() > 0 && i < 500)
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
    const char *priceUSD = data["priceUsd"]; // "6931.5058555666618359"
    const char *changePercent24Hr = data["changePercent24Hr"];
    const char *volumeUsd24Hr = data[" volumeUsd24Hr"];

    result = priceUSD;
    ret = result.substring(0, 7);
    result = changePercent24Hr;
    ret = ret + " " + result.substring(0, 5);

    return (ret);
}