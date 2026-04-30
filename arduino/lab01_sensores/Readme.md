Tarefa LAB-01 – Aquisição de Dados de Sensores
Licenciatura em Engenharia Informática – IPBeja
Unidade Curricular: Física Aplicada à Computação
Ano Letivo: 2025/2026

1. Objetivo do trabalho
O objetivo deste trabalho é desenvolver um sketch modular para o Arduino Nano RP2040 Connect que permita:
  - A aquisição em simultâneo dos dados fornecidos pelos sensores integrados na placa.
  - A associação de uma etiqueta temporal (timestamp) a cada conjunto de leituras.
  - A aplicação de uma filtragem por média móvel simples às leituras.
  - A visualização dos dados no Serial Monitor ou no Serial Plotter, selecionada por variável (modo).
  - Uma estrutura preparada para extensão futura à Tarefa 2 (Arduino Cloud).
[Baseado no enunciado da Tarefa 1 – Aquisição de dados de sensores.]

2. Hardware utilizado

Placa:
  - Arduino Nano RP2040 Connect (ABX00052).

Sensores físicos on-board:
  1) IMU LSM6DSOX (acelerómetro + giroscópio + sensor de temperatura interno).
  2) Microfone PDM (MP34DT06J).

Parâmetros medidos (total: 3 sensores físicos, 9 parâmetros):
  - Aceleração (g): ax, ay, az.
  - Velocidade angular (deg/s): gx, gy, gz.
  - Temperatura interna da IMU (ºC): temp.
  - Som:
      • soundRMS – nível de som RMS (valor médio quadrático).
      • soundPeak – valor pico absoluto das amostras.

3. Organização do código

O sketch foi dividido em vários ficheiros, seguindo a estrutura sugerida no enunciado:

3.1. Ficheiro principal
  - lab01_sensores.ino
    · Contém setup() e loop().
    · Define as variáveis de configuração:
        - outputMode: escolhe o modo de saída (0=Monitor, 1=Plotter, 2=Cloud).
        - windowSize: tamanho da janela da média móvel.
    · Inicializa os sensores e entra em loop de amostragem periódica.

3.2. Ficheiros de configuração
  - header.h: inclui bibliotecas, define constantes (MODE_SERIAL_MONITOR, etc.) e parâmetros.
  - types.h: define as estruturas SensorReadings e Sample.

3.3. Ficheiros de sensores
  - sensor_imu.h / sensor_imu.cpp: funções imuInit() e imuRead(...).
  - sensor_mic.h / sensor_mic.cpp: funções micInit(), micUpdate(), micComputeFeatures(...).

3.4. Ficheiros de funções genéricas
  - functions.h / functions.cpp:
    · initSystem(): inicializa sensores.
    · readAllSensors(...): lê todos os sensores.
    · updateMovingAverage(...): calcula média móvel simples.
    · processAndSendData(...): centraliza o envio/visualização dos dados (ponte para Tarefa 2).
    · printData(...): imprime no Serial Monitor ou Plotter, consoante outputMode.

4. Como usar

4.1. Escolher modo de saída
Abre o ficheiro lab01_sensores.ino e muda a linha:

    int outputMode = MODE_SERIAL_MONITOR;

Opções:
  - MODE_SERIAL_MONITOR (0): formato detalhado com legendas (Serial Monitor).
  - MODE_SERIAL_PLOTTER (1): formato tabulado (Serial Plotter).
  - MODE_CLOUD (2): reservado para Tarefa 2 (Arduino Cloud).

Não é preciso comentar/descomentar nada; basta mudar o valor de outputMode.

4.2. Ajustar tamanho da média móvel
Também em lab01_sensores.ino:

    int windowSize = DEFAULT_WINDOW;  // muda para 5, 10, 20, etc.

O valor máximo é MAX_WINDOW (50 por omissão em functions.cpp).

4.3. Desativar filtro
Em header.h, muda:

    #define USE_FILTER true   // ou false

Quando false, os valores "avgReadings" são cópia dos "rawReadings" (sem filtragem).

5. Funcionamento

5.1. Aquisição simultânea
Em cada ciclo de amostragem (SAMPLE_INTERVAL ms), todos os sensores são lidos:
  - IMU (aceleração, giroscópio, temperatura).
  - Microfone PDM (nível RMS e pico).

5.2. Etiqueta temporal
Cada conjunto de leituras recebe um timestamp em milissegundos (função millis()).

5.3. Média móvel simples
Cada parâmetro é passado através de um filtro de média móvel com janela N:
  - Utiliza buffer circular de tamanho máximo MAX_WINDOW.
  - Soma as N últimas amostras e divide por N.
  - Resultado: valores filtrados em avgReadings.

5.4. Visualização
Baseado em outputMode:
  - Monitor: imprime uma linha com "t=...", "acc_raw", "acc_avg", etc., com legendas e unidades.
  - Plotter: imprime valores em formato tabulado (ts, ax_avg, ay_avg, az_avg, soundRMS_avg).
  - Cloud: (Tarefa 2) enviará os dados para Arduino Cloud.

6. Estrutura preparada para Tarefa 2

A função processAndSendData(...) em functions.cpp é o ponto de entrada centralizado para envio de dados.
No Trabalho 1, chama apenas printData(...).
Na Tarefa 2, será estendida para chamar updateCloudVariables(...) e enviar para Arduino Cloud.

Isto torna fácil a transição entre Tarefa 1 e Tarefa 2 sem refazer o código inteiro.

7. Compilação e upload

  1. Abre o Arduino IDE.
  2. Carrega o ficheiro lab01_sensores.ino.
  3. Seleciona Tools → Board → Arduino Mbed OS RP2040 Boards → Arduino Nano RP2040 Connect.
  4. Seleciona a porta série correta em Tools → Port.
  5. Carrega em Verify (✓) para compilar.
  6. Carrega em Upload (→) para enviar para a placa.
  7. Abre Tools → Serial Monitor (ou Serial Plotter) a 115200 baud.

8. Notas finais

  - O código cumpre todos os requisitos da Tarefa 1.
  - Está preparado para ser estendido na Tarefa 2 sem grandes alterações.
  - Todos os ficheiros estão comentados para facilitar compreensão e manutenção.
  - Boa estrutura modular para fins educacionais.

Fim do README




chaves e id geradas no arduino cloud

device id: 3a52bac7-eeca-4fb8-8897-a0eb8648eec1
secret key: l?aHRpbnbq3xz#@T5I1OyAgbm

