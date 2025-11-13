#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    char *key;
    char *data;
    int status;
} Node;

typedef struct
{
    Node *node;
    int size;
    int count;
} HashTable;

HashTable *createHashTable(int size);
void *rehash(HashTable *ht);
int h_1(char *key, int size);
int colisao(char *key, int size, int t);
void Menu(HashTable *ht);
void inserir(HashTable *ht, char *key, char *data);
int buscar(HashTable *ht, char *key);
void remover(HashTable *ht, char *key);
void abrirArquivo(HashTable *ht);
int ehPrimo(int n);
int encontrarProximoPrimo(int n);


int main()
{

    int size = 2003;
    HashTable *ht = createHashTable(size);
    abrirArquivo(ht);
    Menu(ht);

    return 0;
}

void Menu(HashTable *ht)
{
    int opcao = 0;

    while (1)
    {
        printf("Escolha uma opcao:\n1. Inserir uma palavra\n2. Buscar uma palavra\n3. Remover uma palavra\n4. Sair\n");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
        case 1:
        {
            char new[256];
            printf("Digite a palavra e os dados: ");
            fgets(new, 256, stdin);
            new[strcspn(new, "\n")] = '\0';

            char *key = strtok(new, ",");

            char *data = strtok(NULL, "");

            inserir(ht, key, data);
            break;
        }

        case 2:
        {
            char palavra[100];
            printf("Digite a palavra a ser buscada: ");
            fgets(palavra, 100, stdin);
            palavra[strcspn(palavra, "\n")] = '\0';

            int index = buscar(ht, palavra);
            if (index != -1)
            {
                printf("\n================================\n");
                printf("\nPalavra: %s", palavra);
                printf("\nDados: %s\n", ht->node[index].data);
                printf("\n================================\n");
            }
            else
                printf("\nPalavra nao encontrada\n\n");
            break;
        }
        case 3:
            char palavra[100];
            printf("Digite a palavra a ser removida: ");
            fgets(palavra, 100, stdin);
            palavra[strcspn(palavra, "\n")] = '\0';

            remover(ht, palavra);
            break;
        case 4:
            return;
        }
    }
}

HashTable *createHashTable(int size)
{
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    ht->size = size;
    ht->count = 0;
    ht->node = (Node *)malloc(sizeof(Node) * size);

    for (int i = 0; i < size; i++)
    {
        ht->node[i].key = NULL;
        ht->node[i].data = NULL;
        ht->node[i].status = -1;
    }

    return ht;
}

void *rehash(HashTable *ht)
{
    int oldSize = ht->size;
    Node *oldHt = ht->node;

    int newSize = encontrarProximoPrimo(oldSize * 2);

    ht->size = newSize;
    ht->count = 0;
    ht->node = (Node *)malloc(sizeof(Node) * newSize);

    for (int i = 0; i < newSize; i++)
    {
        ht->node[i].key = NULL;
        ht->node[i].data = NULL;
        ht->node[i].status = -1;
    }

    for (int i = 0; i < oldSize; i++)
    {
        if (oldHt[i].status == 0)
        {
            inserir(ht, oldHt[i].key, oldHt[i].data);
        }
    }
    free(oldHt);
}

int h_1(char *key, int size)
{
    int n = strlen(key);
    int base = 31;
    int sum = 0;

    for (int i = 0; i < n - 1; i++)
    {
        sum += abs(key[i]) * base;
    }

    return sum % size;
}

int colisao(char *key, int size, int t)
{
    int n = strlen(key);
    int sum = 0;
    int h1 = h_1(key, size);

    for (int i = 0; i < n - 1; i++)
    {
        sum += key[i];
    }

    int h2 = sum % size;

    return (h1 + (h2 * t)) % size;
}

void inserir(HashTable *ht, char *key, char *data)
{

    if (ht->count > ht->size / 1.3)
        rehash(ht);

    int index = h_1(key, ht->size);
    int t = 1;

    while (ht->node[index].status == 0)
    {
        index = colisao(key, ht->size, t);
        t++;
    }
    ht->node[index].key = strdup(key);
    ht->node[index].data = strdup(data);
    ht->node[index].status = 0;

    ht->count++;

    return;
}

int buscar(HashTable *ht, char *key)
{
    int index = h_1(key, ht->size);
    int t = 1;
    while (1)
    {
        if (ht->node[index].status == 0)
        {
            if (strcmp(ht->node[index].key, key) == 0)
                return index;
            else
            {
                index = colisao(key, ht->size, t);
                t++;
            }
        }
        else
        {
            index = colisao(key, ht->size, t);
            t++;
        }
        if (t == ht->size)
        {
            return -1;
        }
    }
}

void remover(HashTable *ht, char *key){
    int index = buscar(ht, key);
    if (index != -1){
        ht->node[index].key = NULL;
        ht->node[index].data = NULL;
        ht->node[index].status = -2;

        printf("\nPalavra removida com sucesso!\n");
        return;
    }
        printf("\nPalavra nao encontrada\n");
        return;
}

    void abrirArquivo(HashTable *ht)
{
    FILE *file = fopen("C:\\Users\\gustavo\\OneDrive\\Desktop\\output\\data.txt", "r");
    if (!file)
    {
        printf("Erro ao abrir arquivo");
        exit(1);
    }

    char bufferDescarte[80]; 
    int linhaAtual = 1;
    int linhaDeInicio = 525;
    
    while (linhaAtual < linhaDeInicio) 
    {
        fgets(bufferDescarte, 80, file);
        linhaAtual++;
    }

    char linhaArq[80];
    while (fgets(linhaArq, 80, file))
    {
        linhaArq[strcspn(linhaArq, "\n")] = '\0';

        char *key = strtok(linhaArq, ",");
        char *data = strtok(NULL, "");
        printf("%s %s\n", key, data);

        inserir(ht, key, data);
    }
    fclose(file);
}

int ehPrimo(int n)
{
    if (n % 2 == 0) return 0;

    for (int i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

int encontrarProximoPrimo(int n)
{ 
    if (n % 2 == 0)
    {
        n++;
    }

    while (!ehPrimo(n))
    {
        n += 2;
    }
    
    return n;
}