#include <stdio.h>
#include "prof.h"
#include "menu.h"

int main() {
    Aluno aluno1;

    // Inicialização opcional
    aluno1.nota = 0.0;
    aluno1.faltas = 0;

    // Testando as funções
    atribuir_nota(&aluno1, 9.2);
    atribuir_faltas(&aluno1, 15);

    // Conferir o resultado
    printf("Nota do aluno: %.2f\n", aluno1.nota);
    printf("Faltas do aluno: %d\n", aluno1.faltas);

    return 0;
}
