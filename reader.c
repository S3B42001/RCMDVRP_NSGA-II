/* Routine for evaluating population members  */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include "global.h"
# include "rand.h"


void findDef(FILE *file, char *def) {
    char word[1024];
    /* assumes no word exceeds length of 1023 */
    while (fscanf(file, " %1023s", word)) {
        if(strcmp(word,def) == 0) break;
    }
}

void removeSemicolon(char *line){
    strtok(line, ";");
}

int countWords(char *line){
    int words;
    char linet[1024], *token;
    strcpy(linet, line);

    words = 0;
    token = strtok(linet, " ");

    while( token != NULL ) {
        words ++;
        token = strtok(NULL, " ");
    }
   return words;
}

/*
void readCTrucks(FILE *f, problem_instance *pi) {
    int debug=0, capacity, id;

    char line[1024];
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    if (debug) printf("Line: %s\n", line);
    removeSemicolon(line);
    capacity = atoi(line);
    if (debug) printf("Capacity: %d\n", capacity);
    for ( id=0; id<pi->nTrucks; id++ )
        pi->trucks[id].capacity = capacity;
}

void readDepots(FILE *f, problem_instance *pi) {
    int debug=0, id=0;
    char *token;

    char line[1024];
    fgets(line, sizeof(line), f);

    if (debug) printf("Line: %s\n", line);
    removeSemicolon(line);
    if (debug) printf("Line: %s\n", line);
    pi->nDepots=countWords(line);

    pi->depots=malloc(pi->nDepots*sizeof(depot));

    if (debug) printf("nDepots: %d\n", pi->nDepots);

    token=strtok(line, " ");
    while( token != NULL ) {
        if (debug) printf("%s\n", token);
        pi->depots[id].id=id;
        pi->depots[id].names[0] = atoi(token); 
        token=strtok(NULL, " ");
        id++;
    }
    if (debug) {
        for ( id=0; id<pi->nDepots; id++ )
            printf ("%d, %ld\n", pi->depots[id].id, pi->depots[id].names[0]);
    }
    if (debug) getchar();
}

void readAlternativeNames(FILE *f, problem_instance *pi, int index) {
    int debug=0, id=0;
    char *token;

    char line[1024];
    fgets(line, sizeof(line), f);

    if (debug) printf("Line: %s\n", line);
    removeSemicolon(line);

    token=strtok(line, " ");
    while( token != NULL ) {
        if (debug) printf("%s\n", token);
        if (debug) getchar();
        pi->depots[id].names[index]=atoi(token);
        token=strtok(NULL, " ");
        id++;
    }
    if (debug) {
        for ( id=0; id<pi->nDepots; id++ )
            printf ("%d, %ld\n", pi->depots[id].id, pi->depots[id].names[index]);
    }
    if (debug) getchar();
}

void readClients(FILE *f, problem_instance *pi) {
    int debug=0, id=0;
    char * token;

    char line[1024];
    fgets(line, sizeof(line), f);

    if (debug) printf("Line: %s\n", line);
    removeSemicolon(line);
    if (debug) printf("Line: %s\n", line);
    pi->nClients=countWords(line);

    pi->clients=malloc(pi->nClients*sizeof(client));

    if (debug) printf("nClients: %d\n", pi->nClients);

    token=strtok(line, " ");
    while( token != NULL ) {
        pi->clients[id].id=id;
        pi->clients[id].name=atoi(token);
        if (debug) printf("%s\n", token);
        token = strtok(NULL, " ");

        id++;
    }
    if (debug) {
        for ( id=0; id<pi->nClients; id++ )
            printf ("%d, %d\n", pi->clients[id].id, pi->clients[id].name);
    }
    if (debug) getchar();
}


void readTrucks(FILE *f, problem_instance *pi) {
    int debug=0, id=0, i;
    char * token;

    char line[1024];
    fgets(line, sizeof(line), f);

    if (debug) printf("Line: %s\n", line);
    removeSemicolon(line);
    if (debug) printf("Line: %s\n", line);
    pi->nTrucks=countWords(line);

    pi->trucks=malloc(pi->nTrucks*sizeof(truck));

    if (debug) printf("nTrucks: %d\n", pi->nTrucks);

    token=strtok(line, " ");
    while( token != NULL ) {
        pi->trucks[id].id = id;
        pi->trucks[id].ndStart = 0;
        pi->trucks[id].ndIn = 0;
        pi->trucks[id].ndEnd = 0;
        pi->trucks[id].dStart = malloc(pi->nDepots*sizeof(int));
        pi->trucks[id].dIn = malloc(pi->nDepots*sizeof(int));
        pi->trucks[id].dEnd = malloc(pi->nDepots*sizeof(int));
        if (debug) printf("############# %d ###################\n", id);
        for(i=0;i<pi->nDepots; i++){
            if (debug) printf("%d ", pi->trucks[id].dStart[i]);
            pi->trucks[id].dStart[i] = -1;
        }

        if (debug) printf("###################################\n");
        /*if (debug) printf("%s\n", token);*/ /*
        token = strtok(NULL, " ");
        id++;
    }
    if (debug) getchar();
}

void readDistances(FILE *f, problem_instance *pi) {
    int debug=0, i;
    int totalNodes = pi->nDepots + pi->nClients;
    int idFrom, idTo;
    char *from, *to, *distance;
    char line[1024];

    pi->distances = (int **)malloc(totalNodes * sizeof(int*));
    for(i=0; i<totalNodes; i++)
        pi->distances[i] = (int *)malloc(totalNodes * sizeof(int));

    fgets(line, sizeof(line), f);

    while (1){
        fgets(line, sizeof(line), f);
        if (debug) printf("Line: %s\n", line);

        from=strtok(line, " ");
        if (strchr(from, ';') != NULL) break;

        to  = strtok(NULL, " ");
        distance = strtok(NULL, " ");
        idFrom = getNodeId(atoi(from), *pi);
        idTo = getNodeId(atoi(to), *pi);

        if (debug) printf("from: %s, to:%s , distance: %s\n", from, to, distance);
        pi->distances[idFrom][idTo] = atoi(distance);

    }
}

void readDemands(FILE *f, problem_instance *pi) {
    int debug=0;
    int idNode;
    char *id, *demand;
    char line[1024];

   fgets(line, sizeof(line), f);

    while (1){
        fgets(line, sizeof(line), f);
        if (debug) printf("Line: %s\n", line);

        id=strtok(line, " ");
        if (strchr(id, ';') != NULL) break;

        if (!isADepot(atoi(id), *pi)){
            demand = strtok(NULL, " ");
            idNode = getNodeIndex(atoi(id), *pi);
            if (debug) printf("id: %s, idNode: %d demand: %s\n", id, idNode, demand);
            pi->clients[idNode].demand = atoi(demand);
        }
    }
}

void readTrucksDepots(FILE *f, problem_instance *pi, char * type) {
    int debug=0;
    int idDepot;
    char *depot, *truck, *flag;
    char line[1024];

    fgets(line, sizeof(line), f);

    while (1){
        fgets(line, sizeof(line), f);
        if (debug) printf("Line: %s\n", line);

        depot=strtok(line, " ");
        if (strchr(depot, ';') != NULL) break;

        truck = strtok(NULL, " ");
        int idTruck = atoi(truck)-1;
        flag = strtok(NULL, " ");
        if(atoi(flag) == 1){
            idDepot = getNodeId(atoi(depot), *pi);

            if (debug) printf("depot: %d, truck: %d, type: %s\n", idDepot, idTruck, type);

            if(strcmp(type, "start") == 0){
                if (debug) printf("start menu\n");
                if (debug) printf("agregando depot start: %d a camión %d\n", idDepot, idTruck);
                pi->trucks[idTruck].dStart[pi->trucks[idTruck].ndStart] = idDepot;
                pi->trucks[idTruck].ndStart++;
            }
            if(strcmp(type, "in") == 0){
                if (debug) printf("in menu\n");
                if (debug) printf("agregando depot in: %d a camión %d\n", idDepot, idTruck);
                pi->trucks[idTruck].dIn[pi->trucks[idTruck].ndIn] = idDepot;
                pi->trucks[idTruck].ndIn++;
            }
            if(strcmp(type, "end") == 0){
                if (debug) printf("end menu\n");
                if (debug) printf("agregando depot end: %d a camión %d\n", idDepot, idTruck);
                pi->trucks[idTruck].dEnd[pi->trucks[idTruck].ndEnd] = idDepot;
                pi->trucks[idTruck].ndEnd++;
            }
        }
    }
}


int readInputFile(char* filePath, problem_instance *pi) {
    int debug=0;
    FILE* fh=fopen(filePath, "r");

    if ( fh==NULL ){
        printf("File does not exists %s", filePath);
        return 0;
    }

    if(debug) printf("Reading: %s \n", filePath);
    
    findDef(fh, "D_start:=");
    readDepots(fh, pi);
    if(debug) printf("End readDepots! \n");

    findDef(fh, "C:=");
    readClients(fh, pi);
    if(debug) printf("End readClients! \n");

    findDef(fh, "D_in:=");
    readAlternativeNames(fh, pi, 1);

    findDef(fh, "D_end:=");
    readAlternativeNames(fh, pi, 2);
    if(debug) printf("End readNames! \n");
    
    findDef(fh, "K:=");
    readTrucks(fh, pi);
    if(debug) printf("End readTrucks! \n ");

    findDef(fh, "time_:=");
    readDistances(fh, pi);
    if(debug) printf("End readDistances!\n ");

    findDef(fh, "dem:=");
    readDemands(fh, pi);
    if(debug) printf("End readDemands! \n");

    findDef(fh, "cap:=");
    readCTrucks(fh, pi);
    if(debug) printf("End readCTrucks! \n");

    findDef(fh, "ava_start:=");
    readTrucksDepots(fh, pi, "start");
    
    findDef(fh, "ava_end:=");
    readTrucksDepots(fh, pi, "end");

    findDef(fh, "com:=");
    readTrucksDepots(fh, pi, "in");
    if(debug) printf("End readTrucksDepots! \n");

    fclose(fh);
    if(debug) printf("End Reading! \n");
    
    printProblemInstance(pi);
    if(debug) getchar();

    return 0;
}
*/


