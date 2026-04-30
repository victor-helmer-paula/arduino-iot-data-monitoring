#include "sensor_mic.h"

static int16_t micBuffer[MIC_BUFFER_SIZE];
static volatile int samplesRead = 0;

static void onPDMdata() {
  // callback chamado pela lib PDM
  int bytesAvailable = PDM.available();
  PDM.read(micBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2; // 2 bytes por amostra (int16_t)
}

bool micInit() {
  // Configura PDM
  PDM.onReceive(onPDMdata);
  if (!PDM.begin(1, 16000)) { // 1 canal, 16 kHz
    Serial.println("Erro a inicializar microfone!");
    return false;
  }
  Serial.println("Microfone inicializado.");
  return true;
}

void micUpdate() {
  // Aqui não precisamos fazer nada especial;
  // o callback enche micBuffer quando ha dados.
}

void micComputeFeatures(SensorReadings &readings) {
  if (samplesRead <= 0) {
    readings.soundRMS = 0.0f;
    readings.soundPeak = 0.0f;
    return;
  }

  long sumSquares = 0;
  int16_t peak = 0;

  for (int i = 0; i < samplesRead; i++) {
    int16_t v = micBuffer[i];
    sumSquares += (long)v * (long)v;
    if (abs(v) > peak) {
      peak = abs(v);
    }
  }

  float rms = sqrt((float)sumSquares / samplesRead);

  readings.soundRMS = rms;
  readings.soundPeak = (float)peak;

  samplesRead = 0; // marcamos como consumidos
}
