from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt

import networkx as nx
import ctypes as ct
import _ctypes as ct_deb
import time
from operator import attrgetter

#########################################################################
# Generation DLL:

class AMATRIX(ct.Structure):
    _fields_ = [("C_ADJ", ct.POINTER(ct.POINTER(ct.c_int))),
                ("C_N", ct.c_int),
                ("C_E", ct.c_int)]



def generate_random_matrix(nodes_num, edges_num, weight_min, weight_max):

    gen_dll = ct.CDLL(r"./generation.dll") # Attach the DLL

    # define types of functions' arguments
    gen_dll.AMatrixSet.argtypes = [ct.c_int]
    gen_dll.RandomGraph.argtypes = [ct.c_int, ct.c_int, ct.POINTER(AMATRIX), ct.c_int, ct.c_int]

    # define types of functions' returns
    gen_dll.AMatrixSet.restype = ct.POINTER(AMATRIX)


    # Generate matrix
    matrix = []
    ptr = gen_dll.AMatrixSet(nodes_num)
    gen_dll.RandomGraph(edges_num, nodes_num, ptr, weight_min, weight_max)


    # Translate int** from DLL to matrix with ints on Python
    arr_ptr_int = ct.cast(ptr.contents.C_ADJ, ct.POINTER(ct.POINTER(ct.c_int)*nodes_num)).contents
    for i in range(nodes_num):
        matrix.append(ct.cast(arr_ptr_int[i], ct.POINTER(ct.c_int * nodes_num)).contents)

    # debug print
    for i in range(nodes_num):
        for j in range(nodes_num):
            print(matrix[i][j], end=' ')
        print()


    ct_deb.FreeLibrary(gen_dll._handle) # Detach the DLL
    return matrix


#########################################################################
# Algorithms DLL:
alg_dll = ct.CDLL(r"./algorithms.dll") # attach the DLL

class Node(ct.Structure):

    # Data for NetworkX/UI
    node_id      = None
    name         = None
    value        = None
    temp_value   = None
    degree       = None
    eccentricity = None
    color        = "#999999"

    # Data from DLL
    ptr = None  # NODE*
    _fields_ = [("C_ID", ct.c_int),
                ("C_VALUE", ct.c_int)]

    def __init__(self, node_id, value=1):
        self.ptr = alg_dll.NodeSet(node_id, value)
        self.node_id = node_id
        self.value = value

    def __str__(self):
        string =  f"Node {self.node_id}:\n   "
        string += f"name: {self.name}\n   "
        string += f"value: {self.value}\n   "
        string += f"color: {self.color}\n"
        return string


class Edge(ct.Structure):

    # Data for NetworkX/UI
    source  = None
    target  = None
    weight  = None
    flow    = None
    color   = "#999999"

    # Data from DLL
    ptr = None  # EDGE*
    _fields_ = [("C_WEIGHT", ct.c_int),
                ("C_SOURCE", ct.POINTER(Node)),  # NODE*
                ("C_TARGET", ct.POINTER(Node))]  # NODE*

    def __init__(self, source, target, weight):
        self.ptr = alg_dll.EdgeSet(source.ptr, target.ptr, weight)
        self.source = source
        self.target = target
        self.weight = weight

    def __str__(self):
        return f"({self.source.node_id}-{self.target.node_id})"


