#include <iostream>
#include "queue.h"
// 깊은 복사 함수: value가 가리키는 메모리(1~1KB)를 복제
void* deep_copy_value(const void* src, size_t size) {
	if (!src || size == 0) return nullptr;
	void* dst = malloc(size);
	if (dst) memcpy(dst, src, size);
	return dst;
}

Queue* init(void) {
	return NULL;
}


void release(Queue* queue) {
	return;
}


Node* nalloc(Item item) {
	// Node 생성, item으로 초기화
	return NULL;
}


void nfree(Node* node) {
	return;
}


Node* nclone(Node* node) {
	return NULL;
}


Reply enqueue(Queue* queue, Item item) {
	Reply reply = { false, NULL };
	return reply;
}

Reply dequeue(Queue* queue) {
	Reply reply = { false, NULL };
	return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
	return NULL;
}
