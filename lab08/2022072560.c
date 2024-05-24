#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h> // for Adjacency Matrix

FILE *fin;
FILE *fout;

typedef struct _QueueNode QueueNode;

struct _QueueNode{ // linked-list 형태 for node
    QueueNode* next; // for linked-list
    int data; // 값 저장
};

typedef struct _Queue { // 위의 node들을 묶은 Queue == FIFO 
	QueueNode* front; // 가장 먼저 dequeue할 node 가리킴
	QueueNode* rear; // 가장 최근 enqueue된 node 가리킴
	int size; // en/dequeue마다 갱신 -> 나중에 graph의 총 개수와 비굣
}Queue;

typedef struct _Graph {
	int size; // 아래 node 배열의 길이 저장
	int* node; // key 값 오름차순으로 저장 -> for 출력 시 별도 row/column
	bool** matrix; // bool 로 이루어진 실질적 Adjacency Matrix <- Two Dimension Array
}Graph;

/*
    -----   구현해야 하는 함수들    -----
*/

// ---  Graph 함수
Graph* CreateGraph(int* nodes, int n);
void DeleteGraph(Graph* G);

void InsertEdge(Graph* G, int a, int b);
void PrintGraph(Graph* G);

void TopologicalSort(Graph* G);

// ---  Queue 함수
Queue* CreateQueue();
void DeleteQueue(Queue* Q);

bool IsEmpty(Queue* Q);

void Enqueue(Queue* Q, int X);
int Dequeue(Queue* Q);


/*
    -----   구현하면 좋은 함수들    -----
*/
// Indegree 배열 설정
void SetIndegree(Graph* G, int* idg);

// Bubble sort도 무방
void SortNode(int* arr, int n);


/*
    -----  기본제공 함수    ------
*/
void countInput(int* n, char* str) { // 문자열 base이기에 ASCII로 값 들어감 => %d 출력하기
	int len = strlen(str), i;
	for (i = 0; i < len; i++)
		if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
}

void parseInput(int* arr, char* str, int n) { // 문자열 base이기에 ASCII로 값 들어감 => %d 출력하기
	int len = strlen(str), i;
	int cnt = 0;
	for (i = 0; i < len; i++)
	if (0 <= str[i] - '0' && str[i] - '0' < 10) arr[cnt++] = str[i] - '0'; // ' '(blank) 제외 1st line의 숫자들만 배열 삽입
}

/*
    -----   main    -----
*/
int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");
    
    // Input 1st line
	char numbers[100], inputs[100];
	fgets(numbers, 100, fin); // fin에서 [99개 or EOF]까지 문자열('s')로 저장
	int n = 0;
	countInput(&n, numbers); // 입력받은 numbers의 총 개수 -> [n에 저장]
	int* nodes = (int*)malloc(sizeof(int)*n); // input받은 수 만큼 노드 수 malloc
	parseInput(nodes, numbers, n); // malloc으로 할당받은 nodes(int형 배열)에 입력받은 numbers index 앞에서부터 저장
	
    SortNode(nodes, n); // 위에서 베열에 저장된 값들 오름차순 정렬 for Adjacency Matrix 표기할 indexing
	Graph* G = CreateGraph(nodes, n); // '[sorted] nodes array'로 creating Adjacency Matrix [n * n] size 

    // Input 2nd line
	fgets(inputs, 100, fin);
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) { // '[a]-[b] '의 (= 'n', '-', ' ')로 구성된 4글자 단위로 input 2nd line 쪼개서 숫자만 뽑아내기
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b); // 위에서 만든 Graph(Adjacency Matrix)에 삽입
	}

	PrintGraph(G);
    fprintf(fout, "\n");
	TopologicalSort(G);
	
	DeleteGraph(G);

	fclose(fin);
	fclose(fout);

	return 0;
}

/*
    -----   구현해야 하는 함수들    -----
*/

