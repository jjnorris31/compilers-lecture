#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#define _GNU_SOURCE

// Data structure for stack
struct stack
{
	int maxsize;	// define max capacity of stack
	int top;		
	int *items;
};

struct par {
	char c;
	int i;
};

// Utility function to initialize stack
struct stack* newStack(int capacity)
{
	struct stack *pt = (struct stack*)malloc(sizeof(struct stack));

	pt->maxsize = capacity;
	pt->top = -1;
	pt->items = (int*)malloc(sizeof(int) * capacity);

	return pt;
}

// Utility function to return the size of the stack
int size(struct stack *pt)
{
	return pt->top + 1;
}

// Utility function to check if the stack is empty or not
int isEmpty(struct stack *pt)
{
	return pt->top == -1;	// or return size(pt) == 0;
}

// Utility function to check if the stack is full or not
int isFull(struct stack *pt)
{
	return pt->top == pt->maxsize - 1;	// or return size(pt) == pt->maxsize;
}

// Utility function to add an element x in the stack
void push(struct stack *pt, int x)
{
	// check if stack is already full. Then inserting an element would 
	// lead to stack overflow
	if (isFull(pt))
	{
		printf("OverFlow\nProgram Terminated\n");
		exit(EXIT_FAILURE);
	}
	
	// add an element and increments the top index
	pt->items[++pt->top] = x;
}

// Utility function to return top element in a stack
int peek(struct stack *pt)
{
	// check for empty stack
	if (!isEmpty(pt))
		return pt->items[pt->top];
	else
		exit(EXIT_FAILURE);
}

// Utility function to pop top element from the stack
int pop(struct stack *pt)
{
	// check for stack underflow
	if (isEmpty(pt))
	{
		printf("UnderFlow\nProgram Terminated\n");
		exit(EXIT_FAILURE);
	}

	// decrement stack size by 1 and (optionally) return the popped element
	return pt->items[pt->top--];
}

// main function
int main(int argc, char **argv){

	if (argc < 1) {
		printf("No arguments...\n");
		exit(EXIT_FAILURE);
	} 



	// read a file
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int currentLine = 0;
	// stacks for checking
	struct stack *parenthesis = newStack(100);
	struct stack *brackets = newStack(100);
	struct stack *s_quotes = newStack(100);
	struct stack *d_quotes = newStack(100);
	// auxiliars flags
	bool sqFlag = false;
	bool dqFlag = false;
	bool errorFlag = false;
	bool firstCharacter = true;
	bool firstSlash = true;

	// read the file
	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		exit(EXIT_FAILURE);
	}

	while ((read = getline(&line, &len, fp)) != -1) {
        int i = 0;
		firstCharacter = true;
		firstSlash = true;
		while(line[i] != '\n' && i < len) {

			while(firstCharacter && line[i] == ' ') {
				i++;
			}
			firstCharacter = false;

			if (firstCharacter == false && line[i] == '/' && firstSlash) {
				firstSlash = false;
				if (line[i + 1] == '/') {
					break;
				} else if (line[i + 1] != '/') {
					errorFlag = true;
					getError('/', currentLine);
				}
				
			}

			if (line[i] == '(') {
                push(parenthesis, currentLine);
            } else if (line[i] == ')') {
                if (isEmpty(parenthesis)) {
					errorFlag = true;
					getError(')', currentLine);
                } else  {
                    pop(parenthesis);
                }
            } else if (line[i] == '{') {
                push(brackets, currentLine);
			} else if (line[i] == '}') {
				 if (isEmpty(brackets)) {
					errorFlag = true;
					getError('}', currentLine);
                } else  {
                    pop(brackets);
                }
			} else if (line[i] == '\'') {
				if (sqFlag) {
                    pop(s_quotes);
					sqFlag = false;
				} else {
                	push(s_quotes, currentLine);
					sqFlag = true;
				}
			} else if (line[i] == '\"') {
				if (dqFlag) {
                    pop(d_quotes);
					dqFlag = false;
				} else {
                	push(d_quotes, currentLine);
					dqFlag = true;
				}
			}
            i++;
        }
		currentLine++;
	}


	// emtpy the stacks
	getMissings(parenthesis, '(', errorFlag);
	getMissings(brackets, '{', errorFlag);
	getMissings(s_quotes, '\'', errorFlag);
	getMissings(d_quotes, '\"', errorFlag);

	if (errorFlag == false) {
		printf("There is no errors\n");
	}


    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);

	
	return 0;
}

void getMissings(struct stack *st, char ch, bool *error) {
	while(!isEmpty(st)) {
		error = true;
		printf("There is a missing %c in this line: %d\n", ch, pop(st) + 1);
	}
}

void getError(char missChar, int line) {
	printf("There is a missing %c in this line: %d\n", missChar, line + 1);
}