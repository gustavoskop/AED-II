#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
typedef struct
{
    int x;
    int y;
} coordenadas;

typedef struct Nodo
{
    int cidade;
    int distancia;
    struct Nodo *prox;
} Nodo;

FILE *abrir_arquivo(const char *nome_arquivo);
int encontrar_numero_de_cidades(FILE *file);
coordenadas *criar_vetor_coordenadas(FILE *file, int n);


int **criar_matriz_de_adjacencia(int n, coordenadas *coords);
void liberar_matriz(int **matriz, int n);
void adicionar_cidade_matriz(int *n, coordenadas **coords, int ***matriz, int x, int y);
void remover_cidade_matriz(int *n, coordenadas **coords, int ***matriz, int x, int y);
void editar_cidade_matriz(int n, coordenadas *coords, int **matriz, int x_ant, int y_ant, int x_novo, int y_novo);
void buscar_cidade_matriz(int n, coordenadas *coords, int **matriz, int x1, int y1, int x2, int y2);


Nodo **criar_lista_de_adjacencia(int n, coordenadas *coords);
void libera_lista(Nodo **lista, int n);
void adicionar_cidade_lista(int *n, coordenadas **coords, Nodo ***lista, int x, int y);
void remover_cidade_lista(int *n, coordenadas **coords, Nodo ***lista, int x, int y);
void editar_cidade_lista(int n, coordenadas *coords, Nodo **lista, int x_ant, int y_ant, int x_novo, int y_novo);
void buscar_cidade_lista(int n, coordenadas *coords, Nodo **lista, int x1, int y1, int x2, int y2);
void imprimir_lista(int n, coordenadas *coords, Nodo **lista);

int distancia_euclidiana(int x1, int y1, int x2, int y2);
int buscar_indice(int n, coordenadas *coords, int x, int y);
void menu_principal(int *n, coordenadas **coords, int ***matriz, Nodo ***lista);


int main()
{

    int tamanho = 0;
    coordenadas *coords = NULL;
    int **matriz = NULL;
    Nodo **lista = NULL;


    FILE *file = abrir_arquivo("../wi29.tsp");
    if (file) {
        tamanho = encontrar_numero_de_cidades(file);
        coords = criar_vetor_coordenadas(file, tamanho);
        fclose(file);
    } else {
        printf("Nao foi possivel abrir o arquivo!");
        return;
    }

    matriz = criar_matriz_de_adjacencia(tamanho, coords);
    lista = criar_lista_de_adjacencia(tamanho, coords);

    menu_principal(&tamanho, &coords, &matriz, &lista);

    liberar_matriz(matriz, tamanho);
    libera_lista(lista, tamanho);
    free(coords);

    return 0;
}


