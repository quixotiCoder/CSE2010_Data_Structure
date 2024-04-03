#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct BinaryTree TREE;

struct BinaryTree{
    int data;
    TREE* left_pointer;
    TREE* right_pointer;
};

void CreateFromArray(TREE* current_tree, int current_index, int* inserted_data_array, int length);
void DeleteTree(TREE* current);
void printPreorder(TREE* current);
void printInorder(TREE* current);
void printPostorder(TREE* current);


void DeleteTree(TREE* current){
    if (current == NULL) // 재귀를 통해 DeleteTree가 진행되기에, 마지막 돌아올 지점을 위한 NULL 체크 후 return
        return;

    DeleteTree(current->left_pointer); // 왼쪽 서브트리 삭제
    DeleteTree(current->right_pointer); // 오른쪽 서브트리 삭제
    free(current); // 양쪽 서브트리 삭제 후, 본 트리 삭제   
}

void printPreorder(TREE* current){
    if (current == NULL) // 재귀를 통해 순회가 진행되기에, 마지막 돌아올 지점을 위한 NULL 체크 후 return
        return;

    fprintf(fout, "%d ", current->data); // 먼저, 현재 가리키는 트리 노드의 data 출력 [전위순회]
    printPreorder(current->left_pointer); // 재귀를 통해 왼쪽 서브트리 출력
    printPreorder(current->right_pointer); // 재귀를 통해 오른쪽 트리 출력
}

void printInorder(TREE* current){
    if (current == NULL) // 재귀를 통해 순회가 진행되기에, 마지막 돌아올 지점을 위한 NULL 체크 후 return
        return;

    printInorder(current->left_pointer); // 먼저, 트리 왼쪽 노드의 data 출력 
    fprintf(fout, "%d ", current->data); // 재귀를 통해 현재 트리의 노드 출력 [중위순회]
    printInorder(current->right_pointer); // 재귀를 통해 오른쪽 트리 출력
}

void printPostorder(TREE* current){
    if (current == NULL) // 재귀를 통해 순회가 진행되기에, 마지막 돌아올 지점을 위한 NULL 체크 후 return
        return;

    printPostorder(current->left_pointer); // 먼저, 트리 왼쪽 노드의 data 출력 
    printPostorder(current->right_pointer); // 재귀를 통해 오른쪽 트리 출력
    fprintf(fout, "%d ", current->data); // 재귀를 통해 현재 트리의 노드 출력 [후위순회]
}

void CreateFromArray(TREE* current, int current_index, int* inserted_data_array, int length){
    if (current_index < length){ // 먼저 배열에 해당 index number가 존재하는지 확인
        current->data = inserted_data_array[current_index]; // input 파일의 arrary 인덱스 '1'번부터 순차적 삽입

        if(current_index*2 < length){ // [현재 인덱스*2] 가 length를 넘는지 for 왼쪽 자식
            TREE* left_node = malloc(sizeof(TREE)); // 왼쪽 노드 메모리 할당
            left_node->left_pointer = NULL; // 새로 만든 노드 초기화 과정
            left_node->right_pointer = NULL; // 새로 만든 노드 초기화 과정
            current->left_pointer = left_node; // current의 왼쪽 노드로 새로 할당한 왼쪽 노드(left_node) 지정
            CreateFromArray(left_node, current_index*2, inserted_data_array, length); // 왼쪽 노드에 대한 재귀
        }

        if(current_index*2 +1 < length){ // [현재 인덱스*2 + 1] 가 length를 넘는지 for 오른쪽 자식
            TREE* right_node = malloc(sizeof(TREE)); // 오른쪽 노드 메모리 할당
            right_node->left_pointer = NULL; // 새로 만든 노드 초기화 과정
            right_node->right_pointer = NULL; // 새로 만든 노드 초기화 과정
            current->right_pointer = right_node; // current의 오른쪽 노드로 새로 할당한 오른쪽 노드(right_node) 지정
            CreateFromArray(right_node, current_index*2 + 1, inserted_data_array, length); // 오른쪽 노드에 대한 재귀
            }
    }
}

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char command;
	int input_data;
	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES);
	int* array = malloc(sizeof(int) * (NUM_NODES + 1));
	array[0] = -1;
	for(int i = 0; i < NUM_NODES; i++){
	    fscanf(fin, "%d", &input_data);
	    array[i + 1] = input_data;
	}
	TREE* header = malloc(sizeof(TREE));
	CreateFromArray(header, 1, array, NUM_NODES + 1);
	printPreorder(header);
	fprintf(fout, "\n");
	printInorder(header);
	fprintf(fout, "\n");
	printPostorder(header);
	DeleteTree(header);
	fclose(fin);
	fclose(fout);
	return;
}