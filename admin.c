#include <stdio.h>
#include <string.h>
#include "admin.h"
#include "prof.h"

extern int proximo_id;

Aluno alunos[MAX_ALUNOS];
int total_alunos = 0;

Turma lista_turmas[MAX_TURMAS];
int num_turmas = 0;

Professor professores[MAX_PROFESSORES];
int total_professores = 0;

Aula aulas[MAX_AULAS];
int total_aulas = 0;

// Turmas :::::::::::::::::::::::::::::::::::::::::::::::
//
// Cadastrar turma
void cadastrar_turma(Turma *turma){
    printf("Cadastre uma nova turma\n");

    printf("Ano (número do ano): ");
    scanf(" %d",&turma->ano);

    printf("Letra da turma [A-E]: ");
    scanf(" %c",&turma->letra);

    printf("\nProfessores disponiveis: \n");
    for (int i = 0; i < total_professores; i++) {
        printf("%d - %s | %s\n", i + 1, professores[i].nome, professores[i].disciplina);
    }

    int escolha_prof;
    printf("Escolha o professor responsavel pelo numero: \n");
    scanf("%d", &escolha_prof);
    getchar();

    if (escolha_prof < 1 || escolha_prof > total_professores) {
        printf("Opcao invalida. Favor tentar novamente\n");
        return;
    }

    strcpy(turma->nome_professor, professores[escolha_prof - 1].nome);

//antiga inserção manual
//printf("Professor responsável: ");
//scanf(" %[^\n]",turma->nome_professor);

    printf("\nTurma cadastrada:\n");
    printf("Ano: %d\n",turma->ano);
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
        printf("Escolha a opção 'cadastrar nova turma' no menu anterior.\n");
        return;
    }
    
    printf("Lista de turmas:\n");
    for (int i = 0; i < num_turmas; i++) {
        printf("%d - %d Ano %c. Professor responsavel: %s\n", i + 1, lista_turmas[i].ano, lista_turmas[i].letra, lista_turmas[i].nome_professor);
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
    salvar_turmas_em_arquivo();
    printf("Turma excluída.\n");

    mostrar_turmas();
}

//fazer um arquivo pra não precisar ficar refazendo turma toda vez que executa o programa
void salvar_turmas_em_arquivo(void) {
    FILE *fp = fopen("turmas.txt", "w");
    if (!fp) {
        perror("Erro ao abrir arquivo para salvar turmas");
        return;
        }   for (int i = 0; i < num_turmas; i++) {
                fprintf(fp, "Ano: %d\n", lista_turmas[i].ano);
                fprintf(fp, "Letra: %c\n", lista_turmas[i].letra);
                fprintf(fp, "Professor: %s\n", lista_turmas[i].nome_professor);
                fprintf(fp, "--------------\n");
            }

        fclose(fp);
}

void salvar_aulas_em_arquivo(void) {
    FILE *fp = fopen("aulas.txt", "a");
    if (!fp) {
        perror("Erro ao abrir arquivo 'aulas.txt' para salvar");
        return;
    }

    // grava só a última aula (índice total_aulas - 1)
    int i = total_aulas - 1;
    fprintf(fp, "ID: %d\n", aulas[i].id);
    fprintf(fp, "Ano: %d\n", aulas[i].ano);
    fprintf(fp, "Matéria: %s\n", aulas[i].materia);
    fprintf(fp, "Turma: %s\n", aulas[i].turma);
    fprintf(fp, "Corpo: %s\n", aulas[i].corpo);
    fprintf(fp, "--------------------------\n");

    fclose(fp);
    printf("Aula adicionada ao arquivo 'aulas.txt'.\n");
}

//pra usar no começo da main
int carregar_turmas_de_arquivo(void) {
    num_turmas = 0;

    FILE *fp = fopen("turmas.txt", "r");
    if (!fp) return 0;

    num_turmas = 0;
    char linha[200];
    
    while (fgets(linha, sizeof(linha), fp) && num_turmas < MAX_TURMAS) {
        Turma *t = &lista_turmas[num_turmas];
        if (sscanf(linha, "Ano: %d", &t->ano) == 1) {
            fgets(linha, sizeof(linha), fp);
            sscanf(linha, "Letra: %c", &t->letra);
            fgets(linha, sizeof(linha), fp);
            sscanf(linha, "Professor: %[^\n]", t->nome_professor);
            fgets(linha, sizeof(linha), fp); //linha ----------
            
            num_turmas++;
        }
    }

    fclose(fp);
    return num_turmas;
}

