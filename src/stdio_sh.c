#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stdio_sh.h"

int ArgLineCheck(const int given)
{
	if (given < NUM_ARGS_CMD)
	{
		printf("\nERROR: Not enough arguments. ");
		printf("Expected at least %d. Given %d\n", NUM_ARGS_CMD, given);
		exit(EXIT_FAILURE);
	}
	return 0;
}

FILE* FileOpen(const char* name, const char* mode)
{
	FILE* file;
	//fopen_s(&file, name, mode);

	if (!file)
	{
		if (*mode == 'r')
			puts("\nERROR: The file was not opened for reading");
		else if (*mode == 'w')
			puts("\nERROR: The file was not opened for writing");
		exit(EXIT_FAILURE);
	}
	return file;
}

int ConvertStrToInt(char* string)
{
	int N = 0, i = 0, c = 1;
	if (*string == 45)
	{
		c = -1;
		string++;
	}
	while ((int)string[i])
		N = N * 10 + (int)string[i++] - 48;
	return N * c;
}

// Parsing of string on 2 or 3 substrings
char** ObjectRead(FILE* fr, char* string, int flag)
{
	// block of data which will be returned
	char** obj = (char**)calloc(3, sizeof(char*));
	obj[0] = (char*)calloc(STR_LEN_MAX+1, sizeof(char));
	obj[1] = (char*)calloc(STR_LEN_MAX+1, sizeof(char));
	obj[2] = (char*)calloc(STR_LEN_MAX+1, sizeof(char));
	if (!obj || !obj[0] || !obj[1] || !obj[2]) exit(EXIT_FAILURE);


	// before first whitespace
	char* begin = string;
	
	while (*string != 32 && *string != 10) string++;
	memmove(obj[0], begin, string-begin);
	string++;

	// after first and before seconf whitespace
	char* first_space = string;

	while (*string != 32 && *string != 10) string++;
	memmove(obj[1], first_space, string - first_space);
	string++;

	// after second whitespace
	if (flag)
	{
		char* second_space = string;
		while (*string != 32 && *string != 10) string++;
		memmove(obj[2], second_space, string - second_space);
	}

	return obj;
}

int StringFind(FILE* fr, char* str, long file_size)
{
	// Go to the begin of file
	fseek(fr, 0, SEEK_SET);

	// Buffer for checking
	char* string = (char*)calloc(STR_LEN_MAX, sizeof(char));
	if (!string) exit(EXIT_FAILURE);

	// Skip the strings while it is not a neccessary string
	while (strcmp(string, str))
	{
		// Read a string from file
		fgets(string, STR_LEN_MAX, fr);

		// if the end of file
		if (ftell(fr) == file_size)
		{
			free(string);
			return 0;
		}
	}

	free(string);
	return 1;
}

// Read the block of data which includes some strings ( block names: [nodes] and [edges] )
char*** BlockRead(FILE* fr, char* str, long file_size, int* count)
{
	// Parsing depends on block name
	int flag = 0;
	if (*(++str) == 'e') // 'e' means "edges"
		flag = 1;

	// Common buffer of all the data nodes/edges
	char*** data = (char***)calloc(OBJ_COUNT_MAX, sizeof(char**));
	if (!data) exit(EXIT_FAILURE);

	*count = 0; // counter of objects

	// Local buffer
	char* string = (char*)calloc(STR_LEN_MAX, sizeof(char));
	if (!string) exit(EXIT_FAILURE);

	fgets(string, STR_LEN_MAX, fr);
	while (*string == 0 || *string != 10)
	{
		// Parsing the current string
		char** obj = ObjectRead(fr, string, flag);
		data[(*count)++] = obj;

		// If the end of file
		if (ftell(fr) == file_size)
			break;

		// Read again
		fgets(string, STR_LEN_MAX, fr);
	}

	free(string);
	return data;
}

