# Simulação do servidor em máquina virtual

Esta pasta guarda a configuração usada para rodar o Home Assistant em uma máquina virtual (VirtualBox), reproduzindo o comportamento do sistema sem nenhum equipamento físico.

| Arquivo | Conteúdo |
|---|---|
| `configuration.yaml` | Entidades simuladas e lógica que imita o CLP |
| `scripts.yaml` | Roteiros auxiliares da simulação (prefixo `aux_`) |
| `paineis_morador_cuidador_bancada.yaml` | Painéis, no editor de configuração bruta |

## Como reproduzir

1. Instale o VirtualBox e importe a imagem oficial do Home Assistant OS (arquivo .vdi).
2. Configure a rede da VM em modo bridge e inicie a máquina.
3. Abra o endereço mostrado na tela da VM (porta 8123) e crie o usuário.
4. Instale o complemento File Editor e copie os arquivos desta pasta para `/config`.
5. Reinicie o Home Assistant e importe os painéis pelo editor de configuração bruta de cada painel.