void read_dat_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error al abrir archivo: %s\n", filename);
        exit(1);
    }
/*     printf("Reading file: %s\n", filename); */

    char line[512];
    int i, j, idx;
    double val; 
    char *p, *end, *token;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "param sigma", 11) == 0) {
            for (i = 0; i < 11; i++) {
                fgets(line, sizeof(line), file);
                if (strchr(line, ';')) break;
                sscanf(line, "%d %lf %lf", &idx, &sigma[i][0], &sigma[i][1]);
         /*        printf("sigma[0][0]: %lf, sigma[0][1]: %lf\n", sigma[0][0], sigma[0][1]); */
            }
        }

        else if (strncmp(line, "set O", 5) == 0) {
            n_depots = 0;
            p = strstr(line, ":=");
            if (!p) continue;
            p += 2;
            end = strchr(p, ';');
            if (end) *end = '\0';
            token = strtok(p, " \t\n\r");
            while (token) {
                set_O[n_depots++] = atoi(token);
                token = strtok(NULL, " \t\n\r");
            }
        }

        else if (strncmp(line, "set R", 5) == 0) {
            n_customers = 0;
            p = strstr(line, ":=");
            if (!p) continue;
            p += 2;
            end = strchr(p, ';');
            if (end) *end = '\0';
            token = strtok(p, " \t\n\r");
            while (token) {
                set_R[n_customers++] = atoi(token);
                token = strtok(NULL, " \t\n\r");
            }
        }

        else if (strncmp(line, "set S", 5) == 0) {
            int n = 0;
            p = strstr(line, ":=");
            if (!p) continue;
            p += 2;
            end = strchr(p, ';');
            if (end) *end = '\0';
            token = strtok(p, " \t\n\r");
            while (token) {
                set_S[n++] = atoi(token);
                token = strtok(NULL, " \t\n\r");
            }
        }

        else if (strncmp(line, "set K", 5) == 0) {
            n_vehicles = 0;
            p = strstr(line, ":=");
            if (!p) continue;
            p += 2;
            end = strchr(p, ';');
            if (end) *end = '\0';
            token = strtok(p, " \t\n\r");
            while (token) {
                set_K[n_vehicles++] = atoi(token);
                token = strtok(NULL, " \t\n\r");
            }
        }

        else if (strncmp(line, "param b", 7) == 0) {
            sscanf(line, "param b := %d", &b);
         /*    printf("b: %d\n", b); */
        }

        else if (strncmp(line, "param theta", 11) == 0) {
            sscanf(line, "param theta := %lf", &theta);
         /*    printf("theta: %lf\n", theta); */
        }

        else if (strncmp(line, "param peso_vacio", 16) == 0) {
            sscanf(line, "param peso_vacio := %lf", &peso_vacio);
           /*  printf("peso_vacio: %lf\n", peso_vacio); */
        }

        else if (strncmp(line, "param alpha", 11) == 0) {
            for (i = 0; i < 5; i++) {
                fgets(line, sizeof(line), file);
                if (strchr(line, ';')) break;
                sscanf(line, "%d %lf", &idx, &alpha[i]);
               /*  printf("alpha[%d]: %lf\n", idx, alpha[i]); */
            }
        }

        else if (strncmp(line, "param beta", 10) == 0) {
            for (i = 0; i < 5; i++) {
                fgets(line, sizeof(line), file);
                if (strchr(line, ';')) break;
                sscanf(line, "%d %lf", &idx, &beta[i]);
                /* printf("beta[%d]: %lf\n", idx, beta[i]); */
            }
        }

        else if (strncmp(line, "param gamma", 11) == 0) {
            for (i = 0; i < 5; i++) {
                fgets(line, sizeof(line), file);
                if (strchr(line, ';')) break;
                sscanf(line, "%d %lf", &idx, &gamma_param[i]);
               /*  printf("gamma_param[%d]: %lf\n", idx, gamma_param[i]); */
            }
        }

        else if (strncmp(line, "param delta", 11) == 0) {
            for (i = 0; i < 5; i++) {
                fgets(line, sizeof(line), file);
                if (strchr(line, ';')) break;
                sscanf(line, "%d %lf", &idx, &delta_param[i]);
           /*      printf("delta_param[%d]: %lf\n", idx, delta_param[i]); */
            }
        }

        else if (strncmp(line, "param epsilon", 13) == 0) {
            for (i = 0; i < 5; i++) {
                fgets(line, sizeof(line), file);
                if (strchr(line, ';')) break;
                sscanf(line, "%d %lf", &idx, &epsilon[i]);
             /*    printf("epsilon[%d]: %lf\n", idx, epsilon[i]); */
            }
        }

        else if (strncmp(line, "param zeta", 10) == 0) {
            for (i = 0; i < 5; i++) {
                fgets(line, sizeof(line), file);
                if (strchr(line, ';')) break;
                sscanf(line, "%d %lf", &idx, &zeta[i]);
              /*   printf("zeta[%d]: %lf\n", idx, zeta[i]); */
            }
        }

        else if (strncmp(line, "param hta", 9) == 0) {
            for (i = 0; i < 5; i++) {
                fgets(line, sizeof(line), file);
                if (strchr(line, ';')) break;
                sscanf(line, "%d %lf", &idx, &hta[i]);
             /*    printf("hta[%d]: %lf\n", idx, hta[i]); */
            }
        }

        else if (strncmp(line, "param dm", 8) == 0) {
            while (fgets(line, sizeof(line), file)) {
                if (strchr(line, ';')) break;
                if (sscanf(line, "%d %d", &i, &j) == 2){
                    dm[i] = j;
/*                     printf("dm[%d]: %d\n", i, dm[i]); */
                }
            }
        }

        else if (strncmp(line, "param d", 7) == 0) {
            while (fgets(line, sizeof(line), file)) {
                if (strchr(line, ';')) break;
                if (sscanf(line, "%d %d %lf", &i, &j, &val) == 3){
                    d[i][j] = val;
/*                     printf("d[%d][%d]: %lf\n", i, j, d[i][j]); */
                }
            }
        }

        else if (strncmp(line, "param v", 7) == 0) {
            while (fgets(line, sizeof(line), file)) {
                if (strchr(line, ';')) break;
                if (sscanf(line, "%d %d %lf", &i, &j, &val) == 3){
                    v[i][j] = (int)val;
                /*     printf("v[%d][%d]: %d\n", i, j, v[i][j]); */
                }
            }
        }

        else if (strncmp(line, "param f", 7) == 0) {
            while (fgets(line, sizeof(line), file)) {
                if (strchr(line, ';')) break;
                if (sscanf(line, "%d %d", &i, &j) == 2){
                    f[i] = j;
                /*     printf("f[%d]: %d\n", i, f[i]); */
                }
            }
        }
    }

    fclose(file);

    n_nodes = n_customers + n_depots;

    printf("Instancia cargada:\n");
    printf(" - Depósitos: %d\n", n_depots);
    printf(" - Clientes: %d\n", n_customers);
    printf(" - Vehículos: %d\n", n_vehicles);
}


int readInputFile(char* filePath) {
    int debug=0;
    FILE* fh=fopen(filePath, "r");

    /*check if file exists*/
    if ( fh==NULL ){
        printf("File does not exists %s", filePath);
        return 0;
    }

    if(debug) printf("Reading: %s \n", filePath);
    read_dat_file(filePath);
    return 0;
}

