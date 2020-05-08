from main_tab import main_tab
import PySimpleGUI as psg
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, FigureCanvasAgg, NavigationToolbar2Tk
from matplotlib.figure import Figure
import networkx as nx

FONT_12 = 'Calibri 12'
FONT_14 = 'Calibri 14'


class App(psg.Window):
    def __init__(self):
        menu_def = [['File', ['Open', '&Save', '---', 'Exit']],
                    ['Help', ['About...']], ]

        tab2 = [[psg.Text(text="Preview tab", size=(8, 8))]]
        tab3 = [[psg.Text(text="Log tab", size=(8, 8))]]
        tab4 = [[psg.Text(text='Algorithms tab', size=(8, 8))]]

        super().__init__(title="Graph AL",
                         layout=[
                                [psg.Menu(menu_def)],
                                [psg.TabGroup(layout=[[psg.Tab('Main', main_tab),
                                                       psg.Tab('Preview', tab2),
                                                       psg.Tab('Log', tab3),
                                                       psg.Tab('Algorithms', tab4)]],
                                              tab_location="topleft")]             # choose tab_location
                                 ],
                         size=(1200, 550),
                         resizable=True,
                         finalize=True
                         )
        self.fig_agg = self.draw_figure()

    def draw_figure(self):
        canvas_fig = self['-CANVAS-'].TKCanvas
        canvas_toolbar = self['-TOOLBAR-'].TKCanvas

        # draw the initial plot in the window
        figure = Figure(figsize=(7, 4))
        ax = figure.add_subplot(111)
        ax.set_axis_off()

        figure_canvas_agg = FigureCanvasTkAgg(figure, canvas_fig)
        figure_canvas_agg.draw()
        toolbar = NavigationToolbar2Tk(figure_canvas_agg, canvas_toolbar)
        toolbar.update()
        figure_canvas_agg.get_tk_widget().pack(side='top', fill='both')

        return figure_canvas_agg


def main():
    app = App()

    while True:
        event, values = app.read()
        print(event, values)
        if event in (None, "Exit"):
            break


if __name__ == "__main__":
    main()
