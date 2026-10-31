# Projeto elétrico

Projeto da instalação elétrica da residência de referência (69 m², térrea, oito cômodos) conforme a ABNT NBR 5410:2004 e a CEMIG ND-5.1.

| Item | Valor |
|---|---|
| Fornecimento | Trifásico, 4 condutores (A, B, C e PEN), 127/220 V |
| Carga instalada | 19.780 VA |
| Demanda provável | 15.925 VA (faixa C2) |
| Disjuntor geral | 63 A tripolar, curva C |
| Alimentador | 5 × 16 mm², método D, 10 m |
| Circuitos terminais | 11, com DR de 30 mA individual |
| Desequilíbrio entre fases | 1,8 % |

## Desenhos

- [Figura 8 - Disposição física dos componentes no quadro de comando](../figuras/fig08_layout_quadro_de_comando.png)
- [Figura 9 - Distribuição de energia no quadro de comando](../figuras/fig09_distribuicao_energia_qc.png)
- [Figura 10 - Planta esquemática com pontos de utilização e circuitos](../figuras/fig10_planta_eletrica.png)
- [Figura 11 - Divisão dos circuitos terminais por tipo de carga](../figuras/fig11_divisao_dos_circuitos.png)
- [Figura 12 - Equilíbrio de fases e demanda provável](../figuras/fig12_fases_e_demanda.png)
- [Figura 13 - Diagrama unifilar do padrão de entrada e do QDC](../figuras/fig13_diagrama_unifilar.png)
- [Figura 14 - Separação entre eletrodutos de sinal e de força](../figuras/fig14_eletrodutos_sinal_e_forca.png)

## Memória de cálculo

### Quadro L.1 - Cálculo dos condutores pelos critérios CCC e CQT

| Circ. | S (VA) | U (V) | I_CIRC (A) | FCNC | I_PROJ (A) | CCC (mm²) | Sd (VA·m) | CQT (mm²) | Mín. (mm²) | Final (mm²) | ΔU (%) |
|---|---|---|---|---|---|---|---|---|---|---|---|
| C1 | 520 | 127 | 4,1 | 1,00 | 4,1 | 1,5 | 3.017 | 1,5 | 1,5 | 1,5 | 0,43 |
| C2 | 460 | 127 | 3,6 | 0,70 | 5,2 | 1,5 | 2.897 | 1,5 | 1,5 | 1,5 | 0,41 |
| C3 | 1.300 | 127 | 10,2 | 0,80 | 12,8 | 1,5 | 8.919 | 1,5 | 2,5 | 2,5 | 0,76 |
| C4 | 2.200 | 127 | 17,3 | 0,70 | 24,7 | 4,0 | 17.446 | 2,5 | 2,5 | 4,0 | 0,93 |
| C5 | 2.000 | 127 | 15,7 | 0,70 | 22,5 | 2,5 | 10.700 | 1,5 | 2,5 | 2,5 | 0,92 |
| C6 | 1.900 | 127 | 15,0 | 0,80 | 18,7 | 2,5 | 9.125 | 1,5 | 2,5 | 2,5 | 0,78 |
| C7 | 4.500 | 220 | 20,5 | 0,80 | 25,6 | 4,0 | 36.315 | 1,5 | 2,5 | 4,0 | 0,65 |
| C8 | 1.500 | 127 | 11,8 | 1,00 | 11,8 | 1,5 | 11.438 | 1,5 | 2,5 | 2,5 | 0,98 |
| C9 | 3.500 | 220 | 15,9 | 0,80 | 19,9 | 2,5 | 26.775 | 1,5 | 2,5 | 2,5 | 0,76 |
| C10 | 1.200 | 220 | 5,5 | 1,00 | 5,5 | 1,5 | 13.800 | 1,5 | 2,5 | 2,5 | 0,39 |
| C11 | 700 | 220 | 3,2 | 1,00 | 3,2 | 1,5 | 700 | 1,5 | 2,5 | 2,5 | 0,02 |

### Quadro L.2 - Verificação da proteção contra sobrecorrentes

