import tkinter as tk
from tkinter import simpledialog, messagebox
from admin import buscar_aluno_individual

def consultar_alunos(janela_pai, text_area_prof):
    entrada = simpledialog.askstring(
        "Consultar aluno",
        "Digite o nome ou matrícula:",
        parent=janela_pai
    )
    if entrada:
        conteudo = buscar_aluno_individual(entrada)
        text_area_prof.delete('1.0', tk.END)
        text_area_prof.insert(tk.END, conteudo)

def buscar_aluno_individual(entrada):
    try:
        with open('../alunos.txt', 'r', encoding='utf-8') as f:
            buffer = []
            encontrou = False
            nome = None
            matricula = None
            for linha in f:
                linha_lida = linha.strip()
                if linha_lida.startswith("ID:"):
                    buffer = [linha.rstrip('\n')]  # começa novo buffer
                    nome = None
                    matricula = None
                elif linha_lida.startswith("Nome:"):
                    nome = linha_lida.split(": ",1)[1].strip()
                    buffer.append(linha.rstrip('\n'))
                elif linha_lida.startswith("Matricula:"):
                    matricula = linha_lida.split(": ",1)[1].strip()
                    buffer.append(linha.rstrip('\n'))
                elif linha_lida == "------------------------------":
                    buffer.append(linha.rstrip('\n'))
                    if nome and (entrada.lower() == nome.lower()) or (matricula and entrada == matricula):
                        encontrou = True
                        return "\n".join(buffer)
                    buffer = []
                else:
                    buffer.append(linha.rstrip('\n'))
            return "Aluno não encontrado."
    except FileNotFoundError:
        return "Arquivo de alunos não encontrado."

def ver_notas(janela_pai, text_area_prof):
    entrada = simpledialog.askstring(
        "Ver Notas",
        "Digite o nome ou matrícula do aluno:",
        parent=janela_pai
    )
    if entrada:
        conteudo = buscar_detalhe_aluno(entrada, campo="Notas")
        text_area_prof.delete('1.0', tk.END)
        text_area_prof.insert(tk.END, conteudo)

def ver_faltas(janela_pai, text_area_prof):
    entrada = simpledialog.askstring(
        "Ver Faltas",
        "Digite o nome ou matrícula do aluno:",
        parent=janela_pai
    )
    if entrada:
        conteudo = buscar_detalhe_aluno(entrada, campo="Faltas")
        text_area_prof.delete('1.0', tk.END)
        text_area_prof.insert(tk.END, conteudo)

def buscar_detalhe_aluno(entrada, campo):
    try:
        with open('../alunos.txt', 'r', encoding='utf-8') as f:
            nome = None
            matricula = None
            ano = None
            turma = None
            detalhe = None
            situacao = None
            for linha in f:
                linha_lida = linha.strip()
                if linha_lida.startswith("ID:"):
                    nome = matricula = ano = turma = detalhe = situacao = None
                elif linha_lida.startswith("Nome:"):
                    nome = linha_lida.split(": ", 1)[1].strip()
                elif linha_lida.startswith("Matricula:"):
                    matricula = linha_lida.split(": ", 1)[1].strip()
                elif linha_lida.startswith("Ano:"):
                    ano = linha_lida.split(": ", 1)[1].strip()
                elif linha_lida.startswith("Turma:"):
                    turma = linha_lida.split(": ", 1)[1].strip()
                elif linha_lida.startswith(f"{campo}:"):
                    detalhe = linha_lida.split(": ", 1)[1].strip()
                elif linha_lida.startswith("Situacao:"):
                    situacao = linha_lida.split(": ", 1)[1].strip()
                elif linha_lida == "------------------------------":
                    if (nome and entrada.lower() == nome.lower()) or (matricula and entrada == matricula):
                        if detalhe:
                            return (f"Ano: {ano}\nTurma: {turma}\n\n"
                                    f"{campo} do aluno {nome} ({matricula}):\n{detalhe}\n\n"
                                    f"Situação atual: {situacao or 'Indefinida'}")
                        else:
                            return (f"Ano: {ano}\nTurma: {turma}\n\n"
                                    f"{campo} não encontrada para o aluno {nome} ({matricula}).\n\n"
                                    f"Situação atual: {situacao or 'Indefinida'}")
            return "Aluno não encontrado."
    except FileNotFoundError:
        return "Arquivo de alunos não encontrado."
