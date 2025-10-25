#!/bin/bash
# Recorre archivos APF_<instancia>.dat, toma el peor valor (máximo) de las dos primeras columnas,
# le suma 1 a cada uno y lo guarda en Peores_optimos_<instancia>.dat
# Uso: ./compute_peores_optimos.sh [directorio]

set -euo pipefail
DIR=${1:-./Results_nsga2_100gen}

shopt -s nullglob
found=0
for f in "$DIR"/APF_*.dat; do
  [ -f "$f" ] || continue
  found=1
  base=$(basename "$f")
  inst=${base#APF_}
  inst=${inst%.dat}

  # Calcular máximos de las dos primeras columnas y sumar 1
  read -r out1 out2 < <(awk '
    NF>=1 {
      if (!seen1) {max1=$1; seen1=1} else if ($1>max1) max1=$1;
      if (NF>=2) { if (!seen2) {max2=$2; seen2=1} else if ($2>max2) max2=$2; }
    }
    END {
      if (!seen1) { max1=0 }
      if (!seen2) { max2=0 }
      printf "%.6f %.6f\n", (max1+1), (max2+0.1)
    }
  ' "$f")

  out_file="${DIR}/Peores_optimos_${inst}.dat"
  printf "%s %s\n" "$out1" "$out2" > "$out_file"
  echo "Escrito $out_file (a partir de $base)"

done

if [ $found -eq 0 ]; then
  echo "No se encontraron archivos APF_*.dat en $DIR" >&2
  exit 1
fi
