#include <stdio.h>
#include "prof.h"

void atribuir_notas(Aluno *aluno, int num_alunos) {
    if (num_alunos == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    printf("\nLista de alunos disponíveis:\n");
    for (int i = 0; i < num_alunos; i++) {
        printf("%d - %s (Turma %s)\n", i+1, alunos[i].nome, alunos[i].turma);
    }

    int escolha;
    printf("Digite o número do aluno para escolher e dar as notas (0 para sair):");
    scanf(" %d",&escolha);
    getchar();

    if (escolha == 0) {
        printf("Sair.\n");
        return;
    }

    int indice = escolha - 1;
    if (indice < 0 || indice >= num_alunos) {
        printf("Aluno inexistente.\n");
        return;
    }

    printf("\nAtribuir notas para o aluno: %s\n", alunos[indice].nome);
    for (int j = 0; j < 4; j++) {
        printf("Digite a nota %d: ", j + 1);
        scanf("%f", &alunos[indice].notas[j]);
    }

    salvar_alunos_em_arquivo();

    printf("\nNotas atribuídas para %s: \n", alunos[indice].nome);
    for (int j = 0; j < 4; j++){
        printf("Nota :%d: %.2f\n", j+1, alunos[indice].notas[j]);
    }
}

void atribuir_faltas(Aluno *aluno, int num_alunos) {
    if (num_alunos == 0)
    {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    printf("\nLista de alunos disponíveis:\n");
    for (int i = 0; i < num_alunos; i++)
    {
        printf("%d - %s (Turma %s)\n", i + 1, aluno[i].nome, aluno[i].turma);
    }

    int escolha;
    printf("Digite o número do aluno para atribuir faltas (0 para sair): ");
    scanf("%d", &escolha);
    getchar();

    if (escolha == 0)
    {
        printf("Operação cancelada.\n");
        return;
    }

    int indice = escolha - 1;
    if (indice < 0 || indice >= num_alunos)
    {
        printf("Aluno inexistente.\n");
        return;
    }

    int faltas;
    printf("Digite o número de faltas (0 a 100): ");
    scanf("%d", &faltas);
    getchar();

    if (faltas < 0 || faltas > 100)
    {
        printf("Valor incorreto de faltas.\n");
        return;
    }

    aluno[indice].faltas = faltas;
    printf("Faltas atribuídas para %s: %d\n", aluno[indice].nome, faltas);

    salvar_alunos_em_arquivo();
}