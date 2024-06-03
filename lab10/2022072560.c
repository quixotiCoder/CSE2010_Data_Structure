#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right, int depth);
int Partition(Array* array, int left, int right, int depth);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size); // sorting할 데이터의 개수
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]); // sorting할 데이터 입력
    }

    QuickSort(array, 0, size - 1, 0);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);
    fclose(fin);
    fclose(fout);

	return 0;
}

Array* CreateArray(int size){
    Array* array = (Array*)malloc(sizeof(Array)); // Array 구조체 동적할당
    if(array == NULL) // 할당 실패 시
        return NULL; // NULL 반환

    array->size = size; // input받은 size 값
    array->values = (int*)malloc(sizeof(int) * size); // size만큼의 배열 동적할당
    if(array->values == NULL) // 할당 실패 시
    {
        free(array); // 이전 할당 해제
        return NULL; // NULL 반환
    }
    return array;
}

void QuickSort(Array* array, int left, int right, int depth){
    if (left < right) // 정상 인덱스 범위 확인
    {
        int pivot_index = Partition(array, left, right, depth); // pivot_index 찾기
        QuickSort(array, left, pivot_index - 1, depth + 1);     // pivot_index 기준 왼쪽 Quicksort
        QuickSort(array, pivot_index + 1, right, depth + 1);    // pivot_index 기준 오른쪽 Quicksort
    }
}

int Partition(Array* array, int left, int right, int depth){
    int pivot_value = array->values[right]; // pivot_value == 맨 오른쪽 인덱스의 '값'
    fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, pivot_value); // 인덱스 & pivot_value 출력
    int i = left; // 가장 처음 (= main에서의 호출) 0번째 인덱스부터 시작
    int j = right - 1; // pivot에서 한 칸 빠진 인덱스부터 시작
    
    while (i <= j) // i가 j보다 작거나 같을 때까지 반복 bc 교차 가능
    { 
        while (i <= j && array->values[i] <= pivot_value) // 왼쪽에서부터 pivot보다 큰 값 찾기 (커지면 반복문 탈출)
            i++;
        while (i <= j && array->values[j] > pivot_value) // 오른쪽에서부터 pivot보다 작은 값 찾기 (작아지면 반복문 탈출)
            j--;
        
        if (i < j) { 
            PrintArray(array, left, right); // swap 전 배열 출력
            swap(&array->values[i], &array->values[j]); // pivot보다 큰 값과 작은 값 교환
        }
    }

    PrintArray(array, left, right); // swap 후 배열 출력
    if(array->values[i] > pivot_value) // pivot보다 큰 값이면 pivot과 교환
    {
        swap(&array->values[i], &array->values[right]); // pivot과 교환
        PrintArray(array, left, right); // 최종 pivot 교환 후 배열 출력
    }
    return i; // return 'pivot의 index'
}

void PrintArray(Array* array, int left, int right){
    int i;
    for(i = 0; i < array->size; i++){
        if(i < left || right < i){
            fprintf(fout, "%-3s", "-");
        }
        else{
            fprintf(fout, "%-3d", array->values[i]);
        }
    }
    fprintf(fout, "\n");
    return;
}

void DeleteArray(Array* array){
    free(array->values); // 동적할당 받은 values 메모리 해제
    free(array); // Array 구조체 메모리 해제
    return;
}

void swap(int* a, int* b) { // a, b 값 교환
    int temp = *a;
    *a = *b;
    *b = temp;
    return;
}
