#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Verticies;
int readFile(int*** lines, char* filename);                                         //Function declaration
int* getIntsFromString(char* line, char seperator, int indexCount, int *indexes);   //Function declaration
int getDifferentColoredConnections(struct Verticies *verticies, int node);          //Function declaration
int getMinimumColor(struct Verticies *verticies, int node);                         //Function declaration
int addColorToNeighbor(struct Verticies *verticies, int node, int color);           //Function declaration
int colorNext(struct Verticies *verticies, int arrayLen);                           //Function declaration

//Vertex structure
struct Verticies
{
    int color;                  //Vertex color
    int coloredEdges;           //Colored neighbor count
    int connectionCount;        //Number of edges
    int *connectedTo;           //Neighbors
    int *connectionColors;      //Used colors for neighbors
    int connectionColorCount;   //Amount of used colors on neighbors
};

//Reads a given input file and parses it into a string array
int readFile(int*** lines, char* filename){
    int lineIndex = 0, edgeCount = 0;
    FILE *file = fopen(filename, "r");                      //Open input file to be read
    if (file == NULL){                                      //If opening file for read operation failes print error and exit program
        printf("Failed to open file!\n");
        fflush(stdout);
        exit(1);
    }

    //No need to check for input file to be correct since the pdf states "No other input format will be accepted!".
    char *line = malloc(sizeof(char) * 26);                 //"p INT_MAX INT_MAX" is 23 chars at most so 26 is a safe bet with the newline char "\r\n" on MS-DOS systems and '\0' termination.
    if(line == NULL){                                       //If allocating of a line fails print error and exit program
        printf("Failed to allocate char array!\n");
        fflush(stdout);
        exit(1);
    }

    fscanf(file, "%[^\n] ", line);                          //Read the first line.
    rewind(file);                                           //Rewind the file pointer
    //int edgeCount = getIntFromString(line, ' ', 2);       //Gets the edgeCount from input file
    int j[] = {2};
    edgeCount = getIntsFromString(line, ' ', 1, j)[0];      //Gets the edgeCount from input file

    *lines = malloc(sizeof(void*) * (edgeCount + 1));       //Allocate memory to hold every connection info
    if(*lines == NULL){                                     //If the allocation failes print error and exit program
        printf("Failed to allocate int array!\n");
        fflush(stdout);
        exit(1);
    }

    int indexes[] = {1, 2};                                 //Indexes for getIntsFromString function call
    while (fscanf(file, "%[^\n] ", line) != EOF) {          //Read the input file line by line until end of file
        int *currentLine = getIntsFromString(line, ' ', 2, indexes);    //Read the current line into buffer
        (*lines)[lineIndex] = getIntsFromString(line, ' ', 2, indexes); //Parse the buffer and fill lineIndex with values
        lineIndex++;                                        //Add 1 to line count
    }
    
    free(line);         //Free memory for allocated for line
    fclose(file);       //Close the file handle
    return lineIndex;   //Return the line count

}

//Gets pozitive integers from given string with a seperator and at specific indexes
//Needs sorted index values to work properly. Won't be sorting input since this code doesn't need it and it would be bloat
int* getIntsFromString(char* line, char seperator, int indexCount, int *indexes){
    int index = 0, i = 0, atIndex = 0;                      //Returns -1 if cant find a valid pozitive integer.
    int *result = malloc(sizeof(int) * indexCount);
    memset(result, -1, sizeof(int) * indexCount);

    for(;; i++){                                            //Loop indefinitely
        if(index == indexes[atIndex]) {                     //If at the correct seperation index step in 
            if(line[i] == '\0') return result;              //If reached end of line return current result
            if(line[i] == seperator){                       //Check for the seperator
                if(result[atIndex] == -1) continue;         //Escape multiple seperators
                if(!(atIndex < indexCount)) return result;  //If reached a seperator return current result
                atIndex++;                                  //Found Int counts
                index++;                                    //Seperation index
                continue;
            } 
            if(line[i] < '0' || line[i] > '9') continue;    //If current character is not an integer continue
            if(result[atIndex] == -1) result[atIndex] = (int)line[i] - (int)'0'; //If result is not set yet set it to current char
            else {                                          //If result is set multiply it by 10 and add current char to its value
                result[atIndex] *= 10;                      //Adds the next digit to result
                result[atIndex] += (int)line[i] - (int)'0'; //Adds the next digit to result
            }
        }
        else {                                              //If seperation index is different from the requested index step in
            if(line[i] == '\0') return result;              //If reached end of line without reaching the correct index return -1
            if(line[i] == seperator) index++;               //If reached a seperator reduce 1 from seperatoion index 
        }
    }
}

