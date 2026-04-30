// Trabalho Física - Lab02 (Arduino Cloud)
// Lê sensores (IMU + microfone) e atualiza dashboard
// Controla LED com base em alertas e input do dashboard

#include "arduino_secrets.h"



#include "thingProperties.h"

// ============================================================================
// Includes especificos da placa
// ============================================================================

#include <Arduino_LSM6DSOX.h>
#include <PDM.h>

// ---------------------------------------------------------------------------
// Configuraçao geral
// ---------------------------------------------------------------------------

#define SAMPLE_INTERVAL_MS  200   // periodo entre leituras (ms) ~5 Hz
#define MIC_BUFFER_SIZE     256   // amostras PDM por bloco
#define FILTER_WINDOW       10    // tamanho da janela da media movel
#define USE_FILTER          true  // se false, envia valores brutos

// Limites de alerta (podes ajustar)
#define TEMP_LIMIT_C        35.0f
#define SOUND_LIMIT_RMS     2000.0f

// ============================================================================
// Estruturas de dados (simples, versao resumida do Trabalho 1)
// ============================================================================

typedef struct {
  float ax, ay, az;
  float gx, gy, gz;
  float temp;
  float soundRMS;
  float soundPeak;
} SensorReadings;

// Leituras brutas e filtradas
SensorReadings rawReadings;
SensorReadings avgReadings;

// ============================================================================
// VariÃ¡veis para media movel (versao reduzida)
// ============================================================================

#define MAX_WINDOW 50
static SensorReadings buffer[MAX_WINDOW];
static int bufferCount = 0;
static int bufferIndex = 0;

// ============================================================================
// Microfone PDM
// ============================================================================

static int16_t micBuffer[MIC_BUFFER_SIZE];
static volatile int samplesRead = 0;

static void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(micBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2; // 2 bytes por amostra (int16_t)
}

// ============================================================================
// Funçoes auxiliares de sensores
// ============================================================================

bool imuInit() {
  if (!IMU.begin()) {
    Serial.println("Erro a inicializar IMU!");
    return false;
  }
  Serial.println("IMU inicializada.");
  return true;
}

bool imuRead(SensorReadings &readings) {
  float ax, ay, az;
  float gx, gy, gz;
  int tempInt;
  bool ok = false;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    readings.ax = ax;
    readings.ay = ay;
    readings.az = az;
    ok = true;
  }

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);
    readings.gx = gx;
    readings.gy = gy;
    readings.gz = gz;
    ok = true;
  }

  if (IMU.temperatureAvailable()) {
    IMU.readTemperature(tempInt);
    readings.temp = (float)tempInt;
    ok = true;
  }

  return ok;
}

bool micInit() {
  PDM.onReceive(onPDMdata);
  if (!PDM.begin(1, 16000)) { // 1 canal, 16 kHz
    Serial.println("Erro a inicializar microfone!");
    return false;
  }
  Serial.println("Microfone inicializado.");
  return true;
}

void micUpdate() {
 
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

  samplesRead = 0;
}

// ============================================================================
// MÃ©dia movel (aplicada a alguns parametros usados na Cloud)
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
  float stemp = 0;
  float srms = 0;

  for (int i = 0; i < n; i++) {
    int idx = (bufferIndex - 1 - i);
    if (idx < 0) idx += MAX_WINDOW;

    const SensorReadings &r = buffer[idx];

    sax += r.ax;  say += r.ay;  saz += r.az;
    stemp += r.temp;
    srms += r.soundRMS;
  }

  avg = raw; // copia tudo primeiro

  avg.ax = sax / n;
  avg.ay = say / n;
  avg.az = saz / n;
  avg.temp = stemp / n;
  avg.soundRMS = srms / n;
}

// ============================================================================
// Variaveis de controlo de tempo
// ============================================================================

unsigned long lastSampleTime = 0;

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(1500);

  // Configura LED_BUILTIN para controlo de alertas e da Cloud
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Inicializa IMU e microfone (Trabalho 1)
  imuInit();
  micInit();

  // Inicializa propriedades da Arduino Cloud (variaveis + device)
  initProperties();

  // Conecta ao  Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  Serial.println("Sketch Trabalho Fisica - Cloud iniciado.");
}

// ============================================================================
// LOOP PRINCIPAL
// ============================================================================

void loop() {
  // Atualiza ligaçao com a Cloud 
  ArduinoCloud.update();

  unsigned long now = millis();

  // Leitura periodica dos sensores
  if (now - lastSampleTime >= SAMPLE_INTERVAL_MS) {
    lastSampleTime = now;

    // 1) faz a leitura da IMU + microfone
    imuRead(rawReadings);
    micComputeFeatures(rawReadings);

    // 2) Aplica media movel (opcional)
    if (USE_FILTER) {
      updateMovingAverage(rawReadings, avgReadings, FILTER_WINDOW);
    } else {
      avgReadings = rawReadings;
    }

    // 3) Calcula parametros agregados para a Cloud
    //    Aceleraçao total (magnitude)
    float accelTotal = sqrt(
      avgReadings.ax * avgReadings.ax +
      avgReadings.ay * avgReadings.ay +
      avgReadings.az * avgReadings.az
    );

    // Tempo em segundos desde o arranque (por enquanto)
    float timeSec = now / 1000.0f;

    // 4) Atualiza variaveis da Cloud
    sensorAccel   = accelTotal;
    sensorSound   = avgReadings.soundRMS;
    sensorTemp    = avgReadings.temp;
    timestampSec  = timeSec;
    // ledControl sera atualizado tanto pela Cloud (botao) como localmente (alertas)

    bool alertTemp  = (avgReadings.temp > TEMP_LIMIT_C);
    bool alertSound = (avgReadings.soundRMS > SOUND_LIMIT_RMS);

    // Se algum alerta ativo, muda o ledControl = true (LED aceso)
    if (alertTemp || alertSound) {
      ledControl = true;
    }

    // 6) Aplica o estado do LED fisico (LED_BUILTIN) com base em ledControl
    digitalWrite(LED_BUILTIN, ledControl ? HIGH : LOW);

    // Debug na porta Serial
    Serial.print("t[s]="); Serial.print(timeSec);
    Serial.print(" | accel="); Serial.print(accelTotal);
    Serial.print(" | soundRMS="); Serial.print(avgReadings.soundRMS);
    Serial.print(" | temp="); Serial.print(avgReadings.temp);
    Serial.print(" | ledControl="); Serial.println(ledControl);
  }

  // Mantem o microfone ativo
  micUpdate();
}

// ============================================================================
// FUNÇOES DE CALLBACK DAS VARIAVEIS DA CLOUD
// ============================================================================
// Chamadas quando o dashboard altera os valores (READ_WRITE)

void onSensorAccelChange()  {
  // Normalmente, esta variavel foi escrita pelo Arduino, nao pela Cloud, e pode ser usada para debugingdo codigo.

}

void onSensorSoundChange()  {

}

void onSensorTempChange()  {

}

void onLedControlChange()  {
  // Quando o utilizador carrega no botao/switch do dashboard,
  // ledControl e atualizado e este callback sera chamado.
  // Aqui sincronizamos o LED fisico.
  digitalWrite(LED_BUILTIN, ledControl ? HIGH : LOW);
}

void onTimestampSecChange()  {

}
