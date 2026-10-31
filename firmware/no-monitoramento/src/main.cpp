// =====================================================================
//  No de monitoramento - deteccao local de queda e crise
//  Hardware: ESP32 DevKitC + AMG8833 (I2C) + LD2410C (pino OUT) + VL53L1X (I2C)
//  O no escreve apenas resultados binarios nos registradores Modbus TCP
//  do LOGO! 1. Nenhuma imagem termica sai do no.
// =====================================================================

#include <Arduino.h>
#include <algorithm>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include <VL53L1X.h>
#include <ModbusIP_ESP8266.h>

// ------------------- CONFIGURACAO DO COMODO (unica parte editada) ----
// Comodos: 0=SE 1=CZ 2=CP 3=CC 4=QS 5=Q1 6=BO 7=AS
#define COMODO          0           // indice deste no
#define NO_COZINHA      (COMODO == 1)
#define NO_BANHEIRO     (COMODO == 6)
#define NO_AREA_SERV    (COMODO == 7)
#define MODO_LOG        0           // 1 = registro CSV pela serial (bancada)

static const char* WIFI_SSID  = "CASA-AUTOMACAO";
static const char* WIFI_SENHA = "trocar-na-instalacao";
static const IPAddress IP_LOGO1(192, 168, 1, 10);
static const uint16_t  MODBUS_PORT = 503;   // porta do servidor Modbus do LOGO!

// Registradores (enderecos Modbus de holding register)
static const uint16_t HR_QUEDA    = 100 + COMODO;  // HR100..HR107
static const uint16_t HR_CHAMA    = 108;           // so o no da cozinha
static const uint16_t HR_FLUXO_CZ = 109;
static const uint16_t HR_FLUXO_BO = 110;
static const uint16_t HR_FLUXO_AS = 111;
static const uint16_t HR_RADAR_BO = 112;           // so o no do banheiro
static const uint16_t HR_VIDA     = 120 + COMODO;  // HR120..HR127

// ------------------- PINOS ------------------------------------------
static const int PIN_RADAR_OUT = 27;   // LD2410C, saida de presenca
static const int PIN_CHAMA     = 34;   // KY-026 digital (so cozinha)
static const int PIN_FLUXO     = 35;   // YF-S201 (CZ, BO e AS)

// ------------------- LIMIARES DO ALGORITMO ---------------------------
// O VL53L1X aponta do teto (2,80 m) para o piso.
static const uint16_t D_DE_PE_MAX = 1500;  // mm: <= 1,50 m -> pessoa de pe
static const uint16_t D_CHAO_MIN  = 2200;  // mm: >= 2,20 m -> corpo no chao
static const uint16_t D_VAZIO     = 2650;  // mm: >= 2,65 m -> vertical livre
static const float    DT_BLOB     = 2.0f;  // graus acima do ambiente
static const uint32_t T_QUEDA_MS   = 20000; // permanencia no chao
static const uint32_t T_CRISE_MS   = 10000; // oscilacao sustentada
static const uint32_t T_TRANS_MS   = 1500;  // transicao rapida de pe -> chao
static const float    OSC_LIMIAR   = 0.8f;  // deslocamento do centroide (pixel)
static const uint32_t T_LIMPA_MS   = 60000; // sem deteccao -> zera o registrador

Adafruit_AMG88xx amg;
VL53L1X tof;
ModbusIP mb;

// ------------------- FILTROS DE MEDIANA ------------------------------
template <int N> struct Mediana {
  uint16_t v[N] = {0}; int i = 0;
  uint16_t add(uint16_t x) {
    v[i] = x; i = (i + 1) % N;
    uint16_t c[N]; memcpy(c, v, sizeof(c));
    std::sort(c, c + N); return c[N / 2];
  }
};
Mediana<5> medDist;

// ------------------- ESTADO ------------------------------------------
enum Postura { AUSENTE, DE_PE, INTERMEDIARIA, NO_CHAO };
Postura postura = AUSENTE;
uint32_t tDePe = 0, tChegouChao = 0, tOscilaIni = 0, tUltimaDet = 0;
uint32_t tVida = 0, tCiclo = 0;
float cxAnt = 0, cyAnt = 0;
bool emQueda = false, emCrise = false, hrAtivo = false;

static void escreveHR(uint16_t hr, uint16_t valor) {
  mb.writeHreg(IP_LOGO1, hr, &valor, 1);
}

