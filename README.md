# ALGO-PROJECT-2

Project Specification:
You are asked to design and implement an algorithm for Graph Coloring problem.
Your goal is not to design an algorithm for the optimal solution, but you are requested to do your best. This is an open-ended assignment.

Input format:
  Inputs will always be given as a text file. Input file format should be as follows:
	The first line is the problem line which indicates the number of vertices and the number of edges. It starts with “p”, and there is only one such line.
	p <NumVertices> <NumEdges>The rest of the lines are edge lines. Each line starts with “e” and indicates an edge between two vertices.
	e <VertexNumber1> <VertexNumber2>
	The above line means that there is an edge between VertexNumber1 and VertexNumber2.
	Vertices should always be numbered from 1 to n, where n is the number of vertices.
	No other input format will be accepted!

Output format:
  The output should be a text file that consists of two lines:
	- The first line should include a single integer value, the number of colors used (k).
	- The second line should include the labels of the colors used for all vertices, from vertex 1 to vertex n. Label values are integers between zero and k-1 and they are separated by a space character.
	No other output format will be accepted!
