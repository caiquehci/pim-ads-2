#include <stdio.h>
#include <string.h>
#include "prof.h"
#include "admin.h"
#include "menu.h"

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

    calcular_situacao(&alunos[indice]);
    salvar_alunos_em_arquivo();

    printf("\nNotas atribuídas para %s: \n", alunos[indice].nome);
    for (int j = 0; j < 4; j++){
        printf("Nota :%d: %.2f\n", j+1, alunos[indice].notas[j]);
    }
}

void atribuir_faltas(Aluno *aluno, int num_alunos) {
    if (num_alunos == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    printf("\nLista de alunos disponíveis:\n");
    for (int i = 0; i < num_alunos; i++) {
        printf("%d - %s (Turma %s)\n", i + 1, aluno[i].nome, aluno[i].turma);
    }

    int escolha;
    printf("Digite o número do aluno para atribuir faltas (0 para sair): ");
    scanf("%d", &escolha);
    getchar();

    if (escolha == 0) {
        printf("Operação cancelada.\n");
        return;
    }

    int indice = escolha - 1;
    if (indice < 0 || indice >= num_alunos) {
        printf("Aluno inexistente.\n");
        return;
    }

    int faltas;
    printf("Digite o número de faltas (0 a 100): ");
    scanf("%d", &faltas);
    getchar();

    if (faltas < 0 || faltas > 100) {
        printf("Valor incorreto de faltas.\n");
        return;
    }

    aluno[indice].faltas = faltas;

    calcular_situacao(&aluno[indice]);

    printf("Faltas atribuídas para %s: %d\n", aluno[indice].nome, faltas);
    salvar_alunos_em_arquivo();

}

void calcular_situacao(Aluno *aluno) {
    float soma_notas = 0.0;
    int num_notas = 4; // ou quantas notas usa

    for (int i = 0; i < num_notas; i++)
    {
        soma_notas += aluno->notas[i];
    }

    float media = soma_notas / num_notas;
    int faltas_maximas = 25;

    if (media < 6.0 || aluno->faltas > faltas_maximas)
    {
        strcpy(aluno->situacao, "reprovado");
    }
    else
    {
        strcpy(aluno->situacao, "aprovado");
    }
}

void cadastrar_aulas(Aula *aula, int num_aulas)
{
    if (num_aulas >= MAX_AULAS)
    {
        printf("Limite máximo de aulas atingido.\n");
        return;
    }

    Aula nova_aula;

    printf("\n=== Cadastro de Aula ===\n");

    gerar_id_aula(&nova_aula.id);

    printf("Ano (ex: 2024): ");
    scanf("%d", &nova_aula.ano);
    getchar();

    printf("Matéria: ");
    fgets(nova_aula.materia, sizeof(nova_aula.materia), stdin);
    nova_aula.materia[strcspn(nova_aula.materia, "\n")] = '\0';

    printf("Turma (ex: A, B, etc): ");
    fgets(nova_aula.turma, sizeof(nova_aula.turma), stdin);
    nova_aula.turma[strcspn(nova_aula.turma, "\n")] = '\0';

    printf("Conteúdo / corpo da aula (até 1000 caracteres):\n");
    fgets(nova_aula.corpo, sizeof(nova_aula.corpo), stdin);
    nova_aula.corpo[strcspn(nova_aula.corpo, "\n")] = '\0';

    // Adiciona no array recebido
    aula[num_aulas] = nova_aula;
    total_aulas++; // incrementa o global

    salvar_aulas_em_arquivo();

    printf("\nAula cadastrada com sucesso! (ID: %d)\n", nova_aula.id);
}