#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define M 5

typedef struct BTPage
{
    short int numChavesAtual;
    int chave[M - 1];
    struct BTPage *filhos[M];

} Page;

typedef struct Aluno
{
    int matricula;
    char nome[256];
} Aluno;

typedef struct Disciplina
{
    int codigo;
    char nome[256];
} Disciplina;

typedef struct Matricula
{
    int id_matricula;
    int matricula_aluno;
    int codigo_disciplina;
    float media;
} Matricula;

void Menu(Page *root);

void Criar();
void ler();
void Atualizar();
void Remover();

void LerArquivo();
void EscreverArquivo();

Page *CriarNo();

int main()
{
    Page *root = CriarNo();
    Menu(root);

    return 0;
}

Page *CriarNo()
{
    Page *node = (Page *)calloc(1, sizeof(Page));
    node->numChavesAtual = 0;
    return node;
}
