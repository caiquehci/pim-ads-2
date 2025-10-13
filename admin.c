#include <stdio.h>
#include <string.h>
#include "admin.h"

extern int proximo_id; 

//Cadastrar aluno, clássico
void cadastrar_aluno(Aluno *aluno) {
    aluno->id = proximo_id++;

    strcpy(aluno->status, "ativo"); //Começa como ativo
    aluno->possui_pendencias = 0; //Começa sem pendências

    aluno->faltas = 0;//tava dando -8927392873 faltas no .txt
    aluno->nota = 0.0f;//mesma coisa ↑

    printf("Cadastre um novo aluno\n");

    printf("Nome: ");
    scanf(" %[^\n]",aluno->nome); //ler com espaço até ENTER

    printf("Endereço: ");
    scanf(" %[^\n]",aluno->endereco);

    printf("CPF: ");
    scanf(" %[^\n]",aluno->cpf);
    
    printf("Data de nascimento (DD/MM/AAAA): ");
    scanf(" %[^\n]",aluno->data_nascimento);

    printf("Turma [A/B/C/D/E]: ");
    scanf(" %[^\n]",aluno->turma);

    printf("\nAluno cadastrado:\n");
    printf("Nome: %s\nEndereço: %s\nCPF: %s\nData de nascimento: %s\nTurma: %s\n",aluno->nome,aluno->endereco,aluno->cpf,aluno->data_nascimento,aluno->turma);

    printf("Status: %s\nPossui pendências: ", aluno->status);
        if (aluno->possui_pendencias) 
            printf("Sim\n");
        else
    printf("Não\n");

    char nome_arquivo[50];
//    sprintf(nome_arquivo, "aluno_%d.txt", aluno->id);
//isso era pra fazer um arquivo por aluno
    arquivo_aluno(aluno, "alunos.txt");
}

//Cadastrar professor, clássico
void cadastrar_professor(Professor *prof) {
    printf("Cadastro de professor: \n");

    printf("Nome: ");
    scanf(" %[^\n]",prof->nome);
    
    printf("CPF: ");
    scanf(" %[^\n]",prof->cpf);
    
    printf("Disciplina: ");
    scanf(" %[^\n]",prof->disciplina);
    
    printf("E-mail: ");
    scanf(" %[^\n]",prof->email);

    printf("Professor cadastrado:\n");
    printf("Nome: %s\nCPF: %s\nDisciplina: %s\nE-mail: %s\n",prof->nome, prof->cpf, prof->disciplina, prof->email);
}

//Alterar status do aluno
void alterar_status_aluno(Aluno *aluno, const char *novo_status) {
    //pelo visto tem que ser ponteiro pq senão recebe cópia e não o valor vigente
    if (aluno->possui_pendencias) {
        printf("Não é possível alterar status. Aluno possui pendências.\n");
        return; //Sair sem fazer nada
    }
    strncpy(aluno->status, novo_status, sizeof(aluno->status) -1);
    aluno->status[sizeof(aluno->status) -1] = '\0';

    printf("Status do aluno alterado para: %s\n",aluno->status);
}

//Criar arquivo do aluno
void arquivo_aluno(const Aluno *aluno, const char *alunosss) {
    FILE *fp = fopen(alunosss, "a");// de w pra a, de write para append
    if (fp == NULL ) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fprintf(fp, "ID: %d\n", aluno->id);
    fprintf(fp, "Nome: %s\n", aluno->nome);
    fprintf(fp, "Endereco: %s\n", aluno->endereco);
    fprintf(fp, "CPF: %s\n", aluno->cpf);
    fprintf(fp, "Data de nascimento: %s\n", aluno->data_nascimento);
    fprintf(fp, "Turma: %s\n", aluno->turma);
    fprintf(fp, "Status: %s\n", aluno->status);
    fprintf(fp, "Possui pendencias: %s\n", aluno->possui_pendencias ? "Sim" : "Nao");
    fprintf(fp, "Faltas: %d\n", aluno->faltas);
    fprintf(fp, "Nota: %.2f\n", aluno->nota);

    fclose(fp);

    printf("Dados do aluno salvos em '%s'.\n", alunosss);
}