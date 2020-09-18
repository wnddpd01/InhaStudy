// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
#include <fstream>
#include <iostream>

#define MAX_SIZE 10000

struct POINT
{
	int x;
	int y;
};

struct NODE
{
	int x, y;
	char tile_state;
	size_t g_val;
	size_t h_val;
	size_t f_val;
};

typedef struct priorityQueue {
	NODE * heap[MAX_SIZE];
	int count = 0;
}priorityQueue;

inline void swap(NODE **a, NODE **b) {
	NODE * temp = *a;
	*a = *b;
	*b = temp;
}

inline void push(priorityQueue *pq, NODE * data) {
	if (pq->count >= MAX_SIZE) return;
	pq->heap[pq->count] = data; //완전 이진트리의 마지막 원소에 데이터 삽입
	int now = pq->count; //삽입된 데이터에 해당하는 노드의 인덱스
	int parent = (pq->count - 1) / 2; //삽입된 노드의 부모노드
	while (now > 0 && pq->heap[now]->f_val < pq->heap[parent]->f_val) {
		swap(&pq->heap[now], &pq->heap[parent]); //부모노드와 삽입된 노드 교체
		now = parent;
		parent = (parent - 1) / 2;
	}
	pq->count++;
}

inline NODE* pop(priorityQueue *pq) {
	if (pq->count <= 0) return nullptr;
	NODE * res = pq->heap[0];
	pq->count--;
	pq->heap[0] = pq->heap[pq->count];
	int now = 0, leftChild = 1, rightChild = 2;
	int target = now;

	//원소가 존재할 때 까지만 반복
	while (leftChild < pq->count) {
		if (pq->heap[target]->f_val > pq->heap[leftChild]->f_val) target = leftChild;
		if (pq->heap[target]->f_val > pq->heap[rightChild]->f_val && rightChild < pq->count) target = rightChild;
		if (target == now) break;
		else {
			swap(&pq->heap[now], &pq->heap[target]);
			now = target;
			leftChild = now * 2 + 1;
			rightChild = now * 2 + 2;
		}
	}
	return res;
}