| Circ. | Seção (mm²) | I_FIO (A) | FCNC | I_z (A) | I_CIRC (A) | I_DISJ (A) | Condição |
|---|---|---|---|---|---|---|---|
| C1 | 1,5 | 17,5 | 1,00 | 17,5 | 4,1 | 10 | atende |
| C2 | 1,5 | 17,5 | 0,70 | 12,2 | 3,6 | 10 | atende |
| C3 | 2,5 | 24,0 | 0,80 | 19,2 | 10,2 | 16 | atende |
| C4 | 4,0 | 32,0 | 0,70 | 22,4 | 17,3 | 20 | atende |
| C5 | 2,5 | 24,0 | 0,70 | 16,8 | 15,7 | 16 | atende |
| C6 | 2,5 | 24,0 | 0,80 | 19,2 | 15,0 | 16 | atende |
| C7 | 4,0 | 32,0 | 0,80 | 25,6 | 20,5 | 25 | atende |
| C8 | 2,5 | 24,0 | 1,00 | 24,0 | 11,8 | 16 | atende |
| C9 | 2,5 | 24,0 | 0,80 | 19,2 | 15,9 | 16 | atende |
| C10 | 2,5 | 24,0 | 1,00 | 24,0 | 5,5 | 10 | atende |
| C11 | 2,5 | 24,0 | 1,00 | 24,0 | 3,2 | 10 | atende |


## Interligação de campo

### Quadro I.1 - Réguas de bornes X5 e X6, do LOGO! 1

| Borne | Endereço | Dispositivo | Conexão ou tensão |
|---|---|---|---|
| X5.1 a X5.13 | L1-I1 a I13 | Sensores de campo do LOGO! 1 | Contato seco ou PNP de 3 fios |
| X5.14 a X5.17 | L1-I14 a I17 | Entradas de cena de reserva | Contato seco |
| X5.18 | L1-I18 | Botão de verificação | Contato seco |
| X5.19 | L1-I19 | Relé de interface do sensor de chuva | Contato seco interno |
| X5.24 | L1-I24 | Reed da persiana | Contato seco |
| X6.1 e X6.2 | L1-Q1 e Q2 | Válvulas de água | 220 V AC |
| X6.3 e X6.4 | L1-Q3 e Q4 | Válvula de gás e fechadura da entrada | 12 V DC |
| X6.5 a X6.8 | L1-Q5 a Q8 | Buzzer e semáforo | 24 V DC |
| X6.9 | L1-Q9 | Difusor da sala | 220 V AC |
| X6.10 e X6.11 | L1-Q10 e Q11 | Motor da persiana | 220 V AC |
| X6.13 | L1-Q13 | Sirene audiovisual externa | 12 V DC |

### Quadro I.2 - Réguas de bornes X7 e X8, do LOGO! 2

| Borne | Endereço | Dispositivo | Conexão ou tensão |
|---|---|---|---|
| X7.1 a X7.8 | L2-I1 a I8 | Sensores da suíte, do banheiro e do quarto social | Contato seco ou PNP de 3 fios |
| X8.1 | L2-Q1 | Fechadura da suíte | 12 V DC |
| X8.5 | L2-Q5 | Difusor de aroma da suíte | 220 V AC |

### Quadro I.3 - Tipos de cabo de campo por aplicação

| Aplicação | Tipo de cabo | Formação |
|---|---|---|
| Sensores de contato seco | CCI | 2 × 0,75 mm² |
| Sensores PNP de 3 fios | CCI | 4 × 0,75 mm² |
| Atuadores de corrente contínua | PP | 2 × 1,0 mm² |
| Difusores de 220 V | PP | 3 × 1,0 mm² |
| Válvulas de 220 V | PP | 3 × 1,5 mm² |
| Motor da persiana | PP | 4 × 1,0 mm² |
| Sensor de chuva | CCI | 4 × 0,75 mm² |
| Fitas indiretas de 24 V | PP | 2 × 1,5 mm² |
| ESP32 e câmeras de 12 V | PP | 2 × 1,0 mm² |
| Barramento RS-485 | Par trançado blindado | 2 × 0,5 mm² |
| Rede Ethernet | Cat5e UTP | 4 pares |
| Antena 4G | Coaxial com conector SMA |  |

### Quadro I.4 - Troncos de eletroduto de campo e taxas de ocupação

| Tronco | Tipo | Zona atendida | Eletroduto | Ocupação |
|---|---|---|---|---|
| T1-S e T1-F | Sinal e força | Sala e entrada | Ø 32 e Ø 20 mm | 34 % e 15 % |
| T2-S e T2-F | Sinal e força | Cozinha | Ø 32 e Ø 25 mm | 31 % e 28 % |
| T3-S e T3-F | Sinal e força | Área de serviço | Ø 25 e Ø 20 mm | 30 % e 22 % |
| T4-S e T4-F | Sinal e força | Suíte e banheiro | Ø 32 e Ø 20 mm | 28 % e 15 % |
| T5-S | Sinal | Quarto social e circulação | Ø 32 mm | 31 % |
| T6 | Dados | Forro (RS-485) e sala (Ethernet) | Ø 20 mm | 13 % |
