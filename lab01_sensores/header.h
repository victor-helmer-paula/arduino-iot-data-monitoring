#ifndef HEADER_H
#define HEADER_H

#include <Arduino.h>
#include "types.h"

// Bibliotecas específicas da placa Nano RP2040 Connect
#include <Arduino_LSM6DSOX.h>   // IMU
#include <PDM.h>                // Microfone PDM

// ============================================================================
// DEFINIÇÃO DOS MODOS DE SAÍDA
// ============================================================================
// Modos de visualização/envio dos dados:
//   MODE_SERIAL_MONITOR = 0  → imprime no Serial Monitor (texto com legendas)
//   MODE_SERIAL_PLOTTER = 1  → imprime para Serial Plotter (valores tabulados)

// Para usar um modo, muda a variável outputMode em lab01_sensores.ino

#define MODE_SERIAL_MONITOR  0
#define MODE_SERIAL_PLOTTER  1
#define MODE_CLOUD           2  // Tarefa 2

// ============================================================================
// PARÂMETROS DE CONFIGURAÇÃO
// ============================================================================

#define DEFAULT_WINDOW 10        // tamanho por omissão da janela da média móvel
#define SAMPLE_INTERVAL 100      // período de amostragem em ms (10 Hz)
#define MIC_BUFFER_SIZE 256      // número de amostras PDM por bloco

// ============================================================================
// FLAG PARA ATIVAR/DESATIVAR MÉDIA MÓVEL (fácil para testes/Tarefa 2)
// ============================================================================
// Se USE_FILTER for true, a média móvel é aplicada.
// Se for false, só os valores brutos são usados (útil para testes ou Tarefa 2).

#define USE_FILTER true

#endif
