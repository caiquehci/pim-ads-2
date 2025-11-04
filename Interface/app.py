import tkinter as tk
from tkinter import scrolledtext
from tkinter import messagebox, filedialog
from admin import abrir_relatorios, mostrar_alunos, buscar_aluno_individual
from professor import consultar_alunos, ver_notas, ver_faltas

from reportlab.lib.pagesizes import A4
from reportlab.pdfgen import canvas


# ============================================================
#   FUNÇÕES DE LOGIN E INICIALIZAÇÃO
# ============================================================

def tela_login():
    login_window = tk.Tk()
    login_window.title("Login")
    login_window.geometry("240x200")

    tk.Label(login_window, text="Usuário").pack(pady=12)
    entry_user = tk.Entry(login_window, width=24)
    entry_user.pack()

    tk.Label(login_window, text="Senha").pack(pady=4)
    entry_pass = tk.Entry(login_window, width=24, show="*")
    entry_pass.pack()

    def validar_login():
        user = entry_user.get()
        password = entry_pass.get()

        try:
            with open('../usuarios.txt', 'r', encoding='utf-8') as arquivo:
                for linha in arquivo:
                    linha = linha.strip()
                    if not linha:
                        continue

                    campos = linha.split('|')
                    if len(campos) < 5:
                        continue

                    login_arq, senha_arq, tipo_arq, nome_arq, id_arq = campos

                    if user == login_arq and password == senha_arq:
                        tipo_usuario = tipo_arq
                        nome_usuario = nome_arq
                        login_window.destroy()
                        iniciar_app(tipo_usuario, nome_usuario)
                        return

                messagebox.showerror("Erro", "Usuário ou senha errados")
        except FileNotFoundError:
            messagebox.showerror("Erro", "Lista de usuários não encontrado")

    frame_botoes = tk.Frame(login_window)
    frame_botoes.pack(side=tk.BOTTOM, pady=16)

    botao_login = tk.Button(frame_botoes, text="Entrar",
                            command=validar_login, width=8)
    botao_login.pack(side=tk.LEFT, pady=5, padx=(0, 8))

    botao_fechar = tk.Button(frame_botoes, text="Sair",
                             command=login_window.destroy, width=8)
    botao_fechar.pack(side=tk.LEFT, padx=(8, 0), pady=5)

    login_window.mainloop()


def iniciar_app(tipo_usuario, nome_usuario):
    global tipo
    tipo = tipo_usuario
    if tipo_usuario == "admin":
        criar_janela_admin(nome_usuario)
    elif tipo_usuario == "professor":
        criar_janela_prof(nome_usuario)
    elif tipo_usuario == "aluno":
        criar_janela_aluno(nome_usuario)
    else:
        messagebox.showerror("Erro", f"Tipo de usuário desconhecido: {tipo_usuario}")


# ============================================================
#   FUNÇÕES GLOBAIS USADAS NAS INTERFACES
# ============================================================

def logout():
    global janela_admin
    janela_admin.destroy()
    tela_login()


def logout_prof():
    global janela_prof
    janela_prof.destroy()
    tela_login()


def mostrar_alunos_interface():
    conteudo = mostrar_alunos()
    text_area.delete('1.0', tk.END)
    text_area.insert(tk.END, conteudo)
    verificar_conteudo()


def verificar_conteudo(event=None):
    global text_area, botao_exportar_pdf
    if text_area.get('1.0', tk.END).strip():
        botao_exportar_pdf.config(state=tk.NORMAL)
    else:
        botao_exportar_pdf.config(state=tk.DISABLED)


def verificar_conteudo_prof(event=None):
    global text_area_prof, botao_exportar_pdf_prof
    if text_area_prof.get('1.0', tk.END).strip():
        botao_exportar_pdf_prof.config(state=tk.NORMAL)
    else:
        botao_exportar_pdf_prof.config(state=tk.DISABLED)


# ============================================================
#   FUNÇÃO PARA LER AULAS (usada na janela do aluno)
# ============================================================

