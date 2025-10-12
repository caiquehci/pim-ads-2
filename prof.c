#include <stdio.h>
#include "prof.h"

void atribuir_nota(Aluno *aluno, float nota) {
    aluno->nota = nota;
    printf("Nota atribuída com sucesso.\n");
}

void atribuir_faltas(Aluno *aluno, int faltas) {
    if (faltas >=0 && faltas <= 100) {
        aluno->faltas = faltas;
        printf("Faltas atribuídas.\n");
    }   else {
            printf("Valor incorreto de faltas.\n");
    }
}