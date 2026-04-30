#include "header.h"
#include "functions.h"

// ============================================================================
// CONFIGURAÇÃO DO MODO DE SAÍDA
// ============================================================================
// Mudar este valor para escolher o modo de funcionamento:
//   0 = MODE_SERIAL_MONITOR  (formato detalhado)
//   1 = MODE_SERIAL_PLOTTER  (formato tabulado para gráficos)

int outputMode = MODE_SERIAL_MONITOR;  // <- MUDA AQUI PARA TROCAR DE MODO

// ============================================================================
// CONFIGURAÇÃO DO FILTRO (MÉDIA MÓVEL)
// ============================================================================
// Tamanho da janela da média móvel (ajustável em tempo de compilação).
// Valores típicos: 5, 10, 20. Máximo: MAX_WINDOW (definido em functions.cpp).

int windowSize = DEFAULT_WINDOW;

// ============================================================================
// VARIÁVEIS GLOBAIS
// ============================================================================

SensorReadings rawReadings;      // leituras brutas dos sensores
SensorReadings avgReadings;      // leituras filtradas (média móvel)

unsigned long lastSampleTime = 0;

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // espera pela porta série (importante em placas com USB nativa)
  }

  // Inicializa sensores
  initSystem();

  // Mensagem de boas-vindas
  Serial.println("========================================");
  Serial.println("LAB-01: Aquisição de Dados de Sensores");
  Serial.println("Arduino Nano RP2040 Connect");
  Serial.println("========================================");
  Serial.print("Modo de saída: ");
  if (outputMode == MODE_SERIAL_MONITOR) {
    Serial.println("Serial Monitor (detalhado)");
  } else if (outputMode == MODE_SERIAL_PLOTTER) {
    Serial.println("Serial Plotter (tabulado)");
  } else if (outputMode == MODE_CLOUD) {
    Serial.println("Cloud (Tarefa 2)");
  }
  Serial.print("Tamanho da janela (média móvel): ");
  Serial.println(windowSize);
  Serial.println("========================================\n");
}

// ============================================================================
// LOOP PRINCIPAL
// ============================================================================

void loop() {
  unsigned long now = millis();

  // Amostragem periódica
  if (now - lastSampleTime >= SAMPLE_INTERVAL) {
    lastSampleTime = now;

    // 1) Ler todos os sensores (IMU + microfone)
    readAllSensors(rawReadings);

    // 2) Aplicar média móvel (se USE_FILTER estiver ativo)
    if (USE_FILTER) {
      updateMovingAverage(rawReadings, avgReadings, windowSize);
    } else {
      // Se filtro desativado, copia os valores brutos para avgReadings
      avgReadings = rawReadings;
    }

    // 3) Enviar dados (centralizado em processAndSendData)
    processAndSendData(now, rawReadings, avgReadings, outputMode);
  }

  // Garante que o microfone continua a receber dados em background
  micUpdate();
}
