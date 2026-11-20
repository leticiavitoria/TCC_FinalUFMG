# Configuração da casa real

Arquivos do servidor local para a instalação com os equipamentos. A fronteira com o CLP é a mesma descrita no texto: leitura do estado do LOGO! 1 e escrita apenas nos registradores de comando HR130 a HR136, como pulsos de 1 s.

| Arquivo | Conteúdo |
|---|---|
| `configuration.yaml` | Integração Modbus TCP com o LOGO! 1, preferências do morador, voz, avisos e sugestões |
| `scripts.yaml` | Roteiros de comando das cenas, da verificação e da persiana |
| `automations.yaml` | Rotinas de conforto de cada cena, avisos ao cuidador e sugestão diária |
| `intents.yaml` | Respostas aos comandos de voz |
| `custom_sentences/pt-BR/cenas.yaml` | Frases aceitas pelos assistentes de voz locais |
| `sugestao/sugestao.py` | Sugestão de rotinas a partir da agenda, do ciclo e do histórico da cena Calmante |
| `themes.yaml` | Tema dos painéis |

Os painéis do morador e do cuidador são os da pasta [`simulacao/`](../simulacao/), sem alteração, porque as entidades têm os mesmos nomes nas duas configurações. O painel de bancada é exclusivo da simulação.

## Dispositivos de conforto

As rotinas de conforto usam os nomes abaixo. Os dispositivos são pareados pela interface da plataforma e devem receber estes identificadores.

| Entidade | Dispositivo |
|---|---|
| `light.lampadas` | Grupo com as nove lâmpadas Zigbee |
| `light.fita_sala` e `light.fita_suite` | Fitas indiretas da sala e da suíte |
| `light.nao_perturbe` | Fita RGB do indicador de não perturbe |
| `cover.cortina_da_sala` | Cortina da sala |
| `remote.emissor_ir_suite` | Emissor infravermelho do ar-condicionado, com os comandos `frio_22`, `frio_23` e `frio_24` aprendidos |
| `media_player.assistente_suite` | Assistente de voz da suíte |
| `notify.mobile_app_celular_morador` e `notify.mobile_app_celular_cuidador` | Celulares com o aplicativo da plataforma |
| `calendar.agenda` | Calendário que o morador decidir integrar |

## Sugestão de rotinas

A automação diária chama o `sugestao.py`, que lê a agenda, o registro do ciclo e o histórico da cena Calmante pelo próprio servidor e devolve um texto. A rotina apenas sugere: nenhuma cena é ativada por ela e nenhum dado sai da residência. Ela só funciona com a opção "Receber sugestões de rotina" ligada pelo morador, e só usa o histórico e o ciclo com as opções correspondentes ligadas.

O programa precisa de um token de acesso de longa duração, criado no perfil do usuário e salvo em `/config/sugestao/token.txt`. Esse arquivo não faz parte do repositório.

## Estado

Os endereços seguem o mapa de memória do LOGO! 8.4 e os tempos de leitura são valores de projeto. A operação com o controlador e os dispositivos reais depende da etapa de bancada.
