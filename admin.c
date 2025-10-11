#include <stdio.h>
#include <string.h>
#include "admin.h"

typedef struct { //Todas as infos são declaradas aqui, mesmo que não sejam utilizadas em todas as funções. Bom que centraliza tudo
    char nome[100];
    char endereco[200];
    char cpf[15];
    char data_nascimento[12];
    char turma[2]; //A, B, C, D ou E
    char status[10]; //"ativo" ou "inativo"
    int possui_pendencias; //1 para sim, 0 para não
} Aluno;

void cadastrar_aluno() {
    Aluno aluno; //var aluno é do tipo Aluno ali de cima
    
    strcpy(aluno.status, "ativo"); //Começa como ativo
    aluno.possui_pendencias = 0; //Começa sem pendências

    printf("Cadastre um novo aluno\n");

    printf("Nome: ");
    scanf(" %[^\n]",aluno.nome); //ler com espaço até ENTER

    printf("Endereço: ");
    scanf(" %[^\n]",aluno.endereco);

    printf("CPF: ");
    scanf(" %[^\n]",aluno.cpf);
    
    printf("Data de nascimento (DD/MM/AAAA): ");
    scanf(" %[^\n]",aluno.data_nascimento);

    printf("Turma [A/B/C/D/E]: ");
    scanf(" %[^\n]",aluno.turma);

    printf("\nAluno cadastrado:\n");
    printf("Nome: %s\nEndereço: %s\nCPF: %s\nData de nascimento: %s\nTurma: %s\n",aluno.nome,aluno.endereco,aluno.cpf,aluno.data_nascimento,aluno.turma);

    printf("Status: %s\nPossui pendências: ", aluno.status);
        if (aluno.possui_pendencias)
            printf("Sim\n");
        else
    printf("Não\n");

}