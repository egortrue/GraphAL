####################################################################
# Standart imports

import PySimpleGUI as psg

from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib import pyplot as plt

import networkx as nx

from json import (load as jsonload, dump as jsondump)

import pyautogui as pag
from time import sleep

####################################################################
# Our imports

from constants import *
import graphs
import algorithms

import sys
sys.path.append("./interface")
from main_tab import main_tab
from preview_tab import preview_tab
from algorithms_tab import algorithms_tab
from log_tab import log_tab

####################################################################



class NavigationToolbar(NavigationToolbar2Tk):  # edited NavigationToolbar2Tk
    NavigationToolbar2Tk.toolitems = (
        ('Home', 'Reset original view', 'home', 'home'),
        ('Back', 'Back to previous view', 'back', 'back'), # remove this button
        ('Forward', 'Forward to next view', 'forward', 'forward'), # remove this button
        (None, None, None, None),
        ('Pan', 'Pan axes with left mouse, zoom with right', 'move', 'pan'),
        ('Zoom', 'Zoom to rectangle', 'zoom_to_rect', 'zoom'),
        # ('Subplots', 'Configure subplots', 'subplots', 'configure_subplots'),
        # (None, None, None, None), # remove this separate
        # ('Save', 'Save the figure', 'file_save', 'save_figure'), # remove this button
    )


