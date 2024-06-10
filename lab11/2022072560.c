#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
	int size_cell;	// 셀의 개수
	int* ptr_cell;	// 각 셀의 parent 저장 배열
	int size_wall;	// Maze 벽의 개수
	bool* ptr_wall;	// 각 벽의 존재 여부 저장 배열
}DisjointSets;

DisjointSets* init(int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets);

// 추가 함수
void swap(int *x, int *y);
void shuffle(int* array, int num);

int main(int argc, char* agrv[]) {
	srand((unsigned int)time(NULL));	// random 값 seed -> shuffle의 randomness

	int num, i;
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	fscanf(fin, "%d", &num);			// (num * num) Maze size (num = 3 ~ 100)

	DisjointSets* sets = init(num);		
	createMaze(sets, num);				
	printMaze(sets, num);

	freeMaze(sets);

	fclose(fin);
	fclose(fout);

	return 0;
}

DisjointSets* init(int num) {
	DisjointSets* sets = (DisjointSets*)malloc(sizeof(DisjointSets));	// 마지막에 return 할 DisjointSets 구조체
	if(sets == NULL)	// 할당 실패 시 
		return NULL;	// NULL 반환
	
	sets->size_cell = num * num;	 							// num * num 으로 size_cell 초기화
	sets->ptr_cell = (int*)malloc(sizeof(int) * (num * num));	// num * num 으로 ptr_cell 초기화
	if(sets->ptr_cell == NULL)	// 할당 실패 시
	{
		free(sets);		// 위의 할당 해제
		return NULL;	// NULL 반환
	}

	for(int i = 0; i < sets->size_cell; i++)
		sets->ptr_cell[i] = i;		// 모든 셀의 parent를 자신으로 초기화
	
	sets->size_wall = num * num * 2;								// num * num * 2 로 size_wall 초기화 [좌측, 상단 벽 포함 X -> 벽의 수 = num * num * 2]
    sets->ptr_wall = (bool*)malloc(sizeof(bool) * (num * num * 2));	// num * num * 2 로 ptr_wall 초기화
	if(sets->ptr_wall == NULL)	// 할당 실패 시
	{
		free(sets->ptr_cell);	// 위의 할당 해제
		free(sets);				// ''
		return NULL;			// NULL 반환
	}

	for(int i = 0; i < sets->size_wall; i++)
		sets->ptr_wall[i] = true;	// 모든 벽 존재하는 상태로 초기화

	// (i 번째 셀의 우측은 i*2, 하단은 i*2+1로 정의) == {ex. num = 4인 경우, 0~15번 셀 -> 0~31번 벽 존재}	
	sets->ptr_wall[sets->size_wall - 2] = false;	// (최우측 하단은 출구) == {ex. num = 4인 경우, 15번 셀의 오른벽 (30번 벽 존재 X)}
	
	return sets;
}

void Union(DisjointSets *sets, int i, int j) {
    int pi = find(sets, i);	// i의 parent
    int pj = find(sets, j);	// j의 parent 

	if(pi == pj)	// 같은 group set에 속하면 
		return;		// return

    sets->ptr_cell[pi] = pj;	// i의 set을 j의 set 밑으로 편입
}

int find(DisjointSets *sets, int i) {	
	if(sets->ptr_cell[i] == i)	// 자기 자신이 parent인 경우 == root인 경우
		return i;				// 자기 자신 반환

	if(sets->ptr_cell[i] != i)								// 자기 자신이 parent가 아닌 경우
		sets->ptr_cell[i] = find(sets, sets->ptr_cell[i]);	// 자신의 parent를 찾아서 갱신
	
	return sets->ptr_cell[i];								// 갱신으로 찾은 parent 반환	
}

