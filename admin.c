#include <stdio.h>
#include <string.h>
#include "admin.h"

extern int proximo_id;

Aluno alunos[MAX_ALUNOS];
int total_alunos = 0;

Turma lista_turmas[MAX_TURMAS];
int num_turmas = 0;

Professor professores[MAX_PROFESSORES];
int total_professores = 0;

//Cadastrar turma
void cadastrar_turma(Turma *turma){
    printf("Cadastre uma nova turma\n");

    printf("Série (número do ano): ");
    scanf(" %[^\n]",turma->serie);

    printf("Letra da turma [A-Z]: ");
    scanf(" %c",&turma->letra);

    printf("Professor responsável: ")/
    scanf(" %[^\n]",turma->nome_professor);

    printf("\nTurma cadastrada:\n");
    printf("Série: %s\n",turma->serie);
    printf("Letra: %c\n",turma->letra);
    printf("Professor responsável: %s\n",turma->nome_professor);
}

//Adicionar uma nova turma
void adicionar_turma(Turma *turma){
    if (num_turmas < MAX_TURMAS){
        lista_turmas[num_turmas] = *turma;
        num_turmas++;
    } else {
        printf("Não podemos acrescentar mais turmas.\n");
    }
}

//Ver turmas
void mostrar_turmas(void){
    if (num_turmas == 0){
        printf("Sem turmas até agora.\n");
        printf("Vamos cadastrar a primeira\n.");

        Turma nova_turma;
        cadastrar_turma(&nova_turma);
        adicionar_turma(&nova_turma);
    } else {
        printf("Lista de turmas:\n");
        for (int i = 0; i<num_turmas; i++) {
            printf("%d - %sª Série, Turma: %c. Professor responsável: %s\n", i+1, lista_turmas[i].serie, lista_turmas[i].letra,lista_turmas[i].nome_professor);
        }
    }
}

//Excluir turmas
void excluir_turmas(void){
    if (num_turmas == 0){
        printf("Nenhuma turma para excluir.\n");
        return;
    }

    mostrar_turmas();

    int escolha;
    printf("Digite o número da turma que deseja excluir (0 para desistir): ");
    scanf("%d",&escolha);
    getchar();//novamente a lenda aparece e eu n entendo a motivação

    if (escolha == 0){
        printf("Exclusão cancelada.\n");
        return;
    }

    int indice = escolha - 1;
    if (indice <0 || indice >= num_turmas){
        printf("Número inválido. Não excluímos nada.\n");
        return;
    }

    for (int i = indice; i < num_turmas - 1; i++){
        lista_turmas[i] = lista_turmas[i+1];
    }

    num_turmas--;

    printf("Turma excluída.\n");

    mostrar_turmas();
}



//Ler arquivos de aluno
int ler_alunos_de_arquivo(Aluno alunos[], int max_alunos) {
    FILE *fp = fopen("alunos.txt", "r");
    if (!fp) {
        perror("Erro ao abrir arquivo alunos.txt");
        return 0;
    }

    int i = 0;
    char linha[256];
    while (i < max_alunos && !feof(fp)) {
        if (fgets(linha, sizeof(linha), fp) == NULL) break;

        if (sscanf(linha, "ID: %d", &alunos[i].id) == 1) {
            // Leia os demais campos, linha a linha

            fgets(linha, sizeof(linha), fp);
            sscanf(linha, "Nome: %[^\n]", alunos[i].nome);

            fgets(linha, sizeof(linha), fp);
            sscanf(linha, "Endereco: %[^\n]", alunos[i].endereco);

            fgets(linha, sizeof(linha), fp);
            sscanf(linha, "CPF: %[^\n]", alunos[i].cpf);

            fgets(linha, sizeof(linha), fp);
            sscanf(linha, "Data de nascimento: %[^\n]", alunos[i].data_nascimento);

            fgets(linha, sizeof(linha), fp);
            sscanf(linha, "Turma: %s", alunos[i].turma);

            fgets(linha, sizeof(linha), fp);
            sscanf(linha, "Status: %s", alunos[i].status);

            fgets(linha, sizeof(linha), fp);
            char pend[4];
            sscanf(linha, "Possui pendencias: %s", pend);
            alunos[i].possui_pendencias = (strcmp(pend, "Sim") == 0);

            fgets(linha, sizeof(linha), fp);
            sscanf(linha, "Faltas: %d", &alunos[i].faltas);

            fgets(linha, sizeof(linha), fp);
            sscanf(linha, "Notas: %f %f %f %f ", &alunos[i].notas[0], &alunos[i].notas[1], &alunos[i].notas[2], &alunos[i].notas[3]);

            // Pular o separador
            fgets(linha, sizeof(linha), fp);

            i++;
        }
    }

    fclose(fp);
    return i; // quantidade de alunos lidos
}

