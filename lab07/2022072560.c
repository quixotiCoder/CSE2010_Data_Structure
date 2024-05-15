#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct AVLTreeNode AVLNode;
typedef int Element;

struct AVLTreeNode{
	Element element;
	AVLNode* left;
	AVLNode* right;
	int height;
};

/*
---function 선언---
*/

// 추가 구현 함수
int Max(int a, int b);
int GetHeight(AVLNode* T);
AVLNode* UpdateHeight(AVLNode* T);

// rotate 함수
AVLNode* RotateRight(AVLNode* node);
AVLNode* RotateLeft(AVLNode* node);
AVLNode* rebalance(AVLNode* T);

// insert / delete 함수
AVLNode* Insert(Element X, AVLNode* T);
AVLNode* Delete(Element X, AVLNode* T);

// 출력 / Tree 삭제 함수
void PrintInorder(AVLNode* T);
void DeleteTree(AVLNode* T);

/*
---function 구현---
*/

int Max(int a, int b){
    return (a > b) ? a : b; // 두 값 중 큰 값 return
}

int GetHeight(AVLNode* T){ // '특정 노드'의 양 옆 서브트리 높이 계산 -> 추후 rebalance의 bf 계산을 위함
    if (T == NULL)
        return -1; // 재귀적으로 height 찾기 위해, NULL 일때 '-1' return (아래의 else 문 속에서 return 되면 결국 '0'으로 반환)

    else {
        int left_height = GetHeight(T->left); // 왼쪽 서브트리의 높이 계산
        int right_height = GetHeight(T->right); // 오른쪽 서브트리의 높이 계산
        return 1 + Max(left_height, right_height); // 왼쪽과 오른쪽 서브트리 중 더 높은 높이 + 1
    }
}

AVLNode* UpdateHeight(AVLNode* T){
    int left_height = (T->left == NULL) ? 0 : T->left->height; // 왼쪽 서브트리의 높이 / NULL인 경우 오류 방지를 위해 삼항 연산으로 NULL일 땐, '0'
    int right_height = (T->right == NULL) ? 0 : T->right->height; // 오른쪽 서브트리의 높이 / NULL인 경우 오류 방지 위해 삼항 연산으로 NULL일 땐, '0'

    T->height = 1 + Max(left_height, right_height); // 현재 노드의 높이 갱신 (= 양쪽 서브트리의 높이 중 큰 것 {+ 1 : 현재 노드까지 높이 하나 더 계산})

    return T;
}

AVLNode* RotateRight(AVLNode* node){ //우회전
    AVLNode* L = node->left; // node->left를 L이라 할 때
    node->left = L->right; // node->left를 L->right로
    L->right = node; // L->right를 node로

    node = UpdateHeight(node); // 1st) node의 height update (bc node는 이제 L의 아래쪽에 있을테니 !먼저!) 
    L = UpdateHeight(L); // 2nd) R의 height update (반영된 node의 height까지 참고하고 높이 조정)
    
    return L; // 'L'이 새로운 root인 Tree return
}

AVLNode* RotateLeft(AVLNode* node){ //좌회전
    AVLNode* R = node->right; // node->right를 R이라 할 때
    node->right = R->left; // node->right를 R->left로
    R->left = node; // R->left를 node로

    node = UpdateHeight(node); // 1st) node의 height update (bc node는 이제 R의 아래쪽에 있을테니 !먼저!) 
    R = UpdateHeight(R); // 2nd) R의 height update (반영된 node의 height까지 참고하고 높이 조정)
    
    return R; // 'R'이 새로운 root인 Tree return
}

AVLNode* rebalance(AVLNode* T){
    if(T == NULL) // NULL 노드는 균형을 고려할 필요 X
        return NULL;

    int balance_factor = GetHeight(T->left) - GetHeight(T->right); // bf 계산

    if(balance_factor > 1) { // L == 왼쪽 서브트리 > 오른쪽 서브트리
        if(GetHeight(T->left->left) < GetHeight(T->left->right)) // LR 케이스인 경우
            T->left = RotateLeft(T->left); // 좌회전

        T = RotateRight(T); // 우회전
    }
    else if(balance_factor < -1) { // R == 오른쪽 서브트리 > 왼쪽 서브트리
        if(GetHeight(T->right->left) > GetHeight(T->right->right)) // RL 케이스인 경우
            T->right = RotateRight(T->right); // 우회전

        T = RotateLeft(T); // 좌회전
    }

    T = UpdateHeight(T); // 회전 모두 완료된 후의 높이 갱신      

    return T;
}

