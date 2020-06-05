import PySimpleGUI as psg
from constants import *

psg.theme('DefaultNoMoreNagging')

# Adjacency list - список смежности, Adjacency matrix - матрица смежности, list of edges - список ребер

layout_adjacency_matrix = [[psg.Column(layout=[[psg.Text(size=(500, 500), key='-MATRIX-ADJ-', font=FONT_12,
                                                         background_color=COLOR_IN_PREVIEW)]],
                                       scrollable=True,
                                       size=(380, 640),
                                       )]]
frame_adjacency_matrix = psg.Frame(title='Adjacency matrix', layout=layout_adjacency_matrix, font=FONT_14)


layout_adjacency_list = [[psg.Column(layout=[[psg.Text(size=(500, 500), key='-LIST-ADJ-', font=FONT_12,
                                                       background_color=COLOR_IN_PREVIEW)]],
                                     scrollable=True,
                                     size=(390, 640),
                                     )]]
frame_adjacency_list = psg.Frame(title='Adjacency list', layout=layout_adjacency_list, font=FONT_14)


layout_list_of_edges = [[psg.Column(layout=[[psg.Text(size=(500, 500), key='-LIST-EDGES-', font=FONT_12,
                                                      background_color=COLOR_IN_PREVIEW)]],
                                    scrollable=True,
                                    size=(420, 640),
                                    )]]
frame_list_of_edges = psg.Frame(title='List of edges', layout=layout_list_of_edges, font=FONT_14)


preview_tab = [[frame_adjacency_matrix, frame_adjacency_list, frame_list_of_edges]]


