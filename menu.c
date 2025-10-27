#include <stdio.h>
#include <string.h>
#include "admin.h"
#include "menu.h"
#include "prof.h"


//função deprecada
//  void run_menu(const char *tipo_usuario) {
// chamando a lista de alunos mais atualizada, para que todos possam usar a melhor versão
//total_alunos = ler_alunos_de_arquivo(alunos, MAX_ALUNOS);

//DEBUG SECTION
//    for (int i = 0; i < total_alunos; i++)
//    {
//        printf("Aluno %s, notas: %.2f %.2f %.2f %.2f\n", alunos[i].nome, alunos[i].notas[0], alunos[i].notas[1], alunos[i].notas[2], alunos[i].notas[3]);
//    }

//    printf("DEBUG: total_alunos = %d\n", total_alunos);

//void gerar_relatorio_alunos(const char *filtro_turma, const char *filtro_status, int filtrar_pendencias);

//if (strcmp(tipo_usuario, "admin") == 0) {
//menu_admin();
//} else if (strcmp(tipo_usuario, "professor") == 0) {
//menu_professor();
//} else if (strcmp(tipo_usuario, "aluno") == 0) {
//menu_aluno();
//} else {
//printf("Tipo não cadastrado.\n");
//}
//}

void menu_admin() {
    int opcao;
    Aluno aluno;
    Professor prof;
    Turma nova_turma;

    do {
        printf("\nMenu [A] - Opções de alto nível de gerenciamento.\n");
        printf("Escolha uma das opções abaixo para continuar:\n");
        printf("1 - Gerenciamento de alunos\n");
        printf("2 - Gerenciamento de professores\n");
        printf("3 - Gerenciamento de turmas\n");
        printf("4 - Gerar cadastros\n");
        printf("0 - Sair\n");
        printf("Sua opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o '\n' deixado pelo scanf

        switch (opcao) {
        case 1: {
            int op_aluno;
            do {
                printf("\n :::Gerenciamento de Alunos:::\n");
                printf("1 - Cadastrar novo aluno\n");
                printf("2 - Alterar status do aluno\n");
                printf("3 - Buscar aluno\n");
                printf("4 - Boletins\n");
                printf("0 - Voltar ao menu anterior\n");
                printf("Escolha: ");
                scanf("%d", &op_aluno);
                getchar();

                switch (op_aluno) {
                case 1:
                    if (total_professores == 0) {
                        printf("Cadastre um professor antes de matricular alunos.\n");
                        continue;
                    }

                    if (num_turmas == 0) {
                        printf("Cadastre uma turma antes de matricular alunos.\n");
                        continue; //volta ao menu de alunos, sem ir pro logout
                    }

                    cadastrar_aluno(&aluno);

                    break;
                case 2:
                    alterar_status_aluno_por_id();
                    break;
                case 3:
                    visualizar_aluno_buscado();
                    break;
                case 4: {
                    int op_boletim;
                    do {
                        printf("\n ::: Gerenciamento de boletins :::\n");
                        printf("1 - Gerar boletim geral\n");
                        printf("2 - Gerar boletim individual\n");
                        printf("0 - Voltar ao menu anterior\n");
                        printf("Escolha: ");
                        scanf("%d", &op_boletim);
                        getchar();

                        switch (op_boletim) {
                            case 1:
                                gerar_boletim_todos();
                                break;
                            case 2:
                                gerar_boletim_individual();
                                break;
                            case 0:
                                printf("Voltando ao menu anterior...\n");
                                break;
                            default:
                                printf("Opção inválida.\n");
                        }
                    } while (op_boletim != 0);
                    break;
                }

                case 0:
                    printf("Voltando ao menu principal...\n");
                    break;
                default:
                    printf("Opção inválida.\n");
                }
            } while (op_aluno != 0);
            break;
        }

        case 2:
        {
            int op_prof;
            do
            {
                printf("\n :::Gerenciamento de Professores:::\n");
                printf("1 - Cadastrar novo professor\n");
                printf("2 - Listar professores\n");
                printf("3 - Excluir professor\n");
                printf("0 - Voltar ao menu anterior\n");
                printf("Escolha: ");
                scanf("%d", &op_prof);
                getchar();

                switch (op_prof)
                {
                case 1:
                    cadastrar_professor(&prof);
                    dados_usuario usuario_gerado = cadastrar_usuario_novo("professor", prof.nome);
                    printf("\nLogin de professor criado\n");
                    printf("Login: %s\n", usuario_gerado.login);
                    printf("Senha: %s\n", usuario_gerado.senha);
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

        case 3: {
            int op_turma;
            do {
                printf("\n :::Gerenciamento de Turmas:::\n");
                printf("1 - Cadastrar nova turma\n");
                printf("2 - Ver turmas cadastradas\n");
                printf("3 - Excluir turmas\n");
                printf("0 - Voltar ao menu anterior\n");
                printf("Escolha: ");
                scanf("%d", &op_turma);
                getchar();

                switch (op_turma) {
                case 1:
                    if (total_professores == 0) {
                        printf("Cadastre um professor antes de criar uma turma.\n");
                        break;
                    }

                    cadastrar_turma(&nova_turma);
                    adicionar_turma(&nova_turma); 
                    //lista_turmas[num_turmas] = nova_turma;
                    //num_turmas++;
                    salvar_turmas_em_arquivo();
                    break;
                case 2:
                    mostrar_turmas();
                    break;
                case 3:
                    excluir_turmas();
                    break;
                case 0:
                    printf("Voltando ao menu principal...\n");
                    break;
                default:
                    printf("Opção inválida.\n");
                }
            } while (op_turma != 0);
            break;
        }
        case 4: {
            int op_cadastro;
            do {
                printf("\n :::Gerenciamento de Cadastros:::\n");
                printf("\n Este e um cadastro manual para convidados\n");
                printf("1 - Criar login para novo professor\n");
                printf("2 - Criar login para novo aluno\n");
                printf("0 - Voltar ao menu anterior\n");
                printf("Escolha: ");
                scanf("%d", &op_cadastro);
                getchar();

                switch (op_cadastro) {
                    case 1: {
                        char nome_prof_temp[50];
                        printf("Nome do professor: ");
                        getchar();
                        fgets(nome_prof_temp, sizeof(nome_prof_temp), stdin);
                        nome_prof_temp[strcspn(nome_prof_temp, "\n")] = '\0';

                        cadastrar_usuario_novo("professor", nome_prof_temp);
                        printf("\nLogin de professor criado\n");
                        break;
                    }
                    case 2: {
                        char nome_aluno_temp[50];
                        printf("Nome do aluno: ");
                        getchar();
                        fgets(nome_aluno_temp, sizeof(nome_aluno_temp), stdin);
                        nome_aluno_temp[strcspn(nome_aluno_temp, "\n")] = '\0';

                        cadastrar_usuario_novo("aluno", nome_aluno_temp);
                        printf("\nLogin de aluno criado\n");
                        break;
                    }
                    case 0:
                        printf("Voltando ao menu principal...\n");
                        break;
                    default:
                        printf("Opção invalida.\n");
                }
            } while (op_cadastro != 0);
            break;
        }
        case 0:
            printf("Saindo...\n");
            break;

        default:
            printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);
}

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

