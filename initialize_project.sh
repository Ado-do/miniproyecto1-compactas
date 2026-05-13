#!/bin/bash
set -e

declare -A texts=(
    ["english.50MB"]="https://pizzachili.dcc.uchile.cl/texts/nlang/english.50MB.gz"
    ["dna.50MB"]="https://pizzachili.dcc.uchile.cl/texts/dna/dna.50MB.gz"
    ["xml.50MB"]="https://pizzachili.dcc.uchile.cl/texts/xml/dblp.xml.50MB.gz"
)

echo "* Descargando textos desde Pizza&Chili..."
for name in "${!texts[@]}"; do
    url="${texts[$name]}"
    file="input/${name}"
    file_gz="${file}.gz"

    if [ -f "$file" ]; then
        echo "Texto $file ya existe!"
        continue
    fi

    wget -q "$url" -O "$file_gz"
    gunzip -c "$file_gz" > "$file"
    rm "$file_gz"

    echo "Texto $file descargado!"
done

echo "* Compilando..."
chmod +x compile.sh execute_benchmarks.sh execute_tests.sh plot.sh
cmake -S . -B build
./compile.sh
