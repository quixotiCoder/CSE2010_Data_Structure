#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity; // heap 최대 가능 크기
	int Size; // heap의 '현재' 크기
	int *Elements; // 원소들을 담을 배열 (부모 <-> 자식 인덱스 규칙 존재)
}Heap;

Heap* CreateHeap(int heapSize);
void DeleteHeap(Heap* heap);
void Push(Heap *heap, int value);
bool Find(Heap *heap, int value);
int Pop(Heap* heap);
void PrintHeap(Heap* heap);

Heap* CreateHeap(int heapSize){
    Heap* h = (Heap*)malloc(sizeof(Heap)); // heap 동적할당
	
	if(h == NULL) // 할당이 되지 않은 경우
		return NULL;
	
	h->Capacity = heapSize; // heap의 가용 최대 크기
	h->Size = 0; // 현재까지 사용된 size
	h->Elements = (int*)malloc(sizeof(int) * (heapSize + 1)); // 힙의 원소를 담을 배열의 크기
	h->Elements[0] = INF; // heap은 인덱스 1부터 시작, 0번 인덱스 선처리

	return h;
}

void DeleteHeap(Heap* heap){
    free(heap->Elements); // 동적으로 할당되는 heap 의 Elements 해제
    free(heap);  // 동적으로 할당되는 heap 해제
}

void Push(Heap *heap, int value){
    if (heap->Size >= heap->Capacity) { // 가용 범위 초과한 경우
        fprintf(fout, "push error : heap is full\n");
        return;
    }

	else if(Find(heap, value)){ // 이미 값이 존재하는 경우
		fprintf(fout, "push error : %d is already in the heap\n", value);
        return;
	}
	
	heap->Size = heap->Size + 1; // 사용되는 힙 크기 한 칸 증가
	int index = heap->Size; // size의 크기 == 새로운 원소가 들어갈 마지막 칸 인덱스 넘버
	
	while ((index != 0) && (value > heap->Elements[index/2])){ // 부모를 찾아가되 0번 인덱스까지는 X && 부모보다 큰 경우(max heap이기에)
		heap->Elements[index] = heap->Elements[index / 2]; // 부모를 자식 칸으로 내리며 swap
		index = (index / 2); // 부모 칸으로 인덱스 변경(= 부모와 비교 연산으로 위치 변경) 후, while 재귀
	}
	heap->Elements[index] = value; // 부모가 자신보다 큰 경우, 해당 자리에 삽입
	
}

bool Find(Heap *heap, int value){
    for(int i = 1; i <= heap->Size; i++){ // 1번 인덱스(루트) 부터 size 인덱스(= 마지막)까지 탐색
		if(heap->Elements[i] == value) // 값 찾기
			return true;
	}
    return false;
}

int Pop(Heap* heap){
	if(heap->Size == 0){ // heap이 비어 있는 경우
        fprintf(fout, "pop error : heap is empty\n");
        return -INF; // main 문 141번 라인 처리를 위해 return 반환
    }

	int maxElement = heap->Elements[1]; // 최대값 저장
    heap->Elements[1] = heap->Elements[heap->Size--]; // 힙의 마지막 요소를 루트로 옮기고 크기를 줄임

    int parent = 1; // 항상 가장 윗 노드 인덱스 (= 1) 부터 비교 시작이기에 : n
    int child = 2; // 힙에서의 부모-자식 규칙 : 2n [우선은 왼 자식을 기본으로 설정]

    while(child <= heap->Size){ // max size를 넘지 않는 한에서 반복
		if((heap->Elements[child]) < heap->Elements[child + 1]) // 만약 오른 자식 > 왼 자식인 경우
			child++; // [자식++] == [2n -> 2n+1] 오른 자식을 child로 지정

        if(heap->Elements[parent] < heap->Elements[child]){ // 루트로 올린 기존 leaf 노드와 자식을 비교
            int temp = heap->Elements[parent]; // 부모와 자식 swap
            heap->Elements[parent] = heap->Elements[child]; // 부모와 자식 swap
            heap->Elements[child] = temp; // 부모와 자식 swap

            parent = child; // 옮긴 자리를 다시 parent로
            child = parent * 2; // 자식의 자식을 child로
        }
		else
            break; // 부모-자식 대소 조건 미충족시
    }
    return maxElement; // 삭제된 노드 (=최대값) 반환
}



void PrintHeap(Heap* heap){
	if(heap->Size == 0){ // heap이 비어있는 경우 
		fprintf(fout, "print error : heap is empty\n");
		return;
	}

    for(int i = 1; i <= heap->Size; i++)
		fprintf(fout, "%d ", heap->Elements[i]); // heap->Elements 순서대로 print

	fprintf(fout, "\n");
}

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap;
	int heapSize, key, max_element;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'n': 
				fscanf(fin, "%d", &heapSize); // heap의 capacity
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i':
				fscanf(fin, "%d", &key); 
				Push(maxHeap, key); // 말단에 넣고 올라가며 비교
				break;
			case 'd': 
				max_element = Pop(maxHeap); // return 으로 삭제되는 max 반환
				if(max_element != -INF){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
				break;
			case 'p':
				PrintHeap(maxHeap);
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key); // 찾는 값 있는 경우 출력
				else fprintf(fout, "%d is not in the heap\n", key); // 없는 경우 출력
				break;
		}
	}
	DeleteHeap(maxHeap);
	fclose(fin);
	fclose(fout);
	return 0;
}