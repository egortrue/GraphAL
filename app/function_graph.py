from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib import pyplot as plt
import networkx as nx


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


def draw_graph(app, fig_agg: FigureCanvasTkAgg, graph: nx.Graph, pos, ax: plt.Axes):
    """

    :param fig_agg:
    :param graph:
    :param pos:
    :param ax:
    :return:
    """
    app.draw_graph_flag = True
    nx.draw(graph, pos, ax)
    fig_agg.draw()


def to_adjacency_matrix(dict_of_edges, weighted=None):  # todo definition
    """

    :param app:
    :param dict_of_edges:
    :param weighted:
    :return:
    """
    data_to_adjacency_matrix_preview = ""

    return data_to_adjacency_matrix_preview


def to_adjacency_list(dict_of_edges, weighted=None):  # todo definition
    """

    :param app:
    :param dict_of_edges:
    :param weighted:
    :return:
    """

    data_to_adjacency_list_preview = ""

    if weighted:
        for node, neighbors in dict_of_edges.items():
            data_to_adjacency_list_preview += f"{node}:  "

            for neighbor, attributes in neighbors.items():
                data_to_adjacency_list_preview += f"{neighbor} {attributes['weight']},  "

            data_to_adjacency_list_preview += "\n"

    else:
        for node, neighbors in dict_of_edges.items():
            data_to_adjacency_list_preview += f"{node}:  "

            for neighbor in neighbors:
                data_to_adjacency_list_preview += f"{neighbor},  "

            data_to_adjacency_list_preview += "\n"

    return data_to_adjacency_list_preview


def to_list_edges(dict_of_edges, weighted=None):  # todo definition
    """

    :param app:
    :param dict_of_edges:
    :param weighted:
    :return:
    """

    list_edge = ""

    if weighted:
        for node, neighbors in dict_of_edges.items():

            for neighbor, attributes in neighbors.items():
                list_edge += f"{node} {neighbor}  {attributes['weight']}\n"

    else:
        for node, neighbors in dict_of_edges.items():

            for neighbor in neighbors:
                list_edge += f"{node} {neighbor}\n"

    return list_edge


def preview(app, graph=nx.Graph, weighted=None):  # todo definition
    """

    :param app:
    :param graph:
    :param weighted:
    :return:
    """

    dict_of_edges = nx.to_dict_of_dicts(graph)

    app['-MATRIX-ADJ-'].update(value=to_adjacency_matrix(dict_of_edges, weighted))
    app['-LIST-ADJ-'].update(value=to_adjacency_list(dict_of_edges, weighted))
    app['-LIST-EDGES-'].update(value=to_list_edges(dict_of_edges, weighted))


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
