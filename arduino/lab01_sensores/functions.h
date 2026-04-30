#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "header.h"
#include "sensor_imu.h"
#include "sensor_mic.h"

// ============================================================================
// FUNÇÕES DE INICIALIZAÇÃO E LEITURA
// ============================================================================

void initSystem();
void readAllSensors(SensorReadings &raw);
void updateMovingAverage(const SensorReadings &raw, SensorReadings &avg, int window);

// ============================================================================
// FUNÇÃO DE ENVIO/PROCESSAMENTO DE DADOS
// ============================================================================
// Esta função centra o envio/visualização dos dados.
// No Trabalho 1: chama printData(...)
// Na Tarefa 2: chama printData(...) + atualiza variáveis da Arduino Cloud

void processAndSendData(unsigned long ts, const SensorReadings &raw, const SensorReadings &avg, int outputMode);

// ============================================================================
// FUNÇÃO AUXILIAR DE IMPRESSÃO (mantém compatibilidade com código anterior)
// ============================================================================

void printData(unsigned long ts, const SensorReadings &raw, const SensorReadings &avg, int outputMode);

#endif
