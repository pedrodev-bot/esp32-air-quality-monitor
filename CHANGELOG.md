# Changelog

## \[0.2.0\] \- 2026-11-05

### Adicionado

- Implementação de arquitetura assíncrona no loop principal utilizando \`millis()\` para evitar bloqueios.  
- Rotina de filtragem por média móvel (Moving Average) para suavização de ruído do sensor MQ-5.  
- Integração da leitura periódica de temperatura e umidade (DHT11/DHT22) no fluxo principal.&nbsp;

### Mudanças

- Refatoração do \`qualidade\_ar.ino\` para suportar concorrência de tarefas de hardware e chamadas futuras de rede.&nbsp;

## \[0.1.0\] \- 2026-10-29

### Adicionado

- Leitura bruta do MQ-5 (ADC1, GPIO34) com media de 16 amostras.  
- Leitura de temperatura/umidade via DHT (DHTesp; DHT22 no Wokwi, DHT11 em hardware).  
- Classificacao provisoria BOM/ATENCAO/CRITICO por limiar do MQ-5.  
- Wi-Fi com reconexao nao bloqueante.  
- Alertas via Telegram com cooldown e aviso de normalizacao.  
- Segredos em `secrets.h` (ignorado pelo git) \+ template `secrets.example.h`.  
- Projeto Wokwi (`wokwi/`) e script `tools/gerar_wokwi.sh`.

### Limitacoes conhecidas

- Limiares do MQ-5 sao provisorios (calibracao na v0.2.0).  
- Envio ao Telegram e bloqueante (\~1-3 s).  
- TLS com `setInsecure()` (validacao de certificado prevista para v0.9.0).