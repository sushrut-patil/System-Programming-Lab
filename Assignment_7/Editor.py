from tkinter import *
from tkinter import messagebox as message
from tkinter import filedialog as fd

class Window:
    def __init__(self):

        self.isFileOpen = True
        self.File = ""
        self.isFileChange = False
        self.elecnt = 0
        self.fileTypes = [('Text Document', '*.txt')]

        self.window = Tk()
        self.window.geometry("1200x700+200+150")
        self.window.wm_title("Untitled")

        self.TextBox = Text(self.window, highlightthickness=0, font=("Helvetica", 14))

        self.menuBar = Menu(self.window, bg="#eeeeee", font=("Helvetica", 13), borderwidth=0)
        self.window.config(menu=self.menuBar)

        self.fileMenu = Menu(self.menuBar, tearoff=0, activebackground="#d5d5e2", bg="#eeeeee", bd=2, font="Helvetica")
        self.fileMenu.add_command(label="    New       Ctrl+N", command=self.new_file, )
        self.fileMenu.add_command(label="    Open...      Ctrl+O", command=self.open_file)
        self.fileMenu.add_command(label="    Save         Ctrl+S", command=self.retrieve_input)
        self.fileMenu.add_separator()
        self.fileMenu.add_command(label="    Exit          Ctrl+D", command=self._quit)
        self.menuBar.add_cascade(label="   File   ", menu=self.fileMenu)
        
        self.editMenu = Menu(self.menuBar, tearoff=0, activebackground="#d5d5e2", bg="#eeeeee", bd=2,
                             font="Helvetica", )
        self.editMenu.add_command(label="    Cut    Ctrl+X", command=self.cut)
        self.editMenu.add_command(label="    Copy    Ctrl+C", command=self.copy)
        self.editMenu.add_command(label="    Paste   Ctrl+V", command=self.paste)
        self.menuBar.add_cascade(label="   Edit   ", menu=self.editMenu)
        
    def new_file(self):
        self.TextBox.config(state=NORMAL)
        if self.isFileOpen:
            if len(self.File) > 0:
                if self.isFileChange:
                    self.save_file(self.File)
                self.window.wm_title("Untitled")
                self.TextBox.delete('1.0', END)
                self.File = ''
            else:
                if self.isFileChange:
                    result = message.askquestion('Window Title', 'Do You Want to Save Changes')
                    self.save_new_file(result)
                self.window.wm_title("Untitled")
                self.TextBox.delete('1.0', END)
        else:
            self.isFileOpen = True
            self.window.wm_title("Untitled")

        self.isFileChange = False

    def open_file(self):
        self.TextBox.config(state=NORMAL)
        if self.isFileOpen and self.isFileChange:
            self.save_file(self.File)
        filename = fd.askopenfilename(filetypes=self.fileTypes, defaultextension=".txt")
        if len(filename) != 0:
            self.isFileChange = False
            outfile = open(filename, "r")
            text = outfile.read()
            self.TextBox.delete('1.0', END)
            self.TextBox.insert(END, text)
            self.window.wm_title(filename)
            self.isFileOpen = True
            self.File = filename

    def save_file(self, file):
        result = message.askquestion('Window Title', 'Do You Want to Save Changes')
        if result == "yes":
            if len(file) == 0:
                saveFile = fd.asksaveasfile(filetypes=self.fileTypes, defaultextension=".txt")
                print(saveFile.name)
                self.write_file(saveFile.name)
                self.TextBox.delete('1.0', END)
            else:
                self.write_file(file)

    def save_new_file(self, result):
        self.isFileChange = False
        if result == "yes":
            saveFile = fd.asksaveasfile(filetypes=self.fileTypes, defaultextension=".txt")
            self.write_file(saveFile.name)
            self.File = saveFile.name
        else:
            self.TextBox.delete('1.0', END)

    def write_file(self, file):
        inputValue = self.TextBox.get("1.0", "end-1c")
        outfile = open(file, "w")
        outfile.write(inputValue)

    def retrieve_input(self):
        if self.isFileOpen and len(self.File) != 0:
            self.write_file(self.File)
            self.isFileChange = False
        else:
            self.save_new_file("yes")
            self.window.wm_title(self.File)
            self.isFileOpen = True

    def _quit(self):
        self.window.quit()
        self.window.destroy()

    def copy(self):
        self.TextBox.clipboard_clear()
        text = self.TextBox.get("sel.first", "sel.last")
        self.TextBox.clipboard_append(text)

    def cut(self):
        self.copy()
        self.TextBox.delete("sel.first", "sel.last")

    def paste(self):
        text = self.TextBox.selection_get(selection='CLIPBOARD')
        self.TextBox.insert('insert', text)

if __name__ == "__main__":

    TextEditor = Window()
    TextEditor.TextBox.pack(expand=1, fill="both")
    TextEditor.window.mainloop()