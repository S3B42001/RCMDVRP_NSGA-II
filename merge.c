/* Routine for mergeing two populations */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Routine to merge two populations into one */
void merge(population *pop1, population *pop2, population *pop3)
{
    int i, j, k;
    for (i=0; i<popsize; i++)
    {
        /* printf("Consolidating individual %d from first population into merged population.\n", i);
        printf("route length: %d\n", pop1->ind[i].route_length);
        printf("Source route: ");
        for (j = 0; j < pop1->ind[i].route_length; j++) {
            printf("%d ", pop1->ind[i].route[j]);
        } */
        copy_ind (&(pop1->ind[i]), &(pop3->ind[i]));
    }
    for (i=0, k=popsize; i<popsize; i++, k++)
    {
        copy_ind (&(pop2->ind[i]), &(pop3->ind[k]));
    }
    return;
}

/* Routine to copy an individual 'ind1' into another individual 'ind2' */
void copy_ind (individual *ind1, individual *ind2)
{
    int i, j;
    ind2->rank = ind1->rank;
    ind2->constr_violation = ind1->constr_violation;
    ind2->crowd_dist = ind1->crowd_dist;
    if (nreal!=0)
    {
        for (i=0; i<nreal; i++)
        {
            ind2->xreal[i] = ind1->xreal[i];
        }
    }
    for (i=0; i<nobj; i++)
    {
        ind2->obj[i] = ind1->obj[i];
    }
    if (ncon!=0)
    {
        for (i=0; i<ncon; i++)
        {
            ind2->constr[i] = ind1->constr[i];
        }
    }
    ind2->route_length = ind1->route_length;
    if (ind1->route_length > 0 && ind1->route != NULL && ind2->route != NULL) {
        /*
        printf("Copying route of length: %d\n", ind1->route_length);
        printf("Source route: ");
        */
        for (i = 0; i < ind1->route_length; i++) {
            ind2->route[i] = ind1->route[i];
            /* printf("%d ", ind2->route[i]); */
        }
        /* printf("\n"); */
    }
    return;
}
