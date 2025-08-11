/* This file contains the variable and function declarations */

# ifndef _GLOBAL_H_
# define _GLOBAL_H_

# define INF 1.0e14
# define EPS 1.0e-14
# define E  2.71828182845905
# define PI 3.14159265358979
# define GNUPLOT_COMMAND "gnuplot -persist"

#define MAX_NODES 100
#define MAX_VEHICLES 10

typedef struct
{
    int rank;                       
    double constr_violation;        
    double *xreal;                  
    int **gene;                     
    double *xbin;                   
    double *obj;                    
    double *constr;                 
    double crowd_dist;              
    int *route;                     
    int route_length;               
}
individual;

typedef struct
{
    individual *ind;
}
population;

typedef struct lists
{
    int index;
    struct lists *parent;
    struct lists *child;
}
list;

typedef struct
{
    int id;
    int x, y;
    int nVehicles;
    long int names[3];
}depot;

typedef struct
{
    int id, capacity;
    int currentLoad;
    int currentEmmission;
    int currentDistance;
    int *dStart;
    int *dEnd;
    int *dIn;
    int ndStart, ndEnd, ndIn;
}truck;

typedef struct
{
    int id, name, demand, x, y;
}client;

typedef struct
{
    int **distances;

    depot *depots;
    client *clients;
    truck *trucks;
    int nDepots, nClients, nTrucks;
}problem_instance;

extern int n_nodes;      
extern int n_depots;     
extern int n_customers;  
extern int n_vehicles;   

extern int set_O[MAX_NODES]; 
extern int set_R[MAX_NODES];
extern int set_S[MAX_NODES];
extern int set_K[MAX_VEHICLES];

extern int demanda;

extern double sigma[11][2];
extern int b;
extern double theta;
extern double peso_vacio;
extern double alpha[5], beta[5], gamma_param[5], delta_param[5], epsilon[5], zeta[5], hta[5];

extern int dm[MAX_NODES];
extern double d[MAX_NODES][MAX_NODES];
extern int v[MAX_NODES][MAX_NODES]; 

/* extern int Rinit[MAX_NODES][MAX_VEHICLES]; */
extern int f[MAX_NODES];

extern int cliente;
extern int cliente_anterior;
extern int separador;

extern int nreal;
extern int nbin;
extern int nobj;
extern int ncon;
extern int popsize;
extern double pcross_real;
extern double pcross_bin;
extern double pmut_real;
extern double pmut_bin;
extern double eta_c;
extern double eta_m;
extern int ngen;
extern int nbinmut;
extern int nrealmut;
extern int nbincross;
extern int nrealcross;
extern int *nbits;
extern double *min_realvar;
extern double *max_realvar;
extern double *min_binvar;
extern double *max_binvar;
extern int bitlength;
extern int choice;
extern int obj1;
extern int obj2;
extern int obj3;
extern int angle1;
extern int angle2;

/*
char * instance_route;
char * line[512];
*/

void allocate_memory_pop (population *pop, int size);
void allocate_memory_ind (individual *ind);
void deallocate_memory_pop (population *pop, int size);
void deallocate_memory_ind (individual *ind);

double maximum (double a, double b);
double minimum (double a, double b);

void crossover (individual *parent1, individual *parent2, individual *child1, individual *child2);
void realcross (individual *parent1, individual *parent2, individual *child1, individual *child2);
void bincross (individual *parent1, individual *parent2, individual *child1, individual *child2);

void assign_crowding_distance_list (population *pop, list *lst, int front_size);
void assign_crowding_distance_indices (population *pop, int c1, int c2);
void assign_crowding_distance (population *pop, int *dist, int **obj_array, int front_size);

void decode_pop (population *pop);
void decode_ind (individual *ind);

void onthefly_display (population *pop, FILE *gp, int ii);

int check_dominance (individual *a, individual *b);

void evaluate_pop (population *pop);
void evaluate_ind (individual *ind);

void fill_nondominated_sort (population *mixed_pop, population *new_pop);
void crowding_fill (population *mixed_pop, population *new_pop, int count, int front_size, list *cur);

void initialize_pop (population *pop);
void initialize_ind (individual *ind);

void insert (list *node, int x);
list* del (list *node);

void merge(population *pop1, population *pop2, population *pop3);
void copy_ind (individual *ind1, individual *ind2);

void mutation_pop (population *pop);
void mutation_ind (individual *ind);
void bin_mutate_ind (individual *ind);
void real_mutate_ind (individual *ind);

void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr);

void assign_rank_and_crowding_distance (population *new_pop);

void report_pop (population *pop, FILE *fpt);
void report_feasible (population *pop, FILE *fpt);
void report_ind (individual *ind, FILE *fpt);

void quicksort_front_obj(population *pop, int objcount, int obj_array[], int obj_array_size);
void q_sort_front_obj(population *pop, int objcount, int obj_array[], int left, int right);
void quicksort_dist(population *pop, int *dist, int front_size);
void q_sort_dist(population *pop, int *dist, int left, int right);

void selection (population *old_pop, population *new_pop);
individual* tournament (individual *ind1, individual *ind2);

/* int readInputFile(char *file_path); */

# endif
