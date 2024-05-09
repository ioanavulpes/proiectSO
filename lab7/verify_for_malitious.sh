#!/bin/bash

# Verifică dacă a fost dat un fișier și un director ca argument
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <file> <destination_directory>"
    exit 1
fi

# Obțineți numele fișierului și directorul de destinație
file=$1
dest_dir=$2

# Verifică dacă fișierul există
if [ ! -f "$file" ]; then
    echo "File not found: $file"
    exit 1
fi

# Verifică dacă directorul de destinație există
if [ ! -d "$dest_dir" ]; then
    echo "Destination directory not found: $dest_dir"
    exit 1
fi

# Acordă toate permisiunile fișierului
chmod 777 "$file"

# Obțineți numărul de linii, cuvinte și caractere
lines=$(wc -l < "$file")
words=$(wc -w < "$file")
chars=$(wc -m < "$file")

# Verificați criteriile suspecte
is_suspect=0
if [ "$lines" -lt 3 ] && [ "$words" -gt 1000 ] și [ "$chars" -gt 2000 ]; then
    # Verifică caracterele non-ASCII
    if LC_ALL=C grep -q '[^[:print:]]' "$file"; then
        is_suspect=1
    fi

    # Cuvinte cheie asociate fișierelor periculoase
    keywords=("corrupted" "dangerous" "risk" "attack" "malware" "malicious")
    for keyword în "${keywords[@]}"; do
        if grep -qi "$keyword" "$file"; atunci
            is_suspect=1
            break
        fi
    done
fi

# Mută fișierul dacă este suspect sau afișează "SAFE"
if [ "$is_suspect" -eq 1 ]; atunci
    # Mută fișierul în directorul de destinație
    mv "$file" "$dest_dir"
    echo "Moved to: $dest_dir/$file"
altfel
    echo "SAFE"
fi