import tkinter as tk
from tkinter import simpledialog
from tkinter import scrolledtext

def ler_arquivo_max():
    try:
        with open('../alunos.txt', 'r', encoding='utf-8') as f:
            return f.read()
    except FileNotFoundError:
        return "Arquivo não encontrado."

def buscar_aluno_individual(matricula_ou_nome):
    try:
        with open('../alunos.txt', 'r', encoding='utf-8') as f:
            linhas = f.readlines()

        buffer = ""
        aluno_encontrado = []
        capturando = False

        for linha in linhas:
            # Verifica se chegou ao separador de alunos
            if linha.strip() == "------------------------------":
                if capturando:
                    # fim do aluno encontrado
                    break
                else:
                    aluno_encontrado = []  # limpa para o próximo aluno
                    continue

            # Verifica se linha contém o termo buscado
            if matricula_ou_nome.lower() in linha.lower() and (linha.startswith("Nome:") or linha.startswith("Matricula:")):
                capturando = True

            # Se estamos capturando, adiciona a linha atual
            if capturando:
                aluno_encontrado.append(linha)

        # Junta o conteúdo capturado e adiciona o separador no final
        if aluno_encontrado:
            buffer = "".join(aluno_encontrado) + "------------------------------\n"
            return buffer
        else:
            return "Aluno não encontrado."

    except FileNotFoundError:
        return "Arquivo não encontrado."

def abrir_relatorios(janela, text_area, verificar_conteudo):
    rel_window = tk.Toplevel(janela)
    rel_window.title("Relatórios")
    rel_window.geometry("480x160")

    def exibir_boletim_todos():
        conteudo = ler_arquivo_max()
        text_area.delete('1.0', tk.END)
        text_area.insert(tk.END, conteudo)
        verificar_conteudo()
        rel_window.destroy()

    def exibir_boletim_individual():
        entrada = simpledialog.askstring("Boletim Individual", "Digite o nome ou matrícula do aluno:", parent=rel_window)
        if entrada:
            conteudo = buscar_aluno_individual(entrada)
            text_area.delete('1.0', tk.END)
            text_area.insert(tk.END, conteudo)
            verificar_conteudo()
        rel_window.destroy()

    frame_botoes = tk.Frame(rel_window)
    frame_botoes.pack(pady=32)

    btn_todos = tk.Button(frame_botoes, text="Exibir boletim de todos os alunos", command=exibir_boletim_todos)
    btn_individual = tk.Button(frame_botoes, text="Exibir boletim individual", command=exibir_boletim_individual)
    btn_todos.pack(side=tk.LEFT, padx=16)
    btn_individual.pack(side=tk.LEFT, padx=16)

def mostrar_alunos():
    try:
        with open('../alunos.txt', 'r', encoding='utf-8') as f:

            conteudo_formatado = ""
            conteudo_formatado += f"{'Nome':<20} {'Matricula':<10} {'Ano':<4} {'Turma':<6}\n"
            conteudo_formatado += "-"*44 + "\n"

            aluno_linhas = []
            while True:
                linha = f.readline()
                if not linha:  # fim do arquivo
                    break
                linha = linha.rstrip('\n')

                if linha == '------------------------------':
                    # Processa o bloco de linhas do aluno atual
                    campos = {'Nome': None, 'Matricula': None, 'Ano': None, 'Turma': None}
                    for l in aluno_linhas:
                        l = l.strip()
                        for campo in campos.keys():
                            if l.startswith(f"{campo}:"):
                                valor = l.split(": ", 1)[1].strip()
                                if campo == 'Ano':
                                    try:
                                        valor = int(valor)
                                    except:
                                        pass
                                campos[campo] = valor
                    # Adiciona ao texto final se tiver nome e matricula
                    if campos['Nome'] and campos['Matricula']:
                        ano_val = campos['Ano'] if campos['Ano'] is not None else "?"
                        turma_val = campos['Turma'] if campos['Turma'] is not None else "?"
                        conteudo_formatado += f"{campos['Nome']:<20} {campos['Matricula']:<10} {ano_val:<4} {turma_val:<6}\n"
                    aluno_linhas = []  # reseta para próximo aluno

                    # Olha se a próxima linha começa com ID para continuar ou não
                    pos_atual = f.tell()
                    prox_linha = f.readline()
                    if not prox_linha:  # fim do arquivo, sai
                        break
                    if not prox_linha.startswith('ID:'):
                        # não tem mais aluno, para
                        break
                    else:
                        # tem mais alunos, volta uma linha para ler normalmente
                        f.seek(pos_atual)

                else:
                    aluno_linhas.append(linha)

        return conteudo_formatado
    except FileNotFoundError:
        return "Arquivo não encontrado."