int distancia_euclidiana(int x1, int y1, int x2, int y2)
{
    return (int)sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

int buscar_indice(int n, coordenadas *coords, int x, int y)
{
    for (int i = 0; i < n; i++)
    {
        if (coords[i].x == x && coords[i].y == y)
            return i;
    }
    return -1;
}

FILE *abrir_arquivo(const char *nome_arquivo)
{
    FILE *file = fopen(nome_arquivo, "r");
    if (!file)
    {
        printf("\nErro ao abrir o arquivo: %s\n", nome_arquivo);
    }
    return file;
}

int encontrar_numero_de_cidades(FILE *file)
{
    char *qtd = NULL;
    int n = 0;
    char linhaInfo[80];
    
    rewind(file);

    while (fgets(linhaInfo, 80, file))
    {
        linhaInfo[strcspn(linhaInfo, "\n")] = '\0';
        if (strcmp(linhaInfo, "NODE_COORD_SECTION") == 0)
            break;

        char *type = strtok(linhaInfo, " ");

        if (type && strcmp(type, "DIMENSION:") == 0)
        {
            qtd = strtok(NULL, "");
            if(qtd) n = atoi(qtd);
        }
    }
    return n;
}

coordenadas *criar_vetor_coordenadas(FILE *file, int n)
{
    char linha[50];
    coordenadas *c = (coordenadas *)malloc(sizeof(coordenadas) * n);
    
    int cont = 0;
    while (cont < n && fgets(linha, 50, file) != NULL)
    {
        char *ind = strtok(linha, " ");
        char *x = strtok(NULL, " ");
        char *y = strtok(NULL, "");

        if(ind && x && y) {
            int i = atoi(ind);
            i--; 
            if (i >= 0 && i < n) {
                c[i].x = atoi(x);
                c[i].y = atoi(y);
            }
        }
        cont++;
    }
    return c;
}


int **criar_matriz_de_adjacencia(int n, coordenadas *coords)
{
    if (n == 0) return NULL;

    int **m = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        m[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
        {
            m[i][j] = distancia_euclidiana(coords[i].x, coords[i].y, coords[j].x, coords[j].y);
        }
    }
    return m;
}

void liberar_matriz(int **matriz, int n)
{
    if (!matriz) return;
    for (int i = 0; i < n; i++)
    {
        free(matriz[i]);
    }
    free(matriz);
}

void adicionar_cidade_matriz(int *n, coordenadas **coords, int ***matriz, int x, int y)
{

    liberar_matriz(*matriz, *n);


    coordenadas *temp = realloc(*coords, (*n + 1) * sizeof(coordenadas));
    if (!temp) {
        printf("Erro de memoria\n");

        *matriz = criar_matriz_de_adjacencia(*n, *coords);
        return;
    }
    *coords = temp;

    (*coords)[*n].x = x;
    (*coords)[*n].y = y;

    (*n)++;

    *matriz = criar_matriz_de_adjacencia(*n, *coords);

    printf("\nCidade adicionada na matriz com sucesso!\n");
}

void remover_cidade_matriz(int *n, coordenadas **coords, int ***matriz, int x, int y)
{
    int idx = buscar_indice(*n, *coords, x, y);
    if (idx == -1)
    {
        printf("\nCidade nao encontrada!\n");
        return;
    }

    liberar_matriz(*matriz, *n);

    for (int i = idx; i < *n - 1; i++)
        (*coords)[i] = (*coords)[i + 1];

    if (*n > 1) {
        coordenadas *temp = realloc(*coords, (*n - 1) * sizeof(coordenadas));
        if (temp) *coords = temp;
    } else {
        free(*coords);
        *coords = NULL;
    }

    (*n)--;

    *matriz = criar_matriz_de_adjacencia(*n, *coords);

    printf("\nCidade removida da matriz com sucesso!\n");
}

void editar_cidade_matriz(int n, coordenadas *coords, int **matriz, int x_ant, int y_ant, int x_novo, int y_novo)
{

    int idx = buscar_indice(n, coords, x_ant, y_ant);
    if (idx == -1) {
        printf("Cidade nao encontrada.\n");
        return;
    }

    coords[idx].x = x_novo;
    coords[idx].y = y_novo;
    
    for(int i=0; i<n; i++) {
        int dist = distancia_euclidiana(coords[idx].x, coords[idx].y, coords[i].x, coords[i].y);
        matriz[idx][i] = dist;
        matriz[i][idx] = dist;
    }
    printf("\nCidade editada e distancias recalculadas!\n");
}

void buscar_cidade_matriz(int n, coordenadas *coords, int **matriz, int x1, int y1, int x2, int y2)
{
    int i = buscar_indice(n, coords, x1, y1);
    int j = buscar_indice(n, coords, x2, y2);

    if (i == -1 || j == -1) {
        printf("Cidade nao encontrada!\n");
        return;
    }
    printf("\nDistancia na Matriz: %d\n", matriz[i][j]);
}

Nodo **criar_lista_de_adjacencia(int n, coordenadas *coords)
{
    if (n == 0) return NULL;
    
    Nodo **l = malloc(n * sizeof(Nodo *));
    for (int i = 0; i < n; i++) l[i] = NULL;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j) continue;

            Nodo *novo = malloc(sizeof(Nodo));
            novo->cidade = j;
            novo->distancia = distancia_euclidiana(coords[i].x, coords[i].y, coords[j].x, coords[j].y);
            novo->prox = l[i];
            l[i] = novo;
        }
    }
    return l;
}

void libera_lista(Nodo **lista, int n)
{
    if (!lista) return;
    for (int i = 0; i < n; i++)
    {
        Nodo *atual = lista[i];
        while (atual)
        {
            Nodo *prox = atual->prox;
            free(atual);
            atual = prox;
        }
    }
    free(lista);
}

void adicionar_cidade_lista(int *n, coordenadas **coords, Nodo ***lista, int x, int y)
{

    libera_lista(*lista, *n);


    coordenadas *temp = realloc(*coords, (*n + 1) * sizeof(coordenadas));
    if(!temp) {
        *lista = criar_lista_de_adjacencia(*n, *coords);
        return;
    }
    *coords = temp;
    (*coords)[*n].x = x;
    (*coords)[*n].y = y;

    (*n)++;

    *lista = criar_lista_de_adjacencia(*n, *coords);
    printf("\nCidade adicionada na lista com sucesso!\n");
}

void remover_cidade_lista(int *n, coordenadas **coords, Nodo ***lista, int x, int y)
{
    int idx = buscar_indice(*n, *coords, x, y);
    if (idx == -1) {
        printf("Nao encontrada.\n");
        return;
    }

    libera_lista(*lista, *n);

    for (int i = idx; i < *n - 1; i++)
        (*coords)[i] = (*coords)[i + 1];

    if (*n > 1) {
        coordenadas *temp = realloc(*coords, (*n - 1) * sizeof(coordenadas));
        if(temp) *coords = temp;
    } else {
        free(*coords);
        *coords = NULL;
    }

    (*n)--;
    *lista = criar_lista_de_adjacencia(*n, *coords);
    printf("\nCidade removida da lista.\n");
}