GRAPH* FileRead(FILE* fr)
{
	// Size of file in bytes
	fseek(fr, 0, SEEK_END);
	long file_size = ftell(fr);

	int count_buf = 0;
	// --------------------------------------------------------
	// Read block [nodes]
	// nodes_data[i] <===> [ name, value ]
	if (!StringFind(fr, "[nodes]\n", file_size))
	{
		printf("\nERROR: the read file doesn't have key string: [nodes]\n");
		exit(EXIT_FAILURE);
	}
	char*** nodes_data = BlockRead(fr, "[nodes]\n", file_size, &count_buf);
	int nodes_num = count_buf;

	// --------------------------------------------------------
	// Read block [edges]
	// edges_data[i] <===> [ source, target, weight ]
	if (!StringFind(fr, "[edges]\n", file_size))
	{
		printf("\nERROR: the read file doesn't have key string: [edges]\n");
		exit(EXIT_FAILURE);
	}
	char*** edges_data = BlockRead(fr, "[edges]\n", file_size, &count_buf);
	int edges_num = count_buf;

	// --------------------------------------------------------
	// Init the graph 
	GRAPH* G = GraphSet(nodes_num, edges_num, StringFind(fr, "[directed]\n", file_size));

	// --------------------------------------------------------
	// Init the nodes
	for (int i = 0; i < nodes_num; i++)
		G->nodes[i] = NodeSet(ConvertStrToInt(nodes_data[i][0]),
							  ConvertStrToInt(nodes_data[i][1]));

	// --------------------------------------------------------
	// Init the edges
	NODE* source = NULL;
	NODE* target = NULL;
	for (int i = 0; i < edges_num; i++)
	{
		source = NodeGetAddress(G->nodes, ConvertStrToInt(edges_data[i][0]), nodes_num);
		target = NodeGetAddress(G->nodes, ConvertStrToInt(edges_data[i][1]), nodes_num);

		G->edges[i] = EdgeSet(source, target, ConvertStrToInt(edges_data[i][2]));
	}

	// --------------------------------------------------------
	// Clear big blocks of data
	for (int i = 0; i < nodes_num; i++)
	{
		for (int j = 0; j < 3; j++)
			free(nodes_data[i][j]);
		free(nodes_data[i]);
	}
	free(nodes_data);

	for (int i = 0; i < edges_num; i++)
	{
		for (int j = 0; j < 3; j++)
			free(edges_data[i][j]);
		free(edges_data[i]);
	}
	free(edges_data);

	return G;
}
//------------------------------------------------------------------------------------------------------
//PARSER
AMATRIX *AMatrixRead(FILE *input)
{
    // Size of file in bytes
    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);

    int count_buf = 0;

    //Read block of adjacency matrix
    if (!StringFind(input, "[adjacency matrix]\n", file_size))
    {
        printf("\nERROR: the read file doesn't have key string: [adjacency matrix]\n");
        exit(EXIT_FAILURE);
    }
    long long int pos;
    pos = ftell(input);
    int nodes = AMatrixCountNodes(input);
    AMATRIX *adj_matrix = AMatrixSet(nodes);
    fseek(input, pos, SEEK_SET);
    StringFind(input, "[adjacency matrix]\n", file_size);
    char str_new = fgetc(input);
    char *chr = NULL;
    chr = (char*)malloc(0*sizeof(char*));
    int line = 0, column = 0, j = 0;

    while (line != nodes && column != nodes) {
        if (str_new >= 48 && str_new <= 57){
            chr[j++] = str_new;
        }

        else if (str_new == ' ') {
            adj_matrix->adj[line][column] = atoi(chr);
            if (adj_matrix->adj[line][column] != 0)
                adj_matrix->e++;
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            j = 0;
            column++;
        }

        else if (str_new == '\n') {
            adj_matrix->adj[line][column] = atoi(chr);
            if (adj_matrix->adj[line][column] != 0)
                adj_matrix->e++;
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            column = 0;
            line++;
            j = 0;
        }

        str_new = fgetc(input);
        if (ftell(input) == file_size)
            break;
    }
    adj_matrix->adj[line][column] = atoi(chr);
    if (adj_matrix->adj[line][column] != 0)
        adj_matrix->e++;

    return adj_matrix;
}
//------------------------------------------------------------------------------------------------------
//format:
//1-1 2 3
//4-3 1 5
ALIST** AListRead(FILE* input, FILE* output, int* count_edges)
{
    // Size of file in bytes
    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);

    //Read block of adjacency matrix
    if (!StringFind(input, "[adjacency list]\n", file_size))
    {
        printf("\nERROR: the read file doesn't have key string: [adjacency list]\n");
        exit(EXIT_FAILURE);
    }

    int vertex = AListCountVertex(input);
    ALIST **graph= (ALIST**)calloc(vertex, sizeof(ALIST*));
    int j = 0;
    char *chr = (char*)malloc(sizeof(char*));
    int line = 0;
    char str_new;
    str_new = fgetc(input);

    while (str_new != EOF) {
        if (str_new >= 48 && str_new <= 57){
            chr = realloc(chr, j+1 * sizeof(char*));
            chr[j++] = str_new;
        }

        else if (str_new == '-') {
            line = atoi(chr);
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            j = 0;
        }

        else if (str_new == ' ' && chr != 0) {
            AListAddValue(&graph[line], atoi(chr));
            (*count_edges)++;
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            j = 0;
        }

        else if (str_new == '\n') {
            AListAddValue(&graph[line], atoi(chr));
            (*count_edges)++;
            line = 0;
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            j = 0;
        }

        str_new = fgetc(input);
    }
    AListAddValue(&graph[line], atoi(chr));

    rewind(input);
    return graph;
}

aListg *AListReads(FILE* input, FILE* output, int* count_edges)
{
    // Size of file in bytes
    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);

    //Read block of adjacency matrix
    if (!StringFind(input, "[adjacency list]\n", file_size))
    {
        printf("\nERROR: the read file doesn't have key string: [adjacency list]\n");
        exit(EXIT_FAILURE);
    }

    int vertex = AListCountVertex(input);
    StringFind(input, "[adjacency list]\n", file_size);
    ALIST *inside = (ALIST*)calloc(vertex, sizeof(ALIST*));
    aListg *grap = (aListg*)calloc(vertex,sizeof(aListg));
    grap->g = inside;
    grap->v = vertex;
    int j = 0;
    char *chr = (char*)malloc(sizeof(char*));
    int line = 0;
    char str_new;
    str_new = fgetc(input);

    while (str_new != EOF) {
        if (str_new >= 48 && str_new <= 57){
            chr = realloc(chr, j+1 * sizeof(char*));
            chr[j++] = str_new;
        }

        else if (str_new == '-') {
            line = atoi(chr);
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            j = 0;
        }

        else if (str_new == ' ' && chr != 0) {
            AListAddValue(&grap[line].g, atoi(chr));
            (*count_edges)++;
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            j = 0;
        }

        else if (str_new == '\n') {
            AListAddValue(&grap[line].g, atoi(chr));
            (*count_edges)++;
            line = 0;
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            j = 0;
        }

        str_new = fgetc(input);
    }

   // AListAddValue(&grap[line].g, atoi(chr));
    grap->v = vertex;
    rewind(input);
    return grap;
}
