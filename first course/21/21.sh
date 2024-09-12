#!/bin/bash

# Функция для проверки размера файла
function check_size {
    local file=$1
    local max_size=$2
    local size=$(du -k "$file" | cut -f1)
    if [ $size -le $max_size ]; then
        echo $file >> output.txt
    fi
}

# Функция для поиска файлов с заданным суффиксом
function find_files {
    local suffix=$1
    local max_size=$2
    find . -type f -name "*$suffix" | while read -r file
    do
        check_size "$file" $max_size
    done
}

# Основной код
suffix=".txt"  # Замените на нужный вам суффикс
max_size=1024  # Замените на нужный вам максимальный размер в килобайтах

# Очищаем файл output.txt
> output.txt

# Находим и проверяем файлы
find_files $suffix $max_size
