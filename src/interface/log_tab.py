import PySimpleGUI as psg
from constants import *

psg.theme('DefaultNoMoreNagging')

layout_log = [[psg.Text(text='', font=FONT_14, size=(30, 1), key='-LOG-ALGORITHM-')],
              [psg.Multiline(default_text="",
                             autoscroll=True,
                             size=(152, 40),
                             font=FONT_12,
                             disabled=True,
                             background_color='white',
                             key='-LOG-OUT-')]
              ]
log_tab = layout_log
