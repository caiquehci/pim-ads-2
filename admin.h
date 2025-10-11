#ifndef ADMIN_H
#define ADMIN_H

typedef struct { //Todas as infos são declaradas aqui, mesmo que não sejam utilizadas em todas as funções. Bom que centraliza tudo
    char nome[100];
    char endereco[200];
    char cpf[15];
    char data_nascimento[12];
    char turma[2]; //A, B, C, D ou E
    char status[10]; //"ativo" ou "inativo"
    int possui_pendencias; //1 para sim, 0 para não
} Aluno;


//função de cadastro de alunos no sistema
void cadastrar_aluno(Aluno *aluno);
void alterar_status_aluno(Aluno *aluno, const char *novo_status);

#endif