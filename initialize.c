/* Data initializtion routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <time.h>

# include "global.h"
# include "rand.h"


/* Function to initialize a population randomly */
void initialize_pop (population *pop)
{
    printf("\n Initializing population \n");
    int i;
    for (i=0; i<popsize; i++)
    {
        initialize_ind (&(pop->ind[i]));
    }
    return;
}

/* Function to initialize an individual randomly */
void initialize_ind (individual *ind)
{
    /* printf("\n Initializing individual \n"); */
    int clientes[MAX_NODES];
    int i;
    int j = 0;
    int pos = 0;
    int carga = 0;
    int capacidad = b;
    double riesgo = 0.0;
    double riesgo_max = theta * 0.95;

    int n_veh = 1;

    int n_depositos_usados = 1;
    int deposito = set_O[n_depositos_usados - 1];
    int cliente_anterior = deposito;
    int separador = -1;

    int tmp;

    for (i = 0; i < n_customers; i++) {
        clientes[i] = set_R[i];
    }
    
    for (i = n_customers - 1; i > 0; i--) {
        /* Use the internal RNG (rnd) seeded by program seed via randomize() */
        j = rnd(0, i);
        /* j = rand() % (i + 1); */
        tmp = clientes[i]; 
        clientes[i] = clientes[j]; 
        clientes[j] = tmp;
    }

    for (i = 0; i < n_customers; i++) {
        if (n_veh <= (n_vehicles * n_depots)){
            int c = clientes[i];
            int demanda = dm[c];
            double riesgo_cliente = carga * d[cliente_anterior][c];
            double riesgo_presente = riesgo + riesgo_cliente;
            double riesgo_futuro = riesgo_presente + (demanda + carga) * d[c][deposito];
            if ((carga + demanda > capacidad) || (riesgo_presente > riesgo_max) || (riesgo_futuro > riesgo_max)) {
                ind->route[pos++] = separador;
                riesgo += carga * d[cliente_anterior][deposito];
                separador -= 1;
                carga = 0;
                riesgo = 0.0;
                n_veh++;
                if (n_veh > n_vehicles) {
                    deposito = set_O[n_depositos_usados++]; 
                }
                cliente_anterior = deposito;
                i--; 
            } else {
                cliente_anterior = c; 
                ind->route[pos++] = c;
                carga += demanda;
                riesgo += riesgo_cliente;
            }
        } else {
            ind->route[pos++] = clientes[i];
            riesgo += carga * d[cliente_anterior][clientes[i]];
            carga += demanda;
            cliente_anterior = clientes[i];
        }
    }
    ind->route[pos++] = separador;
    ind->route_length = pos;
    /* printf("\n Individual initialized with %d nodes in route\n", ind->route_length);
    printf("\n Individual route: ");
    for (i = 0; i < ind->route_length; i++) {
        printf("%d ", ind->route[i]);
    }
    printf("\n"); */
    return;
}
