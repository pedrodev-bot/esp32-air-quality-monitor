# Changelog
Todas as alterações notáveis no firmware do Sistema de Detecção da Qualidade do Ar (ESP32) serão documentadas neste arquivo.

O formato é baseado no [Keep a Changelog](https://keepachangelog.com/pt-BR/1.0.0/),
e este projeto adere ao [Versionamento Semântico](https://semver.org/lang/pt-BR/).

## [0.3.0] - 2026-11-05
### Added
- Inclusão das bibliotecas e módulos de rede/alertas (`wifi_manager.h`, `telegram_notifier.h` e `air_quality.h`) no escopo principal.
- Implementação da função auxiliar `formatMessage` para estruturar as mensagens de log/alertas contendo os dados do MQ-5, Temperatura e Umidade de forma consolidada.
- Adição de verificação de aquecimento (`isWarmedUp`) para o sensor MQ-5 antes de iniciar as leituras.

### Fixed
- Correção crítica de sintaxe no cálculo de tempo não-bloqueante (`millis() - lastRead`) que impedia a compilação. Essa correção viabiliza os testes contínuos de bancada para Temperatura e Umidade.

### Changed
- Padronização de todos os comentários do arquivo principal (`qualidade_ar.ino`) para a língua inglesa, conforme regra de padronização do repositório.
- Atualização das saídas do Monitor Serial para exibir simultaneamente o status da rede, as métricas de T/UR e a classificação de qualidade do ar.

## [0.2.0] - 2026-11-05
### Added
- Implementação de arquitetura assíncrona no loop principal utilizando `millis()` para evitar bloqueios de processamento.
- Adicionada rotina de filtragem por média móvel (Moving Average) para estabilizar as leituras brutas do sensor MQ-5.
- Integração da leitura periódica de temperatura e umidade (DHT11/DHT22) no fluxo não-bloqueante.

### Changed
- Refatoração estrutural do `loop()` principal para suportar concorrência de tarefas de hardware (sensores) e rede simultaneamente.

## [0.1.0] - 2026-10-29
### Added
- Configuração inicial do ambiente de desenvolvimento (ESP32).
- Estruturação modular do projeto em múltiplos arquivos (cabeçalhos e implementações separadas).
- Implementação da leitura bruta, direta e bloqueante do sensor de qualidade do ar MQ-5 para validação inicial de hardware (Proof of Concept).