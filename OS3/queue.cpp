#include <iostream>
#include<new>
#include<cstdlib>
#include "queue.h"	
#include <cstring>



// 깊은 복사 함수: value가 가리키는 메모리(1~1KB)를 복제
void* deep_copy_value(const void* src, size_t size) {
	if (!src || size <= 0) return nullptr;
	void* dst = malloc(size);
	if (dst) memcpy(dst, src, size);
	return dst;
}
// 노드 해제 (value도 해제)
void free_node(Node* node, size_t value_size) {
	if (node) {
		if (node->item.value)
			free(node->item.value); // 깊은 복사된 value 해제
		delete node;
	}
}

Queue* init(void) {
	Queue* queue = new (std::nothrow) Queue;
	if (!queue) return nullptr;
	queue->head = nullptr;
	queue->tail = nullptr;
	return queue;
}


void release(Queue* queue) {
	if (!queue) return;
	Node* cur = queue->head;
	while (cur) {
		Node* next = cur->next;
		nfree(cur);
		cur = next;
	}
	delete queue;
}

	
Node* nalloc(Item item) {
	// Node 생성, item으로 초기화
	Node* node = new (std::nothrow) Node;
	if (!node) return nullptr;
	node->item.key = item.key;
	node->item.value_size = item.value_size;
	node->item.value = deep_copy_value(item.value, item.value_size);
	node->next = nullptr;
	return node;
}


void nfree(Node* node) {
	if (!node) return;
	if (node->item.value) free(node->item.value);
	delete node;
}


Node* nclone(Node* node) {
	if (!node) return nullptr;
	return nalloc(node->item);
}


Reply enqueue(Queue* queue, Item item) {
	Reply reply = { false, {0, nullptr, 0} };
	if (!queue||item.value_size<=0) return reply;
	std::lock_guard<std::mutex> lock(queue->mtx);

	Node* prev = nullptr;
	Node* cur = queue->head;

	while (cur && item.key < cur->item.key) {
		prev = cur;
		cur = cur->next;
	}
	if (cur && item.key == cur->item.key) {
		if (cur->item.value) free(cur->item.value);
		cur->item.value = deep_copy_value(item.value, item.value_size);
		cur->item.value_size = item.value_size;
		reply.success = true;
		reply.item.key = cur->item.key;
		reply.item.value_size = cur->item.value_size;
		reply.item.value = deep_copy_value(cur->item.value, cur->item.value_size);
		return reply;
	}
	Node* node = nalloc(item);
	if (!node) return reply;
	
	node->next = cur;
	if (!prev) queue->head = node;
	else prev->next = node;
	if (!cur) queue->tail = node;
	reply.success = true;
	reply.item.key = node->item.key;
	reply.item.value_size = node->item.value_size;
	reply.item.value = deep_copy_value(node->item.value, node->item.value_size);
	return reply;
}


Reply dequeue(Queue* queue) {
	Reply reply = { false, {0, nullptr, 0} };
	if (!queue) return reply;
	std::lock_guard<std::mutex> lock(queue->mtx);

	if (!queue->head) return reply;
	Node* node = queue->head;
	queue->head = node->next;
	
	if (!queue->head) queue->tail = nullptr;

	reply.success = true;
	reply.item.key = node->item.key;
	reply.item.value_size = node->item.value_size;
	reply.item.value = deep_copy_value(node->item.value, node->item.value_size);
	nfree(node);
	return reply;
}


Queue* range(Queue* queue, Key start, Key end) {
	if (!queue) return nullptr;
	Queue* new_queue = init();
	if (!new_queue) return nullptr;
	std::lock_guard<std::mutex> lock(queue->mtx);

	Node* cur = queue->head;
	while (cur) {
		if (cur->item.key >= start && cur->item.key <= end) {
			Item item;
			item.key = cur->item.key;
			item.value_size = cur->item.value_size;
			item.value = deep_copy_value(cur->item.value, cur->item.value_size);
			enqueue(new_queue, item);
			if (item.value) free(item.value);
		}
		cur = cur->next;
	}
	return new_queue;
}