def ler_aulas_por_ano_turma(ano_aluno, turma_aluno):
    try:
        with open("../aulas.txt", "r", encoding="utf-8") as f:
            linhas = f.readlines()

        aulas_filtradas = []
        aula_atual = {}

        for linha in linhas:
            linha = linha.strip()
            if not linha:
                continue

            if linha.startswith("ID:"):
                aula_atual = {"ID": linha.split(":")[1].strip()}
            elif linha.startswith("Ano:"):
                aula_atual["Ano"] = linha.split(":")[1].strip()
            elif linha.startswith("Matéria:"):
                aula_atual["Matéria"] = linha.split(":")[1].strip()
            elif linha.startswith("Turma:"):
                aula_atual["Turma"] = linha.split(":")[1].strip()
            elif linha.startswith("Corpo:"):
                aula_atual["Corpo"] = linha.split(":", 1)[1].strip()
            elif linha.startswith("-"):
                if (
                    aula_atual.get("Turma") == turma_aluno
                    and aula_atual.get("Ano") == str(ano_aluno)
                ):
                    aulas_filtradas.append(aula_atual)
                aula_atual = {}

        if not aulas_filtradas:
            return f"Nenhuma aula encontrada para o {ano_aluno}º Ano, turma {turma_aluno}."

        resultado = f"Aulas do {ano_aluno}º Ano - Turma {turma_aluno}:\n\n"
        for a in aulas_filtradas:
            resultado += (
                f"ID: {a.get('ID', '')}\n"
                f"Ano: {a.get('Ano', '')}\n"
                f"Matéria: {a.get('Matéria', '')}\n"
                f"Turma: {a.get('Turma', '')}\n"
                f"Descrição: {a.get('Corpo', '')}\n"
                f"--------------------------\n"
            )

        return resultado

    except FileNotFoundError:
        return "Arquivo 'aulas.txt' não encontrado."
    except Exception as e:
        return f"Erro ao ler aulas.txt: {e}"


# ============================================================
#   EXPORTAR CONTEÚDO PARA PDF
# ============================================================

def exportar_para_pdf(text_widget):
    conteudo = text_widget.get('1.0', 'end').strip()
    if not conteudo:
        messagebox.showwarning("Aviso", "Não há conteúdo para exportar.")
        return

    caminho = filedialog.asksaveasfilename(
        defaultextension=".pdf",
        filetypes=[("Arquivos PDF", "*.pdf")]
    )

    if caminho:
        try:
            c = canvas.Canvas(caminho, pagesize=A4)
            largura, altura = A4
            y = altura - 50
            espacamento = 14

            for linha in conteudo.split('\n'):
                c.drawString(40, y, linha)
                y -= espacamento
                if y < 50:
                    c.showPage()
                    y = altura - 50

            c.save()
            messagebox.showinfo(
                "Sucesso", "Conteúdo exportado para PDF com sucesso!")
        except Exception as e:
            messagebox.showerror("Erro", f"Falha ao salvar PDF: {e}")


# ============================================================
#   JANELA DO ADMINISTRADOR
# ============================================================