//Gets the different colors a vertex has as neighbors
int getDifferentColoredConnections(struct Verticies *verticies, int node){
    int *colors = malloc(sizeof(int) * verticies[node].connectionCount);            //Allocate a colors array sized to fit all neighbors colors
    memset(colors, -1, sizeof(int) * verticies[node].connectionCount);              //Initialize the whole array as -1 //Only really works for -1 and 0
    
    int i = 0, j = 0;
    for(i = 0; i < verticies[node].connectionCount; i++){                           //Loop all neighbors
        if(verticies[verticies[node].connectedTo[i]].color == -1) continue;         //If current neighbor doesnt have a color set continue
        
        for(j = 0; j < verticies[node].connectionCount; j++){                       //Loop all possible locations on the colors array
            if(verticies[verticies[node].connectedTo[i]].color == colors[j]) break; //If current color is set on the array break loop
            if(colors[j] == -1){                                                    //If current index of colors is not set yet set it to current neighbors color and break the loop
                colors[j] = verticies[verticies[node].connectedTo[i]].color;
                break;
            }
        }
    }

    for(i = 0; i < verticies[node].connectionCount; i++){                           //Get the number of set values on colors array
        if(colors[i] == -1) break;                                                  //If current index is not set break the loop
    }
    return i; //Return the number of different colors
}

//Get minumum available color for a given vertex
int getMinimumColor(struct Verticies *verticies, int node){
    int i = 0, color = 0;

    for(i = 0; i < verticies[node].connectionColorCount; i++){  //Loop all the neighbors
        if(color == verticies[node].connectionColors[i]){       //Step in if current color is used by a neighbor
            color++;                                            //Increase the color by 1
            i = -1;                                             //Reset i to check every neighbor for the new color
        }
    }
    return color; //Returns color
}

//Adds color to all neighboring vertexes of given vertex
int addColorToNeighbor(struct Verticies *verticies, int node, int color /*Don't really have to specify the color it could be read from the given vertex but the code looks ugly that way*/){
    int i = 0, j = 0;

    for(i = 0; i < verticies[node].connectionCount; i++){                                       //Loop all neighbors of current node
        if(verticies[verticies[node].connectedTo[i]].color != -1) continue;                     //If current neighbor is already colored continue to the next neighbor
        verticies[verticies[node].connectedTo[i]].coloredEdges += 1;                            //Add 1 to coloredEdges field of the current neighbor

        for(j = 0; j < verticies[verticies[node].connectedTo[i]].connectionColorCount; j++){    //Loop all connectionColors of current neighbor
            if(verticies[verticies[node].connectedTo[i]].connectionColors[j] == color) break;   //If current neighbor already has the color to be added in connectionColors break the loop
        }

        if(j == verticies[verticies[node].connectedTo[i]].connectionColorCount){                //Step in if current neighbor doesn't have the color to be added in connectionColors
            verticies[verticies[node].connectedTo[i]].connectionColorCount += 1;                //Add 1 to connectionColorCount field of the current neigbor
            //Reallocate the connectionColors array of current neighbor to hold the new color
            verticies[verticies[node].connectedTo[i]].connectionColors = realloc(verticies[verticies[node].connectedTo[i]].connectionColors, sizeof(int) * verticies[verticies[node].connectedTo[i]].connectionColorCount);
            if(verticies[verticies[node].connectedTo[i]].connectionColors == NULL){             //If re allocating connectionColors fails print error and exit program
                printf("Failed to re allocate int array!\n");
                fflush(stdout);
                exit(1);
            }
            
            //Add the new color to connectionColors array of current neighbor
            verticies[verticies[node].connectedTo[i]].connectionColors[verticies[verticies[node].connectedTo[i]].connectionColorCount - 1] = color;
        }
    }
    return 0;
}

