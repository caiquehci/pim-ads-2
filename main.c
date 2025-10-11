#include <stdio.h>
#include <string.h>
#include "admin.h" //aqui mora o cadastro de aluno

//Definindo coisas
typedef struct
{
    char login[50];     //guarda login
    char senha[25];     //guarda senha
    char tipo[15];      //guarda a característica admin, professor ou aluno
} dados_usuario;

//Função para descobrir o tipo de usuário analisando o login
void identificar_tipo(dados_usuario *u) { //u é um ponteiro que aponta para dados_usuario
    if (strstr(u->login, "@admin")) { //strstr é uma função de string.h que procura textos em textos
        strcpy(u->tipo, "admin"); //copia 'admin' e joga dentro do tipo de dados_usuario
    } else if (strstr(u->login, "@professor")) {
        strcpy(u->tipo, "professor");
    } else if (strstr(u->login, "@aluno")) {
        strcpy(u->tipo, "aluno");
    } else { 
        strcpy(u->tipo, "desconhecido");
    }
}

//ler a função login e senha
//o user vai digitar aqui as credentials e a função de cima vai checar o tipo de user
void login(dados_usuario *u) {
    printf("Login ex: nome @[tipo]: ");
    scanf("%49s", u->login);//não precisa de & porque o nome do vetor já é um ponteiro
                            //49 porque o último é \0

    printf("Senha: ");
    scanf("%24s", u->senha );

    identificar_tipo(u); //ao final de login, usamos a função de ver o tipo do user
}



int main(){
    dados_usuario usuarioAtual; //essa aqui guarda os dados da função
    login(&usuarioAtual); //chamei a função login que vai mexer na variável recém declarada

    //com o tipo definido em função, user detectado, vamos mostrar coisas diferentes 

    if (strcmp(usuarioAtual.tipo, "admin") == 0) {
        printf("Bem-vindo, Administrador!\n");
    }   else if (strcmp(usuarioAtual.tipo, "professor") == 0) {
        printf("Bem-vindo, Professor!\n");
    }   else if (strcmp(usuarioAtual.tipo, "aluno") == 0) {
        printf("Bem-vindo, Aluno!\n");
    }   else {
        printf("Login inválido. Tente novamente.\n");
    }
}