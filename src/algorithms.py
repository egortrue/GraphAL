import ctypes as ct
import _ctypes as ct_deb
import networkx as nx
import matplotlib.pyplot as plt
import time

from graphs import *

#########################################################################
# define BFS function

def BFS(graph, start):

    alg_dll.BFS.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
    alg_dll.BFS.restype = ct.POINTER(ct.POINTER(Node))

    path = alg_dll.BFS(graph.ptr, start.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * graph.nodes_num)).contents

    # the check
    print("BFS")
    for i in path:
        print(i.contents.C_ID, end=" ")
    print("\n")

    # networkx Graph:
    nx_graph = nx.Graph()
    nx_graph.add_nodes_from(graph.get_nodes())
    nx_graph.add_edges_from(graph.get_edges())
    pos = nx.shell_layout(nx_graph)

    for node_ptr in path:

        c_node_id = node_ptr.contents.C_ID
        node = graph.get_node(c_node_id)
        node.color = "cyan"
        graph.update_nodes_colors()

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

def DFS(graph, start):

    alg_dll.DFS.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
    alg_dll.DFS.restype = ct.POINTER(ct.POINTER(Node))

    path = alg_dll.DFS(graph.ptr, start.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * graph.nodes_num)).contents

    # the check
    print("DFS")
    for i in path:
        print(i.contents.C_ID, end=" ")
    print("\n")

    # networkx Graph:
    nx_graph = nx.Graph()
    nx_graph.add_nodes_from(graph.get_nodes())
    nx_graph.add_edges_from(graph.get_edges())
    pos = nx.shell_layout(nx_graph)

    for node_ptr in path:

        c_node_id = node_ptr.contents.C_ID
        node = graph.get_node(c_node_id)
        node.color = "cyan"
        graph.update_nodes_colors()

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

    graph.restore_nodes_colors()
    graph.restore_edges_colors()

#########################################################################
#define Dijkstra function

def Dijkstra(graph, start):

    alg_dll.Dijkstra.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
    alg_dll.Dijkstra.restype  = ct.POINTER(ct.POINTER(Node))

    path = alg_dll.Dijkstra(graph.ptr, start.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * graph.nodes_num)).contents

    # the check
    print("Dijkstra")
    for i in path:
        print(i.contents.C_ID, end=" ")
    print("\n")

    # networkx Graph:
    nx_graph = nx.Graph()
    nx_graph.add_nodes_from(graph.get_nodes())
    nx_graph.add_edges_from(graph.get_edges())
    pos = nx.shell_layout(nx_graph)

    for node_ptr in path:

        c_node_id = node_ptr.contents.C_ID
        node = graph.get_node(c_node_id)
        node.color = "cyan"
        graph.update_nodes_colors()

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

    graph.restore_nodes_colors()
    graph.restore_edges_colors()

#########################################################################
#define Prim function

def Prim(graph, root):

    alg_dll.Prim.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
    alg_dll.Prim.restype  = ct.POINTER(ct.POINTER(Edge))

    if graph.directed:
        return 0

    path = alg_dll.Prim(graph.ptr, root.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Edge) * (graph.nodes_num-1))).contents

    # the check
    print("Prim")
    for i in path:
        print(i.contents.C_SOURCE.contents.C_ID, i.contents.C_TARGET.contents.C_ID)


    # networkx Graph:
    nx_graph = nx.Graph()
    nx_graph.add_nodes_from(graph.get_nodes())
    nx_graph.add_edges_from(graph.get_edges())
    pos = nx.shell_layout(nx_graph)

    for edge_ptr in path:

        c_source_id = edge_ptr.contents.C_SOURCE.contents.C_ID
        c_target_id = edge_ptr.contents.C_TARGET.contents.C_ID

        edge = graph.get_edge(c_source_id, c_target_id)
        edge.color = "#FF0000"
        graph.update_edges_colors()

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

    graph.restore_nodes_colors()
    graph.restore_edges_colors()


#########################################################################
#define Prim function

def Kruskal(graph):

    alg_dll.Kruskal.argtypes = [ct.POINTER(Graph)]
    alg_dll.Kruskal.restype  = ct.POINTER(ct.POINTER(Edge))

    if graph.directed:
        return 0

    path = alg_dll.Kruskal(graph.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Edge) * (graph.nodes_num-1))).contents

    # the check
    print("Kruskal")
    for i in path:
        print(i.contents.C_SOURCE.contents.C_ID, i.contents.C_TARGET.contents.C_ID)


    # networkx Graph:
    nx_graph = nx.Graph()
    nx_graph.add_nodes_from(graph.get_nodes())
    nx_graph.add_edges_from(graph.get_edges())
    pos = nx.shell_layout(nx_graph)

    for edge_ptr in path:

        c_source_id = edge_ptr.contents.C_SOURCE.contents.C_ID
        c_target_id = edge_ptr.contents.C_TARGET.contents.C_ID

        edge = graph.get_edge(c_source_id, c_target_id)
        edge.color = "#FF0000"
        graph.update_edges_colors()

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

    graph.restore_nodes_colors()
    graph.restore_edges_colors()


#########################################################################
