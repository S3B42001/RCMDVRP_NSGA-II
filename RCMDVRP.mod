# RCMDVRP con múltiples objetivos

# -------------------- CONJUNTOS --------------------
set O ; # depositos
set S ; # depositos ficticios
set R ; # clientes

set A1 := {i in O, j in R} ; # arcos deposito - clientes
set A2 := {i in R, j in R: i != j } ; # arcos clientes - clientes
set A3 := {i in R, j in S} ; # arcos clientes - deposito ficticio

set N := O union R union S ; # conjunto de nodos de la red
set A := A1 union A2 union A3 ; # conjunto de arcos de la red
set K ; # conjunto de vehiculos

# -------------------- PARÁMETROS --------------------
param b ; # capacidad de los vehiculos
param dm{N} ; # demanda de un cliente
param d{i in N, j in N} >= 0 default 0;    # Distancia entre nodos
# param Dinit{i in O, k in K} >= 0;          # Efectivo transportado al salir del nodo i
param Rinit{i in O, k in K} >= 0;          # Riesgo acumulado al salir del nodo i
# param e;                                   # Término constante en emisiones
param f{l in 1..4};                         # Coeficientes de emisiones
param theta >= 0;                             # Umbral de riesgo permitido
param M default 1000000;                      # Valor grande para MTZ

param peso_vacio ;                 # Peso en vacío del vehículo
param v {i in N, j in N} >= 0 default 10;  # Velocidad del tramo (i,j)

# Coeficientes para f_l (COPERT model)
param alpha{l in 0..4} default 0;
param beta{l in 0..4} default 0;
param gamma{l in 0..4} default 0;
param delta{l in 0..4} default 0;
param epsilon{l in 0..4} default 0;
param zeta{l in 0..4} default 0;
param hta{l in 0..4} default 1; 

/* Conjuntos, Parametros y Variables para la normalización */
param Mi default 0 ;
param Mf default 0 ;

param cantobj := 2 ; # cantidad de objetivos del problema
param cantejc := 11 ; # cantidad de ejecuciones para la frontera de pareto
set objetivos := {1..cantobj} ; # conjunto de objetivos del problema
set ejecuciones := {1..cantejc} ; # conjunto de ejecuciones para la frontera de pareto
param g default 0 ; # identifica un objetivo en particular
param sigma{ejecuciones,objetivos} ; # ponderadores para la frontera de pareto
param betha{objetivos} default 0 ; # ponderadores de cada objetivo
param MV{objetivos} default 999999999 ; # mejor valor alcanzado por cada objetivo
param PV{objetivos} default 0 ; # peor valor alcanzado por cada objetivo
var F{objetivos} >= 0 ; # funciones objetivos del problema

# -------------------- VARIABLES --------------------
var x{i in N, j in N, k in K: (i,j) in A} binary;      # 1 si el vehículo k va de i a j
var y{i in R, k in K} binary;               # 1 si cliente i es atendido por k
var u{i in R, k in K} >= 0;                 # Para MTZ (evitar subrutas)

var carga{i in N, k in K} >= 0;  # efectivo en el vehículo en nodo i

minimize FO1 : F[g] ; # para minimizar cada objetivo por separado
minimize FO2:
    sum {i in objetivos}
        if MV[i] != PV[i] then
            betha[i] * (MV[i] - F[i]) / (MV[i] - PV[i])
        else
            0;

subject to
# -------------------- OBJETIVOS --------------------

O1 : F[1] = sum{(i,j) in A, k in K} d[i,j] * x[i,j,k];

O2 : F[2] = 
    sum{(i,j) in A, k in K} 
        sum{l in 0..4} 
            d[i,j] * (peso_vacio + carga[i,k]) * 
            ((alpha[l]*v[i,j]^2 + beta[l]*v[i,j] + gamma[l] + delta[l]/v[i,j]) / 
             (epsilon[l]*v[i,j]^2 + zeta[l]*v[i,j] + hta[l])) 
            * x[i,j,k];

# -------------------- RESTRICCIONES --------------------

# Cada vehículo comienza su ruta en un único depósito
R1 {k in K}:
    sum {i in O, j in R} x[i,j,k] <= 1;

# Se asegura de que si hay visitas, el vehículo salga de un depot.
R2 {k in K}:
    M*sum {i in O, j in R} x[i,j,k] >= sum {j in R} y[j,k];

# Retorno al depósito original
R3 {i in O, k in K}:
    sum {j in R: (i,j) in A} x[i,j,k] = sum {j in R: (j,i) in A} x[j,i,k];

# Relación entre x e y (solo tiene sentido en nodos cliente)
R4 {i in R, k in K}:
    y[i,k] = sum {j in N: (i,j) in A} x[i,j,k];

# Cada cliente debe ser visitado exactamente una vez
R5 {j in R}:
    sum {i in N, k in K: (i,j) in A} x[i,j,k] = 1;

# Restricción de capacidad
R6 {k in K}:
    sum {i in R} dm[i] * y[i,k] <= b;

#Secuencia de visitas
R7 {m in R, k in K}:
    sum {i in N: (i,m) in A} x[i,m,k] = sum {j in N: (m,j) in A} x[m,j,k] ;


# Restricción de riesgo total por vehículo
R8 {k in K}:
    sum {i in O, j in N: (i,j) in A} Rinit[i,k] * x[i,j,k] <= theta;
    
# MTZ para eliminar subrutas
R9 {i in R, j in R, k in K: i != j}:
    u[i,k] - u[j,k] + card(R) * x[i,j,k] <= card(R) - 1;
    
