from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt

import networkx as nx

import ctypes as ct
import _ctypes as ct_deb

import time
from operator import attrgetter


# path = r"D:\Kursach\src\x64\Debug\mydll.dll"  # path to *.dll

lib = ct.CDLL(r"D:\Kursach\src\x64\Debug\mydll.dll")

gen = ct.CDLL(r"D:\Kursach\generation\x64\Debug\generation_dll.dll")



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
        self.ptr = lib.NodeSet(node_id, value)
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
        self.ptr = lib.EdgeSet(source.ptr, target.ptr, weight)
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
    connected = 0

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

        self.directed  = info & 1
        self.weighted  = info & 2
        self.connected = info & 4

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
        self.ptr = lib.GraphSet(self.nodes_num, nodes_row, self.edges_num, edges_row, info)

        self.nx_graph = nx.Graph()

        self.nx_graph.add_nodes_from(self.get_nodes())
        self.nx_graph.add_edges_from(self.get_edges())
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

    def get_edges(self, with_weight=False):
        if with_weight:
            return [(edge.source.node_id, edge.target.node_id, edge.weight) for edge in self.edges]

        return [(edge.source.node_id, edge.target.node_id) for edge in self.edges]

    def UpdateNodesColor(self):
        self.nodes_color = [node.color for node in self.nodes]

    def UpdateEdgesColor(self):
        self.edges_color = [edge.color for edge in self.edges]

    def RestoreNodesColor(self):
        for node in self.nodes:
            node.color = self.nodes_default_color
        self.UpdateNodesColor()

    def RestoreEdgesColor(self):
        for edge in self.edges:
            edge.color = self.edges_default_color
        self.UpdateEdgesColor()



#########################################################################

# define types of functions' arguments
lib.NodeSet.argtypes  = [ct.c_int, ct.c_int] # node_id, value
lib.EdgeSet.argtypes  = [ct.POINTER(Node), ct.POINTER(Node), ct.c_int] # NODE*, NODE*, weight

lib.GraphSet.argtypes = [ct.c_int, ct.POINTER(ct.POINTER(Node)),       # nodes_num, NODE*
                         ct.c_int, ct.POINTER(ct.POINTER(Edge)),       # edges_num, EDGE*
                         ct.c_int]                                     # info

# define types of functions' returns
lib.NodeSet.restype  = ct.POINTER(Node)
lib.EdgeSet.restype  = ct.POINTER(Edge)
lib.GraphSet.restype = ct.POINTER(Graph)

#########################################################################


class AMATRIX(ct.Structure):

    ptr = None
    _fields_ = [("C_ADJ", ct.POINTER(ct.POINTER(ct.c_int))),
                ("C_N", ct.c_int),
                ("C_E", ct.c_int)]

    matrix = []

    def __init__(self, nodes_num, edges_num, weight_min, weight_max):
        self.ptr = gen.AMatrixSet(nodes_num)
        gen.RandomGraph(edges_num, nodes_num, self.ptr, weight_min, weight_max)

        arr_ptr_int = ct.cast(self.ptr.contents.C_ADJ, ct.POINTER(ct.POINTER(ct.c_int)*nodes_num)).contents
        for i in range(nodes_num):
            self.matrix.append(ct.cast(arr_ptr_int[i], ct.POINTER(ct.c_int * nodes_num)).contents)

        for i in range(nodes_num):
            for j in range(nodes_num):
                print(self.matrix[i][j], end=' ')
            print()


#define Amatrix functions
gen.AMatrixSet.argtypes = [ct.c_int]
gen.AMatrixSet.restype = ct.POINTER(AMATRIX)

gen.RandomGraph.argtypes = [ct.c_int, ct.c_int, ct.POINTER(AMATRIX), ct.c_int, ct.c_int]


########################################################################


def generate_graph(app):
    app.clear_figure_canvas()

    vertexes = int(app['-VERTEX-IN-'].get())
    edges = int(app['-EDGES-IN-'].get())

    # app.graph, app.pos = create_graph()

    """nodes = [Node(i) for i in range(1, 7)]

    edges = [Edge(nodes[0], nodes[i], 1) for i in range(1, 4)]
    edges.extend(Edge(nodes[2], nodes[i], 1) for i in range(3, 6))

    edges.append(Edge(nodes[2], nodes[0], 109))

    edges.append(Edge(nodes[1], nodes[0], 198))
    edges.append(Edge(nodes[5], nodes[2], 98))
    edges.append(Edge(nodes[4], nodes[2], 123))
    edges.append(Edge(nodes[3], nodes[2], 1))
    edges.append(Edge(nodes[3], nodes[0], 76))

    my_graph = Graph(nodes, edges, 6)

    preview(app, my_graph)

    draw_graph(app, my_graph, app.fig_agg, app.ax)
    """
    matrix = AMATRIX(vertexes, edges, 1, 1)



