#include "LIS3DH.h"
#include "blynk.h"

SYSTEM_THREAD(ENABLED);

// Print 10 samples per second to serial
const unsigned long PRINT_SAMPLE_PERIOD = 100;

LIS3DHSPI accel(SPI, D3, WKP);

unsigned long lastPrintSample = 0;

void setup()
{
  Serial.begin(9600);

  //delay(5000);

  // Initialize sensors
  LIS3DHConfig config;
  config.setAccelMode(LIS3DH::RATE_100_HZ);

  bool setupSuccess = accel.setup(config);
  Serial.printlnf("setupSuccess=%d", setupSuccess);

  Blynk.begin("bwumTrfAurZd8Xur-n1i9kLiC9EeaM_m", IPAddress(167, 172, 234, 162), 9090); // connecting to blynk server
}

void loop()
{

  // init blynk
  Blynk.run();

  if (millis() - lastPrintSample >= PRINT_SAMPLE_PERIOD) // gets 10 samples/sec
  {
    lastPrintSample = millis();

    LIS3DHSample sample;
    if (accel.getSample(sample))
    {
      Serial.printlnf("%d,%d,%d", sample.x, sample.y, sample.z);
      Blynk.virtualWrite(V0, sample.x);
      Blynk.virtualWrite(V1, sample.y);
      Blynk.virtualWrite(V2, sample.z);
    }
    else
    {
      Serial.println("no sample");
    }
  }
}
