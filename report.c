/* Routines for storing population data into files */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Function to print the information of a population in a file */
void report_pop (population *pop, FILE *fpt)
{
    int i, j;
/*     int i, j, k; */
    for (i=0; i<popsize; i++)
    {
        for (j=0; j<nobj; j++)
        {
            fprintf(fpt,"%e\t",pop->ind[i].obj[j]);
        }
        if (ncon!=0)
        {
            for (j=0; j<ncon; j++)
            {
                fprintf(fpt,"%e\t",pop->ind[i].constr[j]);
            }
        }
/*         if (nreal!=0)
        {
            for (j=0; j<nreal; j++)
            {
                fprintf(fpt,"%e\t",pop->ind[i].xreal[j]);
            }
        }
        if (nbin!=0)
        {
            for (j=0; j<nbin; j++)
            {
                for (k=0; k<nbits[j]; k++)
                {
                    fprintf(fpt,"%d\t",pop->ind[i].gene[j][k]);
                }
            }
        } */
        fprintf(fpt,"%e\t",pop->ind[i].constr_violation);
        fprintf(fpt,"%d\t",pop->ind[i].rank);
        fprintf(fpt,"%e\n",pop->ind[i].crowd_dist);

        /* // Imprime la ruta en una sola línea debajo de la información principal */
        if (pop->ind[i].route_length > 0 && pop->ind[i].route != NULL) {
            for (j = 0; j < pop->ind[i].route_length; j++)
            {
                fprintf(fpt,"%d ", pop->ind[i].route[j]);
            }
            fprintf(fpt,"\n");
        }
    }
    return;
}

/* Function to print the information of feasible and non-dominated population in a file */
void report_feasible (population *pop, FILE *fpt)
{
    int i, j;
/*     int i, j, k; */
    for (i=0; i<popsize; i++)
    {
        if (pop->ind[i].constr_violation == 0.0 && pop->ind[i].rank==1)
        {
            for (j=0; j<nobj; j++)
            {
                fprintf(fpt,"%e\t",pop->ind[i].obj[j]);
            }
            if (ncon!=0)
            {
                for (j=0; j<ncon; j++)
                {
                    fprintf(fpt,"%e\t",pop->ind[i].constr[j]);
                }
            }
/*             if (nreal!=0)
            {
                for (j=0; j<nreal; j++)
                {
                    fprintf(fpt,"%e\t",pop->ind[i].xreal[j]);
                }
            }
            if (nbin!=0)
            {
                for (j=0; j<nbin; j++)
                {
                    for (k=0; k<nbits[j]; k++)
                    {
                        fprintf(fpt,"%d\t",pop->ind[i].gene[j][k]);
                    }
                }
            } */
            fprintf(fpt,"%e\t",pop->ind[i].constr_violation);
            fprintf(fpt,"%d\t",pop->ind[i].rank);
            fprintf(fpt,"%e\n",pop->ind[i].crowd_dist);

            /* // Imprime la ruta en una sola línea debajo de la información principal */
            if (pop->ind[i].route_length > 0 && pop->ind[i].route != NULL) {
                for (j = 0; j < pop->ind[i].route_length; j++)
                {
                    fprintf(fpt,"%d ", pop->ind[i].route[j]);
                }
                fprintf(fpt,"\n");
            }
        }
    }
    return;
}

void report_quality(population *pop, const char *filename)
{
    FILE *fpt = fopen(filename, "w");
    int i, j;
    if (!fpt) return;
    for (i = 0; i < popsize; i++)
    {
        /* if (pop->ind[i].constr_violation == 0.0 && pop->ind[i].rank == 1) */
        if (pop->ind[i].constr_violation == 0.0)
        {
            for (j = 0; j < nobj; j++)
            {
                fprintf(fpt, "%.6f", pop->ind[i].obj[j]);
                if (j < nobj - 1) fprintf(fpt, " ");
            }
            fprintf(fpt, "\n");
        }
    }
    fclose(fpt);
}