// Blob termico: pixels acima do ambiente, tamanho e centroide
static int blob(float px[64], float &cx, float &cy) {
  float soma = 0;
  for (int k = 0; k < 64; k++) soma += px[k];
  float amb = soma / 64.0f;
  int n = 0; cx = cy = 0;
  for (int k = 0; k < 64; k++)
    if (px[k] > amb + DT_BLOB) { n++; cx += k % 8; cy += k / 8; }
  if (n) { cx /= n; cy /= n; }
  return n;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_RADAR_OUT, INPUT);
  if (NO_COZINHA) pinMode(PIN_CHAMA, INPUT);
  if (NO_COZINHA || NO_BANHEIRO || NO_AREA_SERV) pinMode(PIN_FLUXO, INPUT);

  Wire.begin();
  amg.begin();
  tof.setTimeout(500);
  tof.init();
  tof.setDistanceMode(VL53L1X::Long);
  tof.setMeasurementTimingBudget(50000);
  tof.startContinuous(100);

  WiFi.begin(WIFI_SSID, WIFI_SENHA);
  while (WiFi.status() != WL_CONNECTED) delay(250);
  mb.client();                      // cliente Modbus TCP
  mb.connect(IP_LOGO1, MODBUS_PORT);

  if (MODO_LOG) Serial.println("ms,dist,blob,cx,cy,radar,postura,queda,crise");
}

void loop() {
  mb.task();
  uint32_t agora = millis();

  // pulso de vida: incrementa HR120+COMODO a cada 1 s
  if (agora - tVida >= 1000) {
    tVida = agora;
    static uint16_t vida = 0;
    escreveHR(HR_VIDA, ++vida);
    if (NO_COZINHA) {
      escreveHR(HR_CHAMA, digitalRead(PIN_CHAMA) ? 1 : 0);
      escreveHR(HR_FLUXO_CZ, digitalRead(PIN_FLUXO) ? 1 : 0);
    }
    if (NO_BANHEIRO) {
      escreveHR(HR_FLUXO_BO, digitalRead(PIN_FLUXO) ? 1 : 0);
      escreveHR(HR_RADAR_BO, digitalRead(PIN_RADAR_OUT) ? 1 : 0);
    }
    if (NO_AREA_SERV) escreveHR(HR_FLUXO_AS, digitalRead(PIN_FLUXO) ? 1 : 0);
  }

  // ciclo de classificacao: 100 ms
  if (agora - tCiclo < 100) return;
  tCiclo = agora;

  bool radar = digitalRead(PIN_RADAR_OUT);
  uint16_t dist = medDist.add(tof.read(false));
  float px[64], cx, cy;
  amg.readPixels(px);
  int nBlob = blob(px, cx, cy);

  // postura pela distancia do teto
  Postura nova;
  if (!radar && nBlob < 2 && dist >= D_VAZIO) nova = AUSENTE;
  else if (dist <= D_DE_PE_MAX)               nova = DE_PE;
  else if (dist >= D_CHAO_MIN)                nova = NO_CHAO;
  else                                        nova = INTERMEDIARIA;

  // queda: transicao rapida de pe -> chao e permanencia com presenca
  if (nova == DE_PE) tDePe = agora;
  if (nova == NO_CHAO && postura != NO_CHAO) tChegouChao = agora;
  bool transicaoRapida = (tChegouChao > tDePe) && (tChegouChao - tDePe <= T_TRANS_MS);
  if (nova == NO_CHAO && radar && nBlob >= 2 && transicaoRapida &&
      agora - tChegouChao >= T_QUEDA_MS) emQueda = true;

  // crise: oscilacao rapida do centroide com a pessoa deitada ou no chao
  float desloc = fabsf(cx - cxAnt) + fabsf(cy - cyAnt);
  cxAnt = cx; cyAnt = cy;
  bool oscilando = (nBlob >= 2) && (desloc >= OSC_LIMIAR) && (nova != DE_PE) && radar;
  if (!oscilando) tOscilaIni = agora;
  if (agora - tOscilaIni >= T_CRISE_MS) emCrise = true;

  postura = nova;

  // escrita por evento e limpeza apos 60 s sem deteccao
  if (emQueda || emCrise) {
    tUltimaDet = agora;
    if (!hrAtivo) { escreveHR(HR_QUEDA, 1); hrAtivo = true; }
    emQueda = emCrise = false;
  } else if (hrAtivo && agora - tUltimaDet >= T_LIMPA_MS) {
    escreveHR(HR_QUEDA, 0); hrAtivo = false;
  }

  if (MODO_LOG) {
    Serial.printf("%lu,%u,%d,%.1f,%.1f,%d,%d,%d,%d\n",
                  (unsigned long)agora, dist, nBlob, cx, cy,
                  radar, (int)postura, hrAtivo, (int)emCrise);
  }
}
