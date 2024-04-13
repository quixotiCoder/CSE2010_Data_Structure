#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BinarySearchTreeNode BSTNode;
struct BinarySearchTreeNode{
	int value;
	BSTNode* left;
	BSTNode* right;
};

BSTNode* insertNode(BSTNode* current, int key);
BSTNode* deleteNode(BSTNode* current, int key);
BSTNode* findNode(BSTNode* current, int key);
void printInorder(BSTNode* current);
void deleteTree(BSTNode* current);

BSTNode* insertNode(BSTNode* current, int key){
    if(findNode(current, key) != NULL){ // findNode 함수의 반환값이 '특정 노드'로 존재 == 트리에 key 값 지는 노드 존재하는 경우
		fprintf(fout, "insertion error %d", key); // Tree에 이미 존재하는 Key 값일 경우의 에러 메시지
		return current;
	}

    else{
		if(current == NULL){ 	// [current == NULL인 경우] = [삽입할 장소 찾은 경우]
		BSTNode* new_node = malloc(sizeof(BSTNode)); // BST의 새로운 노드 선언
		new_node->value = key; // key값을 새 노드의 value로 지정
		new_node->left = NULL; // 노드 초기화
		new_node->right = NULL; // 노드 초기화
		return new_node;
		}

		else if (key < current->value) 	// else if (key < current->value) 일 경우 recursive (bc of BST의 성질)
			current->left = insertNode(current->left, key);
	
		else if (key > current->value)     // else if (key > current->value) 일 경우 recursive (bc of BST의 성질)
			current->right = insertNode(current->right, key);
	}	
}

//오른쪽 자식 기준
BSTNode* deleteNode(BSTNode* current, int key){ // main 함수 호출 조건에서 key값 존재 보장된 채로 함수 실행됨 (by 145 line)
	if(key < current->value) // if (key < current->value) 일 경우 recursive (bc of BST의 성질)
		current->left = deleteNode(current->left, key);
	
	else if(key > current->value) // else if (key > current->value) 일 경우 recursive (bc of BST의 성질)
		current->right = deleteNode(current->right, key);

	else{     //else 의 경우 4가지
		if(current->left == NULL && current->right == NULL){     //  자식이 하나도 없을 경우 -> free current -> return NULL
            free(current);
            return NULL;
        }
 
    	//  자식이 하나일 경우 -> free current -> return 자식

		else if(current->left == NULL){	// [else if] (= 일단 자식이 아예 없는 상태는 아닌 것이 보장) 중 오른쪽 자식만 있는 경우
            BSTNode* temp = current->right; // 오른쪽 자식을 임시 저장
            free(current); // 삭제될 노드 free를 통해 삭제
            return temp; // 위 else문 밖의 recursive 조건에 대한 return값 반환
        }

		else if(current->right == NULL){ // [else if] (= 일단 자식이 아예 없는 상태는 아닌 것이 보장) 중 왼쪽 자식만 있는 경우
            BSTNode* temp = current->left; // 왼쪽 자식을 임시 저장
            free(current); // 삭제될 노드 free를 통해 삭제
            return temp; // 위 else문 밖의 recursive 조건에 대한 return값 반환
        }

		else{     //  자식이 둘일 경우 -> 우측 자식의 가장 좌측 자식 찾기 -> 그 노드의 값으로 current->value 설정 -> 우측 자식의 가장 좌측 자식 삭제 recursive
			BSTNode* temp = current->right; // '우측 자식의 가장 좌측 자식 찾기' 를 위해 (1) 우측 자식 찾기

			while (temp->left != NULL) // (2) 가장 좌측 자식 찾기
				temp = temp->left;
			
			current->value = temp->value; // (3) 그 노드의 값(= temp->value)으로 current->value 설정
			current->right = deleteNode(current->right, current->value); // (4) 우측 자식의 가장 좌측 자식 삭제 recursive
		}
		return current;
	}
}

BSTNode* findNode(BSTNode* current, int key){
    if(current == NULL) // current == NULL일 경우, 찾는 값 역시 당연히 존재 X -> 에러문 출력
		return current; // current. 즉, NULL 반환
	 	
	else{ // current != NULL일 경우
		if(current->value == key) // key값을 지닌 노드 발견
			return current; // 해당 노드 반환
			
		else{
			if (key < current->value) 	// if (key < current->value) 일 경우 current->left에 대해 recursive (bc of BST의 성질)
				findNode(current->left, key);
	
			else if (key > current->value) 	//else if (key > current->value) 일 경우 current->right에 대해 recursive (bc of BST의 성질)
				findNode(current->right, key);
		}
	}
	
}
void printInorder(BSTNode* current){
    if (current == NULL) // 재귀를 통해 순회가 진행되기에, 마지막 돌아올 지점을 위한 NULL 체크 후 return
		return;
        
    printInorder(current->left); // 먼저, 트리 왼쪽 노드의 data 출력 
    fprintf(fout, "%d ", current->value); // 재귀를 통해 현재 트리의 노드 출력 [중위순회]
    printInorder(current->right); // 재귀를 통해 오른쪽 트리 출력
}

void deleteTree(BSTNode* current){
    if (current == NULL) // 재귀를 통해 DeleteTree가 진행되기에, 마지막 돌아올 지점을 위한 NULL 체크 후 return
        return;

    deleteTree(current->left); // 왼쪽 서브트리 삭제
    deleteTree(current->right); // 오른쪽 서브트리 삭제
    free(current); // 양쪽 서브트리 삭제 후, 본 트리 삭제   
}

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	BSTNode* root = NULL;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				root = insertNode(root, key);
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key)){
					fprintf(fout, "%d is in the tree\n", key);
				}else{
					fprintf(fout, "%d is not in the tree\n", key);
				}
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){ // 존재 여부 파악한 후, delete 진행 -> value 존재 보장됨
					root = deleteNode(root, key);
				}else{
					fprintf(fout, "deletion error %d\n", key);
				}
				break;
			case 'p':
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL){
						fprintf(fout, "tree is empty");
					}else{
						printInorder(root);
					}
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
}