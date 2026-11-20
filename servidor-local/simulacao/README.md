# Simulação do servidor em máquina virtual

Esta pasta guarda a configuração usada para rodar o Home Assistant em uma máquina virtual (VirtualBox), reproduzindo o comportamento do sistema sem nenhum equipamento físico. Os marcadores, as saídas e as entradas do LOGO! 1 são representados por chaves, e a lógica do CLP é imitada por roteiros. As entidades exibidas nos painéis têm os mesmos nomes da configuração da casa real.

| Arquivo | Conteúdo |
|---|---|
| `configuration.yaml` | Chaves que representam o CLP, entidades dos painéis e temas |
| `scripts.yaml` | Roteiros que imitam a lógica das cenas, da verificação, da persiana e da Emergência |
| `automations.yaml` | Disparo da Emergência pelo detector de gás e avisos na tela |
| `painel_morador.yaml` | Painel do morador (abas Início e Preferências) |
| `painel_cuidador.yaml` | Painel de acompanhamento do cuidador (abas Agora e Histórico) |
| `painel_bancada.yaml` | Painel de bancada (abas Demonstração e Simuladores) |

## Como reproduzir

1. Instale o VirtualBox e importe a imagem oficial do Home Assistant OS (arquivo .vdi).
2. Configure a rede da VM em modo bridge e inicie a máquina.
3. Abra o endereço mostrado na tela da VM (porta 8123) e crie o usuário.
4. Instale o complemento File editor e copie `configuration.yaml`, `scripts.yaml` e `automations.yaml` para `/config`.
5. Instale o HACS e, por ele, os cartões Mushroom e card-mod.
6. Em Ferramentas de desenvolvedor, verifique a configuração e reinicie o Home Assistant.
7. Em Configurações > Painéis, crie três painéis vazios. Em cada um, abra o editor de configuração bruta e cole o conteúdo do arquivo correspondente.

A simulação foi montada no Home Assistant 2026.10, com o Home Assistant OS 18.3.

## Roteiro de demonstração

Na aba Demonstração do painel de bancada, as entradas de campo ficam à esquerda, a planta da casa ao centro e a resposta do sistema à direita.

| Passo | Ação | Resposta esperada |
|---|---|---|
| 1 | Ligar a chave do detector de gás | Cena Emergência: semáforo vermelho, sirene ligada, gás fechado, persiana aberta e a cozinha em destaque na planta |
| 2 | Tocar em Reset com o detector ainda ligado | A Emergência continua ativa |
| 3 | Desligar o detector e tocar em Reset | A Emergência encerra e o gás permanece fechado até a cena Manhã |
| 4 | No painel do morador, tocar em Boa noite | A persiana desce e a verificação responde no semáforo por 10 s |
| 5 | Abrir a porta da entrada e tocar em Verificar | Semáforo amarelo |
| 6 | Tocar em Chuva com a persiana aberta | A persiana desce e a subida fica bloqueada enquanto houver chuva |
| 7 | Tocar em Pânico | Cena Emergência, com o evento de pânico retido até o reset |

Os tempos longos do CLP foram reduzidos na simulação, como o curso da persiana, de 40 s para 4 s.
