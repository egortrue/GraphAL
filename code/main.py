from design import GenerationGraph, DefinitionAlgorithms, FrameVisualizationGraph
import tkinter as tk
from tkinter import ttk

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