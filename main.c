#include <stdio.h>
#include <string.h>
#include "admin.h" //aqui mora o cadastro de aluno
#include "menu.h" //aqui moram as opções de menu

//Definindo coisas
int proximo_id = 1; // ID do aluno


//Função para descobrir o tipo de usuário analisando o login
//vou deprecar isso, uma vez que o primeiro tipo de "login" era só pra testar rápido
//aquilo nem podia se chamar função de login
//void identificar_tipo(dados_usuario *u) { //u é um ponteiro que aponta para dados_usuario
//    if (strstr(u->login, "@admin")) { //strstr é uma função de string.h que procura textos em textos
//        strcpy(u->tipo, "admin"); //copia 'admin' e joga dentro do tipo de dados_usuario
//     } else if (strstr(u->login, "@professor")) {
//        strcpy(u->tipo, "professor");
//    } else if (strstr(u->login, "@aluno")) {
//        strcpy(u->tipo, "aluno");
//    } else { 
//        strcpy(u->tipo, "desconhecido");
//    }
//}

//Função para criar admin padrão se não existir
void criar_admin_padrao()
{
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "r");

    //Se arquivo não existe ou está vazio, cria admin padrão
    if (arquivo == NULL)
    {
        arquivo = fopen(ARQUIVO_USUARIOS, "w");
        if (arquivo != NULL)
        {
            fprintf(arquivo, "admin@admin|admin123|admin|Administrador|0\n");
            fclose(arquivo);
            printf("Admin padrão criado!\n");
            printf("Login: admin@admin\n");
            printf("Senha: admin123\n\n");
        }
    }
    else
    {
        fclose(arquivo);
    }
}

//Função para autenticar
int autenticar_usuario(dados_usuario *u)
{
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo de usuários!\n");
        return 0;
    }

    char linha[200];
    char login_arq[50], senha_arq[25], tipo_arq[15], nome_arq[50];
    int id_arq;

    while (fgets(linha, sizeof(linha), arquivo))
    {
        //Formato: login|senha|tipo|nome|id
        sscanf(linha, "%[^|]|%[^|]|%[^|]|%[^|]|%d", login_arq, senha_arq, tipo_arq, nome_arq, &id_arq);

        if (strcmp(u->login, login_arq) == 0 && strcmp(u->senha, senha_arq) == 0)
        {
            strcpy(u->tipo, tipo_arq);
            strcpy(u->nome, nome_arq);
            u->id = id_arq;
            fclose(arquivo);
            return 1; //Autenticação bem-sucedida
        }
    }

    fclose(arquivo);
    return 0; //Usuário não encontrado ou senha incorreta
}

//Função para gerar o login
void gerar_login(char *nome, const char *tipo, char *login_gerado)
{
    char nome_limpo[50];
    strcpy(nome_limpo, nome);

    //pega primeiro nome (até o espaço)
    char *espaco = strchr(nome_limpo, ' ');
    if (espaco != NULL) {
        *espaco = '\0';
    }

    //converte para minúsculo
    for (int i = 0; nome_limpo[i]; i++) {
        if (nome_limpo[i] >= 'A' && nome_limpo[i] <= 'Z') {
            nome_limpo[i] = nome_limpo[i] + 32;
        }
    }

    sprintf(login_gerado, "%s@%s", nome_limpo, tipo);
}

//Função para gerar a senha (ID+3 primeiras letras do nome)
void gerar_senha(int id, char *nome, char *senha_gerada) {
    char inicial[3] = {0};
    inicial[0] = nome[0];
    inicial[1] = nome[1];

    sprintf(senha_gerada, "%c%c%04d", inicial[0], inicial[1], id);
}

//Novo user
dados_usuario cadastrar_usuario_novo(const char *tipo, const char *nome_aluno) {
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo!\n");
        dados_usuario vazio = {"", "", "", "", 0};
        return vazio;
    }

    dados_usuario novo;
    char login_gerado[50];
    char senha_gerada[25];

    strcpy(novo.nome, nome_aluno);
    strcpy(novo.tipo, tipo);
    novo.id = proximo_id++;

    gerar_login(novo.nome, tipo, login_gerado);
    gerar_senha(novo.id, novo.nome, senha_gerada);

    strcpy(novo.login, login_gerado);
    strcpy(novo.senha, senha_gerada);

    //salvar no arquivo
    fprintf(arquivo, "%s|%s|%s|%s|%d\n", novo.login, novo.senha, novo.tipo, novo.nome, novo.id);
    fclose(arquivo);

    printf("\nUsuário cadastrado com sucesso!\n");
    printf("::::::::\n");
    printf("Nome: %s\n", novo.nome);
    printf("Login: %s\n", novo.login);
    printf("Senha: %s\n", novo.senha);
    printf("Tipo: %s\n", novo.tipo);
    printf("ID: %d\n", novo.id);
    printf("::::::::\n");

    return novo;
}

