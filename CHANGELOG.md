# Changelog

Todas as alterações notáveis no firmware do Sistema de Detecção da Qualidade do Ar (ESP32) serão documentadas neste arquivo.

O formato é baseado no [Keep a Changelog](https://keepachangelog.com/pt-BR/1.0.0/),
e este projeto adere ao [Versionamento Semântico](https://semver.org/lang/pt-BR/).

## [0.3.2] - 2026-11-05

### Added

* Integração com display OLED I2C (SSD1306) adicionando os novos módulos `display_manager.h` e `display_manager.cpp`.
* Lógica de alternância assíncrona (não-bloqueante) na tela OLED para exibir Temperatura (5 segundos) e Umidade (3 segundos).
* Adição das constantes de configuração do display OLED (largura, altura, reset, endereço I2C) no arquivo `config.h`.

### Changed

* Atualização do loop principal no arquivo `qualidade_ar.ino` para alimentar a tela continuamente com as últimas variáveis lidas (temperatura e umidade), mantendo a arquitetura não-bloqueante.

## [0.3.1] - 2026-11-05

### Added

* Implementação de rotina de filtragem de sinal (Moving Average) no arquivo principal para estabilizar o valor lido pelo MQ-5 antes da classificação do nível de qualidade.
* Lógica de preenchimento inicial (warm-up lógico) do buffer da média móvel para evitar disparos falsos de alertas no primeiro ciclo de leitura após a inicialização.

## [0.3.0] - 2026-11-05

### Added

* Inclusão das bibliotecas e módulos de rede e de alertas (`wifi_manager.h`, `telegram_notifier.h` e `air_quality.h`) no escopo principal.
* Implementação da função auxiliar `formatMessage` para estruturar as mensagens de log/alertas contendo os dados do MQ-5, Temperatura e Umidade de forma padronizada.
* Adição de verificação de aquecimento (`isWarmedUp()`) para o sensor MQ-5 antes de iniciar as leituras reais.

### Fixed

* Correção crítica de sintaxe no cálculo de tempo não-bloqueante (`millis() - lastRead`) que impedia a compilação, liberando o sistema para os testes contínuos de bancada.

### Changed

* Padronização de todos os comentários do arquivo principal (`qualidade_ar.ino`) para o idioma inglês.
* Atualização das saídas do Monitor Serial para exibir simultaneamente as métricas de Temperatura, Umidade Relativa e a classificação do ar.

## [0.2.0] - 2026-11-05

### Added

* Implementação inicial de arquitetura assíncrona no loop principal utilizando a função `millis()` para evitar travamentos (delays bloqueantes).
* Integração da leitura periódica de temperatura e umidade (DHT11/DHT22) no fluxo não-bloqueante.

### Changed

* Refatoração estrutural do `loop()` principal visando suportar a concorrência de tarefas de hardware (sensores) com futuras comunicações de rede.

## [0.1.0] - 2026-10-29

### Added

* Configuração inicial do ambiente de desenvolvimento de firmware para o microcontrolador ESP32.
* Estruturação modular do projeto separando a lógica em múltiplos arquivos (cabeçalhos `.h` e implementações `.cpp`).
* Implementação da leitura bruta, direta e bloqueante do sensor de gás MQ-5 para validação inicial de hardware (Prova de Conceito).