def create_graph(vertexes=None, edges=None, max_degree=None, connected=False, directed=False, weighted=False,
                 min_weight=0, max_weight=100):
    """

    :param vertexes: amount of vertex in graph
    :type vertexes: (int)
    :param edges: amount of vertex in graph
    :type edges: (int)
    :param max_degree: max degree of vertex in graph
    :type max_degree: (int)
    :param connected: if True graph is connected, default False
    :type connected: (bool)
    :param directed: if True graph is directed, default False
    :type directed: (bool)
    :param weighted: if True edges with weight, default False
    :type weighted: (bool)
    :param min_weight: min weight of edges in graph
    :type min_weight: (int)
    :param max_weight: max weight of edges in graph
    :type max_weight: (int)
    :return: nx.Graph, pos
    """

    # example graph
    G = nx.Graph()

    G.add_edge('1', '2', weight=0.6)
    G.add_edge('1', '3', weight=0.2)
    G.add_edge('3', '4', weight=0.1)
    G.add_edge('3', '5', weight=0.7)
    G.add_edge('3', '6', weight=0.9)
    G.add_edge('1', '4', weight=0.3)

    graph = G
    # graph = nx.path_graph(13)
    pos = nx.spring_layout(graph)

    for key in pos:
        pos[key] = list(pos[key])

    return graph, pos


def draw_graph(app, graph, fig_agg: FigureCanvasTkAgg, ax: plt.Axes):  # delete arg pos
    """

    :param fig_agg:
    :param app:
    :param graph:
    :param ax:
    :return:
    """
    app.draw_graph_flag = True

    nx.draw_networkx(graph.nx_graph,
                     pos=graph.pos,
                     arrows=True,
                     with_labels=True,
                     ax=ax,
                     node_color=graph.nodes_color,
                     edge_color=graph.edges_color,
                     node_size=800,
                     width=2)
    # nx.draw(graph, pos, ax)
    fig_agg.draw()


def to_adjacency_matrix(graph: Graph):  # todo definition
    """

    :param graph: graph
    :return: adjacency matrix (str)
    """
    data_to_adjacency_matrix_preview = ""

    matrix = [[0] * (graph.nodes_num + 1) for i in range(graph.nodes_num + 1)]

    for i in range(1, graph.nodes_num + 1):
        matrix[0][i] = i
        matrix[i][0] = i

    if graph.weighted:

        for edge in graph.get_edges(with_weight=True):
            # edge[0] - source_id, edge[1] - target, edge[2] - weight
            matrix[edge[0]][edge[1]] = edge[2]
    else:

        for edge in graph.get_edges(with_weight=False):
            # edge[0] - source_id, edge[1] - target
            matrix[edge[0]][edge[1]] = 1

    for row in matrix:
        data_to_adjacency_matrix_preview += " ".join([str(elem) for elem in row]) + "\n"

    return data_to_adjacency_matrix_preview


def to_adjacency_list(graph: Graph):  # todo definition
    """

    :param graph: graph
    :return: adjacency list (str)
    """

    data_to_adjacency_list_preview = ""

    dict_adj_list = dict()

    for node in graph.nodes:
        dict_adj_list[node.node_id] = []

    if graph.weighted:
        for edge in graph.get_edges(with_weight=True):
            # edge[0] - source_id, edge[1] - target_id, edge[3] - weight
            dict_adj_list[edge[0]].append((edge[1], edge[2]))

        for node_id, neighbors in dict_adj_list.items():
            data_to_adjacency_list_preview += f"{node_id}: "

            for neighbor, weight, in neighbors:
                data_to_adjacency_list_preview += f"{neighbor}  {weight}, "

            data_to_adjacency_list_preview += "\n"

        return data_to_adjacency_list_preview

    for edge in graph.get_edges(with_weight=True):
        # edge[0] - source_id, edge[1] - target_id
        dict_adj_list[edge[0]].append(edge[1])

    for node_id, neighbors in dict_adj_list.items():
        data_to_adjacency_list_preview += f"{node_id}: "

        for neighbor in neighbors:
            data_to_adjacency_list_preview += f"{neighbor}, "

        data_to_adjacency_list_preview += "\n"

    return data_to_adjacency_list_preview


def to_list_edges(graph: Graph):  # todo definition
    """

    :param graph: graph
    :return: list edges (str)
    """

    list_edges = ""

    if graph.weighted:

        for edge in graph.get_edges(with_weight=True):
            list_edges += f"{edge[0]} {edge[1]}  {edge[2]},\n"
        return list_edges

    for edge in graph.get_edges():
        list_edges += f"{edge[0]} {edge[1]},\n"

    return list_edges


def preview(app, graph: Graph):  # todo definition
    """

    :param app:
    :param graph:
    :param weighted:
    :return:
    """

    app['-MATRIX-ADJ-'].update(value=to_adjacency_matrix(graph))
    app['-LIST-ADJ-'].update(value=to_adjacency_list(graph))
    app['-LIST-EDGES-'].update(value=to_list_edges(graph))


def graph_from_dict(settings):
    """

    :param settings:  dict of settings graph - vertexes, edges, weighted, connected, directed
    :type settings: (dict)
    :return: graph (nx.Graph)
    """

    graph = nx.Graph()

    edges = settings['edges'].split('\n')
    edges.pop()  # delete last empty element

    if settings['-WEIGHTED-Y-']:

        for edge in edges:
            u_vertex, v_vertex, weight = map(str, edge.split())
            graph.add_edge(u_of_edge=u_vertex,
                           v_of_edge=v_vertex,
                           weight=float(weight))
    else:

        for edge in edges:
            u_vertex, v_vertex = map(str, edge.split())
            graph.add_edge(u_of_edge=u_vertex,
                           v_of_edge=v_vertex)

    graph.add_nodes_from(settings['nodes'])

    return graph
