import tkinter as tk
from tkinter import scrolledtext
from tkinter import messagebox, filedialog
from admin import abrir_relatorios, mostrar_alunos
from professor import consultar_alunos, ver_notas, ver_faltas

from reportlab.lib.pagesizes import A4
from reportlab.pdfgen import canvas


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

        # Login básico só pelo domínio no usuário
        if "@admin" in user:
            tipo_usuario = "admin"
        elif "@professor" in user:
            tipo_usuario = "professor"
        elif "@aluno" in user:
            tipo_usuario = "aluno"
        else:
            messagebox.showerror("Erro", "Usuário inválido")
            return
        
        nome_usuario = user.split("@")[0]

        messagebox.showinfo("Sucesso", f"Login ok! Tipo: {tipo_usuario}")
        login_window.destroy()
        iniciar_app(tipo_usuario, nome_usuario)

    frame_botoes = tk.Frame(login_window)
    frame_botoes.pack(side=tk.BOTTOM, pady=16)

    botao_login = tk.Button(frame_botoes, text="Entrar", command=validar_login, width=8)
    botao_login.pack(side=tk.LEFT, pady=5, padx=(0,8))

    botao_fechar = tk.Button(frame_botoes, text="Sair", command=login_window.destroy, width=8)
    botao_fechar.pack(side=tk.LEFT, padx=(8,0), pady=5)

    login_window.mainloop()

def iniciar_app(tipo_usuario, nome_usuario):
    global tipo
    tipo = tipo_usuario
    if tipo_usuario == "admin":
        criar_janela_admin(nome_usuario)
    elif tipo_usuario == "professor":
        criar_janela_prof(nome_usuario)

def logout():
    global janela_admin
    janela_admin.destroy()  #fecha a janela admin
    tela_login() #chama login dnv

def logout_prof():
    global janela_prof  # se precisar fechar a janela professor globalmente
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

            linhas = conteudo.split('\n')
            y = altura - 50  #margem superior
            espacamento = 14

            for linha in linhas:
                c.drawString(40, y, linha)
                y -= espacamento
                if y < 50:  #nova página
                    c.showPage()
                    y = altura - 50

            c.save()
            messagebox.showinfo("Sucesso", "Conteúdo exportado para PDF com sucesso!")
        except Exception as e:
            messagebox.showerror("Erro", f"Falha ao salvar PDF: {e}")

def criar_janela_admin(nome_usuario):
    global janela_admin, text_area, botao_exportar_pdf
    janela_admin = tk.Tk()
    janela_admin.title("Interface Administrativa PIM-ADS-2")
    janela_admin.geometry("600x400")

    def logout():
        janela_admin.destroy()
        tela_login()

    #CRIE O FRAME PRIMEIRO (fica fixo na esquerda da janela)
    frame_botoes = tk.Frame(janela_admin)
    frame_botoes.pack(side=tk.LEFT, padx=(24, 12), pady=16)

    label_boas_vindas = tk.Label(frame_botoes, text=f"Bem-vindo(a), {nome_usuario}!", font=("Arial", 12, "bold"))
    label_boas_vindas.pack(pady=(24, 24))

    #Botões empilhados verticalmente dentro do frame lateral
    botao_mostrar = tk.Button(frame_botoes, text="Mostrar alunos", command=mostrar_alunos_interface, width=16)
    botao_mostrar.pack(pady=8)

    botao_relatorio = tk.Button(frame_botoes, text="Gerar relatórios", command=lambda: abrir_relatorios(janela_admin, text_area,verificar_conteudo), width=16)
    botao_relatorio.pack(pady=8)

    botao_exportar_pdf = tk.Button(frame_botoes, text="Exportar para PDF", command=lambda: exportar_para_pdf(text_area), width=16)
    botao_exportar_pdf.pack(pady=8)

    botao_limpar = tk.Button(frame_botoes, text="Limpar filtro", command=limpar_texto_admin, width=16)
    botao_limpar.pack(pady=8)


    botao_logout = tk.Button(frame_botoes, text="Logout", command=logout, width=16, bg="#5E0E0E", fg="white")
    botao_logout.pack(side=tk.BOTTOM, pady=(96, 8))

    #Área de texto ocupando o restante da janela, à direita
    #widget
    text_area = scrolledtext.ScrolledText(janela_admin, width=90, height=32)
    text_area.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=(12, 24), pady=16)

    #Bind para detectar mudanças no text_area
    text_area.bind('<KeyRelease>', verificar_conteudo)

    #Verifica já ao iniciar para setar o estado correto do botão
    verificar_conteudo()

    janela_admin.mainloop()

def criar_janela_prof(nome_usuario):
    global text_area_prof, botao_exportar_pdf_prof, janela_prof

    janela_prof = tk.Tk()
    janela_prof.title("Interface Professor - PIM ADS 2")
    janela_prof.geometry("600x400")


    frame_botoes = tk.Frame(janela_prof)
    frame_botoes.pack(side=tk.LEFT, padx=(24, 12), pady=16, fill=tk.Y)


    label_boas_vindas = tk.Label(frame_botoes, text=f"Bem-vindo(a), Prof {nome_usuario}!", font=("Arial", 12, "bold"))
    label_boas_vindas.pack(pady=(24, 24))


    text_area_prof = scrolledtext.ScrolledText(janela_prof, width=90, height=32)
    text_area_prof.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=(12, 24), pady=16)


    def verificar_conteudo_prof(event=None):
        if text_area_prof.get('1.0', tk.END).strip():
            botao_exportar_pdf_prof.config(state=tk.NORMAL)
        else:
            botao_exportar_pdf_prof.config(state=tk.DISABLED)


    botao_consultar = tk.Button(frame_botoes, text="Consultar alunos", command=lambda: [consultar_alunos(janela_prof, text_area_prof), verificar_conteudo_prof()], width=16)
    botao_consultar.pack(pady=8)
    
    botao_notas = tk.Button(frame_botoes, text="Ver notas", command=lambda: [ver_notas(janela_prof, text_area_prof), verificar_conteudo_prof()], width=16)
    botao_notas.pack(pady=8)
    
    botao_faltas = tk.Button(frame_botoes, text="Ver faltas", command=lambda: [ver_faltas(janela_prof, text_area_prof), verificar_conteudo_prof()], width=16)
    botao_faltas.pack(pady=8)
    
    botao_exportar_pdf_prof = tk.Button(frame_botoes, text="Exportar para PDF", command=lambda: exportar_para_pdf(text_area_prof), width=16, state=tk.DISABLED)
    botao_exportar_pdf_prof.pack(pady=8)

    botao_limpar = tk.Button(frame_botoes, text="Limpar filtro", command=limpar_texto_prof, width=16)
    botao_limpar.pack(pady=8)

    botao_logout = tk.Button(frame_botoes, text="Logout", command=logout_prof, width=16, bg="#5E0E0E", fg="white")
    botao_logout.pack(pady=(24, 8))
    

    text_area_prof.bind('<KeyRelease>', verificar_conteudo_prof)
    verificar_conteudo_prof()

    janela_prof.mainloop()


def limpar_texto_admin():
    global text_area  #referente à janela admin
    text_area.delete('1.0', tk.END)
    verificar_conteudo()

def limpar_texto_prof():
    global text_area_prof  # referente à janela professor
    text_area_prof.delete('1.0', tk.END)
    verificar_conteudo_prof()


#Iniciar pelo login
tela_login()
