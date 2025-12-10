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

#define FILE_NAME "../wi29.tsp"
int TAMANHO = 0;
coordenadas *COORDENADAS = NULL;
int **MATRIZ = NULL;
Nodo **LISTA = NULL;

FILE *abrir_arquivo();
int encontrar_numero_de_cidades(FILE *file);
coordenadas *criar_vetor_coordenadas(FILE *file, int n);

int **criar_matriz_de_adjacencia();
int buscar_indice_matriz(int x1, int y1);
void adicionar_cidade_matriz(int x1, int y1);
void remover_cidade_matriz(int x1, int y1);
void editar_cidade_matriz(int x1, int y1, int x2, int y2);
void buscar_cidade_matriz(int x1, int y1, int x2, int y2);
void liberar_matriz();

Nodo **criar_lista_de_adjacencia();
int buscar_indice(int x1, int y1);
void adicionar_cidade_lista(int x1, int y1);
void remover_cidade_lista(int x1, int y1);
void editar_cidade_lista(int x1, int y1, int x2, int y2);
void buscar_cidade_lista(int x1, int y1, int x2, int y2);
void imprimir_lista();
void libera_lista();

int distancia_euclidiana(int x1, int y1, int x2, int y2);

int menu();

int main()
{

    FILE *file = abrir_arquivo();
    TAMANHO = encontrar_numero_de_cidades(file);
    COORDENADAS = criar_vetor_coordenadas(file, TAMANHO);

    MATRIZ = criar_matriz_de_adjacencia();
    LISTA = criar_lista_de_adjacencia();
    menu();

    fclose(file);
}

