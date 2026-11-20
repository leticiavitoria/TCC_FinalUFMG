#!/usr/bin/env python3
"""Sugestão de rotinas do servidor local.

Cruza a agenda do morador e o registro do ciclo, quando habilitado por ele,
com o histórico de acionamento da cena Calmante, e escreve na saída padrão
uma sugestão para o dia. O programa apenas sugere: não ativa cena,
não escreve no CLP e não envia nenhum dado para fora da residência.

É chamado todas as manhãs pela automação "Sugestão de rotina do dia",
que lê a saída e a entrega ao morador. Sem motivo para sugerir, a saída é vazia.

Regras (valores de projeto, ajustáveis nas constantes abaixo):
  1. Compromisso exigente amanhã na agenda (prova, entrega, apresentação...)
     -> cena Foco hoje à tarde e cena Sono mais cedo à noite.
  2. Semana carregada: compromissos dos últimos 7 dias acima da média das
     4 semanas anteriores, ou cena Calmante acionada mais vezes que o habitual
     -> cena Calmante ao fim do dia.
  3. Ciclo: se hoje cai na semana do ciclo em que a cena Calmante foi mais
     acionada no histórico -> cena Calmante de forma preventiva.
"""
import json
import sys
import urllib.parse
import urllib.request
from datetime import datetime, timedelta, timezone

# ------------------------- configuração -------------------------
URL = "http://localhost:8123/api"
ARQUIVO_TOKEN = "/config/sugestao/token.txt"   # token de acesso de longa duração
CALENDARIO = "calendar.agenda"                  # calendário que o morador integrar
CENA_CALMANTE = "binary_sensor.cena_calmante"

PALAVRAS_EXIGENTES = ("prova", "entrega", "apresenta", "reunião", "reuniao",
                      "plantão", "plantao", "viagem", "médico", "medico", "exame")
MIN_COMPROMISSOS_SEMANA = 5     # mínimo para a semana contar como carregada
MIN_CALMANTE_SEMANA = 3         # mínimo de acionamentos da Calmante na semana
FATOR_ACIMA_DA_MEDIA = 1.5      # quanto acima da média das 4 semanas anteriores
DIAS_HISTORICO_CICLO = 120      # janela do histórico usada na regra do ciclo
MIN_CALMANTE_FASE = 3           # mínimo de acionamentos na semana do ciclo


# ------------------------- acesso ao servidor -------------------------
def _get(caminho, parametros=None):
    with open(ARQUIVO_TOKEN, encoding="utf-8") as arq:
        token = arq.read().strip()
    url = URL + caminho
    if parametros:
        url += "?" + urllib.parse.urlencode(parametros)
    req = urllib.request.Request(url, headers={"Authorization": "Bearer " + token})
    with urllib.request.urlopen(req, timeout=20) as resp:
        return json.loads(resp.read().decode("utf-8"))


def estado(entidade):
    try:
        return _get("/states/" + entidade).get("state")
    except Exception:
        return None


def compromissos(inicio, fim):
    """Compromissos do calendário entre dois instantes (lista de títulos)."""
    try:
        eventos = _get("/calendars/" + CALENDARIO,
                       {"start": inicio.isoformat(), "end": fim.isoformat()})
    except Exception:
        return []
    return [str(e.get("summary") or "") for e in eventos]


def acionamentos(entidade, inicio, fim):
    """Instantes em que a entidade passou de desligada para ligada."""
    try:
        dados = _get("/history/period/" + urllib.parse.quote(inicio.isoformat()),
                     {"filter_entity_id": entidade, "end_time": fim.isoformat(),
                      "minimal_response": "", "no_attributes": ""})
    except Exception:
        return []
    instantes = []
    anterior = None
    for registro in (dados[0] if dados else []):
        atual = registro.get("state")
        if atual == "on" and anterior not in (None, "on"):
            quando = registro.get("last_changed") or registro.get("last_updated")
            instantes.append(datetime.fromisoformat(quando))
        anterior = atual
    return instantes


# ------------------------- regras -------------------------
def regra_agenda(agora):
    amanha = (agora + timedelta(days=1)).replace(hour=0, minute=0, second=0, microsecond=0)
    for titulo in compromissos(amanha, amanha + timedelta(days=1)):
        if any(p in titulo.lower() for p in PALAVRAS_EXIGENTES):
            return ("Amanhã tem \"%s\" na agenda. Que tal a cena Foco hoje à tarde "
                    "e a cena Sono um pouco mais cedo à noite?" % titulo.strip())
    return None


def regra_semana(agora, usar_historico):
    semana = timedelta(days=7)
    atual = len(compromissos(agora - semana, agora))
    anteriores = len(compromissos(agora - 5 * semana, agora - semana)) / 4
    carregada = (atual >= MIN_COMPROMISSOS_SEMANA
                 and atual > FATOR_ACIMA_DA_MEDIA * anteriores)
    if not carregada and usar_historico:
        atual = len(acionamentos(CENA_CALMANTE, agora - semana, agora))
        anteriores = len(acionamentos(CENA_CALMANTE, agora - 5 * semana, agora - semana)) / 4
        carregada = (atual >= MIN_CALMANTE_SEMANA
                     and atual > FATOR_ACIMA_DA_MEDIA * anteriores)
    if carregada:
        return "A semana está mais carregada que o habitual. Que tal a cena Calmante ao fim do dia?"
    return None


def regra_ciclo(agora):
    if estado("input_boolean.ciclo_habilitado") != "on":
        return None
    try:
        inicio = datetime.strptime(estado("input_datetime.inicio_ultimo_ciclo"), "%Y-%m-%d").date()
        duracao = int(float(estado("input_number.duracao_ciclo")))
    except (TypeError, ValueError):
        return None
    if duracao <= 0:
        return None

    def semana_do_ciclo(dia):
        return ((dia - inicio).days % duracao) // 7

    por_semana = {}
    for quando in acionamentos(CENA_CALMANTE, agora - timedelta(days=DIAS_HISTORICO_CICLO), agora):
        chave = semana_do_ciclo(quando.astimezone(agora.tzinfo).date())
        por_semana[chave] = por_semana.get(chave, 0) + 1
    if not por_semana:
        return None
    mais_usada = max(por_semana, key=por_semana.get)
    if por_semana[mais_usada] >= MIN_CALMANTE_FASE and semana_do_ciclo(agora.date()) == mais_usada:
        return ("Nesta fase do ciclo a cena Calmante costuma ser mais usada. "
                "Que tal ativá-la hoje antes de precisar?")
    return None


def main():
    if estado("input_boolean.sugestoes_habilitadas") != "on":
        return
    agora = datetime.now(timezone.utc).astimezone()
    usar_historico = estado("input_boolean.registro_habilitado") == "on"
    sugestoes = [regra_agenda(agora), regra_semana(agora, usar_historico)]
    if usar_historico:
        sugestoes.append(regra_ciclo(agora))
    sugestoes = [s for s in sugestoes if s]
    # no máximo duas sugestões por dia, da mais específica para a mais geral
    print(" ".join(sugestoes[:2]))


if __name__ == "__main__":
    try:
        main()
    except Exception as erro:               # falha não gera sugestão nem aviso ao morador
        print("sugestao.py: %s" % erro, file=sys.stderr)
