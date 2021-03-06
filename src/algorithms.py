import ctypes as ct
import _ctypes as ct_deb

import networkx as nx
import matplotlib.pyplot as plt
import time

from graphs import *

colors = ["cyan", "red", "green", "yellow", "orange", "purple"]

#########################################################################

def BFS(app, start):

    alg_dll.BFS.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
    alg_dll.BFS.restype = ct.POINTER(ct.POINTER(Node))

    path = alg_dll.BFS(app.graph.ptr, start.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * app.graph.nodes_num)).contents

    app.graph.networkx_graph()

    for iteration, node_ptr in enumerate(path):

        app.clear_figure_canvas() 

        c_node_id = node_ptr.contents.C_ID
        c_node_val = node_ptr.contents.C_VALUE

        node = app.graph.get_node(c_node_id)
        node.color = colors[c_node_val % 6]
        node.name += f"|{c_node_val}"
        
        app.graph.update_nodes_colors()
        app.graph.update_nodes_labels()
        app.draw_graph()
        app.write_to_log_out(iteration + 1)
        time.sleep(0.5)
        

#########################################################################

def DFS(app, start):

    alg_dll.DFS.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
    alg_dll.DFS.restype = ct.POINTER(ct.POINTER(Node))

    path = alg_dll.DFS(app.graph.ptr, start.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * app.graph.nodes_num)).contents

    for iteration, node_ptr in enumerate(path):

        app.clear_figure_canvas() 

        c_node_id = node_ptr.contents.C_ID
        c_node_val = node_ptr.contents.C_VALUE

        node = app.graph.get_node(c_node_id)
        node.color = colors[c_node_val % 6]
        node.name += f"|{c_node_val}"
        
        app.graph.update_nodes_colors()
        app.graph.update_nodes_labels()
        app.draw_graph()
        app.write_to_log_out(iteration + 1)
        time.sleep(0.5)
        

#########################################################################

def Dijkstra(app, start):

    alg_dll.Dijkstra.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
    alg_dll.Dijkstra.restype  = ct.POINTER(ct.POINTER(Node))

    path = alg_dll.Dijkstra(app.graph.ptr, start.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Node) * app.graph.nodes_num)).contents

    for iteration, node_ptr in enumerate(path):

        app.clear_figure_canvas()   

        c_node_id = node_ptr.contents.C_ID
        c_node_val = node_ptr.contents.C_VALUE

        node = app.graph.get_node(c_node_id)
        node.color = colors[c_node_val % 6]
        node.name += f"|{c_node_val}"
        
        app.graph.update_nodes_colors()
        app.graph.update_nodes_labels()
        app.draw_graph()
        app.write_to_log_out(iteration + 1)
        time.sleep(0.5)



#########################################################################

def Prim(app, root):

    alg_dll.Prim.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
    alg_dll.Prim.restype  = ct.POINTER(ct.POINTER(Edge))

    path = alg_dll.Prim(app.graph.ptr, root.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Edge) * (app.graph.nodes_num-1))).contents

    for iteration, edge_ptr in enumerate(path):

        c_source_id = edge_ptr.contents.C_SOURCE.contents.C_ID
        c_target_id = edge_ptr.contents.C_TARGET.contents.C_ID

        edge = app.graph.get_edge(c_source_id, c_target_id)
        edge.color = "#FF0000"

        app.graph.update_edges_colors()
        app.draw_graph()
        app.write_to_log_out(iteration + 1, info="edge")
        time.sleep(0.5)

    # Hide non-releavant edges
    for i in range(app.graph.edges_num):
        if app.graph.edges_color[i] == app.graph.edges_default_color:
            app.graph.edges_color[i] = "#FFFFFF"  

    app.draw_graph()

#########################################################################

def Kruskal(app):

    alg_dll.Kruskal.argtypes = [ct.POINTER(Graph)]
    alg_dll.Kruskal.restype  = ct.POINTER(ct.POINTER(Edge))

    path = alg_dll.Kruskal(app.graph.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Edge) * (app.graph.nodes_num-1))).contents

    for iteration, edge_ptr in enumerate(path):

        c_source_id = edge_ptr.contents.C_SOURCE.contents.C_ID
        c_target_id = edge_ptr.contents.C_TARGET.contents.C_ID

        edge = app.graph.get_edge(c_source_id, c_target_id)
        edge.color = "#FF0000"

        app.graph.update_edges_colors()
        app.draw_graph()
        app.write_to_log_out(iteration + 1, info="edge")
        time.sleep(0.5)
   
    # Hide non-releavnt edges
    for i in range(app.graph.edges_num):
        if app.graph.edges_color[i] == app.graph.edges_default_color:
            app.graph.edges_color[i] = "#FFFFFF"

    app.draw_graph()

#########################################################################

def FloydWarshall(app):

    alg_dll.FloydWarshall.argtypes = [ct.POINTER(Graph)]
    alg_dll.FloydWarshall.restype  = ct.POINTER(ct.POINTER(ct.POINTER(Edge)))

    matrix_edge = alg_dll.FloydWarshall(app.graph.ptr)
    rows_edge = ct.cast(matrix_edge, ct.POINTER(ct.POINTER(ct.POINTER(Edge)) * (app.graph.nodes_num))).contents

    matrix = []

    for row_edge in rows_edge:

        row_edge = ct.cast(row_edge, ct.POINTER(ct.POINTER(Edge) * (app.graph.nodes_num))).contents
        matrix_row = []

        for edge_ptr in row_edge:
            matrix_row.append(edge_ptr.contents.C_WEIGHT if edge_ptr.contents.C_WEIGHT <= app.graph.max_weight else None)

        matrix.append(matrix_row)

    graph = convert_from_matrix_to_graph(matrix, app.graph.min_weight, app.graph.max_weight, 0b11)

    # Clear data of old graph
    gen_dll.AMatrixDelete(app.graph.matrix_ptr)
    alg_dll.GraphDestroy(app.graph.ptr)

    app.graph = graph
    app.clear_figure_canvas() 
    app.draw_graph()


#########################################################################
##
##                          BETA FUNCTIONS
##             ( Here are a lot of problems with pointers )
##
#########################################################################

def BellmanFord(app, start):

    alg_dll.BellmanFord.argtypes = [ct.POINTER(Graph), ct.POINTER(Node)]
    alg_dll.BellmanFord.restype  = ct.POINTER(ct.POINTER(Edge))

    path = alg_dll.BellmanFord(app.graph.ptr, start.ptr)
    path = ct.cast(path, ct.POINTER(ct.POINTER(Edge) * app.graph.nodes_num)).contents

    for iteration, edge_ptr in enumerate(path):

        c_source_id = edge_ptr.contents.C_SOURCE.contents.C_ID
        c_target_id = edge_ptr.contents.C_TARGET.contents.C_ID

        edge = app.graph.get_edge(c_source_id, c_target_id)
        edge.color = "#FF0000"

        app.graph.update_edges_colors()
        app.draw_graph()
        app.write_to_log_out(iteration + 1, info="edge")
        time.sleep(0.5)

def FordFulkerson(app):

    alg_dll.FordFulkerson.argtypes = [ct.POINTER(Graph), ct.POINTER(Node), ct.POINTER(Node)]
    alg_dll.FordFulkerson.restype  = ct.POINTER(ct.POINTER(ct.POINTER(Node)))

    paths = alg_dll.FordFulkerson(app.graph.ptr, app.graph.nodes[app.graph.nodes_num-2].ptr, app.graph.nodes[app.graph.nodes_num-1].ptr)
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

