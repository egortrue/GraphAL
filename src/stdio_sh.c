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
	fopen_s(&file, name, mode);

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

// Парсинг строки на отдельные строки, разделенные пробелом
char** ObjectRead(FILE* fr, long file_size, char* string, int flag)
{
	// Парсим строку, запсывая данные в obj
	char** obj = (char**)calloc(3, sizeof(char*));

	obj[0] = (char*)calloc(STR_LEN_MAX+1, sizeof(char));
	obj[1] = (char*)calloc(STR_LEN_MAX+1, sizeof(char));
	obj[2] = (char*)calloc(STR_LEN_MAX+1, sizeof(char));

	if (!obj || !obj[0] || !obj[1] || !obj[2]) exit(EXIT_FAILURE);


	// До 1 пробела
	char* begin = string;
	
	while (*string != 32 && *string != 10) string++;
	memmove(obj[0], begin, string-begin);
	string++;

	// С 1 до 2 пробела
	char* first_space = string;

	while (*string != 32 && *string != 10) string++;
	memmove(obj[1], first_space, string - first_space);
	string++;

	// Cо 2 до 3 пробела (конца)
	if (flag)
	{
		char* second_space = string;
		while (*string != 32 && *string != 10) string++;
		memmove(obj[2], second_space, string - second_space);
	}

	return obj;
}

// Кол-во элементов в блоке
int count = 0;

// Чтение блока, обозначенного ключевой строкой ( [nodes] или [edges] )
char*** BlockRead(FILE* fr, char* str, long file_size)
{
	fseek(fr, 0, SEEK_SET);
	char* string = (char*)calloc(STR_LEN_MAX, sizeof(char));
	if (!string) exit(EXIT_FAILURE);

	// Читаем все строки пока не найдем ключевую строку
	while (strcmp(string, str))
	{
		fgets(string, STR_LEN_MAX, fr);

		// Если дошли до конца файла, то ключевой строчки нет
		if (ftell(fr) == file_size)
		{
			printf("\nERROR: the read file doesn't have key string: %s", str);
			exit(EXIT_FAILURE);
		}
	}

	// Если нашли ключевую строку, парсим строки после неё
	// Парсинг зависит от ключевой строки
	int flag = 0;
	if (*(++str) == 'e')
		flag = 1;

	// Массив с массивами строк / Кол-во объектов в блоке
	char*** data = (char***)calloc(OBJ_COUNT_MAX, sizeof(char**));
	if (!data) exit(EXIT_FAILURE);
	count = 0;

	// Читаем первую строку после ключевой строки
	fgets(string, STR_LEN_MAX, fr);

	while (*string == 0 || *string != 10) // Если строка содержит толко \n это конец блока
	{
		// Парсинг конкретной строки
		char** obj = ObjectRead(fr, file_size, string, flag);
		data[count++] = obj;

		// Если не последняя строка в файле, читаем дальше
		if (ftell(fr) == file_size)
			break;
		fgets(string, STR_LEN_MAX, fr);
	}

	free(string);
	return data;
}

GRAPH* FileRead(FILE* fr)
{
	// Узнаeм размер файла в байтах
	fseek(fr, 0, SEEK_END);
	long file_size = ftell(fr);

	// Здесь временно храниться имя блока
	char* block = (char*)calloc(9, sizeof(char));
	if (!block) exit(EXIT_FAILURE);

	// Чтение блока [nodes]
	char str1[] = "[nodes]\n";	 
	char*** nodes_data = BlockRead(fr, memmove(block, &str1, 9), file_size);
	int nodes_num = count;

	// Чтение блока [edges]
	char str2[] = "[edges]\n";
	char*** edges_data = BlockRead(fr, memmove(block, &str2, 9), file_size);
	int edges_num = count;

	free(block);
	 
	// Создание графа
	GRAPH* G = (GRAPH*)malloc(sizeof(GRAPH));
	if (!G) exit(EXIT_FAILURE);

	if (ORIENTED_GRAPH)
		*G = GraphSet(nodes_num, edges_num);
	else
		*G = GraphSet(nodes_num, (edges_num << 1));


	// Инициализация узлов
	for (int i = 0; i < nodes_num; i++)
	{
		G->nodes[i] = (NODE*)malloc(sizeof(NODE));
		if (!(G->nodes[i])) exit(EXIT_FAILURE);

		*G->nodes[i] = NodeSet(ConvertStrToInt(nodes_data[i][0]),
							   ConvertStrToInt(nodes_data[i][1]),
							   nodes_num);
	}


	// Инициализация рёбер
	NODE* source = NULL;
	NODE* target = NULL;

	for (int i = 0; i < edges_num; i++)
	{
		G->edges[i] = (EDGE*)malloc(sizeof(EDGE));
		if (!(G->edges[i])) exit(EXIT_FAILURE);

		source = NodeAddress(G->nodes, ConvertStrToInt(edges_data[i][0]), nodes_num);
		target = NodeAddress(G->nodes, ConvertStrToInt(edges_data[i][1]), nodes_num);

		*G->edges[i] = EdgeSet(source, target, ConvertStrToInt(edges_data[i][2]));
		NodeLink(source, target);
	}

	if (!ORIENTED_GRAPH)
		for (int i = edges_num; i < (edges_num << 1); i++)
		{
			G->edges[i] = (EDGE*)malloc(sizeof(EDGE));
			if (!(G->edges[i])) exit(EXIT_FAILURE);

			source = G->edges[i - edges_num]->target;
			target = G->edges[i - edges_num]->source;

			*G->edges[i] = EdgeSet(source, target, G->edges[i - edges_num]->weight);
			NodeLink(source, target);
		}

	// Освобождение памяти из под большого блока char'ов
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
