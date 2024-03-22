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
	LE* check_insert = Find(header, inserted_key); // 넣을 값(inserted_key)이 이미 존재하는지 검사를 위해
	LE* check_target = Find(header, target_key); //  기준값(target_key)이 없는지 검사를 위해

	if(check_insert != NULL) // inserted_key이 이미 리스트에 존재하는 경우
		fprintf(fout, "insertion %d failed : the key already exists\n", inserted_key); // 해당 케이스 처리 문장 출력
	else if(check_insert == NULL && check_target == NULL) // inserted_key가 존재 X && target_key도 없는 경우 '만' 실행 조건
		fprintf(fout, "insertion %d failed : can not find location\n", inserted_key); // 해당 케이스 처리 문장 출력
    
	else{ // 단방향 연결리스트의 삽입 과정 진행 
		LE* new_insert = (LE*)malloc(sizeof(LE)); // insert를 위해 새로운 노드 생성
		new_insert->key = inserted_key; // inserted_key값 노드의 element에 삽입
		new_insert->next_pointer = check_target->next_pointer; // 기준값을 지닌 노드가 원래 가르키던 next_pointer 승계
		check_target->next_pointer = new_insert; // 기준 노드의 next_pointer로 새로 만든 new_insert 지정
	}
}

void Delete(LE* header, int target_key){
    LE* previous = FindPrevious(header, target_key); // 리스트 내에 targer_key가 존재하는지 판별

	if(previous == NULL || previous->next_pointer == NULL){ //  리스트 내에 targer_key가 존재 X인 경우 or 아무런 노드 없이 header만 존재하는 경우 판별
		fprintf(fout, "deletion %d failed : node is not in the list\n", target_key); // 예외 처리 메세지 출력	
		return;
	}

	LE* delete = previous->next_pointer; // 'target_key를 element로 지닌 노드'인 'current노드의 뒷 노드'를 'delete'로 지정
	previous->next_pointer = delete->next_pointer; // 연결리스트의 삭제 연산 과정 : 삭제될 'delete' 노드의 next_pointer를 delete 노드의 앞 노드 'current'의 next_pointer로 승계 bc 먼저 free(delete) 시, delete->next_pointer 정보를 알 수 없음 
	free(delete); // target_key를 지닌 'delete' 노드 메모리 해제 
}

LE* Find(LE* header, int target_key){
    LE* current = header; // 포인터 current를 header로 설정

    while(current != NULL){ // current 가 NULL인지 체크(루프 탈출 조건)
		if (current->key == target_key) // current의 key가 target_key면 
			return current;	// 해당 노드 반환
		current = current->next_pointer; // current의 key가 target_key가 아닌 경우, current의 다음 노드로 이동하여 탐색 진행
	} 
	
	return NULL; // 못 찾으면 NULL반환
    
}

LE* FindPrevious(LE* header, int target_key){
    LE* current = header; // 포인터 current를 header로 설정
	
	while(current->next_pointer != NULL && current->next_pointer->key != target_key) // 조건 1) current 의 next_pointer가 NULL인지 체크 && 조건 2) current 의 next_pointer의 key 가 target_key 루프 탈출 for target_key의 '직전 노드'의 정보가 필요하기에
		current = current->next_pointer; // 위 조건 위배 전까지, 순차적으로 노드 뒤로 이동하며 탐색
	
	if(current->next_pointer == NULL) // 조건 1)에 의해 루프가 끝난 경우 == 루프 끝까지 탐색했지만 target_key가 존재하지 않는 경우
		return NULL; // NULL을 return
	
	return current; // target_key 존재하는 경우, 저장하고 있는 노드의 앞 노드(current)를 return
}

void PrintPrevious(LE* header, int target_key){
	LE* current = FindPrevious(header, target_key); // 리스트 속 target_key의 존재 유무 && 있다면 주변 위치 정보를 알기 위해 FindPrevious 함수 실행

	if(current == NULL) // FindPrevious의 return이 NULL인 경우 == target_key 존재 X
		fprintf(fout, "finding %d failed : node is not in the list\n", target_key); // 해당 케이스 처리 문장 출력
	
	else{ // FindPrevious의 return이 NULL이 아닌 경우 == target_key 존재 O
		if(current->key == -1) // target_key 앞이 header인 경우
			fprintf(fout, "previous node of %d is head\n", target_key); // 해당 케이스 처리 문장 출력
		else // target_key 앞이 header이 아닌 경우
			fprintf(fout, "previous node of %d is %d\n", target_key, current->key); // 해당 케이스 처리 문장 출력
	}
}

void PrintList(LE* header){
	LE* current = header->next_pointer; // header의 뒤 노드부터 시작하기 위해, 해당 위치 'current'로 선언
	
	if(current == NULL) // List가 비어 있는 경우의 예외 처리를 위해 header 뒤 아무런 노드가 없는 NULL인지 체크
		fprintf(fout, "empty list\n"); // 예외 처리 메세지 출력
	
	while(current != NULL){ // 빈 연결리스트가 아닌 경우
		fprintf(fout, "%d ", current->key); // 노드에 저장된 key 값 출력
		current = current->next_pointer; // 이후, 다음 노드로 current로 변경 후 반복문 진행
	}
	
	fprintf(fout, "\n"); // 모든 노드의 element 출력 후, 개행 문자 출력
}

LE* CreateList(){
	LE* header = (LE*)malloc(sizeof(LE)); // malloc을 이용하여 header 생성
	header->key = -1; // 연결리스트의 header을 나타내는 '-1'을 헤더의 element로 삽입
	header->next_pointer = NULL; // 헤더만이 존재하는 상태이기에 next_pointer는 가르키는 노드가 없는 NULL을 가르킴
	return header; // LE* 타입의 함수의 return 값으로 만들어진 header 제공
}

void DeleteList(LE* header){
    LE* current = header->next_pointer; // input으로 받은 연결리스트의 header의 다음 노드를 'current'로 지정
	
	while(current != NULL){ // [cur == NULL] 인 경우, [즉, 마지막 노드]를 만날 때가지 while 반복문 진행
		LE* delete_current = current; // header 뒤 노드부터 순차적으로 삭제하기 위해 '삭제될 현재 노드'를 'delete_current'로 선언
		current = current->next_pointer; // 삭제될 노드의 '다음 노드'를 먼저 'current'로 지정 (bc 현재의 delete_current 먼저 삭제 시, 다음 노드에 대한 정보 알 수 없음)
		free(delete_current); // header 뒤의 노드부터 순차적 삭제 by free()함수
	} // while문의 조건과 같이 마지막 노드까지 삭제 진행
	
	free(header); // header 뒤의 모든 노드 삭제 후, 최종적으로 연결리스트의 header 삭제
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