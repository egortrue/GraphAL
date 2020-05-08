import PySimpleGUI as psg
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib.figure import Figure


FONT_12 = 'Calibri 12'
FONT_14 = 'Calibri 14'


def draw_figure(canvas_fig, figure, canvas_toolbar):
    figure_canvas_agg = FigureCanvasTkAgg(figure, canvas_fig)
    figure_canvas_agg.draw()
    toolbar = NavigationToolbar2Tk(figure_canvas_agg, canvas_toolbar)
    toolbar.update()
    figure_canvas_agg.get_tk_widget().pack(side='top', fill='both')
    return figure_canvas_agg


layout_setting_graph = [[psg.Column(layout=[[psg.Text('Amount of vertex:', font=FONT_12)],
                                            [psg.Input(key='-VERTEX-IN-', size=(14, 1))],
                                            [psg.Text('Amount of edges:', font=FONT_12)],
                                            [psg.Input(key='-EDGES-IN-', size=(14, 1))],

                                            [psg.Text('Maximum degree of vertex:', font=FONT_12)],
                                            [psg.Input(key='-PW-IN-', size=(14, 1))],

                                            [psg.Text('Connected:', font=FONT_12)],            # radio yes/no
                                            [psg.Radio(text='Yes', group_id="connect"),
                                             psg.Radio(text='No', group_id="connect")],

                                            [psg.Text('Oriented:', font=FONT_12)],    # radio yes/no
                                            [psg.Radio(text='Yes', group_id="orient"),
                                             psg.Radio(text='No', group_id="orient")],

                                            [psg.Text('Weighted:', font=FONT_12)],
                                            [psg.Radio(text='Yes', group_id="weight"),
                                             psg.Radio(text='No', group_id="weight")],
                                            [psg.Input(default_text="0", size=(3, 1), disabled=False,
                                                       tooltip="min weight", key='MIN-WEIGHT'),
                                            psg.Text(text='-', font=FONT_12),
                                            psg.Input(default_text="100", size=(3, 1), disabled=False,
                                                      tooltip="max weight", key='MAX-WEIGHT')]],

                                    size=(240, 420),
                                    )],
                        [psg.Button(button_text="Generate")]
                        ]
setting_graph = psg.Frame('Setting graph', layout_setting_graph, font=FONT_14)

layout_algorithms = [[psg.Column(layout=[[psg.Radio(text="first algorithms", group_id="algorithms", font=FONT_12)],
                                         [psg.Radio(text="second algorithms", group_id="algorithms", font=FONT_12)],
                                         [psg.Radio(text="third algorithms", group_id="algorithms", font=FONT_12)],
                                         [psg.Radio(text="fourth algorithms", group_id="algorithms", font=FONT_12)],
                                         [psg.Radio(text="fifth algorithms", group_id="algorithms", font=FONT_12)],
                                         [psg.Radio(text="sixth algorithms", group_id="algorithms", font=FONT_12)],
                                         [psg.Radio(text="seventh algorithms", group_id="algorithms", font=FONT_12)]],

                                 size=(200, 420),
                                 )],
                     [psg.Button(button_text="Start")]
                     ]
algorithms = psg.Frame('Algorithms', layout_algorithms, font=FONT_14)


layout_visualization = [[psg.Column(layout=[[psg.Canvas(key='-CANVAS-', size=(650, 400))],
                                            [psg.Canvas(key='-TOOLBAR-')]], size=(650, 450))]
                        ]
visualization = psg.Frame('Visualization', layout_visualization, font=FONT_14)

main_tab = [[setting_graph, visualization, algorithms]]


def main():
    window = psg.Window('Main tab', main_tab, finalize=True)

    canvas_fig = window['-CANVAS-']
    canvas = canvas_fig.TKCanvas

    canvas_toolbar = window['-TOOLBAR-']
    toolbar = canvas_toolbar.TKCanvas

    # draw the initial plot in the window
    fig = Figure(figsize=(7, 4))
    ax = fig.add_subplot(111)
    ax.set_axis_off()
    fig_agg = draw_figure(canvas, fig, toolbar)

    while True:
        event, values = window.read()
        print(event, values)
        if event is None:
            break

    window.close()


if __name__ == "__main__":
    main()
