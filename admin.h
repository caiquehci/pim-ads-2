#ifndef ADMIN_H
#define ADMIN_H

#define MAX_ALUNOS 100
#define MAX_TURMAS 100
#define MAX_PROFESSORES 100

extern int num_turmas;
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
    float notas[4];  
} Aluno;

extern Aluno alunos[MAX_ALUNOS];

//Estrutura de professor
typedef struct {
    char nome[100];
    char cpf[15];
    char disciplina[50];
    char email[100];
} Professor;

extern Professor professores[MAX_PROFESSORES];
extern int total_professores; 

//Estrutura de turma
typedef struct {
    char serie[2]; // Exemplo: "1", "2" ... "N" (número do ano)
    char letra; // Letra da turma, de 'A' a 'Z'
    char nome_professor[100]; // Nome do professor responsável
} Turma;

extern Turma lista_turmas[MAX_TURMAS];

//funções de cadastro no sistema
//Turma
void cadastrar_turma(Turma *turma);
void adicionar_turma(Turma *turma);
void mostrar_turmas(void);
void excluir_turmas(void);

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
void adicionar_professor(Professor *prof);
void listar_professores(void);
void excluir_professor(void);
void arquivo_professor(const Professor *prof);
void salvar_professores_em_arquivo(void);
int carregar_professores_de_arquivo(void);


#endif