import ctypes as ct
import _ctypes as ct_deb
import networkx as nx
import matplotlib.pyplot as plt
import time
from operator import attrgetter


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
    color        = "#999999"

    # Data from DLL
    ptr = None # NODE*
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
    ptr = None # EDGE*
    _fields_ = [("C_WEIGHT", ct.c_int),
                ("C_SOURCE", ct.POINTER(Node)), # NODE*
                ("C_TARGET", ct.POINTER(Node))] # NODE*

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
    ptr = None # GRAPH*
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

    # Common functions
    def GetNode(self, c_node_id):
        for node in self.nodes:
            if node.node_id == c_node_id:
                return node

    def GetEdge(self, c_source_id, c_target_id):
        for edge in self.edges:

            if (edge.source.node_id == c_source_id) and (edge.target.node_id == c_target_id):
                return edge 

            if self.directed:
                continue

            if (edge.source.node_id == c_target_id) and (edge.target.node_id == c_source_id):
                return edge


    def GetNodes(self):
        return [node.node_id for node in self.nodes]

    def GetEdges(self):
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

# define BFS function
lib.BFS.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
lib.BFS.restype = ct.POINTER(ct.POINTER(Node))

def BFS(graph, start):

    path = lib.BFS(graph.ptr, start.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * graph.nodes_num)).contents

    # the check
    print("BFS")
    for i in path:
        print(i.contents.C_ID, end=" ")
    print("\n")

    # networkx Graph:
    nx_graph = nx.Graph()
    nx_graph.add_nodes_from(graph.GetNodes())
    nx_graph.add_edges_from(graph.GetEdges())
    pos = nx.shell_layout(nx_graph)

    for node_ptr in path:

        c_node_id = node_ptr.contents.C_ID
        node = graph.GetNode(c_node_id)
        node.color = "cyan"
        graph.UpdateNodesColor()

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
    path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * graph.nodes_num)).contents

    # the check
    print("DFS")
    for i in path:
        print(i.contents.C_ID, end=" ")
    print("\n")

    # networkx Graph:
    nx_graph = nx.Graph()
    nx_graph.add_nodes_from(graph.GetNodes())
    nx_graph.add_edges_from(graph.GetEdges())
    pos = nx.shell_layout(nx_graph)

    for node_ptr in path:

        c_node_id = node_ptr.contents.C_ID
        node = graph.GetNode(c_node_id)
        node.color = "cyan"
        graph.UpdateNodesColor()

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

#define Dijkstra function
lib.Dijkstra.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
lib.Dijkstra.restype  = ct.POINTER(ct.POINTER(Node))

def Dijkstra(graph, start):

    path = lib.Deijkstra(graph.ptr, start.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * graph.nodes_num)).contents

    # the check
    print("Dijkstra")
    for i in path:
        print(i.contents.C_ID, end=" ")
    print("\n")

    # networkx Graph:
    nx_graph = nx.Graph()
    nx_graph.add_nodes_from(graph.GetNodes())
    nx_graph.add_edges_from(graph.GetEdges())
    pos = nx.shell_layout(nx_graph)

    for node_ptr in path:

        c_node_id = node_ptr.contents.C_ID
        node = graph.GetNode(c_node_id)
        node.color = "cyan"
        graph.UpdateNodesColor()

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

#define Prim function
lib.Prim.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
lib.Prim.restype  = ct.POINTER(ct.POINTER(Edge))

def Prim(graph, root):

    if graph.directed:
        return 0

    path = lib.Prim(graph.ptr, root.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Edge) * (graph.nodes_num-1))).contents

    # the check
    print("Prim")
    for i in path:
        print(i.contents.C_SOURCE.contents.C_ID, i.contents.C_TARGET.contents.C_ID)


    # networkx Graph:
    nx_graph = nx.Graph()
    nx_graph.add_nodes_from(graph.GetNodes())
    nx_graph.add_edges_from(graph.GetEdges())
    pos = nx.shell_layout(nx_graph)

    for edge_ptr in path:

        c_source_id = edge_ptr.contents.C_SOURCE.contents.C_ID
        c_target_id = edge_ptr.contents.C_TARGET.contents.C_ID

        edge = graph.GetEdge(c_source_id, c_target_id)
        edge.color = "#FF0000"
        graph.UpdateEdgesColor()

        # Print Graph   
        nx.draw_networkx(nx_graph, pos=pos, with_labels=True,
                         node_color=graph.nodes_color,
                         edge_color=graph.edges_color,
                         node_size=800,
                         width=3)

        plt.axis('off')
        plt.show()
        time.sleep(0.5)
        plt.close('all')

    for i in range(graph.edges_num):
        if graph.edges_color[i] == graph.edges_default_color:
            graph.edges_color[i] = "#FFFFFF"

    nx.draw_networkx(nx_graph, pos=pos, with_labels=True,
                     node_color=graph.nodes_color,
                     edge_color=graph.edges_color,
                     node_size=800,
                     width=3)
    
    plt.axis('off')
    plt.show()
    time.sleep(0.5)
    plt.close('all')

    graph.RestoreNodesColor()
    graph.RestoreEdgesColor()


#########################################################################

#define Prim function
lib.Kruskal.argtypes = [ct.POINTER(Graph)]
lib.Kruskal.restype  = ct.POINTER(ct.POINTER(Edge))

def Kruskal(graph):

    if graph.directed:
        return 0

    path = lib.Kruskal(graph.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Edge) * (graph.nodes_num-1))).contents

    # the check
    print("Kruskal")
    for i in path:
        print(i.contents.C_SOURCE.contents.C_ID, i.contents.C_TARGET.contents.C_ID)


    # networkx Graph:
    nx_graph = nx.Graph()
    nx_graph.add_nodes_from(graph.GetNodes())
    nx_graph.add_edges_from(graph.GetEdges())
    pos = nx.shell_layout(nx_graph)

    for edge_ptr in path:

        c_source_id = edge_ptr.contents.C_SOURCE.contents.C_ID
        c_target_id = edge_ptr.contents.C_TARGET.contents.C_ID

        edge = graph.GetEdge(c_source_id, c_target_id)
        edge.color = "#FF0000"
        graph.UpdateEdgesColor()

        # Print Graph   
        nx.draw_networkx(nx_graph, pos=pos, with_labels=True,
                         node_color=graph.nodes_color,
                         edge_color=graph.edges_color,
                         node_size=800,
                         width=3)

        plt.axis('off')
        plt.show()
        time.sleep(0.5)
        plt.close('all')

    for i in range(graph.edges_num):
        if graph.edges_color[i] == graph.edges_default_color:
            graph.edges_color[i] = "#FFFFFF"

    nx.draw_networkx(nx_graph, pos=pos, with_labels=True,
                     node_color=graph.nodes_color,
                     edge_color=graph.edges_color,
                     node_size=800,
                     width=3)
    
      
    plt.axis('off')
    plt.show()
    time.sleep(0.5)
    plt.close('all')

    graph.RestoreNodesColor()
    graph.RestoreEdgesColor()


#########################################################################

# Create Graph
nodes = [Node(i) for i in range(21)]

edges     = [Edge(nodes[i], nodes[i+1], 1) for i in range(10)]
edges.extend(Edge(nodes[i], nodes[i+1], 1) for i in range(11, 20))

edges.append(Edge(nodes[7], nodes[15], 1))
edges.append(Edge(nodes[6], nodes[16], 1))
edges.append(Edge(nodes[5], nodes[17], 1))
edges.append(Edge(nodes[4], nodes[18], 1))

my_graph = Graph(nodes, edges, 6)

# Run algorithm's animation
Kruskal(my_graph)