class Graph(ct.Structure):

    # Data for NetworkX/UI
    nodes_num = None
    edges_num = None

    nodes = []
    edges = []

    nodes_default_color = "#999999"
    edges_default_color = "#999999"

    nodes_color = []
    edges_color = []

    directed  = 0
    weighted  = 0


    # Data from DLL
    ptr = None  # GRAPH*
    _fields_ = [("SIZE_N", ct.c_int),
                ("SIZE_E", ct.c_int),
                ("C_NODES", ct.POINTER(ct.POINTER(Node))),  # NODE**
                ("C_EDGES", ct.POINTER(ct.POINTER(Edge)))]  # EDGE**


    def __init__(self, nodes, edges, info=0):

        self.nodes_num = len(nodes)
        self.edges_num = len(edges)

        edges = sorted(edges, key=attrgetter('source.node_id', 'target.node_id'))
        nodes = sorted(nodes, key=lambda node: node.node_id)

        self.nodes = nodes
        self.edges = edges

        self.nodes_color = [self.nodes_default_color for i in range(self.nodes_num)]
        self.edges_color = [self.edges_default_color for i in range(self.edges_num)]

        self.directed  = info & 0b01
        self.weighted  = info & 0b10

        # Init nodes
        nodes_row = ct.POINTER(Node) * self.nodes_num
        nodes_row = nodes_row()
        for i in range(self.nodes_num):
            nodes_row[i] = nodes[i].ptr
        nodes_row = ct.cast(nodes_row, ct.POINTER(ct.POINTER(Node)))

        # Init edges
        edges_row = ct.POINTER(Edge) * self.edges_num
        edges_row = edges_row()
        for i in range(self.edges_num):
            edges_row[i] = edges[i].ptr
        edges_row = ct.cast(edges_row, ct.POINTER(ct.POINTER(Edge)))

        # Assign a memory for graph and create pointer on the memory
        self.ptr = alg_dll.GraphSet(self.nodes_num, nodes_row, self.edges_num, edges_row, info)

        # Init the networkx class Graph
        self.nx_graph = self.networkx_graph()
        self.pos = nx.spring_layout(self.nx_graph)


    # Common functions
    def get_node(self, c_node_id):
        for node in self.nodes:
            if node.node_id == c_node_id:
                return node


    def get_edge(self, c_source_id, c_target_id):
        for edge in self.edges:
            if (edge.source.node_id == c_source_id) and (edge.target.node_id == c_target_id):
                return edge
            if self.directed:
                continue
            if (edge.source.node_id == c_target_id) and (edge.target.node_id == c_source_id):
                return edge


    def get_nodes(self):
        return [node.node_id for node in self.nodes]


    def get_edges(self):
        if self.weighted:
            return [(edge.source.node_id, edge.target.node_id, edge.weight) for edge in self.edges]
        else:
            return [(edge.source.node_id, edge.target.node_id) for edge in self.edges]

    def get_adjacency_matrix(self):
        return self.matrix

    def get_adjacency_list(self):
        dict_adj_list = dict()

        for node in self.nodes:
            dict_adj_list[node.node_id] = []

        if self.weighted:

            for edge in self.get_edges():
                # edge[0] - source_id, edge[1] - target_id, edge[2] - weight
                dict_adj_list[edge[0]].append((edge[1], edge[2]))

            return dict_adj_list

        for edge in self.get_edges():
            # edge[0] - source_id, edge[1] - target_id
            dict_adj_list[edge[0]].append(edge[1])

        return dict_adj_list

    def get_list_edges(self):
        return self.get_edges()

    def save_graph(self):

        graph_settings = dict()
        graph_settings['matrix'] = self.matrix
        graph_settings['info'] = self.connected + self.weighted + self.directed

        for key in self.pos:
            self.pos[key] = list(self.pos[key])  # from numpy.ndarray to list, because json don't support it

        graph_settings['pos'] = self.pos

        return graph_settings

    def networkx_graph(self):

        graph = None

        if self.directed:
            graph = nx.DiGraph()

        else:
            graph = nx.Graph()

        graph.add_nodes_from(self.get_nodes())

        if self.weighted:
            for edge in self.get_edges():
                graph.add_edge(v_of_edge=edge[0],
                               u_of_edge=edge[1],
                               weight=edge[2])
        else:
            graph.add_edges_from(self.get_edges())

        return graph

    def update_nodes_colors(self):
        self.nodes_color = [node.color for node in self.nodes]


    def update_edges_colors(self):
        self.edges_color = [edge.color for edge in self.edges]


    def restore_nodes_colors(self):
        for node in self.nodes:
            node.color = self.nodes_default_color
        self.update_nodes_colors()


    def restore_edges_colors(self):
        for edge in self.edges:
            edge.color = self.edges_default_color
        self.update_edges_colors()


#########################################################################
# Algorithms DLL funcitons:

# define types of functions' arguments
alg_dll.NodeSet.argtypes  = [ct.c_int, ct.c_int] # node_id, value
alg_dll.EdgeSet.argtypes  = [ct.POINTER(Node), ct.POINTER(Node), ct.c_int] # NODE*, NODE*, weight
alg_dll.GraphSet.argtypes = [ct.c_int, ct.POINTER(ct.POINTER(Node)),       # nodes_num, NODE*
                             ct.c_int, ct.POINTER(ct.POINTER(Edge)),       # edges_num, EDGE*
                             ct.c_int]                                     # info

