import PySimpleGUI as psg
from constants import *

psg.theme('DefaultNoMoreNagging')  # 'Default1'


# ---------------------------------------------------------------------- #
#                        Frame of setting graph                          #
# ---------------------------------------------------------------------- #


# ----------------------- Create layout -------------------------------- #


layout_setting_graph = [[psg.Column(layout=[[psg.Text('Vertexes:*', font=FONT_12, size=SIZE_LABEL_MAIN_TAB,
                                                      tooltip=" Necessarily "),
                                             psg.Input(key='-VERTEX-IN-', size=SIZE_INPUT_MAIN_TAB,
                                                       tooltip=" Necessarily ")],

                                            [psg.Text('Edges:*', font=FONT_12, size=SIZE_LABEL_MAIN_TAB,
                                                      tooltip=" Necessarily "),
                                             psg.Input(key='-EDGES-IN-', size=SIZE_INPUT_MAIN_TAB,
                                                       tooltip=" Necessarily ")],

                                            [psg.Text('Max degree:', font=FONT_12, size=SIZE_LABEL_MAIN_TAB),
                                             psg.Input(key='-DEGREE-IN-', size=SIZE_INPUT_MAIN_TAB)],

                                            [psg.Text('Connected:', font=FONT_12, size=SIZE_LABEL_MAIN_TAB),
                                             psg.Radio(text='Yes', group_id="connect", key='-CONNECTED-Y-'),
                                             psg.Radio(text='No', group_id="connect", key='-CONNECTED-N-',
                                                       default=True)],

                                            [psg.Text('Directed:', font=FONT_12, size=SIZE_LABEL_MAIN_TAB),
                                             psg.Radio(text='Yes', group_id="directed", key='-DIRECTED-Y-'),
                                             psg.Radio(text='No', group_id="directed", key='-DIRECTED-N-',
                                                       default=True)],

                                            [psg.Text('Weighted:', font=FONT_12, size=SIZE_LABEL_MAIN_TAB),
                                             psg.Radio(text='Yes', group_id="weight", key='-WEIGHTED-Y-',
                                                       enable_events=True),
                                             psg.Radio(text='No', group_id="weight", key='-WEIGHTED-N-', default=True,
                                                       enable_events=True)],

                                            [psg.Text(text='', size=SIZE_LABEL_MAIN_TAB, font=FONT_12,
                                                      key='-MIN-WEIGHT-LABEL-'),
                                             psg.Input(key='-MIN-WEIGHT-IN-', size=SIZE_INPUT_MAIN_TAB,
                                                       tooltip=" Necessarily ", default_text="0", visible=False)],

                                            [psg.Text(text='', size=SIZE_LABEL_MAIN_TAB, font=FONT_12,
                                                      key='-MAX-WEIGHT-LABEL-'),
                                             psg.Input(key='-MAX-WEIGHT-IN-', size=SIZE_INPUT_MAIN_TAB,
                                                       tooltip=" Necessarily ", default_text="100", visible=False)]
                                            ],
                                    size=(225, 235),
                                    )],
                        [psg.Button(button_text="Generate", key='-GENERATE-', font=FONT_14, size=SIZE_BUTTON)]
                        ]
setting_graph = psg.Frame('Graph settings', layout_setting_graph, font=FONT_14, element_justification='center')


# ---------------------------------------------------------------------- #
#                        Frame of algorithms                             #
# ---------------------------------------------------------------------- #


# ----------------------- Create layout -------------------------------- #

layout_algorithms = [[psg.Column(layout=[[psg.Combo(values=["--select--", "DFS", "BFS", "Dijkstra's Algorithm",
                                                            "Ford-Bellman Algorithm", "Floyd–Warshall Algorithm",
                                                            "Prim's Algorithm", "Kruskal's Algorithm",
                                                            "Ford–Fulkerson Algorithm",],
                                                    default_value="--select--",
                                                    font=FONT_12,
                                                    enable_events=True,
                                                    size=(25, 1),
                                                    key='-COMBO-ALGORITHMS-',
                                                    readonly=True,)],
                                         [psg.Multiline(size=(25, 10), font=FONT_12, autoscroll=True, disabled=True,
                                                        key='-EXPLANATION-ALGORITHM-')],

                                         [psg.Text(text='', size=SIZE_LABEL_MAIN_TAB, font=FONT_12,
                                                   key='-START-VERTEX-LABEL-'),
                                          psg.Input(key='-START-VERTEX-IN-', size=SIZE_INPUT_MAIN_TAB, visible=False)],

                                         [psg.Text(text='', size=SIZE_LABEL_MAIN_TAB, font=FONT_12,
                                                   key='-FINISH-VERTEX-LABEL-'),
                                          psg.Input(key='-FINISH-VERTEX-IN-', size=SIZE_INPUT_MAIN_TAB, visible=False)]
                                         ],
                                 size=(225, 295),
                                 )],
                     [psg.Button(button_text="Start", key='-START-', font=FONT_14, size=SIZE_BUTTON)]
                     ]
algorithms = psg.Frame('Algorithms', layout_algorithms, font=FONT_14, element_justification='center')


# ---------------------------------------------------------------------- #
#                        Frame of visualization                          #
# ---------------------------------------------------------------------- #


# ----------------------- Create layout -------------------------------- #

layout_visualization = [[psg.Column(layout=[[psg.Canvas(key='-CANVAS-', size=(970, 545))],
                                            [psg.Canvas(key='-TOOLBAR-')]],
                                    size=(1010, 610))],
                        [psg.Button(button_text="Clear", font=FONT_14, key='-CLEAR-', size=SIZE_BUTTON)]
                        ]
visualization = psg.Frame('Visualization', layout_visualization, font=FONT_14, element_justification='right')


main_tab = [[psg.Column(layout=[[setting_graph], [algorithms]]),
             visualization]]  # create "main" tab
