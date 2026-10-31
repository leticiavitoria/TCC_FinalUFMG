# Programas dos controladores (LOGO! 8.4)

Os dois controladores Siemens LOGO! 8.4 trabalham em configuração mestre e escravo, ligados por Ethernet e trocando marcadores pelo protocolo S7. O LOGO! 1 coordena as cenas globais e as zonas sociais, e o LOGO! 2 atende as zonas íntimas.

## Conteúdo

| Pasta | Conteúdo |
|---|---|
| `logo1/` | Programa do LOGO! 1, lista de conectores e diagramas FBD |
| `logo2/` | Programa do LOGO! 2 e lista de conectores |

Os programas originais (`TCC.lsc` e `TCC_Logo2.lsc`) abrem no LOGO! Soft Comfort V8.4. Para a co-simulação dos dois controladores, abra o projeto de rede com os dois CLPs, inicie as duas simulações e ative a simulação em rede.

## Recursos ocupados (Determinar LOGO!)

| Recurso | LOGO! 1 | LOGO! 2 | Limite |
|---|---|---|---|
| Blocos de função | 140 | 25 | 400 |
| Entradas digitais | 21 | 5 | 24 |
| Saídas digitais | 12 | 2 | 20 |
| Marcadores | 53 | 6 | 64 |
| Entradas de rede | 9 | 4 | 64 |
| Saídas de rede | 4 | 2 | 64 |
| Teclas de função | 2 | 0 | 4 |
| Memória de programa (bytes) | 1.780 | 348 | 8.500 |

## Diagramas FBD do LOGO! 1

| Figura | Rede | Arquivo |
|---|---|---|
| G.1 | Gatilho da cena Sono (M1) | [G01_gatilho_sono.png](logo1/fbd/G01_gatilho_sono.png) |
| G.2 | Gatilho da cena Manhã (M2), com a pré-condição do Sono | [G02_gatilho_manha.png](logo1/fbd/G02_gatilho_manha.png) |
| G.3 | Gatilho da cena Foco (M3) | [G03_gatilho_foco.png](logo1/fbd/G03_gatilho_foco.png) |
| G.4 | Gatilho da cena Calmante (M4), por voz e por ruído com presença | [G04_gatilho_calmante.png](logo1/fbd/G04_gatilho_calmante.png) |
| G.5 | Retenção das cenas Sono, Manhã, Foco e Calmante (M10 a M13) | [G05_retencao_cenas.png](logo1/fbd/G05_retencao_cenas.png) |
| G.6 | Verificação pós-Sono, com casa normal e com anomalia (M17 e M18) | [G06_verificacao_pos_sono.png](logo1/fbd/G06_verificacao_pos_sono.png) |
| G.7 | Anomalia da verificação (M5), com a persiana | [G07_anomalia.png](logo1/fbd/G07_anomalia.png) |
| G.8 | Marcador da Verificação TOC (M15) e leitura pós-Sono | [G08_marcador_toc.png](logo1/fbd/G08_marcador_toc.png) |
| G.9 | Resposta da Verificação TOC (M6, M7 e M16) | [G09_resposta_toc.png](logo1/fbd/G09_resposta_toc.png) |
| G.10 | Botão de verificação (M52) e mensagens do painel | [G10_botao_verificacao.png](logo1/fbd/G10_botao_verificacao.png) |
| G.11 | Botão de pânico (M51), por pressão sustentada ou sucessiva | [G11_botao_panico.png](logo1/fbd/G11_botao_panico.png) |
| G.12 | Reset da Emergência (M54) | [G12_reset_emergencia.png](logo1/fbd/G12_reset_emergencia.png) |
| G.13 | Gatilhos da cena Emergência (M14) | [G13_gatilhos_emergencia.png](logo1/fbd/G13_gatilhos_emergencia.png) |
| G.14 | Distribuição do sinal da Emergência e semáforo vermelho | [G14_distribuicao_emergencia.png](logo1/fbd/G14_distribuicao_emergencia.png) |
| G.15 | Marcadores de evento (M19 a M23) | [G15_marcadores_evento.png](logo1/fbd/G15_marcadores_evento.png) |
| G.16 | Válvulas, fechadura da entrada, difusor da sala e sirene externa | [G16_saidas.png](logo1/fbd/G16_saidas.png) |
| G.17 | Semáforo | [G17_semaforo.png](logo1/fbd/G17_semaforo.png) |
| G.18 | Multiplexação do buzzer (Q5) | [G18_buzzer.png](logo1/fbd/G18_buzzer.png) |
| G.19 | Bips de ruído, de verificação e de confirmação do Sono | [G19_bips.png](logo1/fbd/G19_bips.png) |
| G.20 | Supervisão dos ESP32 pelo watchdog | [G20_watchdog.png](logo1/fbd/G20_watchdog.png) |
| G.21 | Chuva (M42) e comandos da persiana (M43 e M45) | [G21_chuva_persiana.png](logo1/fbd/G21_chuva_persiana.png) |
| G.22 | Persiana: pulsos, intertravamento e reed (Q10, Q11, M46, M47 e M50) | [G22_persiana_pulsos.png](logo1/fbd/G22_persiana_pulsos.png) |

## Comunicação Modbus TCP

O LOGO! 1 é servidor Modbus TCP na porta 503. Os nós ESP32 escrevem os registradores HR100 a HR112 (eventos e sensores) e HR120 a HR127 (pulso de vida), e o servidor local escreve os comandos HR130 a HR136 como pulsos de 1 s. No programa, cada registrador de comando é lido por um bloco Network Input em memória de variáveis (VM): o registrador HRn corresponde ao bit V(2n+1).0, de modo que HR130 é V261.0 e HR136 é V273.0.