class App(psg.Window):
    def __init__(self):

        psg.theme('DefaultNoMoreNagging')

        menu_def = [['File', ['Open', 'Save', 'Clear', '---', 'Exit']],
                    ['Help', ['About...']], ]

        super().__init__(title="GraphAL",
                         layout=[
                             [psg.Menu(menu_def, key='-MENU-')],
                             [psg.TabGroup(layout=[[psg.Tab('Main', main_tab),
                                                    psg.Tab('Preview', preview_tab, element_justification='right'),
                                                    psg.Tab('Log', log_tab, element_justification='left'),
                                                    psg.Tab('Algorithms', algorithms_tab)]],
                                           tab_location="topleft",
                                           background_color='#f0f0f0',  # костыль
                                           key='-TAB-GROUP-'
                                           )]
                         ],
                         icon=".\interface\pictures\GraphAL.ico",
                         size=(1300, 745),
                         finalize=True
                         )

        # default settings graph elements
        self.DEFAULT_SETTINGS_GRAPH_ELEMENT_KEYS = {'-NODES-IN-': '',
                                                    '-EDGES-IN-': '',
                                                    '-DEGREE-IN-': '',
                                                    '-DIRECTED-Y-': False,
                                                    '-DIRECTED-N-': True,
                                                    '-WEIGHTED-Y-': False,
                                                    '-WEIGHTED-N-': True,
                                                    '-MIN-WEIGHT-IN-': '0',
                                                    '-MAX-WEIGHT-IN-': '100'}

        # default algorithms elements
        self.DEFAULT_ALGORITHM_ELEMENT_KEYS = {'-COMBO-ALGORITHMS-': '--select--',
                                               '-EXPLANATION-ALGORITHM-': '',
                                               '-START-NODE-IN-': '',
                                               '-FINISH-NODE-IN-': ''}

        # default preview elements
        self.DEFAULT_PREVIEW_ELEMENT_KEYS = {'-MATRIX-ADJ-': '',
                                             '-LIST-ADJ-': '',
                                             '-LIST-EDGES-': ''}

        self.DEFAULT_LOG_ELEMENT_KEYS = {'-LOG-OUT-': '',
                                         '-LOG-ALGORITHM-': ''}

        self.callback_of_events = {'-WEIGHTED-Y-': lambda: self.range_of_weight_visible(True),
                                   '-WEIGHTED-N-': lambda: self.range_of_weight_visible(False),
                                   '-GENERATE-': self.btn_generate_graph,
                                   '-START-': self.start_algorithm,
                                   '-COMBO-ALGORITHMS-': self.choice_algorithm,
                                   '-COMBO-DEFINITION-ALGORITHMS-': self.choice_definition_algorithm,
                                   'Open': self.open_file,
                                   'Save': self.save_file,
                                   'Clear': self.clear_file,
                                   'Exit': self.close}

        self.func_algorithms = {"DFS": algorithms.DFS,
                                "BFS": algorithms.BFS,
                                "Dijkstra's Algorithm": algorithms.Dijkstra,
                                "Ford-Bellman Algorithm": algorithms.BellmanFord,
                                #"Floyd–Warshall Algorithm": self.passing,
                                "Prim's Algorithm": algorithms.Prim,
                                "Kruskal's Algorithm": algorithms.Kruskal,
                                "Ford–Fulkerson Algorithm": algorithms.FordFulkerson}

        self.fig_agg, self.ax = self.create_figure_canvas()
        self.draw_graph_flag = False

        self.graph = None

    def btn_generate_graph(self):

        if not self.check_settings_frame():
            return

        if self.graph:
            graphs.gen_dll.AMatrixDelete(self.graph.matrix_ptr)
            graphs.alg_dll.GraphDestroy(self.graph.ptr)

        self.clear_figure_canvas()
        self.clear_preview()
        self.clear_log_tab()

        info = 0

        nodes_num = int(self['-NODES-IN-'].get())
        edges_num = int(self['-EDGES-IN-'].get())

        min_weight = 1
        max_weight = 1

        max_degree = nodes_num * 3  # default value

        if self['-DIRECTED-Y-'].get():
            info += 1

        if self['-WEIGHTED-Y-'].get():
            info += 2

            min_weight = int(self['-MIN-WEIGHT-IN-'].get())
            max_weight = int(self['-MAX-WEIGHT-IN-'].get())

        if self['-DEGREE-IN-'].get():
            max_degree = int(self['-DEGREE-IN-'].get())

        self.graph = graphs.generate_graph(nodes_num, edges_num,
                                           max_degree,
                                           info,
                                           min_weight, max_weight)
        if not self.graph:
            self.window_error("Граф не был сгенерирован")
            return

        self.preview(self.graph)
        self.draw_graph()

    def preview(self, graph):

        self['-MATRIX-ADJ-'].update(value=graphs.adjacency_matrix_to_preview(graph))
        self['-LIST-ADJ-'].update(value=graphs.adjacency_list_to_preview(graph))
        self['-LIST-EDGES-'].update(value=graphs.list_edges_to_preview(graph))

    def create_figure_canvas(self):

        canvas_fig = self['-CANVAS-'].TKCanvas
        canvas_toolbar = self['-TOOLBAR-'].TKCanvas

        # draw the initial plot in the window
        figure = plt.figure(figsize=(9.75, 6))
        ax = figure.add_axes([0, 0, 1, 1])

        figure_canvas_agg = FigureCanvasTkAgg(figure, canvas_fig)
        figure_canvas_agg.draw()
        toolbar = NavigationToolbar(figure_canvas_agg, canvas_toolbar)
        toolbar.update()

        figure_canvas_agg.get_tk_widget().pack(side='top', fill='both')

        return figure_canvas_agg, ax

    def draw_graph(self):

        self.clear_figure_canvas()
        self.draw_graph_flag = True

        nx.draw_networkx(self.graph.nx_graph,
                         pos=self.graph.pos,
                         ax=self.ax,
                         arrows=True,
                         with_labels=False,
                         node_color=self.graph.nodes_color,
                         edge_color=self.graph.edges_color,
                         node_size=800,
                         width=2)

        nx.draw_networkx_labels(self.graph.nx_graph,
                                pos=self.graph.pos,
                                ax=self.ax,
                                labels=self.graph.nodes_labels)

        if self.graph.weighted:
            nx.draw_networkx_edge_labels(self.graph.nx_graph,
                                         pos=self.graph.pos,
                                         ax=self.ax,
                                         edge_labels=nx.get_edge_attributes(self.graph.nx_graph, 'weight'))

        self.fig_agg.draw()

    def start_algorithm(self):

        if not self.check_algorithm_frame():
            return

        self.clear_log_tab()

        algorithm = self['-COMBO-ALGORITHMS-'].get()

        if algorithm in ("DFS", "BFS", "Dijkstra's Algorithm", "Ford-Bellman Algorithm", "Prim's Algorithm"):
            start_node = int(self['-START-NODE-IN-'].get())
            self.func_algorithms[algorithm](self, self.graph.nodes[start_node - 1])

        elif algorithm == "Ford–Fulkerson Algorithm":
            # Create flow network
            flow_network_ptr = graphs.gen_dll.RandFlowNetwork(self.graph.matrix_ptr, 1, 100, self.graph.nodes_num * 3)
            flow_network = []

            # Translate int** from DLL to matrix with ints on Python
            arr_ptr_int = ct.cast(ptr.contents.C_ADJ, ct.POINTER(ct.POINTER(ct.c_int)*self.graph.nodes_num+2)).contents
            for i in range(nodes_num):
                flow_network.append(ct.cast(arr_ptr_int[i], ct.POINTER(ct.c_int * self.graph.nodes_num+2)).contents)

            graphs.gen_dll.AMatrixDelete(self.graph.matrix_ptr)
            graphs.alg_dll.GraphDestroy(self.graph.ptr)


            self.graph = graphs.convert_from_matrix_to_graph(flow_network, self.graph.min_weight, self.graph.max_weight, 0b11)
            self.draw_graph()
            self.func_algorithms[algorithm](self)

        else:
            self.func_algorithms[algorithm](self)

        self['-LOG-ALGORITHM-'].update(value=algorithm)
        self.graph.restore_nodes_colors()
        self.graph.restore_edges_colors()
        self.graph.restore_nodes_labels()

    def write_to_log_out(self, ind, info='nodes'):

        output = self['-LOG-OUT-']
        output.update(value=f"{ind} итерация: ", append=True)

        # write nodes
        if info == 'nodes':
            for i in range(self.graph.nodes_num):
                node = self.graph.nodes[i].node_id
                color = self.graph.nodes_color[i]

                if color == self.graph.nodes_default_color:
                    color = 'black'

                output.update(value=f"{node}    ", append=True, text_color_for_value=color)

        # write edges
        else:
            for i in range(self.graph.edges_num):
                edge = self.graph.edges[i]
                color = self.graph.edges_color[i]

                if color == self.graph.edges_default_color:
                    color = 'black'

                output.update(value=f"{edge}  ", append=True, text_color_for_value=color)

        output.update(value="\n", append=True)

    def clear_file(self):

        self.clear_figure_canvas()
        self.clear_settings_graph()
        self.clear_algorithms()
        self.clear_preview()
        self.clear_log_tab()
        self.clear_algorithms_tab()

        self.graph = None

    def clear_figure_canvas(self):

        self.draw_graph_flag = False
        self.ax.clear()
        self.fig_agg.draw()

    def clear_settings_graph(self):

        for key, value in self.DEFAULT_SETTINGS_GRAPH_ELEMENT_KEYS.items():
            self[key].update(value=value)

        self.range_of_weight_visible(visible=False)

    def clear_preview(self):

        for key, value in self.DEFAULT_PREVIEW_ELEMENT_KEYS.items():
            self[key].update(value=value)

    def clear_algorithms(self):

        for key, value in self.DEFAULT_ALGORITHM_ELEMENT_KEYS.items():
            self[key].update(value=value)

        self.parameters_of_algorithms_visible(visible=False)

    def clear_algorithms_tab(self):
        self['-COMBO-DEFINITION-ALGORITHMS-'].update(value='--select--')
        self['-DEFINITION-'].update(value="")
        self['-PIC-CODE-'].update(filename="")

    def clear_log_tab(self):
        self['-LOG-OUT-'].update(value='')
        self['-LOG-ALGORITHM-'].update(value='')

    def range_of_weight_visible(self, visible: bool):

        if visible:

            self['-MIN-WEIGHT-LABEL-'].update(value='Min weight:*')
            self['-MAX-WEIGHT-LABEL-'].update(value='Max weight:*')

        else:

            self['-MIN-WEIGHT-LABEL-'].update(value='')
            self['-MAX-WEIGHT-LABEL-'].update(value='')

        self['-MIN-WEIGHT-IN-'].update(value='0', visible=visible)
        self['-MAX-WEIGHT-IN-'].update(value='100', visible=visible)

    def start_node_visible(self, visible: bool):

        if visible:
            self['-START-NODE-LABEL-'].update(value='Start node:')

        else:
            self['-START-NODE-LABEL-'].update(value='')

        self['-START-NODE-IN-'].update(value='', visible=visible)

    def finish_node_visible(self, visible: bool):

        if visible:
            self['-FINISH-NODE-LABEL-'].update(value='Finish node:')

        else:
            self['-FINISH-NODE-LABEL-'].update(value='')

        self['-FINISH-NODE-IN-'].update(value='', visible=visible)

    def parameters_of_algorithms_visible(self, visible: bool):

        self.start_node_visible(visible)
        self.finish_node_visible(visible)

    def choice_algorithm(self):

        pag.press('left')  # неизбежный костыль

        self.parameters_of_algorithms_visible(False)

        algorithm = self['-COMBO-ALGORITHMS-'].get()

        algorithms = {"--select--": "",
                      "DFS": dfs_main_tab,
                      "BFS": bfs_main_tab,
                      "Dijkstra's Algorithm": Dijkstra_algorithm_main_tab,
                      "Ford-Bellman Algorithm": Ford_Bellman_algorithm_main_tab,
                      "Floyd–Warshall Algorithm": Floyd_Warshall_algorithm_main_tab,
                      "Prim's Algorithm": Prim_algorithm_main_tab,
                      "Kruskal's Algorithm": Kruskal_algorithm_main_tab,
                      "Ford–Fulkerson Algorithm": Ford_Fulkerson_algorithm_main_tab, }

        if algorithm in ("DFS", "BFS", "Dijkstra's Algorithm", "Ford-Bellman Algorithm", "Prim's Algorithm"):

            self.start_node_visible(True)

        self['-EXPLANATION-ALGORITHM-'].update(value=algorithms[algorithm])

    def choice_definition_algorithm(self):

        pag.press('left')  # неизбежный костыль

        algorithms = {"--select--": "",
                      "DFS": [dfs_algorithms_tab, code_dfs],
                      "BFS": [bfs_algorithms_tab, code_bfs],
                      "Dijkstra's Algorithm": [Dijkstra_algorithm_algorithms_tab, code_Dijkstra],
                      "Ford-Bellman Algorithm": [Ford_Bellman_algorithm_algorithms_tab, code_Ford_Bellman],
                      "Floyd–Warshall Algorithm": [Floyd_Warshall_algorithm_algorithms_tab, code_Floyd_Warshall],
                      "Prim's Algorithm": [Prim_algorithm_algorithms_tab, code_Prim],
                      "Kruskal's Algorithm": [Kruskal_algorithm_algorithms_tab, code_Kruskal],
                      "Ford–Fulkerson Algorithm": [Ford_Fulkerson_algorithm_algorithms_tab, code_Ford_Fulkerson]}

        algorithm = self['-COMBO-DEFINITION-ALGORITHMS-'].get()

        self['-DEFINITION-'].update(value=algorithms[algorithm][0])

        self['-PIC-CODE-'].update(filename=algorithms[algorithm][1])

    def save_file(self):
        settings = dict()

        if self.graph is not None:
            settings = self.graph.save_graph()

        for key in self.DEFAULT_SETTINGS_GRAPH_ELEMENT_KEYS:  # save settings graph element
            settings[key] = self[key].get()

        settings['draw_graph_flag'] = self.draw_graph_flag

        try:
            with open(psg.popup_get_file("", file_types=(("JSON File", "*.json"),), no_window=True, save_as=True),
                                          'w') as f:
                json.dump(settings, f)
        except FileNotFoundError:  # when user didn't choose file
            pass

    def open_file(self):

        try:
            settings = dict()

            with open(psg.popup_get_file("", file_types=(("JSON File", "*.json"),), no_window=True), 'r') as f:
                settings = json.load(f)
        except FileNotFoundError:  # when user didn't choose file
            return

        if 'matrix' in settings:
            matrix = settings['matrix']
            info = settings['info']
            min_weight = settings['min_weight']
            max_weight = settings['max_weight']

            self.graph = graphs.convert_from_matrix_to_graph(matrix, info, min_weight, max_weight)

            # после хранения в pos  ключи стали объектом str, а надо int,

            pos = dict()
            for key in settings['pos']:
                pos[int(key)] = settings['pos'][key].copy()

            self.graph.pos = pos

            if settings['draw_graph_flag']:
                self.draw_graph()

        for key in self.DEFAULT_SETTINGS_GRAPH_ELEMENT_KEYS:
            self[key].update(value=settings[key])

        self.range_of_weight_visible(settings['-WEIGHTED-Y-'])

    def window_error(self, text_error: str):

        text_error = '\n' + text_error + '\n'

        psg.theme('Default1')
        window = psg.Window(title="Error", element_justification='center',
                            icon=r".\interface\pictures\icon_error.ico")
        window.AddRow(psg.Text(text=text_error, font=FONT_14, text_color='red'))
        window.AddRow(
            psg.Button(button_text='OK', button_color=psg.DEFAULT_ERROR_BUTTON_COLOR, font=FONT_14, size=(6, 0)))

        while True:
            event, value = window.read()
            if event in (psg.WIN_CLOSED, 'OK'):
                break
        window.close()

    def check_settings_frame(self):  # обработать степень

        nodes_num = None
        edges_num = None

        try:
            # ------- Necessaries fields ------- #
            if self['-NODES-IN-'].get() and self['-EDGES-IN-'].get():
                nodes_num = int(self['-NODES-IN-'].get())
                edges_num = int(self['-EDGES-IN-'].get())

            else:
                self.window_error("Заполните обязательные поля(*)!")
                return False

            if nodes_num <= 1 or edges_num < nodes_num - 1 or edges_num > (nodes_num - 1) * nodes_num / 2:
                raise ValueError

            # -------- Optional fields --------- #
            max_degree = nodes_num * 3  # default value

            if self['-DEGREE-IN-'].get():
                max_degree = int(self['-DEGREE-IN-'].get())

            if self['-DIRECTED-N-'].get() and nodes_num * max_degree < 2 * edges_num:
                raise ValueError

            if self['-DIRECTED-Y-'].get() and nodes_num * max_degree < edges_num:
                raise ValueError

            if self['-WEIGHTED-Y-'].get():
                min_weight = int(self['-MIN-WEIGHT-IN-'].get())
                max_weight = int(self['-MAX-WEIGHT-IN-'].get())

                if min_weight > max_weight:
                    raise ValueError

        except ValueError as error:
            self.window_error("Введите корректные значения!")
            return False

        return True

    def check_algorithm_frame(self):

        start_node = None
        finish_node = None

        if self.graph is None:
            self.window_error("Сгенерируйте граф!")
            return False

        algorithm = self['-COMBO-ALGORITHMS-'].get()

        if algorithm == "--select--":
            self.window_error("Выберите алгоритм")
            return False

        # error checking algorithms and settings graph
        if algorithm == 'Ford–Fulkerson Algorithm' and not self.graph.directed:
            self.window_error("Граф должен быть ориентированным!")
            return False

        if (algorithm in ("Prim's Algorithm", "Kruskal's Algorithm")) and self.graph.directed:
            self.window_error("Граф должен быть неориентированным!")
            return False

        if algorithm == "Dijkstra's Algorithm" and self.graph.min_weight < 0:
            self.window_error("Граф должен быть с положительными весами!")
            return False

        if algorithm in ("DFS", "BFS") and self.graph.weighted:
            self.window_error("Граф должен быть невзвешенным!")
            return False

        # error checking in input fields
        if algorithm in ("DFS", "BFS", "Dijkstra's Algorithm", "Ford-Bellman Algorithm", "Prim's Algorithm"):
            try:

                if self['-START-NODE-IN-'].get():
                    start_node = int(self['-START-NODE-IN-'].get())
                else:
                    self.window_error("Укажите стартовую вершину!")
                    return False

                if start_node not in self.graph.get_arr_nodes():
                    self.window_error("Выбранной вершины нет в графе!")
                    return False

            except ValueError:
                self.window_error("Введите корректные значения!")
                return False

        return True

def main():

    app = App()

    while True:
        event, values = app.read()
        print(event, values)

        if event in app.callback_of_events:
            app.callback_of_events[event]()
        elif event in (None, 'Exit'):
            break
        else:
            break

    app.close()


if __name__ == "__main__":
    main()
