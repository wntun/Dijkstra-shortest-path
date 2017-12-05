/*
 ============================================================================
 Name        : SubwayPathFinder.c
 Author      : wntun
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TRAINSITION_TIME 8

typedef struct station_struct{
	char name[20];
	int code[3];
	int weight;
	int countCode;
	int lineNo;
}station;

typedef struct state_struct{
	int isKnown;
	int cost;
	int previousIndex;
} state;

typedef struct path_struch{
	int *stations;
	int count;
} path;

station *stations;
int countStations = 0;
int **adjacencyMatrix;
state *states;
path *paths;

int searchName(char *sname, int count){
	printf("Searching name for %s from %d stations \n", sname, count);
	for(int i=0; i<count; i++){
		//printf("%s,",stations[i].name);
		if(strcmp(stations[i].name, sname)==0){
			return i;
		}
	}
	return -1;
}

void init(){
	//printf("initialization: ");
	stations =(station*)malloc(sizeof(station)*242);
	for(int i=0; i<242; i++){
		station s = {.name="", .weight=0, .countCode=0, .lineNo=0};
		stations[i] = s;
		//printf("%d\n",stations[i].weight);
	}
}


void string_copy(char *from, char *to) {
    while ((*to++ = *from++) != '\0')
        ;
}

void readCSVFile(char *fileName, int lineIndex){
	printf("reading file %s \n", fileName);
	FILE *fptr;
	fptr = fopen(fileName, "r");
	if(fptr==NULL){
		printf("%s couldn't be opened!", fileName);
		exit;
	}
	char line[10000];
	int count = 0;
	while(!feof(fptr)){
		fscanf(fptr, "%s,", line);
		if(strlen(line)>5){
			printf("%d: %s, \n", count,line);
			if(count >=1){
				printf("Saving to stations structure:\n");
				char *token = strtok(line, ","); // 133,서울역,,426,,
				char **tokenValue = (char**)malloc(sizeof(char*)*3);
				for(int i=0; i<3; i++){
					tokenValue[i] = (char*)malloc(sizeof(token));
					string_copy(token,tokenValue[i]);
					token = strtok(NULL, ",");
					//printf("%s \n", tokenValue[i]);
				}
				int sameNameStation = -1;
				if(count>=2){
					sameNameStation = searchName(tokenValue[1], countStations + count -1);
				}

				if(sameNameStation>=0){
					printf("%s has the same name for those codes :", stations[sameNameStation].name);
					for(int i=0; i<stations[sameNameStation].countCode; i++){
						printf("%d\t", stations[sameNameStation].code[i]);
					}
					printf("\n");
					stations[sameNameStation].code[stations[sameNameStation].countCode] = atoi(tokenValue[0]);
					stations[sameNameStation].countCode++;
					for(int i=0; i<stations[sameNameStation].countCode; i++){
						printf("%d\t", stations[sameNameStation].code[i]);
					}
					printf("\n");
					//printf("%s has added one more code number %d \n", stations[sameNameStation].name, stations[sameNameStation].code[stations[sameNameStation].countCode]);
				}
				else{
					stations[countStations+count-1].code[0] = atoi(tokenValue[0]);
					string_copy(tokenValue[1], stations[countStations+count-1].name);
					stations[countStations+count-1].weight = atoi(tokenValue[2]);
					stations[countStations+count-1].countCode=1;
					stations[countStations+count-1].lineNo = lineIndex;
					printf(">>>> %d: %s\n", countStations+count-1, stations[countStations+count-1].name);
					count++;
					//printf("%d: %s\n", countStations+count-1, tokenValue[1]);
					}
				}
				else{
					count++;
				}
		}
	}
	countStations = countStations + count-1;
	//stationsOverLine[lineIndex] = count;
	//lineIndex ++;
	printf("Stations count: %d\n", countStations);
	fclose(fptr);
}


void initAdjacencyMatrix(){
	// memory allocation
	adjacencyMatrix = (int **)malloc(sizeof(int*) * countStations);
	printf("Initiating Adjacency Matrix \n");
	for(int i=0; i<countStations; i++){
		*(adjacencyMatrix+i) = (int *)malloc(sizeof(int)*countStations);
		char *sname = stations[i].name;
		printf("%d: %s\n", i, sname);
		for(int j=0; j<countStations; j++){
			*(*(adjacencyMatrix+i)+j) = 0;
		}
	}
}

char* getStationNameFromLine(char *line){
	printf("get station name from line\n");
	char *token = strtok(line, ","); // 133,서울역,,426,,
	char **tokenValue = (char**)malloc(sizeof(char*)*3);
	for(int i=0; i<3; i++){
		tokenValue[i] = (char*)malloc(sizeof(token));
		string_copy(token,tokenValue[i]);
		token = strtok(NULL, ",");
							//printf("%s \n", tokenValue[i]);
	}
	return tokenValue[1];
}

void constructAdjacencyMatrix(char *fileName){
		printf("Constructing Adjacency Matrix \n");
		FILE *fptr;
		fptr = fopen(fileName, "r");
		if(fptr==NULL){
			printf("%s couldn't be opened!", fileName);
			exit;
		}
		char line[10000];
		int count = 0;

		while(!feof(fptr)){
			fscanf(fptr, "%s,", line);
			char currentName[20], nextName[20];
			if(strlen(line)>5){
				if(count >=1){
					printf("%d: %s, \n", count,line);
					//printf("Saving to stations structure:\n");
					if(count == 1){
						char *token = strtok(line, ","); // 133,서울역,,426,,
						char **tokenValue = (char**)malloc(sizeof(char*)*3);
						for(int i=0; i<3; i++){
							tokenValue[i] = (char*)malloc(sizeof(token));
							string_copy(token,tokenValue[i]);
							token = strtok(NULL, ",");
												//printf("%s \n", tokenValue[i]);
							}
						string_copy(tokenValue[1], currentName);
						//printf(">>> %s", currentName);
						fscanf(fptr, "%s,", line);
					}

					char *token = strtok(line, ","); // 133,서울역,,426,,
					char **tokenValue = (char**)malloc(sizeof(char*)*3);
					for(int i=0; i<3; i++){
						tokenValue[i] = (char*)malloc(sizeof(token));
						string_copy(token,tokenValue[i]);
						token = strtok(NULL, ",");
																	//printf("%s \n", tokenValue[i]);
					}
					string_copy(tokenValue[1], nextName);

					int currentIndex = searchName(currentName, countStations);
					int nextIndex = searchName(nextName, countStations);
					printf("%s and %s \n", currentName, nextName);
					printf("%d and %d \n", currentIndex, nextIndex);

					int transit = 0;
					if(stations[currentIndex].lineNo != stations[nextIndex].lineNo){
						transit = TRAINSITION_TIME;
					}

					*(*(adjacencyMatrix+currentIndex)+nextIndex) = stations[nextIndex].weight + transit;
					*(*(adjacencyMatrix+nextIndex)+currentIndex) = stations[nextIndex].weight + transit;
					//free(currentName);
					string_copy(nextName, currentName);
					count++;
				}
				else{
					count++;
				}
			}
		}
}

void initState(){
	states = (state *)malloc(sizeof(state)*countStations);
	for(int i=0; i<countStations; i++){
		states[i].isKnown = 0;
		states[i].cost = INT32_MAX-1;
		states[i].previousIndex = 0;
	}
}

void saveMatrix(){
	FILE *fptr;
	fptr = fopen("adjacencyMatrix.txt",  "w");
	for(int i=0; i<countStations; i++){
		//fprintf(fptr, "\t %s", stations[i].name);
	}
	//fprintf(fptr, "\n");
	for(int i=0; i<countStations; i++){
		//fprintf(fptr, " %s \t", stations[i].name);
		for(int j=0; j<countStations; j++){
			fprintf(fptr,"%d \t", *(*(adjacencyMatrix+i)+j));
		}
		fprintf(fptr,"\n");
	}
	fclose(fptr);
	printf("writing file finished\n");
}

void prepareSource(int source){
	states[source].isKnown = 1;
	states[source].cost = 0;
}


int isAllVisited(){
//	printf("is all visited \n");
	int allVisited = 1;
	for(int i=0; i<countStations; i++){
		if(states[i].isKnown==0){
			allVisited = 0;
			//break;
		}
	}
	return allVisited;
}

int minCostFromStates(){
//	printf("min cost from states \n");
	int minCost = INT32_MAX;
	int minIndex = -1;
	for(int i=0; i<countStations; i++){
		//printf("isKnown = %d, cost = %d for %d \n", states[i].isKnown, states[i].cost, i);
		if(states[i].isKnown==0 && minCost>states[i].cost){
			minCost = states[i].cost;
			minIndex = i;
		}
	}
	return minIndex;
}

void doDijkstra(int source){
	printf("Dijkstra Algorithm for source %s and %d \n", stations[source].name, source);
	prepareSource(source);
	int flag = isAllVisited();
//	printf(" isAllVisited() = %d \n", flag);
	while(flag!=1){
		for(int i=0; i<countStations; i++){
			int tempCost = adjacencyMatrix[source][i];
			if(tempCost!=0){
				tempCost = tempCost + states[source].cost;
				//printf("tempCost = %d  of station from %d to %d \n", tempCost, source, i);
				if(tempCost<states[i].cost){
					states[i].previousIndex = source;
					states[i].cost = tempCost;

				}
			}
		}

		states[source].isKnown = 1;
		source = minCostFromStates();
		printf("%d (%s) is known now! \n", source, stations[source].name);
		flag = isAllVisited();
	}
}

void displayPath(int source){
	printf("Displaying the shortest paths \n");
	for(int i=0; i<countStations; i++){
		if(i!=source){
			int dest = i;
			int previousStation = states[dest].previousIndex;
			while(previousStation!=0){
				paths[dest].stations[paths[dest].count++]=previousStation;
				previousStation = states[previousStation].previousIndex;
			}
		}
	}

	FILE *fptr;
	fptr = fopen("ShortestPathsToKonkukStation.txt", "w");

	for(int i=0; i<countStations; i++){
		if(i!=source){
			int dest = i;
			printf("Shortest path from %s to %s : %s, ", stations[source].name, stations[i].name, stations[i].name);
			fprintf(fptr, "Shortest path from %s to %s : %s, ", stations[source].name, stations[i].name, stations[i].name);
			for(int j= 0; j<paths[dest].count; j++){
				printf("%s, ", stations[paths[dest].stations[j]].name);
				fprintf(fptr, "%s, ", stations[paths[dest].stations[j]].name);
			}
			printf(" with total time taken %d mins. \n", states[i].cost);
			fprintf(fptr, " with total time taken %d mins. \n", states[i].cost);
		}
	}

	fclose(fptr);
}

void initShortestPath(){
	paths = (path *)malloc(sizeof(path)*countStations);
	for(int i=0; i<countStations; i++){
		paths[i].stations = (int *)malloc(sizeof(int)*countStations);
		paths[i].count = 0;
	}
}

void freeMemory(){
	for(int i=0; i<countStations; i++){
		free(adjacencyMatrix[i]);
	}
	free(adjacencyMatrix);
	free(stations);
	free(states);
	free(paths);
}

int main(void) {

	printf("Subway path finder");

	init();
	readCSVFile("subway/1호선-Table 1.csv",0);
	readCSVFile("subway/2호선 성수지선-Table 1.csv",1);
	readCSVFile("subway/2호선 신정지선-Table 1.csv",2);
	readCSVFile("subway/2호선-Table 1.csv",3);
	readCSVFile("subway/3호선-Table 1.csv",4);
	readCSVFile("subway/4호선-Table 1.csv",5);
	readCSVFile("subway/5호선 마천지선-Table 1.csv",6);
	readCSVFile("subway/5호선-Table 1.csv",7);
	readCSVFile("subway/6호선-Table 1.csv",8);
	readCSVFile("subway/7호선-Table 1.csv",9);
	readCSVFile("subway/8호선-Table 1.csv",10);

	// construct adjacency matrix
	initAdjacencyMatrix();
	constructAdjacencyMatrix("subway/1호선-Table 1.csv");
	constructAdjacencyMatrix("subway/2호선 성수지선-Table 1.csv");
	constructAdjacencyMatrix("subway/2호선 신정지선-Table 1.csv");
	constructAdjacencyMatrix("subway/2호선-Table 1.csv");
	constructAdjacencyMatrix("subway/3호선-Table 1.csv");
	constructAdjacencyMatrix("subway/4호선-Table 1.csv");
	constructAdjacencyMatrix("subway/5호선 마천지선-Table 1.csv");
	constructAdjacencyMatrix("subway/5호선-Table 1.csv");
	constructAdjacencyMatrix("subway/6호선-Table 1.csv");
	constructAdjacencyMatrix("subway/7호선-Table 1.csv");
	constructAdjacencyMatrix("subway/8호선-Table 1.csv");

	saveMatrix();

	int src = searchName("건대입구", countStations);
//	src = searchName("제기동", countStations);

	initState();
	doDijkstra(src);
	initShortestPath();
	displayPath(src);

	freeMemory();
	return EXIT_SUCCESS;
}
