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

void read_dat_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error al abrir archivo: %s\n", filename);
        exit(1);
    }
/*     printf("Reading file: %s\n", filename); */

    char line[2048];
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

