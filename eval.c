/* Routine for evaluating population members  */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

double compute_emission(int i, int j)
{
    double s = v[i][j];
    if (s == 0) {
        return 0.0; 
    }

    double f = 0.0;
    int l;
    for (l = 1; l < 5; l++) {
        f += ((alpha[l] * pow(s, 2)) + (beta[l] * s) + (gamma_param[l]) + (delta_param[l] / s))/((epsilon[l] * pow(s, 2)) + (zeta[l] * s) + (hta[l]));
        /* printf("f calculado: %lf\n", f); */
    }
    return f;
}

/* Routine to evaluate objective function values and constraints for a population */
void evaluate_pop (population *pop)
{
    printf("\n--- Evaluating population ---\n");
    int i;
    for (i=0; i<popsize; i++)
    {
        evaluate_ind (&(pop->ind[i]));
    }
    return;
}

void evaluate_ind(individual *ind)
{
    ind->constr_violation = 0.0;
    ind->constr[0] = 0.0;
    ind->constr[1] = 0.0;
    ind->constr[2] = 0.0;
    double total_distance = 0.0;
    double total_emissions = 0.0;

    int current_vehicle = 1;
    int current_capacity = 0;
    double current_risk = 0.0;
    
    double dist;
    double emission;

    int depot_counter = 1;
    int current_depot = set_O[depot_counter - 1]; 

    int prev_node = current_depot;
    int current_node;

    int i;

    /* Constraint 1: Capacity
    Constraint 2: risk
    Constraint 3: Vehicles */

    /* printf("Route: ");*/
    for (i = 0; i < ind->route_length; i++) {
        current_node = ind->route[i];
        /* printf("%d ", current_node); */

        if (current_node < 0) {
            /* printf("/   "); */

            current_risk += d[prev_node][current_depot] * current_capacity;
            total_distance += d[prev_node][current_depot];
            total_emissions += d[prev_node][current_depot] * ((peso_vacio + current_capacity) + compute_emission(prev_node, current_depot));
            
            /* printf("ruta finalizada"); */

            if (current_capacity > b) {
                /* printf("Capacity violation: Current capacity %d exceeds vehicle capacity %d\n", current_capacity, b); */
                ind->constr[0] += current_capacity - b;
            }
            if (current_risk > theta) {
                /* printf("Risk violation: Current risk %lf exceeds risk threshold %lf\n", current_risk, theta); */
                ind->constr[1] += current_risk - theta;
            }

            current_capacity = 0;
            current_risk = 0.0;
            current_vehicle++;
            if (current_vehicle > n_vehicles) {
                if (set_O[depot_counter + 1] != 0) {
                    current_depot = set_O[depot_counter++]; 
                }
                current_vehicle = 1;
            }
            prev_node = current_depot;
        } else {
            if (depot_counter > n_depots) {
                ind->constr[2] += 1;
            }
            dist = d[prev_node][current_node];
            demanda = dm[current_node];
            emission = dist * ((peso_vacio + current_capacity) + compute_emission(prev_node, current_node));

            current_risk += dist * current_capacity;
            current_capacity += demanda;
            total_distance += dist;
            total_emissions += emission;


            prev_node = current_node;
            /* printf("%d", prev_node); */
        }
    }
    /* printf("\n"); */

    ind->constr_violation = ind->constr[0] + ind->constr[1] + ind->constr[2];
    
    ind->obj[0] = total_distance;
    ind->obj[1] = total_emissions;

}

