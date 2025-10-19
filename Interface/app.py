import tkinter as tk
from tkinter import scrolledtext
from tkinter import messagebox, filedialog
from admin import abrir_relatorios, mostrar_alunos

from reportlab.lib.pagesizes import A4
from reportlab.pdfgen import canvas


def tela_login():
    login_window = tk.Tk()
    login_window.title("Login")
    login_window.geometry("300x150")

    tk.Label(login_window, text="Usuário (ex: nome@admin)").pack(pady=5)
    entry_user = tk.Entry(login_window)
    entry_user.pack()

    tk.Label(login_window, text="Senha").pack(pady=5)
    entry_pass = tk.Entry(login_window, show="*")
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

    btn_login = tk.Button(login_window, text="Entrar", command=validar_login)
    btn_login.pack(pady=16)

    login_window.mainloop()


def iniciar_app(tipo_usuario, nome_usuario):
    global tipo
    tipo = tipo_usuario
    criar_janela_principal(nome_usuario)

#defini como global porque todo mundo vai precisar dela
def verificar_conteudo(event=None):
    global text_area, botao_exportar_pdf
    if text_area.get('1.0', tk.END).strip():
        botao_exportar_pdf.config(state=tk.NORMAL)
    else:
        botao_exportar_pdf.config(state=tk.DISABLED)


def mostrar_alunos_interface():
    conteudo = mostrar_alunos()
    text_area.delete('1.0', tk.END)
    text_area.insert(tk.END, conteudo)
    verificar_conteudo()
    


def exportar_para_pdf():
    conteudo = text_area.get('1.0', 'end').strip()
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


def criar_janela_principal(nome_usuario):
    global janela, text_area, botao_exportar_pdf
    janela = tk.Tk()
    janela.title("Interface PIM-ADS-2")
    janela.geometry("600x400")

    #CRIE O FRAME PRIMEIRO (fica fixo na esquerda da janela)
    frame_botoes = tk.Frame(janela)
    frame_botoes.pack(side=tk.LEFT, padx=(24, 12), pady=16)

    label_boas_vindas = tk.Label(frame_botoes, text=f"Bem-vindo(a), {nome_usuario}!", font=("Arial", 12, "bold"))
    label_boas_vindas.pack(pady=(24, 24))

    #Botões empilhados verticalmente dentro do frame lateral
    botao_mostrar = tk.Button(frame_botoes, text="Mostrar alunos", command=mostrar_alunos_interface, width=16)
    botao_mostrar.pack(pady=8)

    botao_relatorio = tk.Button(frame_botoes, text="Gerar relatórios", command=lambda: abrir_relatorios(janela, text_area,verificar_conteudo), width=16)
    botao_relatorio.pack(pady=8)

    botao_exportar_pdf = tk.Button(frame_botoes, text="Exportar para PDF", command=exportar_para_pdf, width=16)
    botao_exportar_pdf.pack(pady=8)

    #tá aqui por enquanto
    botao_limpar = tk.Button(frame_botoes, text="Limpar filtro", command=limpar_texto, width=16)
    botao_limpar.pack(pady=8)

    #Área de texto ocupando o restante da janela, à direita
    #widget
    text_area = scrolledtext.ScrolledText(janela, width=90, height=32)
    text_area.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=(12, 24), pady=16)

    #Bind para detectar mudanças no text_area
    text_area.bind('<KeyRelease>', verificar_conteudo)

    #Verifica já ao iniciar para setar o estado correto do botão
    verificar_conteudo()

    janela.mainloop()


def limpar_texto():
    text_area.delete('1.0', tk.END)
    verificar_conteudo()



#Iniciar pelo login
tela_login()
