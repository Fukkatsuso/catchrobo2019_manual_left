/*
 * stack.h
 *
 *  Created on: 2019/08/28
 *      Author: mutsuro
 */

#ifndef MODULE_STACK_H_
#define MODULE_STACK_H_


#include "mbed.h"


template <class T>
class StackNode
{
private:
	T elem;
	StackNode* below;

public:
	StackNode(T elem) {
		this->elem = elem;
		below = NULL;
	}

	void set_below(StackNode* below_node) {
		below = below_node;
	}

	T get_elem() {
		return elem;
	}

	StackNode* get_below() {
		return below;
	}
};


template <class T>
class Stack
{
public:
	Stack() {
		clear();
	}

	int length() {
		return size;
	}

	bool is_empty() {
		return (size == 0);
	}

	void clear() {
		StackNode<T>* node = top;
		StackNode<T>* below_node;

		while (node != NULL) {
			below_node = node->get_below();
			delete node;
			node = below_node;
		}

		size = 0;
		top = NULL;
	}

	void push(T elem) {
		StackNode<T>* new_node = new StackNode<T>(elem);

		if (size > 0) {
			new_node->set_below(top);
		}
		top = new_node;
		size++;
	}

	void pop() {
		StackNode<T>* top_node = top;
		if (size == 1) {
			top = NULL;
		}
		else {
			top = top_node->get_below();
		}
		delete top_node;
		size--;
	}

	T front() {
		return top->get_elem();
	}

private:
	int size;
	StackNode<T>* top;
};


#endif /* MODULE_STACK_H_ */