def criar_janela_admin(nome_usuario):
    global janela_admin, text_area, botao_exportar_pdf
    janela_admin = tk.Tk()
    janela_admin.title("Interface Administrativa PIM-ADS-2")
    janela_admin.geometry("600x400")

    def logout():
        janela_admin.destroy()
        tela_login()

    frame_botoes = tk.Frame(janela_admin)
    frame_botoes.pack(side=tk.LEFT, padx=(24, 12), pady=16)

    label_boas_vindas = tk.Label(
        frame_botoes,
        text=f"Bem-vindo(a), {nome_usuario}!",
        font=("Arial", 12, "bold")
    )
    label_boas_vindas.pack(pady=(24, 24))

    botao_mostrar = tk.Button(
        frame_botoes,
        text="Mostrar alunos",
        command=mostrar_alunos_interface,
        width=16
    )
    botao_mostrar.pack(pady=8)

    botao_relatorio = tk.Button(
        frame_botoes,
        text="Gerar relatórios",
        command=lambda: abrir_relatorios(
            janela_admin, text_area, verificar_conteudo),
        width=16
    )
    botao_relatorio.pack(pady=8)

    botao_exportar_pdf = tk.Button(
        frame_botoes,
        text="Exportar para PDF",
        command=lambda: exportar_para_pdf(text_area),
        width=16
    )
    botao_exportar_pdf.pack(pady=8)

    botao_limpar = tk.Button(
        frame_botoes, text="Limpar filtro", command=limpar_texto_admin, width=16)
    botao_limpar.pack(pady=8)

    botao_logout = tk.Button(
        frame_botoes, text="Sair", command=logout, width=16, bg="#5E0E0E", fg="white")
    botao_logout.pack(side=tk.BOTTOM, pady=(96, 8))

    text_area = scrolledtext.ScrolledText(janela_admin, width=80, height=32)
    text_area.pack(side=tk.LEFT, fill=tk.BOTH, expand=True,
                   padx=(12, 24), pady=16)

    text_area.bind('<KeyRelease>', verificar_conteudo)
    verificar_conteudo()
    janela_admin.mainloop()


# ============================================================
#   JANELA DO PROFESSOR
# ============================================================

def criar_janela_prof(nome_usuario):
    global text_area_prof, botao_exportar_pdf_prof, janela_prof

    janela_prof = tk.Tk()
    janela_prof.title("Interface Professor - PIM ADS 2")
    janela_prof.geometry("600x400")

    frame_botoes = tk.Frame(janela_prof)
    frame_botoes.pack(side=tk.LEFT, padx=(24, 12), pady=16, fill=tk.Y)

    label_boas_vindas = tk.Label(
        frame_botoes,
        text=f"Bem-vindo(a), Prof {nome_usuario}!",
        font=("Arial", 12, "bold")
    )
    label_boas_vindas.pack(pady=(24, 24))

    text_area_prof = scrolledtext.ScrolledText(
        janela_prof, width=90, height=32)
    text_area_prof.pack(side=tk.LEFT, fill=tk.BOTH,
                        expand=True, padx=(12, 24), pady=16)

    def verificar_conteudo_prof(event=None):
        if text_area_prof.get('1.0', tk.END).strip():
            botao_exportar_pdf_prof.config(state=tk.NORMAL)
        else:
            botao_exportar_pdf_prof.config(state=tk.DISABLED)

    botao_consultar = tk.Button(
        frame_botoes,
        text="Consultar alunos",
        command=lambda: [consultar_alunos(janela_prof, text_area_prof),
                         verificar_conteudo_prof()],
        width=16)
    botao_consultar.pack(pady=8)

    botao_notas = tk.Button(
        frame_botoes,
        text="Ver notas",
        command=lambda: [ver_notas(janela_prof, text_area_prof),
                         verificar_conteudo_prof()],
        width=16)
    botao_notas.pack(pady=8)

    botao_faltas = tk.Button(
        frame_botoes,
        text="Ver faltas",
        command=lambda: [ver_faltas(janela_prof, text_area_prof),
                         verificar_conteudo_prof()],
        width=16)
    botao_faltas.pack(pady=8)

    botao_exportar_pdf_prof = tk.Button(
        frame_botoes,
        text="Exportar para PDF",
        command=lambda: exportar_para_pdf(text_area_prof),
        width=16,
        state=tk.DISABLED)
    botao_exportar_pdf_prof.pack(pady=8)

    botao_limpar = tk.Button(
        frame_botoes, text="Limpar filtro", command=limpar_texto_prof, width=16)
    botao_limpar.pack(pady=8)

    botao_logout = tk.Button(
        frame_botoes,
        text="Sair",
        command=logout_prof,
        width=16,
        bg="#5E0E0E",
        fg="white")
    botao_logout.pack(pady=(24, 8))

    text_area_prof.bind('<KeyRelease>', verificar_conteudo_prof)
    verificar_conteudo_prof()
    janela_prof.mainloop()


