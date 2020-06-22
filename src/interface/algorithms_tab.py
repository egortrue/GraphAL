import PySimpleGUI as psg
from constants import *


psg.theme('DefaultNoMoreNagging')

layout_definition_algorithms = [[psg.Column(layout=[[psg.Text(text='Algorithm: ', font=FONT_14),
                                                    psg.Combo(values=["--select--", "DFS", "BFS", "Dijkstra's Algorithm",
                                                            "Ford-Bellman Algorithm", "Floyd–Warshall Algorithm",
                                                            "Prim's Algorithm", "Kruskal's Algorithm",
                                                            "Ford–Fulkerson Algorithm",],
                                                    default_value="--select--",
                                                    font=FONT_12,
                                                    enable_events=True,
                                                    size=(23, 1),
                                                    key='-COMBO-DEFINITION-ALGORITHMS-',
                                                    readonly=True)],

                                                    [psg.Multiline(default_text="",
                                                                   size=(88, 31), autoscroll=True,
                                                                   key='-DEFINITION-', disabled=True,
                                                                   background_color='#f0f0f0', font=FONT_12),
                                                     psg.Column(
                                                         layout=[[psg.Image("", key='-PIC-CODE-')]],
                                                         size=(500, 660),
                                                         # scrollable=True,
                                                         element_justification='left')
                                                     ],
                                                    ],
                                            #scrollable=True,
                                            #vertical_scroll_only=True,
                                            size=(1250, 660))]]


algorithms_tab = layout_definition_algorithms
