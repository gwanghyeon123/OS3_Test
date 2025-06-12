#include <iostream>
#include "queue.h"
// ���� ���� �Լ�: value�� ����Ű�� �޸�(1~1KB)�� ����
void* deep_copy_value(const void* src, size_t size) {
	if (!src || size == 0) return nullptr;
	void* dst = malloc(size);
	if (dst) memcpy(dst, src, size);
	return dst;
}
// ��� ���� (value�� ����)
void free_node(Node* node, size_t value_size) {
	if (node) {
		if (node->item.value)
			free(node->item.value); // ���� ����� value ����
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
		free_node(cur, 0); // value_size�� ��, value�� free�� ����
		cur = next;
	}
	delete queue;
}


Node* nalloc(Item item) {
	// Node ����, item���� �ʱ�ȭ
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