int fazer_login(dados_usuario *u)
{
    printf("\n=== LOGIN - PIM ADS 2 ===\n");
    printf("Login: ");
    scanf("%49s", u->login);

    printf("Senha: ");
    scanf("%24s", u->senha);

    if (autenticar_usuario(u))
    {
//        printf("Bem-vindo(a), %s (%s)\n\n", u->nome, u->tipo);
        return 1;
    }
    else
    {
        printf("\n✗ Login ou senha incorretos!\n\n");
        return 0;
    }
}

// ler a função login e senha
// o user vai digitar aqui as credentials e a função de cima vai checar o tipo de user
// void login(dados_usuario *u) {
// printf("Login ex: nome @[tipo]: ");
// scanf("%49s", u->login);//não precisa de & porque o nome do vetor já é um ponteiro
// 49 porque o último é \0
// printf("Senha: ");
// scanf("%24s", u->senha );
// }
// identificar_tipo(u); //ao final de login, usamos a função de ver o tipo do user

int main(){
    carregar_professores_de_arquivo();
    total_professores = carregar_professores_de_arquivo();
    num_turmas = carregar_turmas_de_arquivo();

    carregar_contador_id(); // para inicializar o proximo_id do arquivo
    total_alunos = ler_alunos_de_arquivo(alunos, MAX_ALUNOS);

    dados_usuario usuarioAtual; //essa aqui guarda os dados da função
    int sair_sistema = 0;
    int login_ok = 0;
    int sair = 0;

    criar_admin_padrao();


    while (!sair_sistema) {
        // essa função de login está deprecada              login(&usuarioAtual); //chamei a função login que vai mexer na variável recém declarada

        //com o tipo definido em função, user detectado, vamos mostrar coisas diferentes 

        //não comparamos a string mais              if (strcmp(usuarioAtual.tipo, "admin") == 0) {
        //                  printf("Bem-vindo, Administrador!\n");
        //              }   else if (strcmp(usuarioAtual.tipo, "professor") == 0) {
        //                      printf("Bem-vindo, Professor!\n");
        //              }   else if (strcmp(usuarioAtual.tipo, "aluno") == 0) {
        //                      printf("Bem-vindo, Aluno!\n");
        //              }   else {
        //                      printf("Login inválido. Tente novamente.\n");
        //                      continue;
        //              }
        
        //run menu desse jeito considerava a lógica de comparação das strings pra acesso de menus          run_menu(usuarioAtual.tipo); //como tá na main, vamos sem ponteiro

        while (!login_ok)
        {
            login_ok = fazer_login(&usuarioAtual);
            if (!login_ok) {
                int tentar;
                printf("Tentar novamente? (1 - Sim, 0 - Sair): ");
                scanf("%d", &tentar);
                getchar();

                if (!tentar) return 0;
            }
        }

        if (strcmp(usuarioAtual.tipo, "admin") == 0) {
            printf("Bem-vindo(a), Administrador %s!\n", usuarioAtual.nome);
            printf("Total de professores carregados: %d\n", total_professores);

            menu_admin();
        } else if (strcmp(usuarioAtual.tipo, "professor") == 0) {
            printf("Bem-vindo(a), Professor %s!\n", usuarioAtual.nome);
            menu_professor(usuarioAtual.nome);
        } else if (strcmp(usuarioAtual.tipo, "aluno") == 0) {
            printf("Bem-vindo(a), Aluno %s!\n", usuarioAtual.nome);
            menu_aluno(usuarioAtual.nome);
        } else {
            printf("Tipo de usuario invalido.\n");
        }

        //pós-logout
        printf("\nDigite:\n1 - Logout (trocar usuário)\n0 - Sair\nOpção: ");
        int opcao;
        scanf("%d", &opcao);
        getchar();

        if (opcao == 1) {
            printf("Saindo...\n");
            login_ok = 0;
        } else {
            printf("Encerrando o sistena. \n");
            sair_sistema = 1;
        }
    }

    return 0;

}