//Colors the next available vertex
int colorNext(struct Verticies *verticies, int arrayLen){
    int i = 0, currentMax = 0, colored = 0;

    for(i = 0; i < arrayLen; i++){                          //Loop every vertex on the array
        if(verticies[i].color != -1) continue;              //If current vertex is already colored continue
        
        int j = 0, currentColored = 0;
        currentColored = verticies[i].connectionColorCount; //Get the number of different colored neigbors on current vertex
        
        if(currentColored > colored){                       //If current vertex has more different colors than the previously selected vertex step in
            currentMax = i;                                 //Set the current max to current vertex
            colored = currentColored;                       //Set colored to currrent vertexes differently colored neighbor count
            continue;                                       //Continue with the next vertex
        }
        
        if(currentColored == colored){                      //If current vertex has the same amount of different colors compared to the previously selected vertex step in
            if(verticies[i].connectionCount - verticies[i].coloredEdges > verticies[currentMax].connectionCount - verticies[currentMax].coloredEdges){  //If current vertex has more neighbors than the previously selected vertex step in
                currentMax = i;                             //Set the current max to current vertex
                colored = currentColored;                   //Set colored to currrent vertexes differently colored neighbor count
                continue;                                   //Continue with the next vertex
            }
        }
    }
    verticies[currentMax].color = getMinimumColor(verticies, currentMax);   //Set the selected vertexes color to the minimum possible color available
    addColorToNeighbor(verticies, currentMax, verticies[currentMax].color); //Add current nodes colors to neighbors edgeColors

    return 0;
}

int main(void){
    int **lines = NULL;                                 //Int array
    readFile(&lines, "input.txt");                      //Read file into lines array
    int i = 0;
    
    struct Verticies* verticies = malloc(sizeof(struct Verticies) * lines[0][0]);   //Allocation of verticies structure with the size given in the input file
    if(verticies == NULL){                              //If allocating of verticies fail print error and exit program
        printf("Failed to allocate memory for vertex array!\n");
        fflush(stdout);
        exit(1);
    }

    for(i = 0; i < lines[0][0]; i++){                   //Loop every vertex
        verticies[i].color = -1;                        //Set current vertexes color to -1
        verticies[i].connectionCount = 0;               //Set current vertexes connection count to 0
        verticies[i].connectedTo = NULL;                //Set current vertexes connectedTo array to NULL
        verticies[i].coloredEdges = 0;                  //Set current vertexes coloredEdges array to 0
        verticies[i].connectionColorCount = 0;          //Set current vertexes connectionColorCount array to 0
        verticies[i].connectionColors = NULL;           //Set current vertexes connectionColors array to NULL
    }

    for(i = 1; i <= lines[0][1]; i++){                  //Loop every edge connections
        verticies[lines[i][0] - 1].connectionCount++;   //Increase the connection count of the vertex given in the input file by 1
        verticies[lines[i][1] - 1].connectionCount++;   //Increase the connection count of the vertex given in the input file by 1
        verticies[lines[i][0] - 1].connectedTo = realloc(verticies[lines[i][0] - 1].connectedTo, sizeof(int) * verticies[lines[i][0] - 1].connectionCount); //Reallocate memory of current vertex to hold the new neighbor
        verticies[lines[i][1] - 1].connectedTo = realloc(verticies[lines[i][1] - 1].connectedTo, sizeof(int) * verticies[lines[i][1] - 1].connectionCount); //Reallocate memory of current vertex to hold the new neighbor
        if(verticies[lines[i][0] - 1].connectedTo == NULL || verticies[lines[i][1] - 1].connectedTo == NULL){       //If re allocating connectedTo fails print error and exit program
            printf("Failed to re allocate int array!\n");
            fflush(stdout);
            exit(1);
        }
        verticies[lines[i][0] - 1].connectedTo[verticies[lines[i][0] - 1].connectionCount - 1] = lines[i][1] - 1;   //Add neighboring vertex to the current vertex
        verticies[lines[i][1] - 1].connectedTo[verticies[lines[i][1] - 1].connectionCount - 1] = lines[i][0] - 1;   //Add neighboring vertex to the current vertex
    }

    for(i = 0; i < lines[0][0]; i++){                   //Loop every vertex
        colorNext(verticies, lines[0][0]);              //Color the next vertex
    }

    //Get the number of colors used for the vertex array
    int colorsused = 0;
    for(i = 0; i < lines[0][0]; i++){                   //Loop every vertex
        if (colorsused < verticies[i].color) colorsused = verticies[i].color;   //If the current vertexes color is larger than colorsused, set colorsused to current vertexes color value
    }

    FILE *out;
    out = fopen("output.txt", "w");                             //Create the output file
    
    fprintf(out, "%i\n%i", colorsused + 1, verticies[0].color); //Write the number of colors used and the first vertexes color to the file.
    for(i = 1; i < lines[0][0]; i++){                           //Using a for loop for writing to files is fine because fprintf is fully buffered.
        fprintf(out, " %i", verticies[i].color);                //Write the rest of the vertexes colors to the output file.
    }
    fflush(out);                                                //Flush file buffers
    fclose(out);                                                //Close file handle
    return 0;
}