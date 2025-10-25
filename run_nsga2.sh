#!/bin/bash

mkdir -p Results_nsga2

# Número de repeticiones (por defecto 1)
REPS=${1:-1}
if ! [[ "$REPS" =~ ^[1-9][0-9]*$ ]]; then
    echo "Uso: $0 [repeticiones_pos_int]" >&2
    echo "Ejemplo: $0 5  # ejecuta todas las instancias 5 veces" >&2
    exit 1
fi
echo "Repeticiones: $REPS"

# Archivo de tiempos (CSV)
TIMES_FILE="Results_nsga2/times.csv"
if [ ! -f "$TIMES_FILE" ]; then
    echo "timestamp,instance,iteration,seed,seconds" > "$TIMES_FILE"
fi

# Lista de instancias
INSTANCIAS=(11 20 26 38 53 65 80 95 126 146 210 338)
# INSTANCIAS=(20)

for rep in $(seq 1 "$REPS"); do
    echo "--- Iteración $rep / $REPS ---"
    for i in "${INSTANCIAS[@]}"; do
            # Generar número aleatorio entre 0 y 1 con 9 decimales usando /dev/urandom
            # Tomamos 6 bytes de /dev/urandom, los convertimos a entero y los normalizamos a (0,1)
            RAND=$(od -An -N6 -tu8 /dev/urandom | tr -d ' ' | awk '{printf("%.9f", ($1 / 281474976710656))}')

            echo "Ejecutando instancia $i (iter $rep) con semilla $RAND"

            # Medición de tiempo de ejecución
            START_NS=$(date +%s%N)

            # Ejecutar el programa
            ./nsga2r "$RAND" "Instances/Instance${i}b.dat" 100 100 2 0.6 0.1

            END_NS=$(date +%s%N)
            ELAPSED_NS=$((END_NS - START_NS))
            # Convertir a segundos con 3 decimales
            ELAPSED_SEC=$(awk -v ns="$ELAPSED_NS" 'BEGIN { printf("%.3f", ns/1e9) }')
            echo "Tiempo instancia $i (iter $rep): ${ELAPSED_SEC}s"
            echo "$(date +%F_%T),Instance${i},${rep},${RAND},${ELAPSED_SEC}" >> "$TIMES_FILE"

            # Renombrar/mover los archivos de salida (si existen)
            [ -f initial_pop.out ] && mv initial_pop.out "Results_nsga2/initial_pop_Instance${i}_it${rep}.out"
            [ -f final_pop.out ]   && mv final_pop.out   "Results_nsga2/final_pop_Instance${i}_it${rep}.out"
            [ -f best_pop.out ]    && mv best_pop.out    "Results_nsga2/best_pop_Instance${i}_it${rep}.out"
    done
done
for i in "${INSTANCIAS[@]}"; do
    [ -f APF_${i}.dat ] && mv APF_${i}.dat "Results_nsga2/APF_${i}.dat"
done