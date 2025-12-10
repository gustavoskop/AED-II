#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    double x;
    double y;
} coordenadas;

coordenadas* ler_arquivo_e_coordenadas();

int main(){

    coordenadas* coordenada = ler_arquivo_e_coordenadas();
    for (int i = 0; i < 38; i++){
    printf("X: %lf", coordenada[i].x);
    printf("     y: %lf\n", coordenada[i].y);
    }
}

coordenadas* ler_arquivo_e_coordenadas(){

    FILE *file = fopen("../dj38.tsp", "r");
    if (!file)
    {
        printf("Erro ao abrir o arquivo.");
        return NULL;
    }


    char *qtd = NULL;
    int n = 0;
    char linhaInfo[80];
    while (fgets(linhaInfo, 80, file))
    {
        linhaInfo[strcspn(linhaInfo, "\n")] = '\0';
        
        if (strcmp(linhaInfo, "NODE_COORD_SECTION") == 0)
            break;
        printf("%s\n", linhaInfo);

        char *type = strtok(linhaInfo, " ");
        if (strcmp(type, "DIMENSION:") == 0){
            qtd = strtok(NULL, "");
            n = atoi(qtd);
        }
    }
    char linha[50];

    coordenadas *coord = (coordenadas*) malloc(sizeof(coordenadas) * n);

    while (fgets(linha, 50, file))
    {
        char *ind = strtok(linha, " ");
        char *x = strtok(NULL, " ");
        char *y = strtok(NULL, "");

        int i = atoi(ind);
        i--;

       coord[i].x = atof(x);
       coord[i].y = atof(y);
    }

    return coord;
}