from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt

import networkx as nx
import ctypes as ct
import _ctypes as ct_deb
import time
from operator import attrgetter

gen_dll = ct.CDLL(r"./generation.dll")
alg_dll = ct.CDLL(r"./algorithms.dll")

#########################################################################
# Generation DLL:

class AMATRIX(ct.Structure):
    _fields_ = [("C_ADJ", ct.POINTER(ct.POINTER(ct.c_int))),
                ("C_N", ct.c_int),
                ("C_E", ct.c_int)]


#########################################################################
# Generation DLL funcitons:

gen_dll.AMatrixSet.argtypes = [ct.c_int, ct.c_int]
gen_dll.AMatrixSet.restype = ct.POINTER(AMATRIX)

gen_dll.AMatrixDelete.argtypes = [ct.POINTER(AMATRIX)]
gen_dll.AMatrixDelete.restype = ct.POINTER(AMATRIX)

gen_dll.RandomGraph.argtypes = [ct.POINTER(AMATRIX), ct.c_int, ct.c_int, ct.c_int]

gen_dll.ChoiceRand.argtypes = [ct.POINTER(AMATRIX), ct.c_int, ct.c_int, ct.c_int, ct.c_int]
gen_dll.ChoiceRand.restype = ct.POINTER(AMATRIX)

#########################################################################

def generate_random_matrix(nodes_num, edges_num, info, max_degree, weight_min, weight_max):

    # Generate matrix
    matrix = []
    ptr = gen_dll.AMatrixSet(nodes_num, edges_num)
    gen_dll.ChoiceRand(ptr, info & 0b01, weight_min, weight_max, max_degree)


    # Translate int** from DLL to matrix with ints on Python
    arr_ptr_int = ct.cast(ptr.contents.C_ADJ, ct.POINTER(ct.POINTER(ct.c_int)*nodes_num)).contents
    for i in range(nodes_num):
        matrix.append(ct.cast(arr_ptr_int[i], ct.POINTER(ct.c_int * nodes_num)).contents)

    # debug print
    for i in range(nodes_num):
        for j in range(nodes_num):
            print(matrix[i][j], end=' ')
        print()


    #ct_deb.FreeLibrary(gen_dll._handle) # Detach the DLL
    return matrix, ptr

#########################################################################
# Algorithms DLL:

class Node(ct.Structure):

    # Data for NetworkX/UI
    node_id      = None
    name         = None
    value        = None
    degree       = None
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

    matrix = []
    matrix_ptr = None

    nodes = []
    edges = []

    nodes_default_color = "#999999"
    edges_default_color = "#999999"

    nodes_color = []
    edges_color = []

    directed = 0
    weighted = 0

    nx_graph = None
    pos      = None


    # Data from DLL
    ptr = None  # GRAPH*
    _fields_ = [("SIZE_N", ct.c_int),
                ("SIZE_E", ct.c_int),
                ("C_NODES", ct.POINTER(ct.POINTER(Node))),  # NODE**
                ("C_EDGES", ct.POINTER(ct.POINTER(Edge)))]  # EDGE**


    def __init__(self, nodes, edges, info=0, min_weight=1, max_weight=1):

        self.nodes_num = len(nodes)
        self.edges_num = len(edges)

        nodes = sorted(nodes, key=lambda node: node.node_id)
        edges = sorted(edges, key=attrgetter('source.node_id', 'target.node_id'))

        self.nodes = nodes
        self.edges = edges

        self.nodes_color = [self.nodes_default_color for i in range(self.nodes_num)]
        self.edges_color = [self.edges_default_color for i in range(self.edges_num)]

        self.directed  = info & 0b01
        self.weighted  = info & 0b10

        self.min_weight = min_weight
        self.max_weight = max_weight

        # Init nodes for assigning the memory in C
        nodes_row = ct.POINTER(Node) * self.nodes_num
        nodes_row = nodes_row()
        for i in range(self.nodes_num):
            nodes_row[i] = nodes[i].ptr
        nodes_row = ct.cast(nodes_row, ct.POINTER(ct.POINTER(Node)))

        # Init edges for assigning the memory in C
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


    # Get instance of node which have current id
    def get_node(self, c_node_id):
        for node in self.nodes:
            if node.node_id == c_node_id:
                return node

    # Get instance of edge which have current source and target id
    def get_edge(self, c_source_id, c_target_id):
        for edge in self.edges:
            if (edge.source.node_id == c_source_id) and (edge.target.node_id == c_target_id):
                return edge
            if self.directed:
                continue
            if (edge.source.node_id == c_target_id) and (edge.target.node_id == c_source_id):
                return edge

    # Get array of nodes' id
    def get_arr_nodes(self):
        return [node.node_id for node in self.nodes]

    # Get array of tuples with sorces' and target's id
    def get_arr_edges(self):
        if self.weighted:
            return [(edge.source.node_id, edge.target.node_id, edge.weight) for edge in self.edges]
        else:
            return [(edge.source.node_id, edge.target.node_id) for edge in self.edges]

    #------------------------------------------------------------
    # Graph's colors menagement

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

    #------------------------------------------------------------

    def get_adjacency_list(self):
        dict_adj_list = dict()

        for node in self.nodes:
            dict_adj_list[node.node_id] = []

        if self.weighted:

            for edge in self.get_arr_edges():
                # edge[0] - source_id, edge[1] - target_id, edge[2] - weight
                dict_adj_list[edge[0]].append((edge[1], edge[2]))

            return dict_adj_list

        for edge in self.get_arr_edges():
            # edge[0] - source_id, edge[1] - target_id
            dict_adj_list[edge[0]].append(edge[1])

        return dict_adj_list

    def save_graph(self):

        graph_settings = dict()
        graph_settings['matrix'] = self.matrix
        graph_settings['info'] = self.weighted + self.directed

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

        graph.add_nodes_from(self.get_arr_nodes())

        if self.weighted:
            graph.add_weighted_edges_from(self.get_arr_edges())
        else:
            graph.add_edges_from(self.get_arr_edges())

        return graph




