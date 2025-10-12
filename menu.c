#include <stdio.h>
#include <string.h>
#include "admin.h"
#include "menu.h"

void run_menu(const char *tipo_usuario) {
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
        do {
            printf("Menu [A] - Opções de alto nível de gerenciamento.\n");
            printf("Escolha uma das opções abaixo para continuar:\n");
            printf("1 - Cadastrar aluno\n");
            printf("2 - Alterar status do aluno\n");
            printf("3 - Cadastrar professor\n");
            printf("0 - Sair\n");
            printf("Sua opção:\n");
            scanf("%d",&opcao);
            getchar(); //em teoria limpa o \n, mas sem isso não vai

            switch (opcao) {
                case 1:
                    cadastrar_aluno(&aluno);
                    break;
                case 2:
                    alterar_status_aluno(&aluno, "inativo");
                    break;
                case 3:
                    cadastrar_professor(&prof);
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
    printf("Menu [P] - Opções de gerência de alunos.\n");
    printf("Escolha uma das opções abaixo para continuar:\n");
    printf("1 - Adicionar nota\n");
    printf("2 - Incluir faltas\n");
    printf("3 - Cadastrar professor\n");
    printf("0 - Sair\n");
    printf("Sua opção:\n");
    scanf("%d",&opcao);
    getchar(); //em teoria limpa o \n, mas sem isso não vai
}

void menu_aluno() {
    printf("Bem-vindo, aluno(a)!\n");
}

