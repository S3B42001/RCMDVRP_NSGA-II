/* Crossover routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"


int valueinarray(int val, int *arr, int n) {
    int i;
    if (n <= 0) {
        /* printf("Array is empty or invalid length: %d\n", n); */
        return 0;
    }
    if (arr == NULL) {
        /* printf("Array pointer is NULL.\n"); */
        return 0;
    }
    for(i = 0; i < n; i++) {
        if(arr[i] == val)
        {
            return 1;
        }
    }
    return 0;
}

/* Function to cross two individuals */
void crossover (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    realcross (parent1, parent2, child1, child2);
    return;
}

/* Routine for real variable SBX crossover */
void realcross (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    /* printf("\n--- Performing Sinusoidal Motion crossover ---\n"); */
    int i;
    individual *parent1_n;
    individual *parent2_n;
    individual *child1_n;
    individual *child2_n;

    int indice1 = 0;
    int indice2 = 0;
    int counter;
    int internal_counter1 = 0;
    int internal_counter2 = 0;

    if (randomperc() <= pcross_real)
    {
        nrealcross++;
        parent1_n = (individual *)malloc(sizeof(individual));
        parent2_n = (individual *)malloc(sizeof(individual));
        child1_n = (individual *)malloc(sizeof(individual));
        child2_n = (individual *)malloc(sizeof(individual));
        parent1_n->route = (int *)malloc(MAX_NODES * sizeof(int));
        parent2_n->route = (int *)malloc(MAX_NODES * sizeof(int));
        child1_n->route = (int *)malloc(MAX_NODES * sizeof(int));
        child2_n->route = (int *)malloc(MAX_NODES * sizeof(int));
        child1_n->route_length = 0;
        child2_n->route_length = 0;

        /* printf("Parent1 route: ");
        for (i = 0; i < parent1->route_length; i++) {
            printf("%d ", parent1->route[i]);
        }
        printf("\nParent2 route: ");
        for (i = 0; i < parent2->route_length; i++) {
            printf("%d ", parent2->route[i]);
        }
        printf("\n"); */
        for (i = 0; i < n_customers; i++) {
            if (parent1->route[internal_counter1] > 0) {
                parent1_n->route[i] = parent1->route[internal_counter1];
            } else {
                i--;
            }
            internal_counter1++;
        }
        for (i = 0; i < n_customers; i++) {
            if (parent2->route[internal_counter2] > 0) {
                parent2_n->route[i] = parent2->route[internal_counter2];
            } else {
                i--;
            }
            internal_counter2++;
        }

        for (i=0; i<nreal; i++)
        {
            if (randomperc()<=0.5 )
            {
                for (counter = 0; counter < n_customers; counter++)
                {
                    if (child1_n->route_length >= MAX_NODES || child2_n->route_length >= MAX_NODES) {
                        printf("Error: Child route length exceeds maximum nodes.\n");
                        exit(1);
                    }
                    if (valueinarray(parent1_n->route[counter],child1_n->route, child1_n->route_length) == 1){
                        child2_n->route[indice2] = parent1_n->route[counter];
                        child2_n->route_length++;
                        /* printf("child2_n->route[%d] = %d\n", indice2, parent1_n->route[counter]); */
                        indice2++;
                    } else {
                        child1_n->route[indice1] = parent1_n->route[counter];
                        child1_n->route_length++;
                        /* printf("child1_n->route[%d] = %d\n", indice1, parent1_n->route[counter]); */
                        indice1++;
                    }

                    if (valueinarray(parent2_n->route[counter],child1_n->route, child1_n->route_length) == 1){
                        child2_n->route[indice2] = parent2_n->route[counter];
                        child2_n->route_length++;
                        /* printf("child2_n->route[%d] = %d\n", indice2, parent2_n->route[counter]); */
                        indice2++;
                    } else {
                        child1_n->route[indice1] = parent2_n->route[counter];
                        child1_n->route_length++;
                        /* printf("child1_n->route[%d] = %d\n", indice1, parent2_n->route[counter]); */
                        indice1++;
                    }
                }

                for (i = 0; i < child1_n->route_length; i++)
                {
                    child1->route[i] = child1_n->route[i];
                }
                child1->route_length = child1_n->route_length;

                for (i = 0; i < child2_n->route_length; i++)
                {
                    child2->route[i] = child2_n->route[i];
                }
                child2->route_length = child2_n->route_length;
            }
            else
            {
                /* printf("No crossover performed, copying parent1 to child1 and parent2 to child2\n"); */
                child1->route_length = parent1->route_length;
                child2->route_length = parent2->route_length;
                for (i = 0; i < parent1->route_length; i++)
                    child1->route[i] = parent1->route[i];
                for (i = 0; i < parent2->route_length; i++)
                    child2->route[i] = parent2->route[i];
            }
        }
        free(parent1_n->route);
        free(parent2_n->route);
        free(child1_n->route);
        free(child2_n->route);
        free(parent1_n);
        free(parent2_n);
        free(child1_n);
        free(child2_n);

    }
    else
    {
        /* printf("No crossover performed, copying parent1 to child1 and parent2 to child2\n"); */
        child1->route_length = parent1->route_length;
        child2->route_length = parent2->route_length;
        for (i = 0; i < parent1->route_length; i++)
            child1->route[i] = parent1->route[i];
        for (i = 0; i < parent2->route_length; i++)
        child2->route[i] = parent2->route[i];
    }
    /* printf("Final child1 route: ");
    for (i=0;i < child1->route_length; i++)
    {
        printf("%d ", child1->route[i]);
    }
    printf("\n");

    printf("Final child2 route: ");
    for (i=0;i < child2->route_length; i++)
    {
        printf("%d ", child2->route[i]);
    }
    printf("\n--- Sinusoidal Motion crossover completed ---\n"); */
    return;
}
