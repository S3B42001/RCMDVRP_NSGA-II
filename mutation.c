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
/*     if (nreal!=0)
    {
        real_mutate_ind(ind);
        }
        if (nbin!=0)
        {
            bin_mutate_ind(ind);
            } */
    real_mutate_ind(ind);
    return;
}

/* Routine for binary mutation of an individual */
void bin_mutate_ind (individual *ind)
{
    int j, k;
    double prob;
    for (j=0; j<nbin; j++)
    {
        for (k=0; k<nbits[j]; k++)
        {
            prob = randomperc();
            if (prob <=pmut_bin)
            {
                if (ind->gene[j][k] == 0)
                {
                    ind->gene[j][k] = 1;
                }
                else
                {
                    ind->gene[j][k] = 0;
                }
                nbinmut+=1;
            }
        }
    }
    return;
}

/* Routine for swap mutation of an individual's route (for RCMDVRP) */
void real_mutate_ind (individual *ind)
{
/*     printf("\n Performing swap mutation on individual with route length %d\n", ind->route_length); */
    int i, j;
    if (ind->route_length < 2) return;

    for (j = 0; j < nreal; j++)
    {
        if (randomperc() <= pmut_real)
        {
            int pos1, pos2, temp;

/*             printf("Child route before mutation: ");
            for (i = 0; i < ind->route_length; i++)
            {
                printf("%d ", ind->route[i]);
            }
            printf("\n");
     */
            do {
                pos1 = rnd(0, ind->route_length - 1);
                pos2 = rnd(0, ind->route_length - 1);
            } while (pos1 == pos2 || ind->route[pos1] < 0 || ind->route[pos2] < 0);
    
            temp = ind->route[pos1];
            ind->route[pos1] = ind->route[pos2];
            ind->route[pos2] = temp;
    
/*             printf("Child route after mutation: ");
            for (j = 0; j < ind->route_length; j++)
            {
                printf("%d ", ind->route[j]);
            } */
            nrealmut += 1;
        }
    }
/*     printf("\n");
    printf("\n Swap mutation completed, total mutations: %d\n", nrealmut); */
    return;
}

/* Routine for real polynomial mutation of an individual */
/* void real_mutate_ind (individual *ind)
{
    int j;
    double rnd, delta1, delta2, mut_pow, deltaq;
    double y, yl, yu, val, xy;
    for (j=0; j<nreal; j++)
    {
        if (randomperc() <= pmut_real)
        {
            y = ind->xreal[j];
            yl = min_realvar[j];
            yu = max_realvar[j];
            delta1 = (y-yl)/(yu-yl);
            delta2 = (yu-y)/(yu-yl);
            rnd = randomperc();
            mut_pow = 1.0/(eta_m+1.0);
            if (rnd <= 0.5)
            {
                xy = 1.0-delta1;
                val = 2.0*rnd+(1.0-2.0*rnd)*(pow(xy,(eta_m+1.0)));
                deltaq =  pow(val,mut_pow) - 1.0;
            }
            else
            {
                xy = 1.0-delta2;
                val = 2.0*(1.0-rnd)+2.0*(rnd-0.5)*(pow(xy,(eta_m+1.0)));
                deltaq = 1.0 - (pow(val,mut_pow));
            }
            y = y + deltaq*(yu-yl);
            if (y<yl)
                y = yl;
            if (y>yu)
                y = yu;
            ind->xreal[j] = y;
            nrealmut+=1;
        }
    }
    return;
} */