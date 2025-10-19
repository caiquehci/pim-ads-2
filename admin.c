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

// Turmas :::::::::::::::::::::::::::::::::::::::::::::::
//
// Cadastrar turma
void cadastrar_turma(Turma *turma){
    printf("Cadastre uma nova turma\n");

    printf("Série (número do ano): ");
    scanf(" %[^\n]",turma->serie);

    printf("Letra da turma [A-Z]: ");
    scanf(" %c",&turma->letra);

    printf("Professor responsável: ");
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

//

// Alunos :::::::::::::::::::::::::::::::::::::::::::::::
//
// Ler arquivos de aluno
int ler_alunos_de_arquivo(Aluno alunos[], int max_alunos)
{
    FILE *fp = fopen("alunos.txt", "r");
    if (!fp)
    {
        perror("Erro ao abrir arquivo alunos.txt");
        return 0;
    }

    char linha[256];
    int i = 0;

    while (i < max_alunos && fgets(linha, sizeof(linha), fp) != NULL)
    {
        // Se a linha inicia com "ID:", então começa um registro
        if (strstr(linha, "ID:") == linha)
        {
            // Limpa o struct do aluno
            memset(&alunos[i], 0, sizeof(Aluno));

            // ID
            sscanf(linha, "ID: %d", &alunos[i].id);

            // Leitura das próximas linhas
            // Cada uma contendo o campo após ":"
            for (int campo = 0; campo < 10; campo++)
            {
                if (fgets(linha, sizeof(linha), fp) == NULL)
                    break;
                char *p = strchr(linha, ':');
                if (p == NULL)
                    continue;
                p++; // avança após o ":"
                while (*p == ' ')
                    p++; // pula espaços
                switch (campo)
                {
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
                    sscanf(p, "%f %f %f %f",
                           &alunos[i].notas[0],
                           &alunos[i].notas[1],
                           &alunos[i].notas[2],
                           &alunos[i].notas[3]);
                    break;
                }
            }
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
    } //antes era uma nota com valor fixo pra não ser lixo, agora são

    printf("Cadastre um novo aluno\n");

    printf("Nome: ");
    scanf(" %[^\n]",aluno->nome); //ler com espaço até ENTER
    
    printf("Endereco: ");
    scanf(" %[^\n]",aluno->endereco);

    printf("CPF: ");
    scanf(" %[^\n]",aluno->cpf);
    
    printf("Data de nascimento (DD/MM/AAAA): ");
    scanf(" %[^\n]",aluno->data_nascimento);

    printf("Turma [A/B/C/D/E]: ");
    scanf(" %[^\n]",aluno->turma);

    printf("\nAluno cadastrado:\n");
    printf("Nome: %s\nMatricula: %s\nEndereco: %s\nCPF: %s\nData de nascimento: %s\nTurma: %s\n",aluno->nome,aluno->matricula,aluno->endereco,aluno->cpf,aluno->data_nascimento,aluno->turma);

    printf("Status: %s\nPossui pendências: ", aluno->status);
        if (aluno->possui_pendencias) 
            printf("Sim\n");
        else
    printf("Não\n");

    char nome_arquivo[50];
//    sprintf(nome_arquivo, "aluno_%d.txt", aluno->id);
//isso era pra fazer um arquivo por aluno
//    printf("Matricula que será gravada: %s\n", aluno->matricula); Coloquei isso no 'aluno cadastrado'
    arquivo_aluno(aluno, "alunos.txt");

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

//


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
    fprintf(fp, "Turma: %s\n", aluno->turma);
    fprintf(fp, "Status: %s\n", aluno->status);
    fprintf(fp, "Possui pendencias: %s\n", aluno->possui_pendencias ? "Sim" : "Nao");
    fprintf(fp, "Faltas: %d\n", aluno->faltas);
    fprintf(fp, "Notas: %.2f %.2f %.2f %.2f\n", aluno->notas[0],aluno->notas[1],aluno->notas[2],aluno->notas[3]);
    fprintf(fp, "------------------------\n");

    fclose(fp);

    printf("Dados do aluno salvos em '%s'.\n", alunosss);
}

//Salvar status novos no arquivo Write
void salvar_alunos_em_arquivo(void) {
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
        fprintf(fp, "Turma: %s\n", alunos[i].turma);
        fprintf(fp, "Status: %s\n", alunos[i].status);
        fprintf(fp, "Possui pendencias: %s\n", alunos[i].possui_pendencias ? "Sim" : "Nao");
        fprintf(fp, "Faltas: %d\n", alunos[i].faltas);
        fprintf(fp, "Notas: %.2f %.2f %.2f %.2f\n", alunos[i].notas[0], alunos[i].notas[1], alunos[i].notas[2], alunos[i].notas[3]);
        fprintf(fp, "------------------------------\n");
    }
    fclose(fp);
}

int buscar_aluno_nome_matr(const char *entrada, Aluno *aluno_encontrado)
{
    FILE *f = fopen("alunos.txt", "r");
    if (!f)
    {
        printf("Erro ao abrir arquivo.\n");
        return 0;
    }

    Aluno temp = {0};
    char linha[256];
    int achou = 0;

    while (fgets(linha, sizeof(linha), f))
    {
        if (strncmp(linha, "ID:", 3) == 0)
        {
            // Se já está lendo um aluno, checa se era o procurado
            if (temp.id != 0)
            {
                if (strcmp(temp.nome, entrada) == 0 || strcmp(temp.matricula, entrada) == 0)
                {
                    *aluno_encontrado = temp;
                    achou = 1;
                    break;
                }
            }
            // Reinicia o struct e começa novo aluno
            memset(&temp, 0, sizeof(Aluno));
            sscanf(linha, "ID: %d", &temp.id);
        }
        else if (sscanf(linha, "Matricula: %15[^\n]", temp.matricula) == 1)
        {
            continue;
        }
        else if (sscanf(linha, "Nome: %99[^\n]", temp.nome) == 1)
        {
            continue;
        }
        else if (sscanf(linha, "CPF: %20[^\n]", temp.cpf) == 1)
        {
            continue;
        }
        else if (sscanf(linha, "Endereco: %99[^\n]", temp.endereco) == 1)
        {
            continue;
        }
        else if (sscanf(linha, "Data de nascimento: %20[^\n]", temp.data_nascimento) == 1)
        {
            continue;
        }
        else if (sscanf(linha, "Turma: %20[^\n]", temp.turma) == 1)
        {
            continue;
        }
        else if (sscanf(linha, "Status: %20[^\n]", temp.status) == 1)
        {
            continue;
        }
        else if (strncmp(linha, "Possui pendências:", 18) == 0)
        {
            char pend[10];
            sscanf(linha, "Possui pendências: %9s", pend);
            temp.possui_pendencias = (strcmp(pend, "Sim") == 0);
        }
        else if (sscanf(linha, "Faltas: %d", &temp.faltas) == 1)
        {
            continue;
        }
        else if (sscanf(linha, "Notas: %f %f %f %f",
                        &temp.notas[0], &temp.notas[1], &temp.notas[2], &temp.notas[3]) == 4)
        {
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
    printf("Turma: %s\n", a->turma);
    printf("Status: %s\n", a->status);
    printf("Possui pendências: %s\n", a->possui_pendencias ? "Sim" : "Não");
    printf("Faltas: %d\n", a->faltas);
    printf("Notas: %.2f %.2f %.2f %.2f\n",
           a->notas[0], a->notas[1], a->notas[2], a->notas[3]);
}

void visualizar_aluno_buscado(void)
{
    char entrada[100];
    Aluno aluno;

    printf("Digite o nome ou matricula do aluno: ");
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = 0;

    if (buscar_aluno_nome_matr(entrada, &aluno))
    {
        exibir_dados_aluno(&aluno);
    } else {
        printf("Aluno não encontrado.\n");
    }
}
