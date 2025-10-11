#include <stdio.h>
#include "admin.h"

void cadastrar_aluno() {
    char nome[100];
    char endereco[200];
    char cpf[15];
    char data_nascimento[12];
    char turma[2];

    printf("Cadastre um novo aluno\n");

    printf("Nome: ");
    scanf(" %[^\n]",nome); //ler com espaço até ENTER

    printf("Endereço: ");
    scanf(" %[^\n]",endereco);

    printf("CPF: ");
    scanf(" %[^\n]",cpf);
    
    printf("Data de nascimento (DD/MM/AAAA): ");
    scanf(" %[^\n]",data_nascimento);

    printf("Turma [A/B/C/D/E]: ");
    scanf(" %[^\n]",turma);

    printf("\nAluno cadastrado:\n");
    printf("Nome: %s\nEndereço: %s\nCPF: %s\nData de nascimento: %s\nTurma: %s\n",nome, endereco,cpf,data_nascimento,turma);

}