// ---  Graph 함수
Graph* CreateGraph(int* nodes, int n){ // node 배열 sorted 된 상태 & 길이 아는 상태
    Graph* graph = (Graph*)malloc(sizeof(Graph)); // malloc으로 graph 형성
    if (graph == NULL) // 메모리 할당 실패 시
        return NULL;
    
    graph->size = n; // index의 수(n)을 size로 할당 

    graph->node = (int*)malloc(n * sizeof(int));
    if (graph->node == NULL) // 메모리 할당 실패 시
    { 
        free(graph); // 이전 할당까지 해제
        return NULL;  
    }
    for(int i =0; i < n; i++)
        graph->node[i] = nodes[i];

    graph->matrix = (bool**)malloc(n * sizeof(bool*)); // matrix의 경우는 bool 2차원 array 
    if (graph->matrix == NULL) // 메모리 할당 실패 시 
    { 
        free(graph->node); // 이전 할당까지 해제
        free(graph);       // ''
        return NULL; 
    }
    for (int i = 0; i < n; i++) { 
        graph->matrix[i] = (bool*)malloc(n * sizeof(bool)); // -> malloc으로 bool* 우선 할당 후 for문으로 내부 bool 크기만큼 할당 -> 한 행씩
        if (graph->matrix[i] == NULL) // 메모리 할당 실패 시 
        { 
            for (int j = 0; j < i; j++) // 이전 할당까지 해제
                free(graph->matrix[j]); // ''

            free(graph->matrix);        // ''
            free(graph->node);          // '' 
            free(graph);                // ''
            return NULL; 
        }
        
        for (int j = 0; j < n; j++)
            graph->matrix[i][j] = false; // matrix 2차원 배열 완성 & 전부 bool false(= 0) 할당 -> [InsertEdge 함수]를 통해 matrix 값 설정
    }
    return graph; // 결과 : '모든 칸 초기화된 n * n 2차원 배열' 생성 후, 출력
}

void InsertEdge(Graph* G, int a, int b){
    int start, end; // {a, b} value의 index 저장용

    for(int i = 0; i < G->size; i++)
    {
        if(G->node[i] == a)
            start = i; // a 값이 저장된 index 저장
        if(G->node[i] == b)
            end = i; // b 값이 저장된 index 저장
    }

    G->matrix[start][end] = true; // directed graph의 특성 상, 이 곳에만 true 표기
}

void PrintGraph(Graph* G){
    fprintf(fout, "0 "); // 1행 1열 출력

    for(int i = 0; i < G->size; i++) // 1행 -> G->node의 '값'들 출력
        fprintf(fout, "%d ", G->node[i]); // 1행 출력
    fprintf(fout, "\n"); // 2행으로 '\n'

    for(int j = 0; j < G->size; j++)
    {
        fprintf(fout, "%d ", G->node[j]); // 모든 행의 1열 == node의 key 값

        for (int k = 0; k < G->size; k++)
        {
            fprintf(fout, "%d ", G->matrix[j][k]); // 각 행마다의 matrix 값 (bool 은 %d로 0 혹은 1로 출력 가능)
        }

        fprintf(fout, "\n"); // 한 행마다 '\n'
    }
}

void DeleteGraph(Graph* G){
    if (G == NULL)
        return; // 빈 graph 상태인 경우
    
    free(G->node); // G 속, node 배열 포인터 free

    for (int i = 0; i < G->size; i++) // matrix의 경우 for문으로 matrix[i]를 free 
        free(G->matrix[i]); // 각 행(matrix[i])
    free(G->matrix); // 후 matrix 구조체 자체를 free

    free(G); // G 구조체 free
}