//Cadastrar aluno, clássico
void cadastrar_aluno(Aluno *aluno) {
    aluno->id = proximo_id++;

    strcpy(aluno->status, "ativo"); //Começa como ativo
    aluno->possui_pendencias = 0; //Começa sem pendências

    aluno->faltas = 0;//tava dando -8927392873 faltas no .txt
    for (int i = 0; i < 4; i++){
        aluno->notas[i] = 0.0f;//mesma coisa ↑
    } //antes era uma nota com valor fixo pra não ser lixo, agora são

    printf("Cadastre um novo aluno\n");

    printf("Nome: ");
    scanf(" %[^\n]",aluno->nome); //ler com espaço até ENTER

    printf("Endereço: ");
    scanf(" %[^\n]",aluno->endereco);

    printf("CPF: ");
    scanf(" %[^\n]",aluno->cpf);
    
    printf("Data de nascimento (DD/MM/AAAA): ");
    scanf(" %[^\n]",aluno->data_nascimento);

    printf("Turma [A/B/C/D/E]: ");
    scanf(" %[^\n]",aluno->turma);

    printf("\nAluno cadastrado:\n");
    printf("Nome: %s\nEndereço: %s\nCPF: %s\nData de nascimento: %s\nTurma: %s\n",aluno->nome,aluno->endereco,aluno->cpf,aluno->data_nascimento,aluno->turma);

    printf("Status: %s\nPossui pendências: ", aluno->status);
        if (aluno->possui_pendencias) 
            printf("Sim\n");
        else
    printf("Não\n");

    char nome_arquivo[50];
//    sprintf(nome_arquivo, "aluno_%d.txt", aluno->id);
//isso era pra fazer um arquivo por aluno
    arquivo_aluno(aluno, "alunos.txt");
}



//Cadastrar professor, clássico
void cadastrar_professor(Professor *prof) {
    printf("Cadastro de professor: \n");

    printf("Nome: ");
    scanf(" %[^\n]",prof->nome);
    
    printf("CPF: ");
    scanf(" %[^\n]",prof->cpf);
    
    printf("Disciplina: ");
    scanf(" %[^\n]",prof->disciplina);
    
    printf("E-mail: ");
    scanf(" %[^\n]",prof->email);

    printf("Professor cadastrado:\n");
    printf("Nome: %s\nCPF: %s\nDisciplina: %s\nE-mail: %s\n",prof->nome, prof->cpf, prof->disciplina, prof->email);
}

//Adicionar professor
void adicionar_professor(Professor *prof) {
    if (total_professores >= MAX_PROFESSORES) {
        printf("Limite máximo de professores.\n");
        return;
    }

    professores[total_professores++] = *prof;
    salvar_alunos_em_arquivo();
}

//Listar professores
void listar_professores(void) {
    if (total_professores == 0) {
        printf("Nenhum professor cadastrado.\n");
        return;
    }

    printf("\nLista de professores cadastrados:\n");
    for (int i = 0; i < total_professores; i++) {
        printf("%d - %s | %s | %s | %s \n", i+1, professores[i].nome, professores[i].cpf, professores[i].disciplina, professores[i].email);
    }
}

//Excluir professor
void excluir_professor(void) {
    if (total_professores == 0) {
        printf("Nenhum professor cadastrado para excluir.\n");
        return;
    }

    listar_professores();
    int escolha;
    printf("Digite o número do professor que deseja excluir (0 para sair): ");
    scanf("%d", &escolha);

    if (escolha <= 0 || escolha > total_professores) {
        printf("Saindo...\n");
        return;
    }

    int indice = escolha - 1;
    for (int i = indice; i < total_professores -1; i++) {
        professores[i] = professores[i+1];
    }

    total_professores--;
    salvar_professores_em_arquivo();
    printf("Professor removido com sucesso.\n");
}

//Criar arquivo do professor A
void arquivo_professor(const Professor *prof) {
    FILE *fp = fopen("professores.txt", "a");
    if (!fp) {
        printf("Erro ao abrir arquivo para adicionar professor.\n");
        return;
    }

    fprintf(fp, "Nome: %s\n", prof->nome);
    fprintf(fp, "CPF: %s\n", prof->cpf);
    fprintf(fp, "Disciplina %s\n", prof->disciplina);
    fprintf(fp, "Email %s\n", prof->email);
    fprintf(fp, "-----------\n");

    fclose(fp);
    printf("Professor %s adicionado ao arquivo.\n", prof->nome);
}

