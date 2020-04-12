import tkinter as tk
from tkinter import ttk


class GenerationGraph(tk.Frame):
    def __init__(self, root):
        super().__init__(root)  # , bg='#d7d8e0', bd=4) #подобрать цвет рамки
        self.init_shape()
        self.init_button()

    def init_shape(self):
        """create label, entry, text and combobox widgets"""
        # интервал между лейблами 30 х=0; между лейблом и энтри 150 а у как у лейбла
        # возвращать все энтри и комбобоксы для реагирования после нажатия кнопки, сделать отдельные атрибуты
        setting_frame = tk.Frame(self, bg='#d7d8e0', bd=4)
        setting_label = tk.Label(setting_frame, text="Генерация графа", font="Arial 14", bg='#d7d8e0')
        setting_label.grid(row=0, columnspan=2)

        amount_vertex_label = tk.Label(setting_frame, text="Кол-во вершин:", font="Arial 12", bg='#d7d8e0')
        amount_vertex_label.grid(row=1, sticky="w")

        amount_edge_label = tk.Label(setting_frame, text="Кол-во ребер:", font="Arial 12", bg='#d7d8e0')
        amount_edge_label.grid(row=2, sticky="w")

        max_power_vertex_label = tk.Label(setting_frame, text="Макс. степень вершины:", font="Arial 12", bg='#d7d8e0')
        max_power_vertex_label.grid(row=3, sticky="w")

        connect_graph_label = tk.Label(setting_frame, text="Связность:", font="Arial 12", bg='#d7d8e0')
        connect_graph_label.grid(row=4, sticky="w")

        oriented_graph_label = tk.Label(setting_frame, text="Ориетированность:", font="Arial 12", bg='#d7d8e0')
        oriented_graph_label.grid(row=5, sticky="w")

        type_show_graph_label = tk.Label(setting_frame, text="Тип представления:", font="Arial 12", bg='#d7d8e0')
        type_show_graph_label.grid(row=6, sticky="w")

        self.amount_vertex_entry = tk.Entry(setting_frame, bd=2)
        self.amount_vertex_entry.grid(row=1, column=1, sticky="e")

        self.amount_edge_entry = tk.Entry(setting_frame, bd=2)
        self.amount_edge_entry.grid(row=2, column=1, sticky="e")

        self.max_power_vertex_entry = tk.Entry(setting_frame, bd=2)
        self.max_power_vertex_entry.grid(row=3, column=1, sticky="e")

        self.connect_graph_combobox = ttk.Combobox(setting_frame, values=["Несвязный", "Связный"], state="readonly",
                                              width=17)
        self.connect_graph_combobox.current(0)
        self.connect_graph_combobox.grid(row=4, column=1, sticky="e")

        self.oriented_graph_combobox = ttk.Combobox(setting_frame, values=["Неориентированный", "Ориентированный"],
                                               state="readonly",
                                               width=17)
        self.oriented_graph_combobox.current(0)
        self.oriented_graph_combobox.grid(row=5, column=1, sticky="e")

        self.type_show_graph_combobox = ttk.Combobox(setting_frame, values=["Матрица смежности", "Список ребер",
                                                "Список смежности"],
                                                state="readonly", width=17)
        self.type_show_graph_combobox.current(0)
        self.type_show_graph_combobox.grid(row=6, column=1, sticky="e")

        setting_frame.pack(side='top')

        space_label1 = tk.Label(self, text=" ")
        space_label1.pack()

        preview_frame = tk.Frame(self, bg='#d7d8e0', bd=4)

        preview_label = tk.Label(preview_frame, text="Превью", font="Ariel 14", bg='#d7d8e0')
        preview_label.pack(side=tk.TOP)

        self.preview_text = tk.Text(preview_frame, font="Arial 12", width=34, height=22)
        self.preview_text.pack(side=tk.LEFT)

        preview_frame.pack(side='top')

        space_label2 = tk.Label(self, text=" ")
        space_label2.pack()

    def init_button(self):
        """create button in frame generation graph"""

        """ в command  кладу функцию в которой создаю экзмепляр класса, отвечающего за кнопки (другой модуль) 
            передаю ему рут = селф и смогу обращаться ко всем энтри и комбоксам
        """

        generation_graph_button = tk.Button(self, text="Генерация графа", font="Ariel 14", bg='#d7d8e0', width=28)
        generation_graph_button.pack()

class DefinitionAlgorithms(tk.Frame):
    def __init__(self, root):
        super().__init__(root)
        self.init_shape()
        self.init_button()

    def init_shape(self):
        """ Create Combobox, label and Text widgets"""
        frame = tk.Frame(self, bg='#d7d8e0', bd=3)
        title = tk.Label(frame, text="Описание алгоритмов", width=33, font="Ariel 14", bg='#d7d8e0')
        title.pack(side='top')

        algorithms_combobox = ttk.Combobox(frame, values=["first", "second", "third", "fourth"], state="readonly",
                                           width=45, justify="center")
        algorithms_combobox.current(0)
        algorithms_combobox.pack(side="top")

        algorithms_text = tk.Text(frame, font="Arial 12", width=40, height=8)
        algorithms_text.pack(side=tk.TOP)

        frame.pack(side='top')

        space_label = tk.Label(self, text=" ")
        space_label.pack()


    def init_button(self):
        """create button in frame definitions algorithms"""

        algorithms_button = tk.Button(self, text="Описать алгоритм", font="Ariel 14", bg='#d7d8e0', width=33)
        algorithms_button.pack(side="bottom")

class FrameVisualizationGraph(tk.Frame):
    def __init__(self, root):
        super().__init__(root)
        self.init_main()

    def init_main(self):
        window_visualization = tk.Text(self, width=104, height=25)
        window_visualization.pack()


def test():
    root = tk.Tk()
    root.geometry("1200x720")
    root.resizable(False, False)
    root.title(string="Visualization graph's algorithms")

    setting = GenerationGraph(root)
    setting.place(x=5, y=5)

    definition_algorithms = DefinitionAlgorithms(root)
    definition_algorithms.place(x=345, y=438)

    window_visualization = FrameVisualizationGraph(root)
    window_visualization.place(x=345, y=5)

    root.mainloop()


if __name__ == "__main__":
    test()