void createMaze(DisjointSets *sets, int num) {
	int* shuffle_arr = (int*)malloc(sizeof(int) * (num * num));	// num*num 크기 배열 선언 -> 지금 만드는 얘 == [sfuffle -> union 진행용]
	if(shuffle_arr == NULL)	// 할당 실패 시
	{
		free(sets->ptr_cell);	// 위의 할당 해제
		free(sets->ptr_wall);	// ''
		free(sets);				// ''
		return;					// return
	}

	for (int i = 0; i < num * num; i++)							// i번째 배열이 i로 차있는 배열 생성
		shuffle_arr[i] = i;

	shuffle(shuffle_arr, num * num);	// 위에서 만든 배열 shuffle -> 이 랜덤하게 배정된 순서로 union 진행

	for(int j = 0; j < num * num; j++) 
	{
		int main_cell = shuffle_arr[j];		// shuffle된 배열에서 처음부터 끝까지 하나씩 뽑기
		int x = main_cell % num;			// x좌표 -> 열 좌표
		int y = main_cell / num;			// y좌표 -> 행 좌표

		if(x + 1 < num && find(sets, main_cell) != find(sets, main_cell + 1))	// 우측 벽 없앨 수 있을 때 (maze 밖이 아니고, x + 1이 num을 넘지 않고, 같은 group이 아닐 때)
		{
			Union(sets, main_cell, main_cell + 1);	// union
			sets->ptr_wall[main_cell * 2] = false;	// 벽 제거
		}
		if(y + 1 < num && find(sets, main_cell) != find(sets, main_cell + num))	// 하단 벽 없앨 수 있을 때(maze 밖이 아니고, y + 1이이 num을 넘지 않고, 같은 group이 아닐 때)
		{
			Union(sets, main_cell, main_cell + num);	// union
			sets->ptr_wall[main_cell * 2 + 1] = false;	// 벽 제거
		}
	}
	free(shuffle_arr);	// 할당 받은 shuffle 배열 free -> 이미 Union해서 다 씀
}

void printMaze(DisjointSets *sets, int num) {
    // 맨 윗줄 출력
	for (int i = 0; i < num; i++) fprintf(fout, "+---");	// 맨 윗줄 출력 -> 알고리즘과 연관 X
    fprintf(fout, "+\n");									// 윗줄 마무리 출력 -> 알고리즘과 연관 X

	int is_start = 1;	// starting point ' '(blank) 출력용 flag
	
	// 내부 행, 열 출력
	for (int i = 0; i < num; i++)
	{
		if(is_start)			// starting point ' '(blank) 출력용
		{
			fprintf(fout, " ");	// ' '(blank) 출력
			is_start = 0;		// 이후부턴 항상 '|' 출력
		}
		else
			fprintf(fout, "|");	// 둘째 행부턴 '|' 출력
		
		// 행 출력
		for (int j = 0; j < num; j++)
		{
			if (sets->ptr_wall[i * num * 2 + j * 2]) // 오른쪽 벽 존재 여부
				fprintf(fout, "   |");		// 벽 'O'
			else
				fprintf(fout, "    ");		// 벽 'X'
		}
		fprintf(fout, "\n");				// 행 fin == 다음 줄 이동

		// 열 출력
		for (int j = 0; j < num; j++)
		{
			if (sets->ptr_wall[i * num * 2 + j * 2 + 1]) // 아래쪽 벽 존재 여부
				fprintf(fout, "+---");		// 벽 'O'
			else 
				fprintf(fout, "+   ");		// 벽 'X'
		}
		fprintf(fout, "+\n");				// 열 fin == 다음 줄 이동
	}
}

void freeMaze(DisjointSets *sets) {
	free(sets->ptr_cell);	// 내부 동적할당 free
	free(sets->ptr_wall);	// ''
	free(sets);				// set 자체 free
}

// 추가 함수
void swap(int *x, int *y) {	// 전형적인 swap 함수
	int temp = *x;
	*x = *y;
	*y = temp;
}

void shuffle(int* array, int num) {
	for(int i = 0; i < num; i++)	// num만큼 반복
	{
		int j = (rand() % num);		// random 값인 j 인덱스 생성 (j = 0 ~ num - 1)	
		swap(&array[i], &array[j]);	// swap으로 배열 내부 shuffle 재배정
	}
}