import ctypes as ct
import _ctypes as ct_deb
import networkx as nx
import matplotlib.pyplot as plt
import time


# link DLL
path = r"D:\Code\GraphAL\solutions\dll\Debug\myDLL.dll" # or full path
lib = ct.CDLL(path)


class Node(ct.Structure):

    # Data for NetworkX/UI
    node_id      = None
    name         = None
    value        = None
    temp_value   = None
    degree       = None
    eccentricity = None
    color        = "#999999"  # default = #1f78b4

    # Data from DLL
    ptr = None
    _fields_ = [("C_ID", ct.c_int),
                ("C_VALUE", ct.c_int)]

    def __init__(self, node_id, value=1):
        self.ptr = lib.NodeSet(node_id, value)
        self.node_id = node_id
        self.value = value


class Edge(ct.Structure):

    # Data for NetworkX/UI
    source = None
    target = None
    weight = None
    flow   = None
    color  = "gray"

    # Data from DLL
    ptr = None
    _fields_ = [("C_WEIGHT", ct.c_int),
                ("C_SOURCE", ct.POINTER(Node)), # NODE*
                ("C_TARGET", ct.POINTER(Node))] # NODE*

    def __init__(self, source, target, weight):
        self.ptr = lib.EdgeSet(source.ptr, target.ptr, weight)
        self.source = source
        self.target = target
        self.weight = weight




class Graph(ct.Structure):

    # Data for NetworkX/UI
    nodes_num = None
    edges_num = None

    nodes = []
    edges = []

    directed  = 0
    weighted  = 0
    connected = 0

    nodes_color = []
    edges_color = []

    # Data from DLL
    ptr = None
    _fields_ = [("SIZE_N", ct.c_int),
                ("SIZE_E", ct.c_int),
                ("C_NODES", ct.POINTER(ct.POINTER(Node))),  # NODE**
                ("C_EDGES", ct.POINTER(ct.POINTER(Edge)))]  # EDGE**

    def __init__(self, nodes, edges):

        self.nodes_num = len(nodes)
        self.edges_num = len(edges)

        self.nodes = nodes
        self.edges = edges

        self.nodes_color = [node.color for node in nodes]
        self.edges_color = [edge.color for edge in edges]

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
        self.ptr = lib.GraphSet(self.nodes_num, nodes_row, self.edges_num, edges_row, 7)

    # Common functions
    def GetNodes(self):
        return [node.node_id for node in self.nodes]

    def GetEdges(self):
        return [(edge.source.node_id, edge.target.node_id) for edge in self.edges]

    def UpdateColors(self):
        self.nodes_color = [node.color for node in self.nodes]
        self.edges_color = [edge.color for edge in self.edges]


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

# define BFS function
lib.BFS.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
lib.BFS.restype = ct.POINTER(ct.POINTER(Node))

def BFS(graph, start):

    path = lib.BFS(graph.ptr, start.ptr)

    print("BFS")
    path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * graph.nodes_num)).contents
    for i in path:
        print(i.contents.C_ID, end=" ")
    print("\n")

    # networkx Graph:
    nx_graph = nx.Graph()
    nx_graph.add_nodes_from(graph.GetNodes())
    nx_graph.add_edges_from(graph.GetEdges())
    pos = nx.shell_layout(nx_graph)

    for node in path:

        node_id = node.contents.C_ID
        for k in graph.nodes:
            if k.node_id == node_id:
                k.color = "cyan"
        graph.UpdateColors()

        # Print Graph   
        nx.draw_networkx(nx_graph, pos=pos, with_labels=True,
                         node_color=graph.nodes_color,
                         edge_color=graph.edges_color,
                         node_size=800,
                         width = 2)

        plt.axis('off')
        plt.show()
        time.sleep(0.25)
        plt.close('all')


#########################################################################

# define DFS function
lib.DFS.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
lib.DFS.restype = ct.POINTER(ct.POINTER(Node))

def DFS(graph, start):

    path = lib.DFS(graph.ptr, start.ptr)

    print("DFS")
    path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * graph.nodes_num)).contents
    for i in path:
        print(i.contents.C_ID, end=" ")
    print("\n")

    # networkx Graph:
    nx_graph = nx.Graph()
    nx_graph.add_nodes_from(graph.GetNodes())
    nx_graph.add_edges_from(graph.GetEdges())
    pos = nx.shell_layout(nx_graph)

    for node in path:

        node_id = node.contents.C_ID
        for k in graph.nodes:
            if k.node_id == node_id:
                k.color = "cyan"
        graph.UpdateColors()

        # Print Graph   
        nx.draw_networkx(nx_graph, pos=pos, with_labels=True,
                         node_color=graph.nodes_color,
                         edge_color=graph.edges_color,
                         node_size=800,
                         width = 2)

        plt.axis('off')
        plt.show()
        time.sleep(0.25)
        plt.close('all')

#########################################################################

#define Deijkstra function
lib.Deijkstra.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
lib.Deijkstra.restype  = ct.POINTER(ct.POINTER(Node))

def Deijkstra(graph, start):

    path = lib.Deijkstra(graph.ptr, start.ptr)

    print("Deijkstra")
    path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * graph.nodes_num)).contents
    for i in path:
        print(i.contents.C_ID, end=" ")
    print("\n")

    # networkx Graph:
    nx_graph = nx.Graph()
    nx_graph.add_nodes_from(graph.GetNodes())
    nx_graph.add_edges_from(graph.GetEdges())
    pos = nx.shell_layout(nx_graph)

    for node in path:

        node_id = node.contents.C_ID
        for k in graph.nodes:
            if k.node_id == node_id:
                k.color = "cyan"
        graph.UpdateColors()

        # Print Graph   
        nx.draw_networkx(nx_graph, pos=pos, with_labels=True,
                         node_color=graph.nodes_color,
                         edge_color=graph.edges_color,
                         node_size=800,
                         width = 2)

        plt.axis('off')
        plt.show()
        time.sleep(0.25)
        plt.close('all')


#########################################################################

# Create Graph
nodes = [Node(i, i*i) for i in range(21)]

edges     = [Edge(nodes[i], nodes[i+1], i+100) for i in range(10)]
edges.extend(Edge(nodes[i], nodes[i+1], i*i) for i in range(11, 20))
edges.append(Edge(nodes[0], nodes[11], 10))

my_graph = Graph(nodes, edges)

# Run algorithm's animation
Deijkstra(my_graph, nodes[0])
