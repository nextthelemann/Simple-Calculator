//Header that contains my Stack implementation. It has a base Stack class, followed by
//two extensions of the base class that include stacks that can hold doubles, and chars.
//Each class contains a pop function to pop out the top data value out of the stack. Followed by
//a isEmpty function to check if the stack is empty, and then a size function to return the size
//of the stack. The two extension classes then contain the functions push, and peek. Push is used
//to push a data value on the top of the stack, and peek is used to see what the top data value 
//that is currently at the top of the stack.

#include <string>
using namespace std;


class Stack {
public:
	int top;
	int capacity;
	Stack() {
		top = -1;
	}

	void pop() {
		if(top == -1) {
			throw string("Stack is empty");
		}
		top--;
	}

	bool isEmpty() {
		return (top == -1);
	}

	int size() {
		return capacity;
	}
};

class Stack_Double: public Stack {
private:
	double *storage;
public:
	Stack_Double(int capacity) {
		if (capacity <= 0) {
			throw string("Stack's capacity must be positive");
		}
		storage = new double[capacity];
		this->capacity = capacity;
		top = -1;
	}
	void push(double value) {
		if(top == capacity) {
			throw string("Stack's underlying storage is overflow");
		}
		top++;
		storage[top] = value;
	}
	double peek() {
		if(top == -1) {
			throw string("Stack is empty");
		}
		return storage[top];
	}
};

class Stack_Char: public Stack {
private:
	char *storage;
public:
	Stack_Char(int capacity) {
		if (capacity <= 0) {
			throw string("Stack's capacity must be positive");
		}
		storage = new char[capacity];
		this->capacity = capacity;
		top = -1;
	}
	void push(char value) {
		if(top == capacity) {
			throw string("Stack's underlying storage is overflow");
		}
		top++;
		storage[top] = value;
	}
	char peek() {
		if(top == -1) {
			throw string("Stack is empty");
		}
		return storage[top];
	}
};