//

// Alunos :::::::::::::::::::::::::::::::::::::::::::::::
//
// Ler arquivos de aluno
int ler_alunos_de_arquivo(Aluno alunos[], int max_alunos) {
    FILE *fp = fopen("alunos.txt", "r");
    if (!fp) {
        perror("Erro ao abrir arquivo alunos.txt");
        return 0;
    }

    char linha[1024];
    int i = 0;

    while (i < max_alunos && fgets(linha, sizeof(linha), fp) != NULL) {
        // Se a linha inicia com "ID:", então começa um registro
        if (strstr(linha, "ID:") == linha) {
            // Limpa o struct do aluno
            memset(&alunos[i], 0, sizeof(Aluno));

            // ID
            sscanf(linha, "ID: %d", &alunos[i].id);

            int situacao_lida = 0;

            // Leitura das próximas linhas
            // Cada uma contendo o campo após ":"
            for (int campo = 0; campo < 10; campo++) {
                if (fgets(linha, sizeof(linha), fp) == NULL)
                    break;
                char *p = strchr(linha, ':');
                if (p == NULL)
                    continue;
                p++; // avança após o ":"
                while (*p == ' ')
                    p++; // pula espaços

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
                        sscanf(p, "%d", &alunos[i].ano);
                        break;
                    case 6:
                        sscanf(p, "%[^\n]", alunos[i].turma);
                        break;
                    case 7:
                        sscanf(p, "%[^\n]", alunos[i].status);
                        break;
                    case 8: {
                        char pend[4];
                        sscanf(p, "%s", pend);
                        alunos[i].possui_pendencias = (strcmp(pend, "Sim") == 0);
                    } break;
                    case 9:
                        sscanf(p, "%d", &alunos[i].faltas);
                        break;
                    case 10:
                        sscanf(p, "%f %f %f %f", &alunos[i].notas[0], &alunos[i].notas[1], &alunos[i].notas[2], &alunos[i].notas[3]);
                        break;
                    case 11:
                        sscanf(p, "%[^\n]", alunos[i].situacao);
                        situacao_lida = 1;
                        break;
                }
            }

            if (!situacao_lida)
                alunos[i].situacao[0] = '\0';

            i++;
            // Leitura do separador
            fgets(linha, sizeof(linha), fp);
        }
    }

    fclose(fp);
    return i;
}

//Cadastrar aluno, clássico
void cadastrar_aluno(Aluno *aluno) {
    
    gerar_matricula_aluno(aluno->matricula); // recém criado pra poupar serviço no menu
    gerar_id_aluno(&aluno->id);

    strcpy(aluno->status, "ativo"); //Começa como ativo
    aluno->possui_pendencias = 0; //Começa sem pendências

    aluno->faltas = 0;//tava dando -8927392873 faltas no .txt
    for (int i = 0; i < 4; i++){
        aluno->notas[i] = 0.0f;//mesma coisa ↑
    } //antes era uma nota com valor fixo pra não ser lixo, agora são 0

    printf("Cadastre um novo aluno\n");

    printf("Nome: ");
    scanf(" %[^\n]", aluno->nome); //ler com espaço até ENTER
    
    printf("Endereco: ");
    scanf(" %[^\n]", aluno->endereco);

    printf("CPF: ");
    scanf(" %[^\n]", aluno->cpf);
    
    printf("Data de nascimento (DD/MM/AAAA): ");
    scanf(" %[^\n]", aluno->data_nascimento);

    printf("\nSelecione uma turma:\n");
    for (int i = 0; i < num_turmas; i++) {
        printf("%d - %d Ano %c (Professor: %s)\n",
            i + 1,
            lista_turmas[i].ano,
            lista_turmas[i].letra,
            lista_turmas[i].nome_professor);
    }

    int escolha;
    printf("Escolha: ");
    scanf("%d", &escolha);
    getchar();

    if (escolha < 1 || escolha > num_turmas) {
        printf("Opção inválida.\n");
        return;
    }

    aluno->ano = lista_turmas[escolha - 1].ano;
    aluno->turma[0] = lista_turmas[escolha - 1].letra;
    aluno->turma[1] = '\0';

    // antiga coleta de ano e turma manual que vai ficar automática conforme a ordenação professor → turma → aluno
    //     printf("Ano do aluno: ");
    //     scanf("%d", &aluno->ano);
    //     printf("Turma [A/B/C/D/E]: ");
    //     scanf(" %[^\n]", aluno->turma);

    printf("\nAluno cadastrado:\n");
    printf("Nome: %s\nMatricula: %s\nEndereco: %s\nCPF: %s\nData de nascimento: %s\nAno: %d\nTurma: %s\n", aluno->nome, aluno->matricula, aluno->endereco, aluno->cpf, aluno->data_nascimento, aluno->ano, aluno->turma);

    printf("Status: %s\nPossui pendências: ", aluno->status);
        if (aluno->possui_pendencias) 
            printf("Sim\n");
        else
    printf("Não\n");

    char nome_arquivo[50];
//    sprintf(nome_arquivo, "aluno_%d.txt", aluno->id);
//isso era pra fazer um arquivo por aluno
//    printf("Matricula que será gravada: %s\n", aluno->matricula); Coloquei isso no 'aluno cadastrado'

    calcular_situacao(aluno);

    arquivo_aluno(aluno, "alunos.txt");

    dados_usuario usuario_gerado = cadastrar_usuario_novo("aluno", aluno->nome);
    printf("\nLogin de aluno criado\n");
    printf("Loging: %s\n", usuario_gerado.login);
    printf("Senha: %s\n", usuario_gerado.senha);

    alunos[total_alunos] = *aluno;
    total_alunos++;
}

