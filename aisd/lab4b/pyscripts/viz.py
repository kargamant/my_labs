import pydot

(graph,) = pydot.graph_from_dot_file('tree_pic.dot')
graph.write_png('your_tree.png')
