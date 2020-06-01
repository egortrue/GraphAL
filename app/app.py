from main_tab import main_tab
from preview_tab import preview_tab
from function_graph import *
from text_for_algorithms import *
from constants import *
import PySimpleGUI as psg
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib import pyplot as plt
from json import (load as jsonload, dump as jsondump)
import networkx as nx

import pyautogui as pag
from time import sleep


class NavigationToolbar(NavigationToolbar2Tk):  # edited NavigationToolbar
    NavigationToolbar2Tk.toolitems = (
        ('Home', 'Reset original view', 'home', 'home'),
        # ('Back', 'Back to previous view', 'back', 'back'), # remove this button
        # ('Forward', 'Forward to next view', 'forward', 'forward'), # remove this button
        (None, None, None, None),
        ('Pan', 'Pan axes with left mouse, zoom with right', 'move', 'pan'),
        ('Zoom', 'Zoom to rectangle', 'zoom_to_rect', 'zoom'),
        ('Subplots', 'Configure subplots', 'subplots', 'configure_subplots'),
        # (None, None, None, None), # remove this separate
        # ('Save', 'Save the figure', 'file_save', 'save_figure'), # remove this button
    )


class App(psg.Window):
    def __init__(self):
        psg.theme('DefaultNoMoreNagging')

        menu_def = [['File', ['Open', 'Save', 'Clear', '---', 'Exit']],
                    ['Help', ['About...']], ]

        super().__init__(title="Graph AL",
                         layout=[
                             [psg.Menu(menu_def, key='-MENU-')],
                             [psg.TabGroup(layout=[[psg.Tab('Main', main_tab),
                                                    psg.Tab('Preview', preview_tab, element_justification='right'),
                                                    psg.Tab('Log', [[]]),
                                                    psg.Tab('Algorithms', [[]])]],
                                           tab_location="topleft",
                                           background_color='#f0f0f0',  # костыль
                                           key='-TAB-GROUP-'
                                           )]
                         ],
                         size=(1300, 745),
                         finalize=True
                         )

        # default settings graph elements
        self.DEFAULT_SETTINGS_GRAPH_ELEMENT_KEYS = {'-VERTEX-IN-': '',
                                                    '-EDGES-IN-': '',
                                                    '-DEGREE-IN-': '',
                                                    '-CONNECTED-Y-': False,
                                                    '-CONNECTED-N-': True,
                                                    '-DIRECTED-Y-': False,
                                                    '-DIRECTED-N-': True,
                                                    '-WEIGHTED-Y-': False,
                                                    '-WEIGHTED-N-': True,
                                                    '-MIN-WEIGHT-IN-': '0',
                                                    '-MAX-WEIGHT-IN-': '100'}

        # default algorithms elements
        self.DEFAULT_ALGORITHM_ELEMENT_KEYS = {'-COMBO-ALGORITHMS-': '--select--',
                                               '-EXPLANATION-ALGORITHM-': '',
                                               '-START-VERTEX-IN-': '',
                                               '-FINISH-VERTEX-IN-': ''}

        # default preview elements
        self.DEFAULT_PREVIEW_ELEMENT_KEYS = {'-MATRIX-ADJ-': '',
                                             '-LIST-ADJ-': '',
                                             '-LIST-EDGES-': ''}

        self.vertexes = 0
        self.edges = 0
        self.max_degree = 0
        self.connected = False
        self.directed = False
        self.weighted = False
        self.min_weighted = 0
        self.max_weighted = 100

        self.fig_agg, self.ax = self.create_figure_canvas()
        self.draw_graph_flag = False

        self.graph = nx.Graph()
        self.pos = nx.spring_layout(self.graph)

        self.callback_of_events = {'-WEIGHTED-Y-': lambda: self.range_of_weight_visible(True),
                                   '-WEIGHTED-N-': lambda: self.range_of_weight_visible(False),
                                   '-GENERATE-': self.generate_graph,
                                   '-START-': self.start_algorithm,
                                   '-CLEAR-': self.clear_figure_canvas,
                                   '-COMBO-ALGORITHMS-': self.choice_algorithm,
                                   'Open': self.open_file,
                                   'Save': self.save_file,
                                   'Clear': self.clear_file}

    def create_figure_canvas(self):
        canvas_fig = self['-CANVAS-'].TKCanvas
        canvas_toolbar = self['-TOOLBAR-'].TKCanvas

        # draw the initial plot in the window
        figure = plt.figure(figsize=(9.75, 5.5))
        ax = figure.add_axes([0, 0, 1, 1])

        figure_canvas_agg = FigureCanvasTkAgg(figure, canvas_fig)
        figure_canvas_agg.draw()
        toolbar = NavigationToolbar(figure_canvas_agg, canvas_toolbar)
        toolbar.update()

        figure_canvas_agg.get_tk_widget().pack(side='top', fill='both')

        return figure_canvas_agg, ax

    def generate_graph(self):
        self.clear_figure_canvas()

        self.graph, self.pos = create_graph()

        preview(self, self.graph, self['-WEIGHTED-Y-'].get())

        draw_graph(self, self.fig_agg, self.graph, self.pos, self.ax)

    def clear_file(self):
        self.clear_figure_canvas()
        self.clear_settings_graph()
        self.clear_preview()
        self.clear_algorithms()

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

    def range_of_weight_visible(self, visible: bool):
        if visible:
            self['-MIN-WEIGHT-LABEL-'].update(value='Min weight:*')
            self['-MAX-WEIGHT-LABEL-'].update(value='Max weight:*')
        else:
            self['-MIN-WEIGHT-LABEL-'].update(value='')
            self['-MAX-WEIGHT-LABEL-'].update(value='')

        self['-MIN-WEIGHT-IN-'].update(value='0', visible=visible)
        self['-MAX-WEIGHT-IN-'].update(value='100', visible=visible)

    def start_algorithm(self):
       pass

    def start_vertex_visible(self, visible: bool):
        if visible:
            self['-START-VERTEX-LABEL-'].update(value='Start vertex:')
        else:
            self['-START-VERTEX-LABEL-'].update(value='')

        self['-START-VERTEX-IN-'].update(value='', visible=visible)

    def finish_vertex_visible(self, visible: bool):
        if visible:
            self['-FINISH-VERTEX-LABEL-'].update(value='Finish vertex:')
        else:
            self['-FINISH-VERTEX-LABEL-'].update(value='')

        self['-FINISH-VERTEX-IN-'].update(value='', visible=visible)

    def parameters_of_algorithms_visible(self, visible: bool):
        self.start_vertex_visible(visible)
        self.finish_vertex_visible(visible)

    def choice_algorithm(self):
        pag.press('left')  # неизбежный костыль

        algorithm = self['-COMBO-ALGORITHMS-'].get()

        algorithms = {"--select--": "",
                      "DFS": dfs,
                      "BFS": bfs,
                      "Dijkstra's Algorithm": Dijkstra_algorithm,
                      "Ford-Bellman Algorithm": Ford_Bellman_algorithm,
                      "Floyd–Warshall Algorithm": Floyd_Warshall_algorithm,
                      "Prim's Algorithm": Prim_algorithm,
                      "Kruskal's Algorithm": Kruskal_algorithm,
                      "Ford–Fulkerson Algorithm": Ford_Fulkerson_algorithm, }

        if algorithm in ("DFS", "BFS", "Dijkstra's Algorithm", "Ford-Bellman Algorithm", "Prim's Algorithm"):
            self.parameters_of_algorithms_visible(False)  # чтобы не было лишнего параметра

            self.start_vertex_visible(True)

        elif algorithm == "Ford–Fulkerson Algorithm":
            self.parameters_of_algorithms_visible(False)

            self.start_vertex_visible(True)
            self.finish_vertex_visible(True)

        else:
            self.parameters_of_algorithms_visible(False)

        self['-EXPLANATION-ALGORITHM-'].update(value=algorithms[algorithm])


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
