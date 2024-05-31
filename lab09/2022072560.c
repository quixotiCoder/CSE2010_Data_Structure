#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INF 1e7 // 모든 노드의 첫 weight == infinity

FILE *fin;
FILE *fout;

typedef struct Node {
    int index;  // index of this node
    int dist;   // distance for [heap]
    int prev;   // heap에서의 순서
} Node;

typedef struct Graph { // 직접적 user input 받고 그 값들 저장
    int size;
    int **distMatrix;
    Node **nodes;
} Graph;

typedef struct Heap {
    int Capacity;
    int Size;
    Node **element; // Node* 자료형 element 저장 배열
} Heap;

/*
---     함수 선언
*/
//---     구현해야 하는 함수
Graph *CreateGraph(int size);
void DeleteGraph(Graph *g);
void PrintShortestPath(Graph *g);

Heap *CreateHeap(int heapSize);
void DeleteHeap(Heap *heap);
void Push(Heap *heap, Node *node);
Node *Pop(Heap *heap);

//---    구현하면 좋은 함수
bool isEmpty(Heap *heap);
void swap(Node **node1, Node **node2);

/*
---     main 함수
*/
int main(int argc, char *argv[]) {
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    Graph *g;
    int size;
    fscanf(fin, "%d\n", &size); // Graph의 node(vertex) 수
    g = CreateGraph(size);      // input받은 node수 만큼 graph 할당

    char tmp = 0;
    while (tmp != '\n' && tmp != EOF) {
        int node1, node2, weight;
        fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
        g->distMatrix[node1 - 1][node2 - 1] = weight; // [n1 - 1][n2 - 1] 2차원 배열 index에 weight 할당
        tmp = fgetc(fin);
    }

    PrintShortestPath(g);

    DeleteGraph(g);
    fclose(fin);
    fclose(fout);
    return 0;
}

/*
---     함수 구현부
*/
//---    graph 관련
Graph *CreateGraph(int size) {
    Graph *graph = (Graph *)malloc(sizeof(Graph)); // malloc으로 graph 할당
    if (graph == NULL)                             // 메모리 할당 실패 시
        return NULL;

    graph->size = size; // index의 수(n)을 size로 할당

    graph->distMatrix = (int **)malloc(size * sizeof(int *)); // distMatrix의 경우는 int 2차원 array
    if (graph->distMatrix == NULL) { // 메모리 할당 실패 시
        free(graph);                 // 이전 할당까지 해제
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        graph->distMatrix[i] = (int *)malloc(size * sizeof(int)); // -> malloc으로 int* 우선 할당 후 for문으로 내부 int 크기만큼 할당 -> 한 행씩
        if (graph->distMatrix[i] == NULL)   // 메모리 할당 실패 시
        {
            for (int j = 0; j < i; j++) 	// 이전 할당까지 해제
                free(graph->distMatrix[j]); // ''

            free(graph->distMatrix); 		// ''
            free(graph); 					// ''
            return NULL;
        }

        for (int j = 0; j < size; j++)
            graph->distMatrix[i][j] = INF; // distMatrix 2차원 배열 완성 & 전부 INF 할당
    }

    graph->nodes = (Node **)malloc(size * sizeof(Node *));
    if (graph->nodes == NULL) 	 // 메모리 할당 실패 시
    {
        for (int i = 0; i < size; i++) 	// 이전 할당까지 해제
            free(graph->distMatrix[i]); // ''

        free(graph->distMatrix); // ''
        free(graph); 			 // ''
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        graph->nodes[i] = (Node *)malloc(sizeof(Node)); // 각 노드 메모리 할당
        if (graph->nodes[i] == NULL) 	// 메모리 할당 실패 시
        {
            for (int j = 0; j < i; j++) // 이전 할당까지 해제
                free(graph->nodes[j]); 	// ''

            free(graph->nodes); 		// ''
            for (int j = 0; j < size; j++) // 이전 할당까지 해제
                free(graph->distMatrix[j]); // ''
            free(graph->distMatrix); // ''
            free(graph); 			 // ''
            return NULL;
        }

        graph->nodes[i]->index = i;		// index 할당 (value보다 1 작음)
        graph->nodes[i]->dist = INF;	// dist와
        graph->nodes[i]->prev = INF;	// prev은 모두 'INF'로 초기화
    }

    return graph; // distMatrix == INF 초기화 상태 && nodes 배열 인덱스 번호마다 인덱스 번호 값 할당
}

void DeleteGraph(Graph *graph) {
    for (int i = 0; i < graph->size; i++) {
        free(graph->distMatrix[i]);
        free(graph->nodes[i]);
    }
    free(graph->distMatrix);
    free(graph->nodes);
    free(graph);
}