int menu()
{
    int op = -1;
    int op_lista = -1;
    int op_matriz = -1;

    do
    {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1 - Usar Matriz de Adjacencia\n");
        printf("2 - Usar Lista de Adjacencia\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &op);
        getchar();

        switch (op)
        {
        case 1:
            do
            {
                int x1, y1, x2, y2;
                printf("\n--- MENU MATRIZ ---\n");
                printf("1 - Adicionar cidade\n");
                printf("2 - Remover cidade\n");
                printf("3 - Editar cidade\n");
                printf("4 - Verificar se cidades tem estrada\n");
                printf("5 - Imprimir matriz\n");
                printf("0 - Voltar\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &op_matriz);

                switch (op_matriz)
                {
                case 1:
                    printf("Escreva a posicao x da cidade: ");
                    scanf("%d", &x1);
                    printf("Escreva a posicao y da cidade: ");
                    scanf("%d", &y1);
                    adicionar_cidade_matriz(x1, y1);
                    break;
                case 2:
                    printf("Escreva a posicao x da cidade: ");
                    scanf("%d", &x1);
                    printf("Escreva a posicao y da cidade: ");
                    scanf("%d", &y1);
                    remover_cidade_matriz(x1, y1);
                    break;
                case 3:
                    printf("Escreva a posicao x antiga da cidade: ");
                    scanf("%d", &x1);
                    printf("Escreva a posicao y antiga da cidade: ");
                    scanf("%d", &y1);

                    printf("Escreva a posicao x nova da cidade: ");
                    scanf("%d", &x2);
                    printf("Escreva a posicao y nova da cidade: ");
                    scanf("%d", &y2);
                    editar_cidade_matriz(x1, y1, x2, y2);
                    break;
                case 4:
                    printf("Escreva a posicao x da primeira cidade: ");
                    scanf("%d", &x1);
                    printf("Escreva a posicao y da primeira cidade: ");
                    scanf("%d", &y1);

                    printf("Escreva a posicao x da segunda cidade: ");
                    scanf("%d", &x2);
                    printf("Escreva a posicao y da segunda cidade: ");
                    scanf("%d", &y2);
                    buscar_cidade_matriz(x1, y1, x2, y2);
                    break;
                case 5:
                    for (int i = 0; i < TAMANHO; i++)
                    {
                        for (int j = 0; j < TAMANHO; j++)
                        {
                            printf("%d ", MATRIZ[i][j]);
                        }
                        printf("\n");
                    }
                    break;
                case 0:
                    break;
                default:
                    printf("\nEscolha uma opcao valida\n");
                    break;
                }
            } while (op_matriz != 0);
            break;
        case 2:
            do
            {
                int x1, y1, x2, y2;
                printf("\n--- MENU LISTA ---\n");
                printf("1 - Adicionar cidade\n");
                printf("2 - Remover cidade\n");
                printf("3 - Editar cidade\n");
                printf("4 - Verificar se cidades tem estrada\n");
                printf("5 - Imprimir lista\n");
                printf("0 - Voltar\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &op_lista);

                switch (op_lista)
                {
                case 1:
                    printf("Escreva a posicao x da cidade: ");
                    scanf("%d", &x1);
                    printf("Escreva a posicao y da cidade: ");
                    scanf("%d", &y1);
                    adicionar_cidade_lista(x1, y1);
                    break;
                case 2:
                    printf("Escreva a posicao x da cidade: ");
                    scanf("%d", &x1);
                    printf("Escreva a posicao y da cidade: ");
                    scanf("%d", &y1);
                    remover_cidade_lista(x1, y1);
                    break;
                case 3:
                    printf("Escreva a posicao x antiga da cidade: ");
                    scanf("%d", &x1);
                    printf("Escreva a posicao y antiga da cidade: ");
                    scanf("%d", &y1);

                    printf("Escreva a posicao x nova da cidade: ");
                    scanf("%d", &x2);
                    printf("Escreva a posicao y nova da cidade: ");
                    scanf("%d", &y2);
                    editar_cidade_lista(x1, y1, x2, y2);
                    break;
                case 4:
                    printf("Escreva a posicao x da primeira cidade: ");
                    scanf("%d", &x1);
                    printf("Escreva a posicao y da primeira cidade: ");
                    scanf("%d", &y1);

                    printf("Escreva a posicao x da segunda cidade: ");
                    scanf("%d", &x2);
                    printf("Escreva a posicao y da segunda cidade: ");
                    scanf("%d", &y2);
                    buscar_cidade_lista(x1, y1, x2, y2);
                    break;
                case 5:
                    imprimir_lista(LISTA);
                    break;
                case 0:
                    break;
                default:
                    printf("\nEscolha uma opcao valida\n");
                    break;
                }
            } while (op_lista != 0);
        case 0:
            break;

        default:
            printf("\nEscolha uma opcao valida\n");
        }

    } while (op != 0);

    libera_lista();
    liberar_matriz();

    printf("\n\n\n\n\nESSA PARTE EU USEI O GPT PRA DEIXAR BONITO\n\n");
    printf("-----------------------------------------------------------------------\n");
    printf("                      Complexidade de Tempo\n");
    printf("-----------------------------------------------------------------------\n");
    printf("%-20s %-15s %-15s %-15s\n", "Operacao", "Matriz", "Lista", "Melhor");
    printf("-----------------------------------------------------------------------\n");
    printf("%-20s %-15s %-15s %-15s\n", "Criar estrutura", "O(N^2)", "O(N^2)", "Empate");
    printf("%-20s %-15s %-15s %-15s\n", "Adicionar cidade", "O(N^2)", "O(N^2)", "Empate");
    printf("%-20s %-15s %-15s %-15s\n", "Remover cidade",   "O(N^2)", "O(N^2)", "Empate");
    printf("%-20s %-15s %-15s %-15s\n", "Editar cidade",    "O(N^2)", "O(N^2)", "Empate");
    printf("%-20s %-15s %-15s %-15s\n", "Buscar estrada",   "O(1)",   "O(N)",   "Matriz");
    printf("%-20s %-15s %-15s %-15s\n", "Imprimir",         "O(N^2)", "O(N^2)", "Empate");
    printf("-----------------------------------------------------------------------\n");

    return 0;
}

FILE *abrir_arquivo()
{
    FILE *file = fopen(FILE_NAME, "r");
    if (!file)
    {
        printf("\nErro ao abrir o arquivo.\n");
    }
    return file;
}

int encontrar_numero_de_cidades(FILE *file)
{

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
        if (strcmp(type, "DIMENSION:") == 0)
        {
            qtd = strtok(NULL, "");
            n = atoi(qtd);
        }
    }

    return n;
}

coordenadas *criar_vetor_coordenadas(FILE *file, int n)
{

    char linha[50];

    coordenadas *coord = (coordenadas *)malloc(sizeof(coordenadas) * n);

    while (fgets(linha, 50, file) != NULL)
    {
        char *ind = strtok(linha, " ");
        char *x = strtok(NULL, " ");
        char *y = strtok(NULL, "");

        int i = atoi(ind);
        i--;
        coord[i].x = atoi(x);
        coord[i].y = atoi(y);
    }

    return coord;
}

int **criar_matriz_de_adjacencia()
{

    int **matriz;
    matriz = malloc(TAMANHO * sizeof(int *));
    for (int i = 0; i < TAMANHO; i++)
    {
        matriz[i] = malloc(TAMANHO * sizeof(int));
    }

    for (int i = 0; i < TAMANHO; i++)
    {
        for (int j = 0; j < TAMANHO; j++)
        {
            matriz[i][j] = distancia_euclidiana(COORDENADAS[i].x, COORDENADAS[i].y, COORDENADAS[j].x, COORDENADAS[j].y);
        }
    }

    return matriz;
}

int distancia_euclidiana(int x1, int y1, int x2, int y2)
{
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

Nodo **criar_lista_de_adjacencia()
{
    Nodo **lista = malloc(TAMANHO * sizeof(Nodo *));
    for (int i = 0; i < TAMANHO; i++)
        lista[i] = NULL;

    for (int i = 0; i < TAMANHO; i++)
    {
        for (int j = 0; j < TAMANHO; j++)
        {
            if (i == j)
                continue;

            Nodo *novo = malloc(sizeof(Nodo));
            novo->cidade = j;
            novo->distancia = distancia_euclidiana(
                COORDENADAS[i].x, COORDENADAS[i].y,
                COORDENADAS[j].x, COORDENADAS[j].y);

            novo->prox = lista[i];
            lista[i] = novo;
        }
    }

    return lista;
}

int buscar_indice(int x1, int y1)
{
    for (int i = 0; i < TAMANHO; i++)
        if (COORDENADAS[i].x == x1 && COORDENADAS[i].y == y1)
            return i;

    return -1;
}

void adicionar_cidade_lista(int x1, int y1)
{
    TAMANHO++;
    coordenadas *teste = realloc(COORDENADAS, TAMANHO * sizeof(coordenadas));
    if (!teste)
    {
        printf("\nErro no realloc!\n");
        TAMANHO--;
        return;
    }
    COORDENADAS = teste;
    COORDENADAS[TAMANHO - 1].x = x1;
    COORDENADAS[TAMANHO - 1].y = y1;
    for (int i = 0; i < TAMANHO - 1; i++)
    {
        Nodo *aux = LISTA[i];
        while (aux)
        {
            Nodo *tmp = aux;
            aux = aux->prox;
            free(tmp);
        }
    }
    free(LISTA);

    LISTA = criar_lista_de_adjacencia();

    printf("\nCidade adicionada com sucesso!\n");
}

void remover_cidade_lista(int x1, int y1)
{
    int idx = buscar_indice(x1, y1);
    if (idx == -1)
    {
        printf("\nCidade nao encontrada!\n");
        return;
    }

    for (int i = idx; i < TAMANHO - 1; i++)
        COORDENADAS[i] = COORDENADAS[i + 1];

    TAMANHO--;
    coordenadas *teste = realloc(COORDENADAS, TAMANHO * sizeof(coordenadas));
    if (!teste)
    {
        printf("\nErro no realloc!\n");
        TAMANHO++;
        return;
    }
    COORDENADAS = teste;

    for (int i = 0; i < TAMANHO + 1; i++)
    {
        Nodo *aux = LISTA[i];
        while (aux)
        {
            Nodo *tmp = aux;
            aux = aux->prox;
            free(tmp);
        }
    }
    free(LISTA);

    LISTA = criar_lista_de_adjacencia();

    printf("\nCidade removida com sucesso!\n");
}

void editar_cidade_lista(int x1, int y1, int x2, int y2)
{
    int idx = buscar_indice(x1, y1);
    if (idx == -1)
    {
        printf("\nCidade nao encontrada!\n");
        return;
    }

    COORDENADAS[idx].x = x2;
    COORDENADAS[idx].y = y2;

    for (int i = 0; i < TAMANHO; i++)
    {
        Nodo *aux = LISTA[i];
        while (aux)
        {
            Nodo *tmp = aux;
            aux = aux->prox;
            free(tmp);
        }
    }
    free(LISTA);

    LISTA = criar_lista_de_adjacencia();

    printf("\nCidade editada com sucesso!\n");
}

void buscar_cidade_lista(int x1, int y1, int x2, int y2)
{
    int i = buscar_indice(x1, y1);
    int j = buscar_indice(x2, y2);

    if (i == -1 || j == -1)
    {
        printf("\nCidade nao encontrada!\n");
        return;
    }

    Nodo *aux = LISTA[i];
    while (aux)
    {
        if (aux->cidade == j)
        {
            printf("\nExiste estrada entre as cidades! Distancia = %d\n",
                   aux->distancia);
            return;
        }
        aux = aux->prox;
    }
}

void imprimir_lista()
{
    for (int i = 0; i < TAMANHO; i++)
    {
        printf("\nCidade %d (%d, %d) -> ",
               i + 1, COORDENADAS[i].x, COORDENADAS[i].y);

        Nodo *aux = LISTA[i];
        while (aux)
        {
            printf("[%d | %d] -> ", aux->cidade + 1, aux->distancia);
            aux = aux->prox;
        }
    }
}

int buscar_indice_matriz(int x1, int y1)
{
    for (int i = 0; i < TAMANHO; i++)
    {
        if (COORDENADAS[i].x == x1 && COORDENADAS[i].y == y1)
            return i;
    }
    return -1;
}

void adicionar_cidade_matriz(int x1, int y1)
{
    TAMANHO++;

    coordenadas *teste = realloc(COORDENADAS, TAMANHO * sizeof(coordenadas));
    if (!teste)
    {
        printf("\nErro no realloc!\n");
        TAMANHO--;
        return;
    }
    COORDENADAS = teste;

    COORDENADAS[TAMANHO - 1].x = x1;
    COORDENADAS[TAMANHO - 1].y = y1;
    TAMANHO--;
    liberar_matriz();
    TAMANHO++;
    MATRIZ = criar_matriz_de_adjacencia();

    printf("\nCidade adicionada na matriz com sucesso!\n");
}

void remover_cidade_matriz(int x1, int y1)
{
    int idx = buscar_indice_matriz(x1, y1);
    if (idx == -1)
    {
        printf("\nCidade nao encontrada na matriz!\n");
        return;
    }

    for (int i = idx; i < TAMANHO - 1; i++)
        COORDENADAS[i] = COORDENADAS[i + 1];

    TAMANHO--;

    coordenadas *teste = realloc(COORDENADAS, TAMANHO * sizeof(coordenadas));
    if (!teste)
    {
        printf("\nErro no realloc!\n");
        return;
    }
    COORDENADAS = teste;

    liberar_matriz(MATRIZ, TAMANHO + 1);

    MATRIZ = criar_matriz_de_adjacencia();

    printf("\nCidade removida da matriz com sucesso!\n");
}

void editar_cidade_matriz(int x1, int y1,
                          int x2, int y2)
{
    int idx = buscar_indice_matriz(x1, y1);
    if (idx == -1)
    {
        printf("\nCidade nao encontrada na matriz!\n");
        return;
    }

    COORDENADAS[idx].x = x2;
    COORDENADAS[idx].y = y2;

    liberar_matriz(MATRIZ, TAMANHO);

    MATRIZ = criar_matriz_de_adjacencia();

    printf("\nCidade editada na matriz com sucesso!\n");
}

void buscar_cidade_matriz(int x1, int y1, int x2, int y2)
{
    int i = buscar_indice_matriz(x1, y1);
    int j = buscar_indice_matriz(x2, y2);

    if (i == -1 || j == -1)
    {
        printf("Cidade nao encontrada!\n");
        return;
    }

    printf("\nExiste estrada entre as cidades! Distancia = %d\n", MATRIZ[i][j]);
}

void liberar_matriz()
{
    if (!MATRIZ)
        return;

    for (int i = 0; i < TAMANHO; i++)
        free(MATRIZ[i]);

    free(MATRIZ);
}

void libera_lista()
{
    if (LISTA == NULL)
        return;

    for (int i = 0; i < TAMANHO; i++)
    {
        Nodo *atual = LISTA[i];
        while (atual != NULL)
        {
            Nodo *prox = atual->prox;
            free(atual);
            atual = prox;
        }
    }

    free(LISTA);
}