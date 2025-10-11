#include <stdio.h>
#include <string.h>
#include "admin.h"

int main() {
    Aluno aluno;

    cadastrar_aluno(&aluno);
    char novo_status[10];

    printf("Digite o novo status para o aluno (ativo/inativo): ");
    scanf(" %9s",novo_status);

    alterar_status_aluno(&aluno, novo_status);

    return 0;
}