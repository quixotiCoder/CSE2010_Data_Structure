#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct Stack{
	int *key;
	int top;
	int max_stack_size;
}Stack;

Stack* CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);

void DeleteStack(Stack* S);
bool IsEmpty(Stack* S);
bool IsFull(Stack* S);

void main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	Stack* stack;
	char input_str[101];
	int max=20, i=0,a,b;

	fgets(input_str,101,fin);
	stack = CreateStack(max);
	
	fprintf(fout, "top numbers : ");
	while(input_str[i] != '#'){
		if('1'<=input_str[i] && input_str[i]<='9'){
			Push(stack,input_str[i]-'0');
		}
		else{
			int b = Pop(stack);
			int a = Pop(stack);
			switch (input_str[i]){
				case '+' : Push(stack, a+b); break;
				case '-' : Push(stack, a-b); break;
				case '*' : Push(stack, a*b); break;

				case '/' :
					if(b == 0){ // '/, % 연산 시 0으로 나누는 경우'
						fprintf(fout, "\nerror : invalid postfix expression, divide by zero\n"); // 해당 case 에러메시지 출력
                        fclose(fin); // 에러메시지 출력 후 프로그램이 종료되도록 구현
                        fclose(fout); // ''
                        DeleteStack(stack); // ''
                        return; // ''
					}
					else
						Push(stack, a/b); 
					break;

				case '%' :
					if(b == 0){ // '/, % 연산 시 0으로 나누는 경우'
							fprintf(fout, "\nerror : invalid postfix expression, divide by zero\n"); // 해당 case 에러메시지 출력
							fclose(fin); // 에러메시지 출력 후 프로그램이 종료되도록 구현
							fclose(fout); // ''
							DeleteStack(stack); // ''
							return; // ''
						}
						else
							Push(stack, a%b); 
						break;

				default : break;
			}
		}
		fprintf(fout, "%d ", Top(stack)); // Stack의 상태가 변할 때마다 Top Element를 출력
		i++;
	}

	if(stack->top > 0){ // 'Stack 내부 원소가 1개보다 많은 상태에서 Expression이 종료되는 경우'. 이때, 조건이 [stack->top > 0] 이유 <- [stack->top이 0인 경우 == 원소 1개 존재인 경우]이기에
		fprintf(fout, "\nerror : invalid postfix expression, %d elements are left", stack->top + 1); // 해당 case 에러메시지 출력. 이때, [stack->top + 1]을 인자로 주는 논리는 위 논리와 동
		fclose(fin); // 에러메시지 출력 후 프로그램이 종료되도록 구현
        fclose(fout); // ''
        DeleteStack(stack); // ''
        return; // ''
	}

	
	fprintf(fout, "\nevaluation result : %d\n", Top(stack)); // Postfix evaluation 최종결과 출력 

	fclose(fin);
	fclose(fout);
	DeleteStack(stack);
}

Stack* CreateStack(int max){
    Stack* S = (Stack*)malloc(sizeof(Stack)); // malloc으로 Stack 포인터 S 할당
    
	if(S == NULL) // malloc으로 메모리 할당 실패한 case 처리
		return NULL;
	
	S->key = (int*)malloc(max * sizeof(int)); // S->key 를 max 크기의 int array pointer 할당 (malloc)
    S->max_stack_size = max; // S->max_stack_size 설정
    S->top = -1;// S->top 을 -1로 설정

    return S; // S 반환
}

void DeleteStack(Stack* S){
    free(S->key); // key를 free
    free(S); // S를 free
	return;
}

void Push(Stack* S, int X){
	if(IsFull(S)){ // 'Stack이 꽉 차있는 상태에서 Push 하는 경우'
		fprintf(fout, "\nerror : invalid postfix expression, stack is full\n"); // 해당 case 에러메시지 출력
		fclose(fin); // 에러메시지 출력 후 프로그램이 종료되도록 구현
		fclose(fout); // ''
		DeleteStack(S); // ''
		return; // ''
	}

    S->top += 1; // S->top += 1
    S->key[S->top] = X;// S->key의 top 에 X 대입
	return;
}

int Pop(Stack* S){
	if(IsEmpty(S)){ // 'Stack이 비어있는 상태에서 Pop 하는 경우'
		fprintf(fout, "\nerror : invalid postfix expression, stack is empty\n"); // 해당 case 에러메시지 출력
		fclose(fin); // 에러메시지 출력 후 프로그램이 종료되도록 구현
		fclose(fout); // ''
		DeleteStack(S); // ''
		return 0;
	}

    return S->key[S->top--]; // S->key의 top 반환 후, top을 하나 낮춤
}

int Top(Stack* S){
    return S->key[S->top]; // S->key의 top 반환
}

bool IsFull(Stack* S){
	return (S->top == S->max_stack_size - 1); // index는 '0'부터 시작이기에, 'S->max_stack_size - 1'로 full인 상태 확인 
}

bool IsEmpty(Stack* S){
	return (S->top == -1); // stack이 빈 상태인 'S->top == -1' 조건 확인
}