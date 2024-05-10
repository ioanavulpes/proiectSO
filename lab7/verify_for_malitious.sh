#!/bin/bash

# Verifică dacă a fost dat un fișier și un director ca argument
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <file>"
    exit 1
fi

# Obțineți numele fișierului și directorul de destinație
file=$1

# Verifică dacă fișierul există
if [ ! -f "$file" ]; then
    echo "File not found: $file"
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


if [ "$lines" -lt 3 ] && [ "$words" -gt 1000 ] && [ "$chars" -gt 2000 ]; then
    
    keywords=("corrupted" "dangerous" "risk" "attack" "malware" "malicious")
    
    for keyword in "${keywords[@]}"; do
	# Verifică dacă cuvântul cheie este în fișier
	if grep -qi "$keyword" "$file"; then
            # Setează is_suspect la 1 dacă este găsit cuvântul cheie
            is_suspect=1
            break  # Iese din ciclu
	fi
    done
    
    if LC_ALL=C grep -q '[^[:print:]]' "$file"; then
	is_suspect=1
    fi
fi

chmod 000 "$file"

# Mută fișierul dacă este suspect sau afișează "SAFE"
if [ "$is_suspect" -eq 1 ]; then
    # Mută fișierul în directorul de destinație
   #mv "$file" "$dest_dir"
    echo "$file"
fi

if [ "$is_suspect" -eq 0 ]; then
   echo "SAFE"
fi