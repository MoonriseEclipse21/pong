#!/bin/bash
# Определение цветов
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color - для возвращения к стандартному цвету терминала

# компиляция программы
function compile() {
    gcc -Wall -Wextra -Werror -std=c11 -o s21_cat s21_cat.c || { echo "Compilation failed"; exit 1; }
    echo "Compilation successful"
}

# очистка созданных файлов
function cleanup_files() {
    echo "Cleaning up..."
    rm -f s21_cat
    rm -f test_output.txt
    rm -f pattern.txt
    rm -f test*.txt
    rm -f sys_output.txt
}

# функция для запуска и сравнения вывода ./s21_cat и системного cat
function run_test() {
    local flags="$1"
    local files="$2"

    echo "Running test with flags: '$flags', files: '$files'"

    eval "./s21_cat $flags $files > my_output.txt"
    eval "cat $flags $files > sys_output.txt"

    if diff -u my_output.txt sys_output.txt; then
        echo -e "${GREEN}SUCCESS!${NC}"
    else
        echo -e "${RED}FAILURE!${NC}"
    fi
}

# создание тестовых файлов
function create_test_files() {
    echo "First line 12345" > test1.txt
    echo "Second line 12345" > test1.txt
    echo "" > test1.txt
    echo "4 line 12345" > test1.txt

    echo "1 line 12345" > test2.txt
    echo "Second line 12345" > test2.txt
    echo "" > test2.txt
    echo "4 line 12345" > test2.txt
}

# Тестирование различных случаев
function run_tests() {
    create_test_files

    # тест без флагов
    run_test "" "test1.txt test2.txt"

    # тесты с каждым флагом по отдельности
    run_test "-b" "test1.txt test2.txt"
    run_test "-e" "test1.txt test2.txt"
    run_test "-n" "test1.txt test2.txt"
    run_test "-s" "test1.txt test2.txt"
    run_test "-t" "test1.txt test2.txt"

    # тесты с несколькими флагами по отдельности
    run_test "-be" "test1.txt test2.txt"
    run_test "-ennnn" "test1.txt test2.txt"
    run_test "-nss" "test1.txt test2.txt"
    run_test "-st" "test1.txt test2.txt"
    run_test "-tbe" "test1.txt test2.txt"

    # очистка
    cleanup_files
}

# основная логика скрипта
compile
run_tests