# ============================================================
#   JANELA DO ALUNO
# ============================================================

def criar_janela_aluno(nome_usuario):
    janela_aluno = tk.Tk()
    janela_aluno.title(f"Boletim - {nome_usuario}")
    janela_aluno.geometry("600x400")

    frame_botoes = tk.Frame(janela_aluno)
    frame_botoes.pack(side=tk.LEFT, padx=(24, 12), pady=16)

    label_boas_vindas = tk.Label(
        frame_botoes,
        text=f"Bem-vindo(a), {nome_usuario}!",
        font=("Arial", 12, "bold")
    )
    label_boas_vindas.pack(pady=(24, 24))

    # Área de texto principal
    text_area = scrolledtext.ScrolledText(
        janela_aluno, width=80, height=32)
    text_area.pack(side=tk.LEFT, fill=tk.BOTH, expand=True,
                   padx=(12, 24), pady=16)

    # Exibe o boletim inicial (informações gerais do aluno)
    conteudo_aluno = buscar_aluno_individual(nome_usuario)
    text_area.insert(tk.END, conteudo_aluno)

    # --- Botão de VER AULAS ---
    def mostrar_aulas():
        texto_boletim = text_area.get('1.0', tk.END)
        ano_linha = next(
            (linha for linha in texto_boletim.splitlines() if linha.startswith("Ano:")), None)
        turma_linha = next(
            (linha for linha in texto_boletim.splitlines() if linha.startswith("Turma:")), None)

        if not ano_linha or not turma_linha:
            messagebox.showerror(
                "Erro", "Não foi possível identificar o ano e a turma deste aluno.")
            return

        try:
            ano_aluno = int(ano_linha.split(":")[1].strip())
        except ValueError:
            messagebox.showerror("Erro", "Ano inválido no boletim.")
            return

        turma_aluno = turma_linha.split(":")[1].strip()
        conteudo_aulas = ler_aulas_por_ano_turma(ano_aluno, turma_aluno)

        text_area.delete('1.0', tk.END)
        text_area.insert(tk.END, conteudo_aulas)

    

    # --- Botão de VER BOLETIM novamente ---
    def mostrar_boletim():
        conteudo_novamente = buscar_aluno_individual(nome_usuario)
        text_area.delete('1.0', tk.END)
        text_area.insert(tk.END, conteudo_novamente)

    botao_boletim = tk.Button(
        frame_botoes,
        text="Ver meus dados",
        command=mostrar_boletim,
        width=16)
    botao_boletim.pack(pady=8)

    botao_aulas = tk.Button(
        frame_botoes,
        text="Ver aulas",
        command=mostrar_aulas,
        width=16)
    botao_aulas.pack(pady=8)

     # --- Botão de EXPORTAR PDF ---
    botao_exportar_pdf = tk.Button(
        frame_botoes,
        text="Exportar seu boletim",
        command=lambda: exportar_para_pdf(text_area),
        width=16)
    botao_exportar_pdf.pack(pady=8)

    # --- Botão de SAIR ---
    def logout():
        janela_aluno.destroy()
        tela_login()

    botao_logout = tk.Button(
        frame_botoes,
        text="Sair",
        command=logout,
        width=16,
        bg="#5E0E0E",
        fg="white")
    botao_logout.pack(side=tk.BOTTOM, pady=(96, 8))

    janela_aluno.mainloop()

# ============================================================
#   LIMPAR TEXTOS
# ============================================================

def limpar_texto_admin():
    global text_area
    text_area.delete('1.0', tk.END)
    verificar_conteudo()


def limpar_texto_prof():
    global text_area_prof
    text_area_prof.delete('1.0', tk.END)
    verificar_conteudo_prof()


# ============================================================
#   EXECUÇÃO INICIAL
# ============================================================

tela_login()