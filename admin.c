#include <stdio.h>
#include <string.h>
#include "admin.h"

//Cadastrar aluno, clássico
void cadastrar_aluno(Aluno *aluno) {
    strcpy(aluno->status, "ativo"); //Começa como ativo
    aluno->possui_pendencias = 0; //Começa sem pendências

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
