#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ALUNOS 50

typedef struct {
    int id;
    char matricula[20];
    char nome[100];
    char endereco[100];
    char cpf[20];
    char data_nascimento[20];
    char turma[10];
    char status[20];
    int possui_pendencias;
    int faltas;
    float notas[4];
} Aluno;

int ler_alunos_de_arquivo(Aluno alunos[], int max_alunos) {
    FILE *fp = fopen("alunos.txt", "r");
    if (!fp) {
        perror("Erro ao abrir arquivo alunos.txt");
        return 0;
    }

    char linha[256];
    int i = 0;

    while (i < max_alunos && fgets(linha, sizeof(linha), fp) != NULL) {
        printf("[linha lida]: %s", linha);

        // Se a linha inicia com "ID:" então registra novo aluno
        if (strstr(linha, "ID:") == linha) {
            memset(&alunos[i], 0, sizeof(Aluno));
            sscanf(linha, "ID: %d", &alunos[i].id);

            for (int campo = 0; campo < 10; campo++) {
                if (fgets(linha, sizeof(linha), fp) == NULL) break;
                printf("[linha lida]: %s", linha);

                char *p = strchr(linha, ':');
                if (p == NULL) continue;
                p++;
                while (*p == ' ') p++;

                switch (campo) {
                    case 0:
                        sscanf(p, "%[^\n]", alunos[i].matricula);
                        break;
                    case 1:
                        sscanf(p, "%[^\n]", alunos[i].nome);
                        break;
                    case 2:
                        sscanf(p, "%[^\n]", alunos[i].endereco);
                        break;
                    case 3:
                        sscanf(p, "%[^\n]", alunos[i].cpf);
                        break;
                    case 4:
                        sscanf(p, "%[^\n]", alunos[i].data_nascimento);
                        break;
                    case 5:
                        sscanf(p, "%[^\n]", alunos[i].turma);
                        break;
                    case 6:
                        sscanf(p, "%[^\n]", alunos[i].status);
                        break;
                    case 7:
                        {
                            char pend[4];
                            sscanf(p, "%s", pend);
                            alunos[i].possui_pendencias = (strcmp(pend, "Sim") == 0);
                        }
                        break;
                    case 8:
                        sscanf(p, "%d", &alunos[i].faltas);
                        break;
                    case 9:
                        sscanf(p, "Notas: %f %f %f %f",
                               &alunos[i].notas[0], &alunos[i].notas[1], &alunos[i].notas[2], &alunos[i].notas[3]);
                        break;
                }
            }
            // pula linha de separador
            fgets(linha, sizeof(linha), fp);
            i++;
        }
    }
    fclose(fp);
    return i;
}

void mostrar_alunos(Aluno alunos[], int total) {
    printf("\nTotal de alunos lidos: %d\n", total);
    for (int i = 0; i < total; i++) {
        printf("%d - %s (Turma %s)\n", i + 1, alunos[i].nome, alunos[i].turma);
    }
}

int main() {
    Aluno alunos[MAX_ALUNOS];
    int total_alunos = ler_alunos_de_arquivo(alunos, MAX_ALUNOS);

    mostrar_alunos(alunos, total_alunos);

    printf("\nMenu simples de teste:\n");
    printf("1 - Mostrar lista de alunos\n");
    printf("0 - Sair\n");

    int opcao;
    do {
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                mostrar_alunos(alunos, total_alunos);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}
