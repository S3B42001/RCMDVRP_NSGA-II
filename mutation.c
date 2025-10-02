/* Mutation routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Function to perform mutation in a population */
void mutation_pop (population *pop)
{
    int i;
    for (i=0; i<popsize; i++)
    {
        mutation_ind(&(pop->ind[i]));
    }
    return;
}

/* Function to perform mutation of an individual */
void mutation_ind (individual *ind)
{
    real_mutate_ind(ind);
    return;
}

/* Routine for swap mutation of an individual's route (for RCMDVRP) */
void real_mutate_ind (individual *ind)
{
    int i, j;
    /* printf("Mutating individual with route length %d\n", ind->route_length);
    printf("Original individual route: ");
    for (i = 0; i < ind->route_length; i++) {
        printf("%d ", ind->route[i]);
    }
    printf("\n"); */

    if (ind->route_length < 2) return;

    int clientes[MAX_NODES];
    int n_clientes = 0;
    for (i = 0; i < ind->route_length; i++) {
        if (ind->route[i] >= 0) {
            clientes[n_clientes++] = ind->route[i];
        }
    }

    for (j = 0; j < nreal; j++) {
        if (randomperc() <= pmut_real && n_clientes > 1) {
            int pos1, pos2, temp;
            do {
                pos1 = rnd(0, n_clientes - 1);
                pos2 = rnd(0, n_clientes - 1);
            } while (pos1 == pos2);
            temp = clientes[pos1];
            clientes[pos1] = clientes[pos2];
            clientes[pos2] = temp;
            nrealmut += 1;
        }
    }

    int pos = 0;
    int carga = 0;
    double riesgo = 0.0;
    int capacidad = b;
    /* double riesgo_max = theta; */
    double riesgo_max = theta * 0.95; 
    printf("Valor de theta: %lf\n", theta);
    printf("Riesgo maximo permitido: %lf\n", riesgo_max);
    int n_veh = 1;
    int n_depositos_usados = 1;
    int deposito = set_O[n_depositos_usados - 1];
    int cliente_anterior = deposito;
    int separador = -1;

    /* for (i = 0; i < n_clientes; i++) {
        if (n_veh <= (n_vehicles * n_depots)) {
            int c = clientes[i];
            int demanda = dm[c];
            double riesgo_cliente = carga * d[cliente_anterior][c];
            double riesgo_presente = riesgo + riesgo_cliente;
            double riesgo_futuro = riesgo_presente + demanda * d[c][deposito];
            /* printf("Cliente %d: %d, Demanda: %d, Carga actual: %d, Carga + Demanda: %d\n", i + 1, c, demanda, carga, carga + demanda);
            printf("distancia de %d a %d: %lf\n", cliente_anterior, c, d[cliente_anterior][c]);
            printf("distancia de %d a %d: %lf\n", c, deposito, d[c][deposito]);
            printf("Riesgo max: %lf, riesgo cliente: %lf, riesgo actual: %lf, Riesgo proximo cliente: %lf, Riesgo cliente a deposito: %lf\n", riesgo_max, riesgo_cliente, riesgo, riesgo_presente, riesgo_futuro); 
            if ((carga + demanda > capacidad) || (riesgo_presente > riesgo_max) || (riesgo_futuro > riesgo_max)) {
            /* if ((carga + demanda > capacidad) || (riesgo_presente > riesgo_max)) { 
                ind->route[pos++] = separador;
                riesgo += carga * d[cliente_anterior][deposito];
                printf("ruta cerrada con riesgo %lf y carga %d\n", riesgo, carga);
                separador -= 1;
                carga = 0;
                riesgo = 0.0;
                n_veh++;
                if (n_veh > n_vehicles) {
                    deposito = set_O[n_depositos_usados++];
                }
                cliente_anterior = deposito;
                i--;
                continue;
            } else {
                cliente_anterior = c;
                ind->route[pos++] = c;
                carga += demanda;
                riesgo += riesgo_cliente;
            }
        } else {
            int c = clientes[i];
            int demanda = dm[c];
            ind->route[pos++] = c;
            riesgo += carga * d[cliente_anterior][c];
            carga += demanda;
            cliente_anterior = c;
        }
    }
    if (n_depositos_usados > n_depots) {
        printf("ruta cerrada con riesgo %lf y carga %d\n", riesgo, carga);
    }
    ind->route[pos++] = separador;
    ind->route_length = pos; */
    for (i = 0; i < n_customers; i++) {
        if (n_veh <= (n_vehicles * n_depots)){
            int c = clientes[i];
            int demanda = dm[c];
            double riesgo_cliente = carga * d[cliente_anterior][c];
            double riesgo_presente = riesgo + riesgo_cliente;
            double riesgo_futuro = riesgo_presente + (demanda + carga) * d[c][deposito];
            /* printf("Cliente %d: %d, Demanda: %d, Carga actual: %d, Carga + Demanda: %d\n", i + 1, c, demanda, carga, carga + demanda);
            printf("distancia de %d a %d: %lf\n", cliente_anterior, c, d[cliente_anterior][c]);
            printf("distancia de %d a %d: %lf\n", c, deposito, d[c][deposito]);
            printf("Riesgo max: %lf, riesgo cliente: %lf, riesgo actual: %lf, Riesgo proximo cliente: %lf, Riesgo cliente a deposito: %lf\n", riesgo_max, riesgo_cliente, riesgo, riesgo_presente, riesgo_futuro); */
            if ((carga + demanda > capacidad) || (riesgo_presente > riesgo_max) || (riesgo_futuro > riesgo_max)) {
            /* if ((carga + demanda > capacidad) || (riesgo_presente > riesgo_max)) { */
                ind->route[pos++] = separador;
                riesgo += carga * d[cliente_anterior][deposito];
                printf("ruta cerrada con riesgo %lf y carga %d\n", riesgo, carga);
                separador -= 1;
                carga = 0;
                riesgo = 0.0;
                n_veh++;
                if (n_veh > n_vehicles) {
                    deposito = set_O[n_depositos_usados++]; 
                }
                cliente_anterior = deposito;
                i--; 
                /*
                if (n_veh > n_vehicles * n_depots) {
                    ind->route[pos++] = separador;
                }
                */
            } else {
                cliente_anterior = c; 
                ind->route[pos++] = c;
                carga += demanda;
                riesgo += riesgo_cliente;
            }
            /* cliente_anterior = c; 
            ind->route[pos++] = c;
            carga += demanda;
            riesgo += riesgo_cliente; */
        } else {
/*             ind->constr[0] += dm[clientes[i]];
            ind->constr[2] += 1; */
            ind->route[pos++] = clientes[i];
            riesgo += carga * d[cliente_anterior][clientes[i]];
            carga += demanda;
            cliente_anterior = clientes[i];
        }
    }
    if (n_depositos_usados > n_depots) {
        printf("ruta cerrada con riesgo %lf y carga %d\n", riesgo, carga);
    }
    ind->route[pos++] = separador;
    ind->route_length = pos;
    /* printf("Mutated individual route length: %d\n", ind->route_length);
    printf("Mutated individual route: ");
    for (i = 0; i < ind->route_length; i++) {
        printf("%d ", ind->route[i]);
    }
    printf("\n"); */
    return;
}