//Gerar ID pq toda vez que recompila o ID reseta
void gerar_id_aluno(int *id)
{
    FILE *f = fopen("contador_id.txt", "r+");
    int numero;

    if (f == NULL)
    {
        // Se o arquivo não existir, cria e inicializa com 1
        f = fopen("contador_id.txt", "w+");
        if (f == NULL)
        {
            fprintf(stderr, "Erro ao criar contador_id.txt\n");
            return;
        }
        numero = 1;
    }
    else
    {
        // Tenta ler o valor salvo
        if (fscanf(f, "%d", &numero) != 1)
            numero = 1;
    }

    *id = numero;

    // Volta para o início e zera o arquivo antes de escrever
    freopen("contador_id.txt", "w", f); // reabre o arquivo em modo de escrita limpa
    fprintf(f, "%d", numero + 1);
    fflush(f);
    fclose(f);
}

void gerar_id_aula(int *id) {
    FILE *f = fopen("contador_aula.txt", "r+");
    int numero;

    if (f == NULL){
        // Se o arquivo não existir, cria e inicia em 1
        f = fopen("contador_aula.txt", "w+");
        if (f == NULL)
        {
            fprintf(stderr, "Erro ao criar contador_aula.txt\n");
            return;
        }
        numero = 1;
    }
    else
    {
        // Lê o último número salvo
        if (fscanf(f, "%d", &numero) != 1)
            numero = 1;
    }

    *id = numero; // usa o número atual

    // Atualiza o contador para o próximo ID
    freopen("contador_aula.txt", "w", f);
    fprintf(f, "%d", numero + 1);
    fclose(f);
}

void gerar_matricula_aluno(char matricula[])
{
    FILE *f = fopen("contador_matricula.txt", "r");
    int numero = 1;

    if (f != NULL)
    {
        if (fscanf(f, "%d", &numero) != 1)
        {
            numero = 1;
        }
        fclose(f);
    }

    snprintf(matricula, 5, "%04d", numero);

    // Abre o arquivo em modo escrita truncando para atualizar o contador
    f = fopen("contador_matricula.txt", "w");
    if (f == NULL)
    {
        fprintf(stderr, "Erro ao abrir arquivo de contador de matrícula para escrita.\n");
        return;
    }
    fprintf(f, "%d", numero + 1);
    fclose(f);
}




//Professores :::::::::::::::::::::::::::::::::::::::::::::::
//
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
    fprintf(fp, "Disciplina: %s\n", prof->disciplina);
    fprintf(fp, "Email: %s\n", prof->email);
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
int carregar_professores_de_arquivo(void) {
    FILE *fp = fopen("professores.txt", "r");
    if (!fp) return 0;

    total_professores = 0;
    char linha[200];

    // Leia o arquivo e preencha professores[]
    while (fgets(linha, sizeof(linha), fp) && total_professores < MAX_PROFESSORES) {
        Professor *p = &professores[total_professores];
        // Exemplo simplificado - leia os campos necessários
        // Ex: fscanf(fp, "Nome: %[^\n]\n", professores[total_professores].nome);
        // e continue para CPF, disciplina, email, etc.

        if (sscanf(linha, "Nome: %[^\n]", p->nome) == 1) {
            fgets(linha, sizeof(linha), fp);

            sscanf(linha, "CPF: %[^\n]", p->cpf);
            fgets(linha, sizeof(linha), fp);
            
            sscanf(linha, "Disciplina: %[^\n]", p->disciplina);
            fgets(linha, sizeof(linha), fp);
            
            sscanf(linha, "Email: %[^\n]", p->email);
            //ler a linha --------- divisoria
            fgets(linha, sizeof(linha), fp);

        //Incrementa o total para cada professor lido
        total_professores++;
        }
    }

    fclose(fp);
    return total_professores;

}
//


