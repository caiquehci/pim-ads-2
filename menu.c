#include <stdio.h>
#include <string.h>
#include "admin.h"
#include "menu.h"
#include "prof.h"

void run_menu(const char *tipo_usuario) {
    // chamando a lista de alunos mais atualizada, para que todos possam usar a melhor versão
    total_alunos = ler_alunos_de_arquivo(alunos, MAX_ALUNOS);
    printf("DEBUG: total_alunos = %d\n", total_alunos);

    if (strcmp(tipo_usuario, "admin") == 0) {
        menu_admin();
    }   else if (strcmp(tipo_usuario, "professor") == 0) {
        menu_professor();
    }   else if (strcmp(tipo_usuario, "aluno") == 0) {
        menu_aluno();
    }   else {
        printf("Tipo não cadastrado.\n");
    }
}

void menu_admin() {
    int opcao;
    Aluno aluno;
    Professor prof;
    Turma nova_turma;
        do {
            printf("Menu [A] - Opções de alto nível de gerenciamento.\n");
            printf("Escolha uma das opções abaixo para continuar:\n");
            printf("1 - Cadastrar aluno\n");
            printf("2 - Alterar status do aluno\n");
            printf("3 - Gerenciar professores\n");
            printf("4 - Cadastrar turma\n");
            printf("5 - Ver turmas cadastradas\n");
            printf("6 - Excluir turmas\n");
            printf("7 - Buscar aluno\n");
            printf("0 - Sair\n");
            printf("Sua opção:\n");
            scanf("%d",&opcao);
            getchar(); //em teoria limpa o \n, mas sem isso não vai

            switch (opcao) {
                case 1:
                    cadastrar_aluno(&aluno);
                    break;
                case 2:
//                  alterar_status_aluno(&aluno, "inativo");
                    alterar_status_aluno_por_id();
                    break;
                case 3: {
                    int op_prof;
                    do {
                        printf("\n :::Gerenciamento de professores:::\n");
                        printf("1 - Cadastrar novo professor\n");
                        printf("2 - Listar professores\n");
                        printf("3 - Excluir professor\n");
                        printf("0 - Voltar ao menu anterior\n");
                        printf("Escolha: \n");
                        scanf("%d",&op_prof);
                        getchar();

                        switch (op_prof) {
                            case 1:
                                cadastrar_professor(&prof);
                                professores[total_professores++] = prof;
                                arquivo_professor(&prof);
                                break;
                            case 2:
                                listar_professores();
                                break;
                            case 3:
                                excluir_professor();
                                break;
                            case 0:
                                printf("Voltando ao menu principal...\n");
                                break;
                            default:
                                printf("Opção inválida.\n");
                        }
                    } while (op_prof != 0);
                    break;
                }
                case 4:
                    cadastrar_turma(&nova_turma);
                    adicionar_turma(&nova_turma);
                    break;
                case 5:
                    mostrar_turmas();
                    break;
                case 6:
                    excluir_turmas();
                    break;
                case 7:
                    visualizar_aluno_buscado();
                    break;
                case 0:
                    printf("Saindo...\n");
                    break;
                default:
                    printf("Opção inválida. Tente novamente\n.");
            }
        } while (opcao != 0);
}

//Depois eu volto aqui. Primeiro ele tem que poder escolher o aluno

void menu_professor() {
    int opcao; 

    do {
        printf("Menu [P] - Opções de gerência de alunos.\n");
        printf("Escolha uma das opções abaixo para continuar:\n");
        printf("1 - Atribuir notas a um aluno\n");
        printf("2 - Incluir faltas\n");
        printf("0 - Sair\n");
        printf("Sua opção:\n");
        scanf("%d",&opcao);
        getchar(); //em teoria limpa o \n, mas sem isso não vai

    switch(opcao){
        case 1:
            atribuir_notas(alunos, total_alunos);
            break;
        case 2:
            atribuir_faltas(alunos, total_alunos);
            break;
        case 0:
            printf("Voltando ao menu anterior...\n");
            break;
        default:
            printf("Opção inválida.\n");
        }
    } while(opcao !=0);
}

void menu_aluno() {
    printf("Bem-vindo, aluno(a)!\n");
}