//Salvar e ler de arquivo W
void salvar_professores_em_arquivo(void) {
    FILE *fp = fopen("Professores.txt", "w");
    if (!fp) {
        perror("Erro ao abrir arquivo para salvar professores");
        return;
    }

    for (int i = 0; i < total_professores; i++) {
        fprintf(fp, "Nome: %s\n", professores[i].nome);
        fprintf(fp, "CPF: %s\n", professores[i].cpf);
        fprintf(fp, "Disciplina: %s\n", professores[i].disciplina);
        fprintf(fp, "Email: %s\n", professores[i].email);
        fprintf(fp, "--------------\n");
    }

    fclose(fp);
    printf("Lista de professores atualizada no arquivo.\n");
}

//Carregar todos os professores do arquivo
int carregar_professores_de_arquivo(void);

//Complexos
//Buscar aluno por id
int buscar_aluno_por_id(int id) {
    for (int i = 0; i < total_alunos; i++) {
        if (alunos[i].id == id) {
            return i;
        }
    }
    return -1;
}

//Buscar aluno e alterar status pelo seu id
void alterar_status_aluno_por_id() {
    int id;
    char novo_status[10];

    printf("Digite o ID do aluno: ");
    scanf("%d",&id);
    getchar();

    int idx = buscar_aluno_por_id(id);

    if (idx == -1) {
        printf("Aluno não encontrado.\n");
        return;
    }

    printf("Novos status: ");
    scanf("%9s", novo_status);
    getchar();

    alterar_status_aluno(&alunos[idx], novo_status);

    salvar_alunos_em_arquivo();
}

//Alterar status do aluno
void alterar_status_aluno(Aluno *aluno, const char *novo_status) {
    //pelo visto tem que ser ponteiro pq senão recebe cópia e não o valor vigente
    if (aluno->possui_pendencias) {
        printf("Não é possível alterar status. Aluno possui pendências.\n");
        return; //Sair sem fazer nada
    }
    strncpy(aluno->status, novo_status, sizeof(aluno->status) -1);
    aluno->status[sizeof(aluno->status) -1] = '\0';

    printf("Status do aluno alterado para: %s\n",aluno->status);
}

//Criar arquivo do aluno A
void arquivo_aluno(const Aluno *aluno, const char *alunosss) {
    FILE *fp = fopen(alunosss, "a");// de w pra a, de write para append
    if (fp == NULL ) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fprintf(fp, "ID: %d\n", aluno->id);
    fprintf(fp, "Nome: %s\n", aluno->nome);
    fprintf(fp, "Endereco: %s\n", aluno->endereco);
    fprintf(fp, "CPF: %s\n", aluno->cpf);
    fprintf(fp, "Data de nascimento: %s\n", aluno->data_nascimento);
    fprintf(fp, "Turma: %s\n", aluno->turma);
    fprintf(fp, "Status: %s\n", aluno->status);
    fprintf(fp, "Possui pendencias: %s\n", aluno->possui_pendencias ? "Sim" : "Nao");
    fprintf(fp, "Faltas: %d\n", aluno->faltas);
    fprintf(fp, "Notas: %.2f %.2f %.2f %.2f\n", aluno->notas[0],aluno->notas[1],aluno->notas[2],aluno->notas[3]);

    fclose(fp);

    printf("Dados do aluno salvos em '%s'.\n", alunosss);
}

//Salvar status novos no arquivo W
void salvar_alunos_em_arquivo(void) {
    FILE *fp = fopen("alunos.txt", "w");
    if (!fp) {
        perror("Erro ao abrir arquivo para salvar");
        return;
    }
    for (int i = 0; i < total_alunos; i++) {
        fprintf(fp, "ID: %d\n", alunos[i].id);
        fprintf(fp, "Nome: %s\n", alunos[i].nome);
        fprintf(fp, "Endereco: %s\n", alunos[i].endereco);
        fprintf(fp, "CPF: %s\n", alunos[i].cpf);
        fprintf(fp, "Data de nascimento: %s\n", alunos[i].data_nascimento);
        fprintf(fp, "Turma: %s\n", alunos[i].turma);
        fprintf(fp, "Status: %s\n", alunos[i].status);
        fprintf(fp, "Possui pendencias: %s\n", alunos[i].possui_pendencias ? "Sim" : "Nao");
        fprintf(fp, "Faltas: %d\n", alunos[i].faltas);
        fprintf(fp, "Notas: %.2f %.2f %.2f %.2f\n", alunos[i].notas[0], alunos[i].notas[1], alunos[i].notas[2], alunos[i].notas[3]);
        fprintf(fp, "------------------------------\n");
    }
    fclose(fp);
}