// Complexos :::::::::::::::::::::::::::::::::::::::::::::::
//

//ID SOS
void atualizar_proximo_id()
{
    int maior_id = 0;
    for (int i = 0; i < total_alunos; i++)
    {
        if (alunos[i].id > maior_id)
        {
            maior_id = alunos[i].id;
        }
    }
    proximo_id = maior_id + 1;
}

void carregar_contador_id()
{
    FILE *f = fopen("contador_id.txt", "r");
    if (f != NULL)
    {
        if (fscanf(f, "%d", &proximo_id) != 1)
        {
            proximo_id = 1; // se falhar leitura, reinicia em 1
        }
        fclose(f);
    }
    else
    {
        proximo_id = 1; // arquivo não existe, inicia em 1
    }
}

// Buscar aluno por id
int buscar_aluno_por_id(int id) {
    for (int i = 0; i < total_alunos; i++) {
        if (alunos[i].id == id) {
            return i;
        }
    }
    return -1;
}

//Buscar aluno e alterar status pelo seu id
void alterar_status_aluno_por_id()
{
    if (total_alunos == 0)
    {
        total_alunos = ler_alunos_de_arquivo(alunos, MAX_ALUNOS);
        if (total_alunos == 0)
        {
            printf("Nenhum aluno cadastrado no sistema.\n");
            return;
        }
    }

    printf("\nLista de alunos disponíveis:\n");
    printf("ID | Nome               | CPF             | Status\n");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < total_alunos; i++) {
        printf("%-2d | %-18s | %-15s | %-10s\n",
               alunos[i].id,
               alunos[i].nome,
               alunos[i].cpf,
               alunos[i].status);
        }

    int escolha;
    printf("Digite o número do aluno para alterar o status (0 para sair): ");
    scanf("%d", &escolha);
    getchar();

    if (escolha == 0)
    {
        printf("Saindo da alteração de status.\n");
        return;
    }

    int indice = escolha - 1;
    if (indice < 0 || indice >= total_alunos)
    {
        printf("Aluno inexistente.\n");
        return;
    }

    char novo_status[10];
    printf("Digite o novo status para %s: ", alunos[indice].nome);
    scanf("%9s", novo_status);
    getchar();

    alterar_status_aluno(&alunos[indice], novo_status);
    salvar_alunos_em_arquivo();

    printf("Status alterado para %s com sucesso.\n", alunos[indice].nome);
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

//Criar arquivo do aluno Append
void arquivo_aluno(const Aluno *aluno, const char *alunosss) {
    FILE *fp = fopen(alunosss, "a");// de w pra a, de write para append
    if (fp == NULL ) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fprintf(fp, "ID: %d\n", aluno->id);
    fprintf(fp, "Matricula: %s\n", aluno->matricula);
    fprintf(fp, "Nome: %s\n", aluno->nome);
    fprintf(fp, "Endereco: %s\n", aluno->endereco);
    fprintf(fp, "CPF: %s\n", aluno->cpf);
    fprintf(fp, "Data de nascimento: %s\n", aluno->data_nascimento);
    fprintf(fp, "Ano: %d\n", aluno->ano);
    fprintf(fp, "Turma: %s\n", aluno->turma);
    fprintf(fp, "Status: %s\n", aluno->status);
    fprintf(fp, "Possui pendencias: %s\n", aluno->possui_pendencias ? "Sim" : "Nao");
    fprintf(fp, "Faltas: %d\n", aluno->faltas);
    fprintf(fp, "Notas: %.2f %.2f %.2f %.2f\n", aluno->notas[0],aluno->notas[1],aluno->notas[2],aluno->notas[3]);
    fprintf(fp, "Situacao: %s\n", aluno->situacao);
    fprintf(fp, "------------------------\n");

    fclose(fp);

    printf("Dados do aluno salvos em '%s'.\n", alunosss);
}

