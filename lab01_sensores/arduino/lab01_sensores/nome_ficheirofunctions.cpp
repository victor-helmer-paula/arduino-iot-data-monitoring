#include "functions.h"

#define MAX_WINDOW 50

static SensorReadings buffer[MAX_WINDOW];
static int bufferCount = 0;
static int bufferIndex = 0;

// ============================================================================
// INICIALIZAÇÃO DO SISTEMA
// ============================================================================

void initSystem() {
  if (!imuInit()) {
    Serial.println("Falha na IMU.");
  }
  if (!micInit()) {
    Serial.println("Falha no microfone.");
  }
}

// ============================================================================
// LEITURA DOS SENSORES
// ============================================================================

void readAllSensors(SensorReadings &raw) {
  imuRead(raw);
  micComputeFeatures(raw);
}

// ============================================================================
// MÉDIA MÓVEL SIMPLES
// ============================================================================

void updateMovingAverage(const SensorReadings &raw, SensorReadings &avg, int window) {
  if (window < 1) window = 1;
  if (window > MAX_WINDOW) window = MAX_WINDOW;

  buffer[bufferIndex] = raw;
  bufferIndex = (bufferIndex + 1) % MAX_WINDOW;
  if (bufferCount < MAX_WINDOW) {
    bufferCount++;
  }

  int n = (bufferCount < window) ? bufferCount : window;

  float sax = 0, say = 0, saz = 0;
  float sgx = 0, sgy = 0, sgz = 0;
  float stemp = 0;
  float srms = 0, speak = 0;

  for (int i = 0; i < n; i++) {
    int idx = (bufferIndex - 1 - i);
    if (idx < 0) idx += MAX_WINDOW;

    const SensorReadings &r = buffer[idx];

    sax += r.ax;  say += r.ay;  saz += r.az;
    sgx += r.gx;  sgy += r.gy;  sgz += r.gz;
    stemp += r.temp;
    srms += r.soundRMS;
    speak += r.soundPeak;
  }

  avg.ax = sax / n;
  avg.ay = say / n;
  avg.az = saz / n;

  avg.gx = sgx / n;
  avg.gy = sgy / n;
  avg.gz = sgz / n;

  avg.temp = stemp / n;
  avg.soundRMS = srms / n;
  avg.soundPeak = speak / n;
}

// ============================================================================
// FUNÇÃO CENTRAL DE ENVIO/PROCESSAMENTO (TAREFA 1 + PREPARAÇÃO PARA TAREFA 2)
// ============================================================================
// Esta função é o "ponto de entrada" para enviar dados para diferentes destinos.
// No Trabalho 1: apenas chama printData(...) baseado no modo escolhido.
// Na Tarefa 2: também chamará updateCloudVariables(...) para enviar à Arduino Cloud.

void processAndSendData(unsigned long ts, const SensorReadings &raw, const SensorReadings &avg, int outputMode) {
  // A lógica de envio está aqui centralizada
  printData(ts, raw, avg, outputMode);
  
  // Placeholder para Tarefa 2:
  // if (outputMode == MODE_CLOUD) {
  //   updateCloudVariables(raw, avg);
  // }
}

// ============================================================================
// IMPRESSÃO/VISUALIZAÇÃO DOS DADOS (Serial Monitor ou Plotter)
// ============================================================================

void printData(unsigned long ts, const SensorReadings &raw, const SensorReadings &avg, int outputMode) {
  if (outputMode == MODE_SERIAL_MONITOR) {
    // ========================================================================
    // MODO SERIAL MONITOR: formato detalhado com legendas
    // ========================================================================
    Serial.print("t=");
    Serial.print(ts);
    Serial.print(" ms");

    Serial.print(" | acc_raw[g]: ");
    Serial.print(raw.ax); Serial.print(", ");
    Serial.print(raw.ay); Serial.print(", ");
    Serial.print(raw.az);

    Serial.print(" | acc_avg[g]: ");
    Serial.print(avg.ax); Serial.print(", ");
    Serial.print(avg.ay); Serial.print(", ");
    Serial.print(avg.az);

    Serial.print(" | gyro_raw[dps]: ");
    Serial.print(raw.gx); Serial.print(", ");
    Serial.print(raw.gy); Serial.print(", ");
    Serial.print(raw.gz);

    Serial.print(" | gyro_avg[dps]: ");
    Serial.print(avg.gx); Serial.print(", ");
    Serial.print(avg.gy); Serial.print(", ");
    Serial.print(avg.gz);

    Serial.print(" | temp_raw[C]: ");
    Serial.print(raw.temp);
    Serial.print(" | temp_avg[C]: ");
    Serial.print(avg.temp);

    Serial.print(" | soundRMS_raw: ");
    Serial.print(raw.soundRMS);
    Serial.print(" | soundRMS_avg: ");
    Serial.print(avg.soundRMS);

    Serial.print(" | soundPeak_raw: ");
    Serial.print(raw.soundPeak);
    Serial.print(" | soundPeak_avg: ");
    Serial.println(avg.soundPeak);

  } else if (outputMode == MODE_SERIAL_PLOTTER) {
    // ========================================================================
    // MODO SERIAL PLOTTER: formato tabulado para visualização gráfica
    // ========================================================================
    Serial.print(ts);           Serial.print("\t");
    Serial.print(avg.ax);       Serial.print("\t");
    Serial.print(avg.ay);       Serial.print("\t");
    Serial.print(avg.az);       Serial.print("\t");
    Serial.print(avg.soundRMS); Serial.println();

  } else if (outputMode == MODE_CLOUD) {
    // ========================================================================
    // MODO CLOUD: (reservado para Tarefa 2)
    // Aqui entraria a lógica de envio para Arduino Cloud
    // Por enquanto, não fazemos nada (ou podemos fazer debug)
    // ========================================================================
    // Serial.println("Modo Cloud (Tarefa 2) - dados a ser preparados para envio...");
  }
}