void editar_cidade_lista(int n, coordenadas *coords, Nodo **lista, int x_ant, int y_ant, int x_novo, int y_novo)
{
    int idx = buscar_indice(n, coords, x_ant, y_ant);
    if (idx == -1) return;

    coords[idx].x = x_novo;
    coords[idx].y = y_novo;

}

void buscar_cidade_lista(int n, coordenadas *coords, Nodo **lista, int x1, int y1, int x2, int y2)
{
    int i = buscar_indice(n, coords, x1, y1);
    int j = buscar_indice(n, coords, x2, y2);

    if (i == -1 || j == -1) {
        printf("Cidade nao encontrada.\n");
        return;
    }

    Nodo *aux = lista[i];
    while (aux)
    {
        if (aux->cidade == j)
        {
            printf("\nDistancia na Lista: %d\n", aux->distancia);
            return;
        }
        aux = aux->prox;
    }
}

void imprimir_lista(int n, coordenadas *coords, Nodo **lista)
{
    for (int i = 0; i < n; i++)
    {
        printf("\nCidade %d (%d, %d) -> ", i + 1, coords[i].x, coords[i].y);
        Nodo *aux = lista[i];
        while (aux)
        {
            printf("[%d | %d] -> ", aux->cidade + 1, aux->distancia);
            aux = aux->prox;
        }
    }
}

void menu_principal(int *n, coordenadas **coords, int ***matriz, Nodo ***lista)
{
    int op = -1;
    do
    {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1 - Operacoes com Matriz\n");
        printf("2 - Operacoes com Lista\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);

        if (op == 1)
        {
            int sub = -1;
            while (sub != 0)
            {
                int x1, y1, x2, y2;
                printf("\n--- MATRIZ (%d cidades) ---\n", *n);
                printf("1 - Adicionar cidade\n2 - Remover cidade\n3 - Editar cidade\n4 - Buscar cidade\n5 - Imprimir matriz\n0 - Voltar\nOpcao: ");
                scanf("%d", &sub);

                switch(sub) {
                    case 1:
                        printf("X: "); scanf("%d", &x1);
                        printf("Y: "); scanf("%d", &y1);
                        adicionar_cidade_matriz(n, coords, matriz, x1, y1);
                        break;
                    case 2:
                        printf("X: "); scanf("%d", &x1);
                        printf("Y: "); scanf("%d", &y1);
                        remover_cidade_matriz(n, coords, matriz, x1, y1);
                        break;
                    case 3:
                        printf("X Antigo: "); scanf("%d", &x1);
                        printf("Y Antigo: "); scanf("%d", &y1);
                        printf("X Novo: "); scanf("%d", &x2);
                        printf("Y Novo: "); scanf("%d", &y2);
                        editar_cidade_matriz(*n, *coords, *matriz, x1, y1, x2, y2);
                        break;
                    case 4:
                        printf("X1: "); scanf("%d", &x1);
                        printf("Y1: "); scanf("%d", &y1);
                        printf("X2: "); scanf("%d", &x2);
                        printf("Y2: "); scanf("%d", &y2);
                        buscar_cidade_matriz(*n, *coords, *matriz, x1, y1, x2, y2);
                        break;
                    case 5:
                        for(int i=0; i<*n; i++) {
                            for(int j=0; j<*n; j++) printf("%3d ", (*matriz)[i][j]);
                            printf("\n");
                        }
                        break;
                }
            }
        }
        else if (op == 2)
        {
            int sub = -1;
            while (sub != 0)
            {
                int x1, y1, x2, y2;
                printf("\n--- LISTA (%d cidades) ---\n", *n);
                printf("1 - Adicionar cidade\n2 - Remover cidade\n3 - Editar cidade\n4 - Buscar cidade\n5 - Imprimir lista\n0 - Voltar\nOpcao: ");
                scanf("%d", &sub);

                switch(sub) {
                    case 1:
                        printf("X: "); scanf("%d", &x1);
                        printf("Y: "); scanf("%d", &y1);
                        adicionar_cidade_lista(n, coords, lista, x1, y1);
                        break;
                    case 2:
                        printf("X: "); scanf("%d", &x1);
                        printf("Y: "); scanf("%d", &y1);
                        remover_cidade_lista(n, coords, lista, x1, y1);
                        break;
                    case 3:
                        printf("X Antigo: "); scanf("%d", &x1);
                        printf("Y Antigo: "); scanf("%d", &y1);
                        printf("X Novo: "); scanf("%d", &x2);
                        printf("Y Novo: "); scanf("%d", &y2);
                        editar_cidade_lista(*n, *coords, *lista, x1, y1, x2, y2);
                        break;
                    case 4:
                        printf("X1: "); scanf("%d", &x1);
                        printf("Y1: "); scanf("%d", &y1);
                        printf("X2: "); scanf("%d", &x2);
                        printf("Y2: "); scanf("%d", &y2);
                        buscar_cidade_lista(*n, *coords, *lista, x1, y1, x2, y2);
                        break;
                    case 5:
                        imprimir_lista(*n, *coords, *lista);
                        break;
                }
            }
        }

    } while (op != 0);
}