void PrintShortestPath(Graph *graph) { // graph node 1번 인덱스부터 시작해서 돌기 -> heap에 dist 작은 순서대로 push -> empty될 때까지 반복
    graph->nodes[0]->dist = 0; // 0번 노드->dist 0으로 설정(시작점이므로)

    Heap *heap = CreateHeap(graph->size); // 힙 생성
    Push(heap, graph->nodes[0]); // 0번 노드 heap에 push

    while (!isEmpty(heap)) // 반복, !isEmpty:
    {
        Node *a = Pop(heap);    // a = pop(heap) == 연결된 노드들 dist 계산할 main node
        int distOA = a->dist;    // distOA = a->dist

        for (int i = 0; i < graph->size; i++) // 반복 b, 0 ~ graph->size:
        {
            if (a->index == i) // a == b -> continue
                continue;

            if (graph->distMatrix[a->index][i] != INF) // disMatrix 상에서 a 와 연결이 된 경우 == ( [!= INF] 인 경우)
            {
                Node *b = graph->nodes[i]; // b = a와 연결된 i 번째 인덱스의 노드
                
                if (distOA + graph->distMatrix[a->index][i] < b->dist) // if distOA + AB거리 < distOB => dist 새로운 값 할당
                {
                    b->dist = distOA + graph->distMatrix[a->index][i]; // 더 짧은 길이로 재할당 (b->dist = distOA + AB거리)
                    b->prev = a->index; // 이전 노드에 대한 정보 기록 (b->prev = a)
                    Push(heap, b); // <Fixed>된 dist 지닌 채로 queue에 삽입 
                }
            }
        }
    }

    for (int i = 1; i < graph->size; i++) // 반복 1 ~ graph->size: [1에서 1까지 에 대한 정보는 출력 X. 노드 2부터 시작]
    {
        if (graph->nodes[i]->dist == INF) // dist가 INF에서 다른 값으로 재할당 안된 노드 == 아무랑도 연결 X인 노드
            fprintf(fout, "can not reach to node %d\n", i + 1);

        else // b->prev를 따라가면서 이전 노드 찾아서 순서 만들기
        {
            Node *b = graph->nodes[i]; // dist가 존재하는 b 노드 찾고
            int previous = b->prev; // prev 따라가기 용

            int *prev_arr = (int *)malloc(sizeof(int) * graph->size); // prev 정보 저장
            for (int j = 0; j < graph->size; j++) // prev_arr 배열 초기화
                prev_arr[j] = -1; // prev 아니면 -1 유지되도록

            while (previous != INF) {
                prev_arr[previous] = previous + 1; // prev index보다 한 칸 더 증가한 real 값을 넣기
                previous = graph->nodes[previous]->prev; // prev의 prev으로 index 변경
            }

            for (int j = 0; j < graph->size; j++) {
                if (prev_arr[j] != -1)  // prev인 경우 <-> prev 아니면 '-1' 갖고 있음
                    fprintf(fout, "%d->", prev_arr[j]); // prev 출력
            }

            fprintf(fout, "%d", b->index + 1); // 마지막 자신의 index 출력
            fprintf(fout, " (cost : %d)\n", b->dist);

            free(prev_arr); // 매 반복마다 해제 후, 다음 인덱스용 배열 할당
        }
    }
    
    DeleteHeap(heap); // heap 삭제
}


//---    heap 관련
Heap *CreateHeap(int heapSize) {
    Heap *h = (Heap *)malloc(sizeof(Heap)); // heap 동적할당
    if (h == NULL) // 할당이 되지 않은 경우
        return NULL;

    h->Capacity = heapSize; // heap의 가용 최대 크기
    h->Size = 0;            // 현재까지 사용된 size

    h->element = (Node **)malloc((heapSize + 2) * sizeof(Node *)); // heap은 1번 인덱스부터 사용하기에 + Node* 형식 저장
    if (h->element == NULL)                                        // 할당이 되지 않은 경우
    {
        free(h);
        return NULL;
    }

    return h;
}

void DeleteHeap(Heap *heap) {
    free(heap->element); // 동적으로 할당되는 heap 의 Elements 해제
    free(heap);          // 동적으로 할당되는 heap 해제
}

void Push(Heap *heap, Node *node) {
    int idx = ++heap->Size;           // size의 크기 == 새로운 원소가 들어갈 마지막 칸 인덱스 넘버

    while (idx > 1 && node->dist < heap->element[idx / 2]->dist) { // 부모를 찾아가되 1번 인덱스까지는 && 부모보다 작은 경우(min heap이기에)
        heap->element[idx] = heap->element[idx / 2];        // 부모를 자식 칸으로 내리며 swap
        idx = idx / 2;                                    // 부모 칸으로 인덱스 변경(= 부모와 비교 연산으로 위치 변경) 후, while 재귀
    }
    heap->element[idx] = node; // 부모가 자신보다 작은 경우, 해당 자리에 삽입
}

Node *Pop(Heap *heap) {
    Node *min_element = heap->element[1]; // 최솟값 저장 heap은 1번 인덱스부터 사용
    Node *last_element = heap->element[heap->Size--]; // 힙의 마지막 요소을 루트로 옮기고 for percolate down 크기를 1만큼 줄임

    int parent = 1;     // 항상 가장 윗 노드 인덱스 (= 1) 부터 비교 시작이기에 : n
    int child = 2;      // 힙에서의 부모-자식 규칙 : 2n [우선은 왼 자식을 기본으로 설정]

    while (child <= heap->Size) { // size보다 크지 않은 선에서 반복
        if (heap->element[child]->dist > heap->element[child + 1]->dist) // 만약 왼 자식 > 오른 자식인 경우
            child++; // [자식++] == [2n -> 2n+1] 오른 자식을 child로 지정

        if (last_element->dist > heap->element[child]->dist) { // 루트로 올린 기존 leaf 노드와 자식을 비교
            swap(&heap->element[parent], &heap->element[child]); // 부모 - 자식 swap
            parent = child; // 옮긴 자리를 다시 parent로
            child = parent * 2; // 자식의 자식을 child로
        } 
        else
            break; // 부모-자식 대소 조건 미충족시
    }

    heap->element[parent] = last_element; // 마지막 요소를 최종적으로 위치시킴
    return min_element; // 삭제된 노드 (= 최솟값) 반환
}

//---    구현하면 좋은 함수
bool isEmpty(Heap *heap) {
    return (heap->Size == 0); // 사용된 heap의 크기 == 0인지 확인
}

void swap(Node **node1, Node **node2) { // 전형적인 swap 함수 (by temp 사용)
    Node *temp = *node1;
    *node1 = *node2;
    *node2 = temp;
}
