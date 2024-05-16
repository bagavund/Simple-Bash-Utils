# Simple Bash Utils

## Описание

Проект "Simple Bash Utils" представляет собой реализацию базовых утилит Bash: `cat` и `grep`, написанных на языке C. Цель проекта - обучение и разработка текстовых утилит.

## Требования

- Python 3.8 или выше
- Необходимые библиотеки Python:
  - pytest
  - pyperclip

## Установка

1. Склонируйте репозиторий:

    ```bash
    git clone https://github.com/bagavund/Simple-Bash-Utils.git
    cd Simple-Bash-Utils
    ```

2. Выполните сборку:

    ```bash
    make
    ```

## Запуск

Используйте следующие команды для запуска утилит:

```bash
./s21_cat [OPTIONS] [FILE]...
./s21_grep [OPTIONS] PATTERN [FILE]...
