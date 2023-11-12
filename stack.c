#include <stdio.h>
#include <string.h>
#define MAX_SIZE 100 // Maximum size of the stack
#define MAX_STRING_LENGTH 50 // Maximum length of a string

struct StringStack {
    char items[MAX_SIZE][MAX_STRING_LENGTH];
    int top;
};

// Initialize the stack
void initializeStringStack(struct StringStack* stack) {
    stack->top = -1;
}

// Check if the stack is full
int isStringStackFull(struct StringStack* stack) {
    return stack->top == MAX_SIZE - 1;
}

// Check if the stack is empty
int isStringStackEmpty(struct StringStack* stack) {
    return stack->top == -1;
}

// Push a string onto the stack
void pushString(struct StringStack* stack, char* value) {
    if (isStringStackFull(stack)) {
        printf("String Stack is full, cannot push element.\n");
    } else {
        stack->top++;
        strcpy(stack->items[stack->top], value);
    }
}

// Pop a string from the stack
char* popString(struct StringStack* stack) {
    if (isStringStackEmpty(stack)) {
        printf("String Stack is empty, cannot pop element.\n");
        return NULL;
    } else {
        return stack->items[stack->top--];
    }
}

// Get the top string of the stack without removing it
char* peekString(struct StringStack* stack) {
    if (isStringStackEmpty(stack)) {
        printf("String Stack is empty.\n");
        return NULL;
    } else {
        return stack->items[stack->top];
    }
}

// Function to print the elements in the string stack
void printStringStack(struct StringStack* stack) {
    if (isStringStackEmpty(stack)) {
        printf("String Stack is empty.\n");
    } else {
        printf("String Stack elements: \n");
        for (int i = stack->top; i >= 0; i--) {
            printf("%s\n", stack->items[i]);
        }
    }
}

// Example usage
int main() {
    struct StringStack stringStack;
    initializeStringStack(&stringStack);

    pushString(&stringStack, "Hello");
    pushString(&stringStack, "Stack");
    pushString(&stringStack, "Implementation");

    printStringStack(&stringStack); // Print the stack elements

    printf("Popped element: %s\n", popString(&stringStack));
    printf("Top element after popping: %s\n", peekString(&stringStack));

    return 0;
}
