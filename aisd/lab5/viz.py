import pydot

(graph,) = pydot.graph_from_dot_file('labirint.dot')
graph.write_png('labirint.png')
