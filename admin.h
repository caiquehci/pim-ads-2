#ifndef ADMIN_H
#define ADMIN_H

#define MAX_ALUNOS 100

extern int total_alunos;

//Estrutura de aluno
typedef struct { //Todas as infos são declaradas aqui, mesmo que não sejam utilizadas em todas as funções. Bom que centraliza tudo
    int id;
    
    char nome[100];
    char endereco[200];
    char cpf[15];
    char data_nascimento[12];
    char turma[2]; //A, B, C, D ou E
    char status[10]; //"ativo" ou "inativo"
    int possui_pendencias; //1 para sim, 0 para não

    int faltas;
    float nota; 
} Aluno;

extern Aluno alunos[MAX_ALUNOS];

//Estrutura de professor
typedef struct {
    char nome[100];
    char cpf[15];
    char disciplina[50];
    char email[100];
} Professor;

//funções de cadastro no sistema
////Aluno
void cadastrar_aluno(Aluno *aluno);
void alterar_status_aluno(Aluno *aluno, const char *novo_status);
void alterar_status_aluno_por_id(void);
int buscar_aluno_por_id(int id);
int ler_alunos_de_arquivo(Aluno alunos[], int max_alunos);

void arquivo_aluno(const Aluno *aluno, const char *nome_arquivo);
void salvar_alunos_em_arquivo(void);

////Professor
void cadastrar_professor(Professor *prof);

#endif