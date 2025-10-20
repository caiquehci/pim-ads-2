import tkinter as tk
from tkinter import simpledialog


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
            nome = matricula = turma = ano = None
            conteudo_formatado = ""
            conteudo_formatado += f"{'Nome':<20} {'Matricula':<10} {'Ano':<4} {'Turma':<6}\n"
            conteudo_formatado += "-"*44 + "\n"

            for linha in f:
                linha = linha.strip()
                if not linha:
                    continue

                if linha.startswith("Nome:"):
                    nome = linha.split(": ", 1)[1]
                elif linha.startswith("Matricula:"):
                    matricula = linha.split(": ", 1)[1]
                elif linha.startswith("Ano:"):
                    ano_str = linha.split(": ", 1)[1]
                    try:
                        ano = int(ano_str)
                    except:
                        ano = ano_str or "?"
                elif linha.startswith("Turma:"):
                    turma = linha.split(": ", 1)[1]
                elif linha == "------------------------------":
                    # Adiciona o aluno no resultado assim que encontra a linha separadora
                    if nome is not None and matricula is not None:
                        ano_val = ano if ano is not None else "?"
                        turma_val = turma if turma is not None else "?"
                        conteudo_formatado += f"{nome:<20} {matricula:<10} {ano_val:<4} {turma_val:<6}\n"
                    nome = matricula = turma = ano = None  # reseta para próximo aluno

            # Caso o último aluno não tenha linha separadora, adiciona depois do loop
            if nome is not None and matricula is not None:
                ano_val = ano if ano is not None else "?"
                turma_val = turma if turma is not None else "?"
                conteudo_formatado += f"{nome:<20} {matricula:<10} {ano_val:<4} {turma_val:<6}\n"

        return conteudo_formatado
    except FileNotFoundError:
        return "Arquivo não encontrado."
