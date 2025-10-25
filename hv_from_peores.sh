#!/bin/bash
# Ejecuta el cálculo de hipervolumen para todas las instancias, usando:
#  - APF_<instancia>.dat (o Apf_<instancia>.dat)
#  - peores_optimos_<instancia>.dat (o Peores_optimos_<instancia>.dat)
# Invoca: <hv_dir>/hv -r "<peor1> <peor2>" <APF_file>
# Uso:
#   ./hv_from_peores.sh [APF_DIR=. ] [HV_DIR=hv-1.3-src] [SALIDA=hv_apf_results.out]
# Salida:
#   - Imprime por stdout y guarda en SALIDA líneas: <archivo_apf> <peor1> <peor2> <hv>
# Notas:
#   - Si no encuentra peores_optimos_<inst>.dat, intenta calcular (max col1 +1, max col2 +1) del APF.
#   - Acepta variantes de mayúsculas/minúsculas en los prefijos APF/Apf y peores/Peores.

set -euo pipefail
shopt -s nullglob

APF_DIR=${1:-./Results_nsga2_10000gen}
HV_DIR=${2:-hv-1.3-src}
OUTPUT=${3:-hv_apf_results.out}

HV_BIN="${HV_DIR}/hv"
if [[ ! -x "${HV_BIN}" ]]; then
  echo "Error: no se encontró ejecutable de hv en '${HV_BIN}'. Compila o ajusta HV_DIR." >&2
  exit 1
fi

echo "=== Cálculo de Hipervolumen para Archivos APF ===" >&2
echo "Directorio APF: ${APF_DIR}" >&2
echo "Directorio HV:  ${HV_DIR}" >&2
echo "Archivo salida: ${OUTPUT}" >&2
echo "" >&2

# Preparar salida
: >"${OUTPUT}"

# Recolectar archivos APF con ambas variantes de nombre
apf_files=("${APF_DIR}"/APF_*.dat "${APF_DIR}"/Apf_*.dat)
found_any=0
total_files=0
for apf in "${apf_files[@]}"; do
  [[ -f "${apf}" ]] || continue
  found_any=1
  total_files=$((total_files + 1))
  base=$(basename "${apf}")

  echo "[$total_files] Procesando: ${base}" >&2

  # Obtener identificador <instancia>
  inst=${base#APF_}
  if [[ "${inst}" == "${base}" ]]; then
    inst=${base#Apf_}
  fi
  inst=${inst%.dat}
  echo "    Instancia ID: ${inst}" >&2

  # Buscar archivo de peores óptimos con distintas variantes
  peor_file="${APF_DIR}/Peores_optimos_${inst}.dat"
  [[ -f "${peor_file}" ]] || peor_file="${APF_DIR}/peores_optimos_${inst}.dat"

  peor1=""
  peor2=""
  if [[ -f "${peor_file}" ]]; then
    # Leer dos valores (puede tener espacios/tabs)
    read -r peor1 peor2 < "${peor_file}" || true
    echo "    Peores óptimos leídos de: $(basename "${peor_file}")" >&2
    echo "    Punto de referencia: (${peor1}, ${peor2})" >&2
  fi

  # Si faltan peores, calcularlos a partir del APF (max col1/col2 + 1)
  if [[ -z "${peor1:-}" || -z "${peor2:-}" ]]; then
    echo "    No se encontró archivo de peores óptimos, calculando desde APF..." >&2
    read -r peor1 peor2 < <(awk '
      NF>=1 {
        if (!s1) {m1=$1; s1=1} else if ($1>m1) m1=$1;
        if (NF>=2) { if (!s2) {m2=$2; s2=1} else if ($2>m2) m2=$2; }
      }
      END {
        if (!s1) m1=0; if (!s2) m2=0;
        printf "%.6f %.6f\n", (m1+1), (m2+1);
      }
    ' "${apf}")
    echo "    Punto de referencia calculado: (${peor1}, ${peor2})" >&2
  fi

  # Contar soluciones en APF
  num_solutions=$(wc -l < "${apf}" | tr -d ' ')
  echo "    Soluciones en APF: ${num_solutions}" >&2

  # Ejecutar HV
  echo "    Ejecutando: ${HV_BIN} -r \"${peor1} ${peor2}\" ${apf}" >&2
  hv_out=$("${HV_BIN}" -r "${peor1} ${peor2}" "${apf}" 2>&1 || true)
  hv=$(echo "${hv_out}" | tail -n1)
  
  echo "    Hipervolumen: ${hv}" >&2
  echo "" >&2

  # Registrar solo instancia y hipervolumen
  echo "${inst} ${hv}" >> "${OUTPUT}"

done

if [[ ${found_any} -eq 0 ]]; then
  echo "No se encontraron archivos APF_*.dat o Apf_*.dat en '${APF_DIR}'." >&2
  exit 1
fi

echo "=== Resumen ===" >&2
echo "Total de archivos procesados: ${total_files}" >&2
echo "Resultados guardados en: ${OUTPUT}" >&2
echo "" >&2
echo "Contenido de ${OUTPUT}:" >&2
cat "${OUTPUT}"
