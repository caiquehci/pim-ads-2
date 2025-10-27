#ifndef ADMIN_H
#define ADMIN_H

#define MAX_ALUNOS 100
#define MAX_TURMAS 100
#define MAX_PROFESSORES 100

extern int num_turmas;
extern int total_alunos;
extern int proximo_id;

typedef struct
{
    char login[50]; // guarda login
    char senha[25]; // guarda senha
    char tipo[15];  // guarda a característica admin, professor ou aluno
    char nome[50];
    int id;
} dados_usuario;

//Estrutura de aluno
typedef struct { //Todas as infos são declaradas aqui, mesmo que não sejam utilizadas em todas as funções. Bom que centraliza tudo
    int id;
    char matricula[7];
    
    char nome[200];
    char endereco[400];
    char cpf[30];
    char data_nascimento[24];
    int ano;
    char turma[4]; //A, B, C, D ou E
    char status[20]; //"ativo" ou "inativo"
    int possui_pendencias; //1 para sim, 0 para não

    int faltas;
    float notas[4];
    char situacao[20];
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
    int ano;

//    char serie[2]; // Exemplo: "1", "2" ... "N" (número do ano)
    char letra; // Letra da turma, de 'A' a 'Z'
    char nome_professor[100]; // Nome do professor responsável
} Turma;

extern Turma lista_turmas[MAX_TURMAS];

dados_usuario cadastrar_usuario_novo(const char *tipo, const char *nome_aluno);
void gerar_login(char *nome, const char *tipo, char *login_gerado);

// funções de cadastro no sistema
////Turma :::::::::::::::::::::::::::::::::::::::::::::::
void cadastrar_turma(Turma *turma);
void adicionar_turma(Turma *turma);
void mostrar_turmas(void);
void excluir_turmas(void);
void salvar_turmas_em_arquivo(void);
int carregar_turmas_de_arquivo(void);

////Aluno :::::::::::::::::::::::::::::::::::::::::::::::
int ler_alunos_de_arquivo(Aluno alunos[], int max_alunos);
void cadastrar_aluno(Aluno *aluno);
void gerar_id_aluno(int *id);
void carregar_contador_id(void);
void salvar_contador_id(void);
void gerar_id_aluno(int *id);
void gerar_matricula_aluno(char matricula[]);
void mostrar_lista_alunos();
void alterar_status_aluno(Aluno *aluno, const char *novo_status);
void alterar_status_aluno_por_id(void);
int buscar_aluno_por_id(int id);
int ler_alunos_de_arquivo(Aluno alunos[], int max_alunos);
int buscar_aluno_nome_matr(const char *entrada, Aluno *aluno_encontrado);
void exibir_dados_aluno(const Aluno *aluno);
void visualizar_aluno_buscado();
void gerar_relatorio_alunos(const char *filtro_turma, const char *filtro_status, int filtrar_pendencias);

void arquivo_aluno(const Aluno *aluno, const char *nome_arquivo);
void salvar_alunos_em_arquivo(void);
void gerar_boletim_aluno(const Aluno *a);
void gerar_boletim_individual();
void gerar_boletim_todos();

////Professor :::::::::::::::::::::::::::::::::::::::::::::::
void cadastrar_professor(Professor *prof);
void adicionar_professor(Professor *prof);
void listar_professores(void);
void excluir_professor(void);
void arquivo_professor(const Professor *prof);
void salvar_professores_em_arquivo(void);
int carregar_professores_de_arquivo(void);


#endif