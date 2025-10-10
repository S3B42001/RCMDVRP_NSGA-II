#!/bin/bash

mkdir -p Results_nsga2

# Lista de instancias
# INSTANCIAS=(11 20 26 38 53 65 80 95 126 146 210 338)
INSTANCIAS=(11 20 26 38 53 65 80)
# INSTANCIAS=(95 126 146 210 338)

for i in "${INSTANCIAS[@]}"; do
    # Generar número aleatorio entre 0 y 1 con 9 decimales
    RAND=$(awk -v seed=$RANDOM 'BEGIN { srand(seed); printf("%.9f\n", rand()) }')

    echo "Ejecutando instancia $i con semilla $RAND"

    # Ejecutar el programa
    ./nsga2r "$RAND" "Instances/Instance${i}.dat" 100 100 2 0.6 0.1

    # Renombrar/mover los archivos de salida
    mv initial_pop.out "Results_nsga2/initial_pop_Instance${i}.out"
    mv final_pop.out   "Results_nsga2/final_pop_Instance${i}.out"
    mv best_pop.out    "Results_nsga2/best_pop_Instance${i}.out"
done
