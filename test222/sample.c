#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct LinkedListElement LE;

struct LinkedListElement{
    int key;
    LE* next_pointer;
};

void PrintPrevious(LE* header, int target_key);
void PrintList(LE* header);
void Insert(LE* header, int inserted_key, int target_key);
void Delete(LE* header, int target_key);
LE* Find(LE* header, int target_key);
LE* FindPrevious(LE* header, int target_key);

void Insert(LE* header, int inserted_key, int target_key){
    if(Find(header, target_key) != NULL)
		printf("insertion [%d] failed : the key already exists", target_key);
	else{
		// Find로 target_key 노드 찾기
		// inserted_key를 malloc을 이용해 생성
		// inserted_key 노드의 next_pointer 를 target_key의 next_pointer로 저장
    	// target_key 노드의 next_pointer 를 inserted_key의 메모리 주소로 저장
	}	
    
}

void Delete(LE* header, int target_key){
    // FindPrevious를 통해 target_key의 직전 노드를 찾기
    // target_key의 노드도 next_pointer로 찾기
    // FindPrevious의 next_pointer를 target_key의 next_pointer로 변경
    // target_key를 메모리 해제
}

LE* Find(LE* header, int target_key){
    LE* current;

	current = header;// 포인터 current를 header로 설정
    while(current!=NULL && current->key != target_key) // current 의 next_pointer가 NULL인지 체크
		current = current->next_pointer;
	if(current->key != NULL)
		return current;
	else	
    return NULL;// NULL일 경우 current 반환
    
}

LE* FindPrevious(LE* header, int target_key){
    LE* current;

	current = header;// 포인터 current를 header로 설정
    // current 의 next_pointer 의 next_pointer가 NULL인지 체크
    // NULL일 경우 current 반환
    return NULL;
}

void PrintPrevious(LE* header, int target_key){
    //FindPrevious를 사용하여 출력
}

void PrintList(LE* header){
    //header부터 next_pointer를 따라가면서 출력
}

LE* CreateList(){
    LE* L;
	L = (LE*)malloc(sizeof(LE)); //malloc을 이용하여 header 생성
	L->key = 0;
	L->next_pointer = NULL;
}

void DeleteList(LE* header){
    //free를 이용하여 header부터 끝까지 전부 해제
}

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char command;
	int key1, key2;

    LE* header = CreateList();
	
	while(1){
		command = fgetc(fin);
		if(feof(fin)) break;
		switch(command){
		    case 'i':
		        fscanf(fin, "%d %d", &key1, &key2);
		        Insert(header, key1, key2);
		        break;
		    case 'd':
		        fscanf(fin, "%d", &key1);
		        Delete(header, key1);
		        break;
		    case 'f':
		        fscanf(fin, "%d", &key1);
		        PrintPrevious(header, key1);
		        break;
		    case 'p':
		        PrintList(header);
		        break;
		}
	}
	DeleteList(header);
	fclose(fin);
	fclose(fout);
	return;
}