# define types of functions' returns
alg_dll.NodeSet.restype  = ct.POINTER(Node)
alg_dll.EdgeSet.restype  = ct.POINTER(Edge)
alg_dll.GraphSet.restype = ct.POINTER(Graph)

########################################################################

def generate_graph(app):
    app.clear_figure_canvas()

    # Common options
    nodes_num = int(app['-VERTEX-IN-'].get())
    edges_num = int(app['-EDGES-IN-'].get())

    min_weight = 1
    max_weight = 1

    info = 0
    if app['-DIRECTED-Y-'].get():
        info += 0b01

    if app['-WEIGHTED-Y-'].get():
        info += 0b10

        min_weight = int(app['-MIN-WEIGHT-IN-'].get())
        max_weight = int(app['-MAX-WEIGHT-IN-'].get())

    # Generate random graph
    matrix = generate_random_matrix(nodes_num, edges_num, min_weight, max_weight)
    graph = convert_from_matrix_to_graph(matrix, info)
    graph.matrix = matrix

    app.graph = graph
    app.draw_graph()
    preview(app, graph)


def convert_from_matrix_to_graph(matrix, info=0):

    nodes = [Node(i) for i in range(1, len(matrix) + 1)]  # nodes = [1, 2, ...]
    edges = []

    for source in range(len(matrix)):
        for target in range(len(matrix)):

            if matrix[source][target]:
                edges.append(Edge(nodes[source], nodes[target], matrix[source][target]))

    return Graph(nodes, edges, info)


#########################################################################
# Preview tab funcions

def adjacency_matrix_to_preview(graph: Graph):  # todo definition
    """

    :param graph: graph
    :return: adjacency matrix (str)
    """
    data_to_adjacency_matrix_preview = ""

    # matrix = [[i for i in range(graph.nodes_num + 1)]]

    """for i in range(1, graph.nodes_num + 1):
        matrix.append([i])
        matrix[i].extend(graph.matrix[i - 1])"""

    for row in graph.get_adjacency_matrix():
        data_to_adjacency_matrix_preview += " ".join([str(elem) for elem in row]) + "\n"

    return data_to_adjacency_matrix_preview


def adjacency_list_to_preview(graph: Graph):  # todo definition
    """

    :param graph: graph
    :return: adjacency list (str)
    """

    data_to_adjacency_list_preview = ""

    dict_adj_list = graph.get_adjacency_list()

    if graph.weighted:

        for node_id, tuples_target_id_and_weight in dict_adj_list.items():
            data_to_adjacency_list_preview += f"{node_id}: "

            for target_id, weight, in tuples_target_id_and_weight:
                data_to_adjacency_list_preview += f"{target_id}  {weight}, "

            data_to_adjacency_list_preview += "\n"

        return data_to_adjacency_list_preview

    for node_id, list_target_id in dict_adj_list.items():

        data_to_adjacency_list_preview += f"{node_id}: "

        for target_id in list_target_id:
            data_to_adjacency_list_preview += f"{target_id}, "

        data_to_adjacency_list_preview += "\n"

    return data_to_adjacency_list_preview


def list_edges_to_preview(graph: Graph):  # todo definition
    """

    :param graph: graph
    :return: list edges (str)
    """

    list_edges = ""

    for edge in graph.get_list_edges():

        if graph.weighted:
            # edge[0] - source_id, edge[1] - target_id, edge[3] - weight
            list_edges += f"{edge[0]} {edge[1]}  {edge[2]},\n"

        else:
            # edge[0] - source_id, edge[1] - target_id
            list_edges += f"{edge[0]} {edge[1]},\n"

    return list_edges


def preview(app, graph: Graph):  # todo definition
    """

    :param app:
    :param graph:
    """

    app['-MATRIX-ADJ-'].update(value=adjacency_matrix_to_preview(graph))
    app['-LIST-ADJ-'].update(value=adjacency_list_to_preview(graph))
    app['-LIST-EDGES-'].update(value=list_edges_to_preview(graph))