import ctypes as ct
import _ctypes as ct_deb

import networkx as nx
import matplotlib.pyplot as plt
import time

from graphs import *

colors = ["cyan", "red", "green", "blue", "orange", "purple"]

#########################################################################
# define BFS function

def BFS(app, start):

    alg_dll.BFS.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
    alg_dll.BFS.restype = ct.POINTER(ct.POINTER(Node))

    path = alg_dll.BFS(app.graph.ptr, start.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * app.graph.nodes_num)).contents

    for node_ptr in path:

        c_node_id = node_ptr.contents.C_ID
        c_node_val = node_ptr.contents.C_VALUE
        node = app.graph.get_node(c_node_id)
        node.color = colors[c_node_val % 6]
        app.graph.update_nodes_colors()

        app.draw_graph()
        time.sleep(0.5)
        

#########################################################################
# define DFS function

def DFS(app, start):

    alg_dll.DFS.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
    alg_dll.DFS.restype = ct.POINTER(ct.POINTER(Node))

    path = alg_dll.DFS(app.graph.ptr, start.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * app.graph.nodes_num)).contents

    # the check
    print("DFS")
    for i in path:
        print(i.contents.C_ID, end=" ")
    print("\n")

    for node_ptr in path:

        c_node_id = node_ptr.contents.C_ID
        node = app.graph.get_node(c_node_id)
        node.color = "cyan"
        app.graph.update_nodes_colors()

        app.draw_graph()
        time.sleep(0.5)
        

#########################################################################
# define Dijkstra function

def Dijkstra(app, start):

    alg_dll.Dijkstra.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
    alg_dll.Dijkstra.restype  = ct.POINTER(ct.POINTER(Node))

    path = alg_dll.Dijkstra(app.graph.ptr, start.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * app.graph.nodes_num)).contents

    # the check
    print("Dijkstra")
    for i in path:
        print(i.contents.C_ID, end=" ")
    print("\n")

    for node_ptr in path:

        c_node_id = node_ptr.contents.C_ID
        node = app.graph.get_node(c_node_id)
        node.color = "cyan"
        app.graph.update_nodes_colors()

        app.draw_graph()
        time.sleep(0.25)



#########################################################################
# define Prim function

def Prim(app, root):

    if app.graph.directed:
        return 0

    alg_dll.Prim.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
    alg_dll.Prim.restype  = ct.POINTER(ct.POINTER(Edge))

    path = alg_dll.Prim(app.graph.ptr, root.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Edge) * (app.graph.nodes_num-1))).contents

    # the check
    print("Prim")
    for i in path:
        print(i.contents.C_SOURCE.contents.C_ID, i.contents.C_TARGET.contents.C_ID)


    for edge_ptr in path:

        c_source_id = edge_ptr.contents.C_SOURCE.contents.C_ID
        c_target_id = edge_ptr.contents.C_TARGET.contents.C_ID

        edge = app.graph.get_edge(c_source_id, c_target_id)
        edge.color = "#FF0000"
        app.graph.update_edges_colors()

        app.draw_graph()
        time.sleep(0.5)

    # Hide non-releavnt edges
    for i in range(app.graph.edges_num):
        if app.graph.edges_color[i] == graph.edges_default_color:
            app.graph.edges_color[i] = "#FFFFFF"  

    app.draw_graph()



#########################################################################
# define Prim function

def Kruskal(app):

    if app.graph.directed:
        return 0


    alg_dll.Kruskal.argtypes = [ct.POINTER(Graph)]
    alg_dll.Kruskal.restype  = ct.POINTER(ct.POINTER(Edge))

    path = alg_dll.Kruskal(app.graph.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Edge) * (app.graph.nodes_num-1))).contents

    # the check
    print("Kruskal")
    for i in path:
        print(i.contents.C_SOURCE.contents.C_ID, i.contents.C_TARGET.contents.C_ID)


    for edge_ptr in path:

        c_source_id = edge_ptr.contents.C_SOURCE.contents.C_ID
        c_target_id = edge_ptr.contents.C_TARGET.contents.C_ID

        edge = app.graph.get_edge(c_source_id, c_target_id)
        edge.color = "#FF0000"
        app.graph.update_edges_colors()

        app.draw_graph()
        time.sleep(0.5)

        
    # Hide non-releavnt edges
    for i in range(app.graph.edges_num):
        if app.graph.edges_color[i] == app.graph.edges_default_color:
            app.graph.edges_color[i] = "#FFFFFF"

    app.draw_graph()

#########################################################################
# define Bellman-Ford function

def BellmanFord(app, start):

    alg_dll.BellmanFord.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
    alg_dll.BellmanFord.restype  = ct.POINTER(ct.POINTER(Edge))

    path = alg_dll.BellmanFord(app.graph.ptr, start.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Edge) * app.graph.nodes_num)).contents

    # the check
    print("BellmanFord")
    for i in path:
        print(i.contents.C_SOURCE.contents.C_ID, i.contents.C_TARGET.contents.C_ID)

    for edge_ptr in path:

        c_source_id = edge_ptr.contents.C_SOURCE.contents.C_ID
        c_target_id = edge_ptr.contents.C_TARGET.contents.C_ID

        edge = app.graph.get_edge(c_source_id, c_target_id)
        edge.color = "#FF0000"
        app.graph.update_edges_colors()

        app.draw_graph()
        time.sleep(0.5)

#########################################################################
# define Ford-Fulkerson function

def FordFulkerson(app, source, target):

    if app.graph.directed == 0:
        return 0

    alg_dll.FordFulkerson.argtypes = [ct.POINTER(Graph), ct.POINTER(Node), ct.POINTER(Node)]
    alg_dll.FordFulkerson.restype  = ct.POINTER(ct.POINTER(ct.POINTER(Node)))

    paths = alg_dll.FordFulkerson(app.graph.ptr, source.ptr, target.ptr)
    paths = ct.cast(paths, ct.POINTER(ct.POINTER(ct.POINTER(Node)) * (app.graph.nodes_num))).contents

    for path in paths:

        path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * (app.graph.nodes_num))).contents
        path_len = 0

        for node_ptr in path:
            try:
                c_node_id = node_ptr.contents.C_ID
                node = app.graph.get_node(c_node_id)

                path_len += 1
                print(c_node_id)

                if node == target:
                    break

                node.color = "cyan"
                app.graph.update_nodes_colors()

                app.draw_graph()
                time.sleep(0.25)
            except ValueError:
                break

        if path_len == 0:
            return 0

#########################################################################