//Salvar status novos no arquivo Write
void salvar_alunos_em_arquivo(void) {
    for (int i = 0; i < total_alunos; i++) {
        calcular_situacao(&alunos[i]);
    }
    
    FILE *fp = fopen("alunos.txt", "w");
    if (!fp) {
        perror("Erro ao abrir arquivo para salvar");
        return;
    }
    for (int i = 0; i < total_alunos; i++) {
        fprintf(fp, "ID: %d\n", alunos[i].id);
        fprintf(fp, "Matricula: %s\n", alunos[i].matricula);
        fprintf(fp, "Nome: %s\n", alunos[i].nome);
        fprintf(fp, "Endereco: %s\n", alunos[i].endereco);
        fprintf(fp, "CPF: %s\n", alunos[i].cpf);
        fprintf(fp, "Data de nascimento: %s\n", alunos[i].data_nascimento);
        fprintf(fp, "Ano: %d\n", alunos[i].ano);
        fprintf(fp, "Turma: %s\n", alunos[i].turma);
        fprintf(fp, "Status: %s\n", alunos[i].status);
        fprintf(fp, "Possui pendencias: %s\n", alunos[i].possui_pendencias ? "Sim" : "Nao");
        fprintf(fp, "Faltas: %d\n", alunos[i].faltas);
        fprintf(fp, "Notas: %.2f %.2f %.2f %.2f\n", alunos[i].notas[0], alunos[i].notas[1], alunos[i].notas[2], alunos[i].notas[3]);
        fprintf(fp, "Situacao: %s\n", alunos[i].situacao);
        fprintf(fp, "------------------------------\n");
    }
    fclose(fp);
}

int buscar_aluno_nome_matr(const char *entrada, Aluno *aluno_encontrado) {
    FILE *f = fopen("alunos.txt", "r");
    if (!f) {
        printf("Erro ao abrir arquivo.\n");
        return 0;
    }

    Aluno temp = {0};
    char linha[256];
    int achou = 0;

    while (fgets(linha, sizeof(linha), f)) {
        if (strncmp(linha, "ID:", 3) == 0) {
            // Se já está lendo um aluno, checa se era o procurado
            if (temp.id != 0)
            {
                if (strcmp(temp.nome, entrada) == 0 || strcmp(temp.matricula, entrada) == 0) {
                    *aluno_encontrado = temp;
                    achou = 1;
                    break;
                }
            }
            // Reinicia o struct e começa novo aluno
            memset(&temp, 0, sizeof(Aluno));
            sscanf(linha, "ID: %d", &temp.id);
        } else if (sscanf(linha, "Matricula: %15[^\n]", temp.matricula) == 1) {
            continue;
        } else if (sscanf(linha, "Nome: %99[^\n]", temp.nome) == 1) {
            continue;
        } else if (sscanf(linha, "CPF: %20[^\n]", temp.cpf) == 1) {
            continue;
        } else if (sscanf(linha, "Endereco: %99[^\n]", temp.endereco) == 1) {
            continue;
        } else if (sscanf(linha, "Data de nascimento: %20[^\n]", temp.data_nascimento) == 1) {
            continue;
        } else if (sscanf(linha, "Turma: %20[^\n]", temp.turma) == 1) {
            continue;
        } else if (sscanf(linha, "Status: %20[^\n]", temp.status) == 1) {
            continue;
        } else if (strncmp(linha, "Possui pendências:", 18) == 0) {
            char pend[10];
            sscanf(linha, "Possui pendências: %9s", pend);
            temp.possui_pendencias = (strcmp(pend, "Sim") == 0);
        } else if (sscanf(linha, "Faltas: %d", &temp.faltas) == 1) {
            continue;
        } else if (sscanf(linha, "Notas: %f %f %f %f",
                        &temp.notas[0], &temp.notas[1], &temp.notas[2], &temp.notas[3]) == 4) {
            continue;
        }
    }

    // Checa o último aluno lido (importante!)
    if (!achou && (strcmp(temp.nome, entrada) == 0 || strcmp(temp.matricula, entrada) == 0))
    {
        *aluno_encontrado = temp;
        achou = 1;
    }

    fclose(f);
    return achou;
}