AVLNode* Insert(Element X, AVLNode* T){
    if (T == NULL) // NULL이면 
    {
        AVLNode* temp = (AVLNode*)malloc(sizeof(AVLNode));
        temp->element = X;
        temp->height = 1;
        temp->left = NULL;
        temp->right = NULL;
        return temp; // AVLNode 생성 후 반환
    }

    else if (T != NULL) // T->element != X면 recursive하게 찾고,
    {
        if(X < T->element) // X < 현재 노드 (= 왼쪽 자식으로 이동)
            T->left = Insert(X, T->left); // T->left = Insert(X, T->left) 형태로 갱신
        else if(X > T->element) // X > 현재 노드 (= 오른쪽 자식으로 이동)
            T->right = Insert(X, T->right); // T->right = Insert(X, T->left) 형태로 갱신

        else if(X == T->element) // T->element == X면 이미 존재하는 값
        {
            fprintf(fout, "insertion error : %d is already in the tree\n", X); // 오류 케이스 출력
            return T; // 별도 삽입 없이 기존의 Tree return
        }
    }

    T = UpdateHeight(T); // 위로 올라가며 height update

    T = rebalance(T); // rebalance (bc, 삽입 시 균형 망가질 수 있음)

    return T;
}

AVLNode* Delete(Element X, AVLNode* T){
    if (T == NULL) // NULL이면 
    {
        fprintf(fout, "deletion error : %d is not in the tree\n", X); // 오류 케이스 출력
        return NULL; // 아무것도 삭제 X == return NULL
    }
        
    else if (T != NULL) //T->element != X면 recursive하게 찾고,
    {
        if(X < T->element) // X < 현재 노드 (= 왼쪽 자식으로 이동)
            T->left = rebalance(T->left = Delete(X, T->left)); //T->left = Delete(X, T->left) 형태로 갱신
        else if(X > T->element) // X > 현재 노드 (= 오른쪽 자식으로 이동)
            T->right = rebalance(T->right = Delete(X, T->right)); //T->right = Delete(X, T->right) 형태로 갱신

        else if(X == T->element) // 삭제할 노드 찾은 경우
        {
            if(T->left == NULL && T->right == NULL) // 자식이 하나도 없을 경우 -> free T -> return NULL
            {     
                free(T);
                return NULL;
            }
 
    	    //  자식이 하나일 경우 -> free T -> return 자식
		    else if(T->left == NULL) // [else if] (= 일단 자식이 아예 없는 상태는 아닌 것이 보장) 중 오른쪽 자식만 있는 경우
            {	
                AVLNode* temp = T->right; // 오른쪽 자식을 임시 저장
                free(T); // 삭제될 노드 free를 통해 삭제
                return temp; // 위 else문 밖의 recursive 조건에 대한 return값 반환
            }

		    else if(T->right == NULL){ // [else if] (= 일단 자식이 아예 없는 상태는 아닌 것이 보장) 중 왼쪽 자식만 있는 경우
                AVLNode* temp = T->left; // 왼쪽 자식을 임시 저장
                free(T); // 삭제될 노드 free를 통해 삭제
                return temp; // 위 else문 밖의 recursive 조건에 대한 return값 반환
            }

		    else{ // 자식이 둘일 경우 -> 우측 자식의 가장 좌측 자식 찾기 -> 그 노드의 값으로 T->element 설정 -> 우측 자식의 가장 좌측 자식 삭제 recursive
		    	AVLNode* temp = T->right; // '우측 자식의 가장 좌측 자식 찾기' 를 위해 (1) 우측 자식 찾기

		    	while (temp->left != NULL) // (2) 가장 좌측 자식 찾기
		    		temp = temp->left;
    
		    	T->element = temp->element; // (3) 그 노드의 값(= temp->element)으로 T->element 설정
		    	T->right = Delete(temp->element, T->right); // (4) 우측 자식의 가장 좌측 자식 삭제 recursive
		    }
        }
    }

    T = rebalance(T); // rebalance (bc, 삭제 시 균형 망가질 수 있음)
    
    return T;
}

void PrintInorder(AVLNode* T){ // 전위 순회로 element 출력
    if (T == NULL) // 재귀를 통해 순회가 진행되기에, 마지막 돌아올 지점을 위한 NULL 체크 후 return
		return;
        
    PrintInorder(T->left); // 먼저, 트리 왼쪽 노드의 element 출력 
    fprintf(fout, "%d(%d) ", T->element, T->height); // 재귀를 통해 현재 트리의 노드 출력 [중위순회]
    PrintInorder(T->right); // 재귀를 통해 오른쪽 트리 출력
}

void DeleteTree(AVLNode* T){ // 후위 순회로 각 노드 free
    if(T == NULL) // 재귀를 통해 순회가 진행되기에, 마지막 돌아올 지점을 위한 NULL 체크 후 return
        return;

    DeleteTree(T->left); // 좌
    DeleteTree(T->right); // 우
    free(T); // free [후위순회]
}

/*------------------------------------------------------*/

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLNode* Tree = NULL;
	char cv;
	int key;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d\n", &key);
				Tree = Insert(key, Tree);
				break;
			case 'd':
				fscanf(fin, "%d\n", &key);
				Tree = Delete(key, Tree);
				break;
		}
		PrintInorder(Tree);
		fprintf(fout, "\n");
	}

	DeleteTree(Tree);
	fclose(fin);
	fclose(fout);

	return 0;
}
