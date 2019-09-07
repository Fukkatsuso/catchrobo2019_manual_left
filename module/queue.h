/*
 * queue.h
 *
 *  Created on: 2019/08/27
 *      Author: mutsuro
 */

#ifndef MODULE_QUEUE_H_
#define MODULE_QUEUE_H_

#include "mbed.h"


// Queueにノードとして格納するための形式
template <class T>
class QueueNode
{
private:
	T elem;
	QueueNode* next;

public:
	QueueNode(T elem) {
		this->elem = elem;
		next = NULL;
	}

	void set_next(QueueNode* next_node) {
		next = next_node;
	}

	T get_elem() {
		return elem;
	}

	QueueNode* get_next() {
	    return next;
	}
};


// Queue本体
template <class T>
class Queue
{
public:
	Queue() {
		size = 0;
		clear();
	}

	int length() {
		return size;
	}

	bool is_empty() {
		return (size == 0);
	}

	// マイコンでは動作保証できない
	void clear() {
		if (size == 0) {
			head = NULL;
			tail = NULL;
			return;
		}

		QueueNode<T>* node = head;
		QueueNode<T>* next_node;

		while (node != NULL) {
			next_node = node->get_next();
			delete node;
			node = next_node;
		}

		size = 0;
		head = NULL;
		tail = NULL;
	}

	void push(T elem) {
		QueueNode<T>* new_node = new QueueNode<T>(elem);

		if (size > 0) {
			tail->set_next(new_node);
		}
		else {
			head = new_node;
		}
		tail = new_node;
		size++;
	}

	void pop() {
		QueueNode<T>* head_node = head;
		if (size == 1) {
		    head = NULL;
		    tail = NULL;
		}
		else {
			head = head->get_next();
		}
		delete head_node;
		size--;
	}

	T front() {
		return head->get_elem();
	}

	T back() {
		return tail->get_elem();
	}

private:
	int size;
	QueueNode<T>* head;
	QueueNode<T>* tail;
};


#endif /* MODULE_QUEUE_H_ */