void exibir_dados_aluno(const Aluno *a) {
    printf("\n=== Dados do Aluno ===\n");
    printf("Matricula: %s\n", a->matricula);
    printf("Nome: %s\n", a->nome);
    printf("CPF: %s\n", a->cpf);
    printf("Endereco: %s\n", a->endereco);
    printf("Data de nascimento: %s\n", a->data_nascimento);
    printf("Ano: %d\n", a->ano);
    printf("Turma: %s\n", a->turma);
    printf("Status: %s\n", a->status);
    printf("Possui pendências: %s\n", a->possui_pendencias ? "Sim" : "Não");
    printf("Faltas: %d\n", a->faltas);
    printf("Notas: %.2f %.2f %.2f %.2f\n", a->notas[0], a->notas[1], a->notas[2], a->notas[3]);
    printf("Situacao: %s\n", a->situacao);
    printf("------------------------------\n");
}

void visualizar_aluno_buscado(void) {
    char entrada[100];
    Aluno aluno;

    printf("Digite o nome ou matricula do aluno: ");
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = 0;

    if (buscar_aluno_nome_matr(entrada, &aluno)) {
        exibir_dados_aluno(&aluno);
    } else {
        printf("Aluno não encontrado.\n");
    }
}

void gerar_boletim_aluno(const Aluno *a) {
    int todas_zero = 1;
    for (int i = 0; i < 4; i++) {
        if (a->notas[i] != 0.0f) {
            todas_zero = 0;
            break;
        }
    }

    if (todas_zero) {
        printf("Relatório não pode ser gerado:\nTodas as notas estão zeradas para o aluno %s.\n", a->nome);
        return;
    }

    printf("\n=== Boletim do Aluno ===\n");
    printf("Nome: %s\n", a->nome);
    printf("Matrícula: %s\n", a->matricula);
    printf("Ano: %d\n\n", a->ano);
    printf("Turma: %s\n\n", a->turma);

    printf("Notas:\n");
    for (int i = 0; i < 40; i++) {
        printf("Nota %d: %.2f\n", i + 1, a->notas[i]);
    }

    float soma = 0.0f;
    int cont = 0;
    for (int i = 0; i < 4; i++) {
        if (a->notas[i] != 0.0f) {
            soma += a->notas[i];
            cont++;
        }
    }

    float media = cont > 0 ? soma / cont : 0.0f;

    printf("\nMédia final: %.2f\n", media);
    printf("Situação: %s\n", a->situacao);

    if (media >= 6.0f) {
        printf("Status: Aprovado\n");
    } else {
            printf("Status: Reprovado\n");
        }
    
    printf("=======================\n\n");
}

void gerar_relatorio_alunos(const char *filtro_turma, const char *filtro_status, int filtrar_pendencias)
{
    printf("\n== Relatório Completo de Alunos ==\n");

    for (int i = 0; i < total_alunos; i++)
    {
        Aluno *a = &alunos[i];

        // Verificar filtro de turma
        if (filtro_turma != NULL && strlen(filtro_turma) > 0 && strcmp(a->turma, filtro_turma) != 0) {
            continue;
        }

        // Verificar filtro de status
        if (filtro_status != NULL && strlen(filtro_status) > 0 && strcmp(a->status, filtro_status) != 0) {
            continue;
        }

        // Verificar filtro de pendências
        if (filtrar_pendencias && !a->possui_pendencias) {
            continue;
        }

        printf("Nome: %s\nCPF: %s\nEndereco: %s\nAno: %d\nTurma: %s\nStatus: %s\nPendencias: %s\nFaltas: %d\nNotas: %.2f %.2f %.2f %.2f\n----------------------\n",
               a->nome, a->cpf, a->endereco, a->ano, a->turma, a->status, a->possui_pendencias ? "Sim" : "Não", a->faltas, a->notas[0], a->notas[1], a->notas[2], a->notas[3]);

        for (int j = 0; j < 4; j++) {
            printf(" %.2f", a->notas[j]);
        }

        printf("\n----------------------\n");

    }
}

void gerar_boletim_individual() {
    char entrada[100];
    Aluno aluno;

    printf("Digite o nome ou matricula do aluno para o boletim: ");
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = 0; // remove \n

    if (buscar_aluno_nome_matr(entrada, &aluno)) {
        gerar_boletim_aluno(&aluno);
    } else {
        printf("Aluno não encontrado.\n");
    }
}

void gerar_boletim_todos() {
    int total = ler_alunos_de_arquivo(alunos, MAX_ALUNOS);
    if (total == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        gerar_boletim_aluno(&alunos[i]);
    }
}