void TopologicalSort(Graph* G){
    Queue* Q = CreateQueue(); // Q 초기화
    Queue* Printorder = CreateQueue(); // Inorder print할 값들 저장할 Queue
    
    int printorder_size = 0; // printorder_size 초기화 for 오류 발생 시, 오류 메세지 처리
    int* indgree = (int*)malloc(sizeof(int) * G->size); //  G->node 배열과 길이 동일한 배열 하나 생성

    SetIndegree(G, indgree); // indgree 배열 초기화 == 배열 인덱스(오름차순 정렬 기준) 별로 indgree 값 저장됨
    
    int loop_index = 1; // 아래 while문 실행 여부 index

    while(loop_index)
    {
        int num_of_zero_idg = 0; // 만일 모든 노드의 indegree가 '0'인 경우 == 더이상 idg의 수정이 일어나지 않을 경우를 위한 탈출
        loop_index = 0; // cycle 존재 시 탈출 (node들이 남아있지만 idg == 0인 노드가 없는 경우)

        for(int i = 0; i < G->size; i++) 
        {
            if(indgree[i] == 0) // indegree 가 0인 노드 인덱스 <-> 없을 경우 loop_index = 0 으로 다음 loop 실행 X
                {
                    Enqueue(Q, i); // queue에 삽입
                    indgree[i]--; // 다음 loop 시부터 '-1'의 값으로 걸러짐
                    Enqueue(Printorder, G->node[i]); // idg == 0인 index(= i)에 해당하는 [value]를 지닌 G->node의 [value]를 enqueue
                    loop_index = 1; // idg 변환 발생 == 아직은 cycle X
                }
            else if(indgree[i] == -1) // idg == 0안 노드의 수 세기
                num_of_zero_idg++;
        }

        if(num_of_zero_idg == G->size) // 모든 node의 idg == 0인 경우
            break; // 더이상 queue에 대한 입출력 없기에 탈출

        while(!IsEmpty(Q)) // queue가 비어있을 때까지
        {
            int i = Dequeue(Q); // 선 Dequeue
            for(int j = 0; j < G->size; j++)
            {
                if(G->matrix[i][j] == 1) // i를 선행으로 하는 노드를 찾기 (by matrix[i][j] 이용)
                    indgree[j]--; // 그 노드(j)의 indegree를 1개 줄이기
            }
        }
    }
    
    if(Printorder->size != G->size) // 사이클 판단(size, G->size와 비교)
        fprintf(fout, "sorting error : cycle"); // 에러 처리
    
    else // 정상 case
    {
        while(!IsEmpty(Printorder))
            fprintf(fout, "%d ", Dequeue(Printorder)); // 정상값 출력
    }
    
    free(indgree); // 동적할당 해제
    DeleteQueue(Printorder); // 동적할당 해제
    DeleteQueue(Q); // 동적할당 해제
}

// ---  Queue 함수
Queue* CreateQueue(){
    Queue* queue = (Queue*)malloc(sizeof(Queue)); // queue를 malloc으로 할당
    if (queue == NULL) // 메모리 할당 실패 시 
        return NULL;

    queue->front = NULL; // 구조체 변수 초기화
    queue->rear = NULL;  // ''
    queue->size = 0;     // ''
    return queue;        // 생성한 queue 반환
}

bool IsEmpty(Queue* Q){
    if(Q->size == 0) // 빈 상태
        return true;
    else // 아닌 상태
        return false;
}

int Dequeue(Queue* Q){
    QueueNode* temp = Q->front; //queue 맨 앞쪽의 노드를 꺼내기
    Q->size--; //size --
    int value = temp->data;

    if(temp->next == NULL) // 빼면 empty인 경우 front, rear 둘 다 NULL로
    {
        Q->front = NULL;
        Q->rear = NULL;
    }
    else
        Q->front = temp->next; // linked list때와 동일하게 Q->front 포인터를 갱신
    
    free(temp); // temp node 메모리 free
    
    return value; // 가장 앞 노드'였던' data 반환
}

void Enqueue(Queue* Q, int X){
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode)); // 노드 구조체 할당
    if (newNode == NULL) // 메모리 할당 실패 시
        return;

    newNode->data = X; // data 삽입
    newNode->next = NULL; // Queue == FIFO 구조체이기에
    if (IsEmpty(Q)) // 빈 queue ->front, rear 둘 다 새로 만든 node로
    {
        Q->front = newNode; // front와
        Q->rear = newNode;  // rear 모두 생성된 newNode
    }
    else // 뭐라도 든 경우 -> linked list때와 동일하게 Q->rear->next와 Q->rear 포인터를 갱신
    {
        Q->rear->next = newNode; // 기존 노드 중 마지막 노드의 다음 위치로 할당
        Q->rear = newNode; // Q 전체의 rear를 newNode로
    }
    Q->size++;  // size 한 칸 증가
}

void DeleteQueue(Queue* Q){
    while (!IsEmpty(Q)) // 빈 상태될 때까지
        Dequeue(Q); // node 하나씩 제거

    free(Q); // empty 상태가 되면, free
}

/*
    -----   구현하면 좋은 함수들    -----
*/
void SetIndegree(Graph* G, int* idg){ // TopologicalSort 함수 내에서 호출하여 배열 할당해줌
    for(int i = 0; i < G->size; i++)
        idg[i] = 0; // idg 배열 초기화

    for(int j = 0; j < G->size; j++) // 2중 for문
    {
        for (int k = 0; k < G->size; k++)
        {
            if(G->matrix[j][k]) //G->matrix[j][k]가 true면 idg[k]++
                idg[k]++;
        }
    }
}

void SortNode(int* arr, int n){ // bubble sort 구현
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n - i -1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

