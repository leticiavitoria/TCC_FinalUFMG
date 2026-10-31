# Firmware dos nós de monitoramento (ESP32)

Um único firmware atende os oito cômodos. Cada nó reúne um ESP32, a câmera térmica AMG8833, o radar de presença LD2410C e o sensor de distância VL53L1X, instalado no teto e apontado para o piso.

O nó processa tudo localmente e escreve no LOGO! 1 apenas resultados binários, por Modbus TCP. Nenhuma imagem térmica sai do nó.

## Como compilar

1. Instale o VS Code com a extensão PlatformIO.
2. Abra a pasta `no-monitoramento/` (a que contém o `platformio.ini`).
3. No início de `src/main.cpp`, ajuste `#define COMODO` para o cômodo do nó: 0 = sala, 1 = cozinha, 2 = copa, 3 = circulação, 4 = suíte, 5 = quarto social, 6 = banheiro, 7 = área de serviço.
4. Ajuste o nome e a senha da rede Wi-Fi.
5. Clique em Build (✓). A primeira compilação baixa a plataforma do ESP32 e as bibliotecas.

## Registradores escritos

| Registrador | Sinal |
|---|---|
| HR100 a HR107 | Queda ou crise detectada, um por cômodo |
| HR108 | Chama na cozinha |
| HR109 a HR111 | Fluxo de água na cozinha, no banheiro e na área de serviço |
| HR112 | Presença por radar no banheiro |
| HR120 a HR127 | Pulso de vida (watchdog), um por cômodo |

Com `MODO_LOG 1`, o nó envia pela serial as leituras brutas em CSV, para a caracterização em bancada.

O algoritmo de detecção ainda não foi caracterizado experimentalmente: os limiares são valores de projeto, a ajustar em bancada.
