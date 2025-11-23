# Sistema de Gestão Acadêmico Integrado (PIM ADS 2)

## 🏅 Desafio

O projeto visa desenvolver um sistema integrado de gestão escolar para otimizar o cadastro, controle e automação dos processos acadêmicos, como administração de alunos, professores, turmas e disciplinas, atendendo às necessidades da escola e promovendo maior eficiência e organização administrativa.

## 🏅 Solução

Aplicação modular construída nas linguagens C (para módulos de baixo nível e manipulação dos dados) e Python (para interface gráfica e geração de relatórios), utilizando a metodologia Kanban para gerenciamento ágil individual das tarefas, permitindo automação e controle eficiente das rotinas escolares.

## 📋 Backlog do Produto

O controle do desenvolvimento foi realizado mediante quadro Kanban, que organizou as tarefas em estados diversos como A Fazer, Em Progresso e Concluído. Algumas tarefas exemplares:

| Prioridade | Tarefa                              | Status      | Observações                                                       |
|------------|-----------------------------------|-------------|------------------------------------------------------------------|
| Alta       | Cadastro e gerenciamento de professores | Concluído   | Sistema permite cadastro, edição e exclusão de professores, com validação de dados. Inclui interface amigável em Tkinter. |
| Alta       | Cadastro e gerenciamento de alunos       | Concluído   | Funcionalidade completa para registro e atualização de dados acadêmicos dos alunos, com controles para turmas e disciplinas. |
| Alta       | Controle e organização de turmas          | Concluído   | Gestão eficiente das turmas, alocação de alunos e horários integrada no sistema.                               |
| Alta       | Emissão de boletins acadêmicos             | Concluído   | Geração automática de boletins individuais em PDF, baseado nos dados lançados para cada aluno.                |
| Alta       | Interface gráfica e usabilidade             | Concluído   | GUI desenvolvida em Python com Tkinter, focando em facilidade de uso e navegação intuitiva.                   |
| Alta       | Relatórios PDF e documentação técnica      | Concluído   | Uso da biblioteca ReportLab para geração dos relatórios, com documentação técnica detalhada disponível.      |



## 🗂️ Planejamento e Evolução - Kanban

As etapas foram organizadas e acompanhadas visualmente pelo método Kanban, proporcionando flexibilidade e controle do fluxo contínuo das atividades de desenvolvimento.



## ⚙️ Tecnologias Utilizadas

- C: programação estruturada para manipulação de dados e lógica do sistema
- Python: interface gráfica com Tkinter, geração e exportação de relatórios PDF
- ReportLab para geração de PDFs
- Git e GitHub para versionamento e controle do projeto
- Metodologia ágil Kanban para planejamento e acompanhamento



## 📂 Estrutura do Projeto
```
/pim-ads-2
│
├── /C # Código fonte em C - manipulação, lógica, leitura e escrita de arquivos
├── /Interface # Interface GUI, processamento e visualização de relatórios
├── /docs # Documentação UML, diagramas e relatórios técnicos
└── README.md # Documento principal do projeto
```


## 🚀 Como executar, usar e testar

1. Clone o repositório:
git clone https://github.com/caiquehci/pim-ads-2.git
cd pim-ads-2

2. Compile o código C (utilizando GCC ou MinGW):
gcc main.c admin.c menu.c prof.c -o programa

3. Explore as funcionalidades como quiser. Por default o sistema cria um user admin, com todas as features liberadas. Cabe ao user admin criar novos cadastros.

3.1 As criações de arquivo ocorrem apenas através do código em C (seja pelo terminal ou pelo programa.exe)

4. Execute a interface em Python 3+ para explorar as features de visualização, com o mesmo login anterior:
cd interface
python app.py

5. Utilize os scripts de teste e os casos simulados para conferir o funcionamento e a integridade dos dados.



## 📚 Documentação

- Diagramas UML, relatórios e documentos técnicos estão disponíveis na pasta `/docs`.
- [Manual de Uso - Vídeo explicativo](https://youtu.be/EjsPrxFAeAI) – Apresenta o funcionamento completo do sistema e instruções para uso.



## 👥 Equipe

Projeto desenvolvido individualmente por:

| Nome Completo                            | Papel                | GitHub                                         | LinkedIn                                            |
|-----------------------------------------|----------------------|------------------------------------------------|-----------------------------------------------------|
| Carlos Henrique Machado Castanheira Jr. | Desenvolvedor único   | [github.com/caiquehci](https://github.com/caiquehci) | [linkedin.com/in/carloshenrmcj](https://www.linkedin.com/in/carloshenrmcj/) |
