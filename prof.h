#ifndef PROF_H
#define PROF_H

#include "admin.h" //aqui temos as structs do aluno

void atribuir_notas(Aluno *alunos, int num_alunos);
void atribuir_faltas(Aluno *aluno, int num_alunos);
void calcular_situacao(Aluno *aluno);
void gerar_id_aula(int *id);
void salvar_aulas_em_arquivo(void);
void cadastrar_aulas(Aula *aula, int num_aulas);

#endif