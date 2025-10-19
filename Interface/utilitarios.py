import tkinter as tk
from tkinter import simpledialog
from tkinter import scrolledtext

def ler_arquivo():
    try:
        with open('../alunos.txt', 'r', encoding='utf-8') as f:
            conteudo = f.read()
    except FileNotFoundError:
        return "Arquivo não encontrado."