#########################################################################
# Algorithms DLL funcitons:

# define types of functions' arguments
alg_dll.NodeSet.argtypes  = [ct.c_int, ct.c_int] # node_id, value
alg_dll.EdgeSet.argtypes  = [ct.POINTER(Node), ct.POINTER(Node), ct.c_int] # NODE*, NODE*, weight
alg_dll.GraphSet.argtypes = [ct.c_int, ct.POINTER(ct.POINTER(Node)),       # nodes_num, NODE*
                             ct.c_int, ct.POINTER(ct.POINTER(Edge)),       # edges_num, EDGE*
                             ct.c_int]                                     # info

alg_dll.GraphDestroy.argtypes = [ct.POINTER(Graph)]

# define types of functions' returns
alg_dll.NodeSet.restype  = ct.POINTER(Node)
alg_dll.EdgeSet.restype  = ct.POINTER(Edge)
alg_dll.GraphSet.restype = ct.POINTER(Graph)

########################################################################

def generate_graph(nodes_num, edges_num, max_degree, info=0, min_weight=1, max_weight=1):

    matrix, matrix_ptr = generate_random_matrix(nodes_num, edges_num, info, max_degree, min_weight, max_weight)
    graph = convert_from_matrix_to_graph(matrix, min_weight, max_weight, info)   
    graph.matrix = matrix
    graph.matrix_ptr = matrix_ptr

    return graph


def convert_from_matrix_to_graph(matrix, min_weight, max_weight, info=0):

    nodes = [Node(i) for i in range(1, len(matrix) + 1)]  # nodes = [1, 2, ...]
    edges = []

    for source in range(len(matrix)):
        for target in range(len(matrix)):
            if matrix[source][target]:
                edges.append(Edge(nodes[source], nodes[target], matrix[source][target]))


    return Graph(nodes, edges, info, min_weight, max_weight)


#########################################################################
# Preview tab functions

def adjacency_matrix_to_preview(graph: Graph):

    data_to_adjacency_matrix_preview = "Matrix - Nodes_num*Nodes_num, matrix[node1][node2] = weight\n"

    for row in graph.matrix:
        data_to_adjacency_matrix_preview += " ".join([str(elem) for elem in row]) + "\n"

    return data_to_adjacency_matrix_preview


def adjacency_list_to_preview(graph: Graph):

    data_to_adjacency_list_preview = ""

    dict_adj_list = graph.get_adjacency_list()

    if graph.weighted:

        data_to_adjacency_list_preview += "{Node}:_{neighbour1}__{weight1},_{neighbour2}__{weight2},...\n"

        for node_id, tuples_target_id_and_weight in dict_adj_list.items():
            data_to_adjacency_list_preview += f"{node_id}: "

            for target_id, weight, in tuples_target_id_and_weight:
                data_to_adjacency_list_preview += f"{target_id}  {weight}, "

            data_to_adjacency_list_preview += "\n"

        return data_to_adjacency_list_preview

    data_to_adjacency_list_preview += "{Node}:_{neighbour1},_{neighbour2},...\n"

    for node_id, list_target_id in dict_adj_list.items():

        data_to_adjacency_list_preview += f"{node_id}: "

        for target_id in list_target_id:
            data_to_adjacency_list_preview += f"{target_id}, "

        data_to_adjacency_list_preview += "\n"

    return data_to_adjacency_list_preview


def list_edges_to_preview(graph: Graph):

    list_edges = ""
    if graph.weighted:
        list_edges = "{Node1}_{node2}__{weight},\n"
    else:
        list_edges = "{Node1}_{node2},\n"

    for edge in graph.get_arr_edges():

        if graph.weighted:
            # edge[0] - source_id, edge[1] - target_id, edge[3] - weight
            list_edges += f"{edge[0]} {edge[1]}  {edge[2]},\n"

        else:
            # edge[0] - source_id, edge[1] - target_id
            list_edges += f"{edge[0]} {edge[1]},\n"

    return list_edges
