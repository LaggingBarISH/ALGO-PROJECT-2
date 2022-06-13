import sys  #Needed for sys.exit() calls

#Vertex structure
class Vertex:
    def __init__(self):
        self.color = 0          # Vertex color
        self.edgeColors = []    # Colors used on neighboring verticies
        self.edges = []         # Neighboring verticies
        self.coloredEdges = 0   # Number of colored neighbors

#Reads given input file.
def ReadFile(filename):
    try:
        readfrom = open(str(filename), 'r') # Try to open file as readonly.
    except:
        print("Error opening input file!")  # Handle exception inform user and stop execution.
        sys.exit()
    
    allLines = readfrom.readlines()         # Read all lines from the input.
    if (allLines[0].split(' ')[0] != 'p'):  # Check if the first line starts with a 'p'
        print("Input format incorrect!")    # Inform user and stop execution if first line doesn't start with 'p'
        sys.exit()
    
    verticiesCount = (int)(allLines[0].strip().split(' ')[1])   # Get the number of verticies from the first line
    edgesCount = (int)(allLines[0].strip().split(' ')[2])       # Get the number of edge connections from the first line
    
    if (edgesCount + 1 != len(allLines)):   # Check if the file has the same number of lines as expected
        print("Input format incorrect!")    # Inform user and stop execution if line count doesn't match
        sys.exit()

    verticies = []
    for i in range(verticiesCount):
        verticies.append(Vertex())          # Create an list of Vertex objects
    for i in range(edgesCount):             # Add each Vertex neighbors from the input file
        verticies[int(allLines[i + 1].strip().split(' ')[1]) - 1].edges.append(int(allLines[i + 1].strip().split(' ')[2]) - 1)
        verticies[int(allLines[i + 1].strip().split(' ')[2]) - 1].edges.append(int(allLines[i + 1].strip().split(' ')[1]) - 1)
    return verticies                        # Return the newly generated list

verticies = ReadFile("input.txt")           # Call to read file and parse it into variable verticies

for i in range(len(verticies)):
    process = None                          # Currently selected Vertex
    for j in range(len(verticies)):         # Gets and selects the appropiate Vertex to process
        if verticies[j].color != 0: continue    # If current Vertex is already colored continue to the next Vertex
        if process == None:                 
            process = verticies[j]              # If process is not set yet and current vertex is not colored, set process to current vertex and continue to the next vertex
            continue
        if len(process.edgeColors) < len(verticies[j].edgeColors): # If current vertex has more colors on its neighbors than process set process to current vertex and continue to the next vertex
            process = verticies[j]
            continue
        if len(process.edgeColors) > len(verticies[j].edgeColors): # If current vertex has less colors on its neighbors than process continue to the next vertex
            continue

        # If current vertex has more uncolored neighbors than process set process to current vertex and continue to the next vertex
        if len(process.edges) - process.coloredEdges < len(verticies[j].edges) - verticies[j].coloredEdges:
            process = verticies[j]
            continue
    
    for j in range(1, len(process.edges) + 2):  # Color the selected vertex with the minumum possible number/color
        if j in process.edgeColors: continue    # If current color is used by one of the neighboring verticies skip color.
        process.color = j
        break
    
    for j in range(len(process.edges)):         # Update all neighboring verticies
        verticies[process.edges[j]].coloredEdges += 1                               # Add 1 to coloredEdges field of each neighboring vertex
        
        if int(process.color) in verticies[process.edges[j]].edgeColors: continue   # If used color wasn't already in edgeColors of neighboring vertex add current color to edgeColors field of neighboring vertex
        verticies[process.edges[j]].edgeColors.append(int(process.color)) 

highest = 0
buffer = ""

for i in range(len(verticies)):                 # Loop all verticies
    if highest < verticies[i].color: highest = int(verticies[i].color)  # If current vertex has a larger number assigned than current highest, update highest.
    buffer += str(verticies[i].color - 1) + " "                         # Add the color/number of current vertex to a buffer for output.

wrtiteto = open("output.txt", "w")              # Open the output file in write / overwrite mode.
wrtiteto.write(str(highest) + '\n')             # Write the highest color/number to output.
wrtiteto.write(buffer)                          # Write the buffer of vertex colors to output.