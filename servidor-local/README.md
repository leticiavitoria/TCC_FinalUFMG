# Servidor local

Mini PC com Home Assistant, responsável pelos painéis de acompanhamento, pela voz local e pelas rotinas de conforto. O servidor lê o estado do LOGO! 1 em modo somente leitura e escreve apenas nos registradores de comando HR130 a HR136. Nenhuma função de segurança depende dele: se ele cair, a casa perde os painéis, a voz e o conforto, e todas as funções cabeadas e o SMS continuam funcionando.

| Pasta | Uso |
|---|---|
| `casa-real/` | Configuração da instalação real (LOGO! 1 em 192.168.1.10, porta 503) |
| `simulacao/` | Configuração usada para simular o servidor em máquina virtual, sem nenhum equipamento |
| `paineis/` | Capturas dos painéis do morador, do cuidador e de bancada |

## Mapa de endereços usado na configuração

| Recurso do LOGO! | Endereço Modbus |
|---|---|
| Marcador Mn | coil 8255 + n (M10 = 8265, M14 = 8269) |
| Saída Qn | coil n - 1 |
| Entrada In | entrada discreta n - 1 |
| Comandos HR130 a HR136 | holding registers 130 a 136 |

Os sensores reed ficam energizados com a porta fechada (1 = fechada), por isso a configuração inverte a leitura para exibir "aberta" e "fechada" corretamente.
