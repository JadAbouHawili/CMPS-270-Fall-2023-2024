#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

char** ArrayOfSpells;
int numberOfSpells;
int const sizeOfSpells = 150;
#define MAX_SIZE 1000

struct StringStack {
    char items[MAX_SIZE][150];
    int top;
};

// Initialize the stack
/*
 * @param: StringStack*
 * @return: void
 * requires: uninitialized StringStack
 * effects: initializes it, making the StringStack detectable as empty
 *
 */
void initializeStringStack(struct StringStack* stack) {
    stack->top = -1;
}

/*
 *@param: StringStack*
 *@return: int
 * requires: initialized StringStack
 * effects: check if its full or not(based on top)
 */
// Check if the stack is full
int isStringStackFull(struct StringStack* stack) {
    return stack->top == MAX_SIZE - 1;
}

/*
 * @param: StringStack*
 * @return: int
 * requires: initialized StringStack
 * effects: checks if its empty(based on top)
 */
// Check if the stack is empty
int isStringStackEmpty(struct StringStack* stack) {
    return stack->top == -1;
}

/*
 * @param: StringStack*, char* (in that order)
 * @return: void
 * requires: correct order of parameters, initalized StringStack*
 * effects: push a string, now the top of the stack is that string
 */
// Push a string onto the stack
void pushString(struct StringStack* stack, char* value) {
    if (isStringStackFull(stack)) {
        printf("String Stack is full, cannot push element.\n");
    } else {
        stack->top++;
        strcpy(stack->items[stack->top], value);
    }
}

/*
 * @param: StringStack*, char*
 * @return: void
 * required: corrected order of parameters, initalized StringStack*
 * effects: pop a string and put it in the passed char pointer, now the top of the stack is what was under the popped element
 */
void pop(struct StringStack* stack, char* result) {
    if (isStringStackEmpty(stack)) {
        printf("Stack underflow\n");
        return;
    }
    strcpy(result, stack->items[stack->top]);
    free(stack->items[stack->top]);
    stack->top--;
}

/*
 * @param: StringStack*
 * @return: char*
 * required: initalized StringStack*
 * effects: pop a string and return it, now the top of the stack is what was under the popped element 
 */
// Pop a string from the stack
char* popString(struct StringStack* stack) {
    if (isStringStackEmpty(stack)) {
        printf("String Stack is empty, cannot pop element.\n");
        return NULL;
    } else {
        return stack->items[stack->top--];
    }
}

/*
 * @param: StringStack*
 * @return: char*
 * required: initialized StringStack*
 * effects: return the top of the stack
 */
// Get the top string of the stack without removing it
char* peekString(struct StringStack* stack) {
    if (isStringStackEmpty(stack)) {
        printf("String Stack is empty.\n");
        return NULL;
    } else {
        return stack->items[stack->top];
    }
}

/*
 * @param: StringStack*
 * @return: void
 * required: initialized StringStack*
 * effects: print the content of the stack from top to bottom
 */
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
struct Word{
    char * word;
    struct Word * nextWord;
    bool used;
    int branchLength;
};
struct Graph{
    struct Word ** adjList;
};
/*
 * @param: char[]
 * @return: Word*
 * requires: char[] type
 * effects: creates Word* out of the pased word[]
 */
struct Word* createWord(char word[]){
    struct Word* newWord =  (struct Word*)malloc(sizeof(struct Word));

    int i = 0;
    while(word[i] != '\0'){
        ++i;
    }

    newWord->word = (char*) malloc(sizeof(char) * (i+1));

    i=0;
    
    while(word[i] != '\0'){
        newWord->word[i] = word[i];
        ++i;
    }
    newWord->word[i] = '\0';

    newWord->nextWord = NULL;
    newWord->used = false;
    newWord->branchLength = 0;

    return newWord;
}
/* @param: none
 * @return: int
 * requires: file spells.txt with the appropriate formatting specified in the project
 * effects: read spells and populate in arrayofspells
 *
 */
int readSpells(){
    int i;


    FILE * fpointer;

    fpointer = fopen("spells.txt", "r");


    if(fpointer == NULL){
        printf("Could Not Open File! \n");
        numberOfSpells = 0;
        return -1;
    }




    if(fscanf(fpointer, "%d", &numberOfSpells) != 1){
        printf("Please make sure the file starts with an integer to denote the number of Spells.\n");
        numberOfSpells = 0;
        fclose(fpointer);
        return -1;
    }


    ArrayOfSpells = (char **)malloc(numberOfSpells * sizeof(char *));

    for(i=0; i<numberOfSpells; ++i){
        ArrayOfSpells[i] = (char* )malloc((sizeOfSpells+1) * sizeof(char));
        fscanf(fpointer, "%s", ArrayOfSpells[i]);
    }

    fclose(fpointer);
    return 1;
}
/* @param: int
 * @return: Graph*
 * requires: positive integer
 * effects: returns a Graph*, pointer to a Graph
 *
 */
struct Graph * createGraph(int numWords){
    struct Graph * Graph = (struct Graph* ) malloc(sizeof(struct Graph));

    Graph->adjList = (struct Word**)malloc(sizeof(struct Word*) * numberOfSpells);

    int i;
    for(i=0; i<numWords; ++i){
        Graph->adjList[i] = NULL;
    }

    return Graph;
}
/* @param: char[], char[]
 * @return: bool
 * requires: two arrays of words not pointers
 * effects: returning a boolean of whether they are equal or not
 *
 */
bool areEqual(char word1[], char word2[]){
    int size1 = 0;
    int size2 = 0;
    while(word1[size1] != '\0'){
        ++size1;
    }
    while(word2[size2] != '\0'){
        ++size2;
    }
    if(size1 != size2){
        return false;
    }

    int i = 0;
    while(word1[i] != '\0'){
        if(word1[i] != word2[i]){
            return false;
        }
        ++i;
    }

    return true;
}
/* @param: char[]
 * @return: char
 * requires: nonempty array of characters
 * effects: returns the last character
 *
 */
char lastCharOf(char word[]){
    int i = 0;
    while(word[i] != '\0'){
        ++i;
    }
    return word[i-1];
}
/* @param: char[], char[]
 * @return: bool
 * requires: two non empty arrays of char
 * effect: if the last character of the first parameter is equal to the first character of the 2nd
 *
 */
bool condition(char word1[], char word2[]){  
    return lastCharOf(word1) == word2[0];
}
/* @param: Graph*, char[], char[]
 * @return: void
 * requires: nonempty char arrays, nonempty graph
 * effects: adds an edge, i.e an entry from the first char[] to the second in the adjacency list
 *
 */
void addEdge(struct Graph * Graph, char source[], char destination[]){
    struct Word* newWord = createWord(destination);
    int i;
    for(i=0; i<numberOfSpells; ++i){
        struct Word* current = Graph->adjList[i];
        if(areEqual(source, current->word)){
            while(current->nextWord != NULL){
                    current = current->nextWord;
            }
            current->nextWord = newWord;
            break;
        }
    }
}
/* @param: none
 * @return: Graph*
 * requires: global variables are defined : numberOfSpells, sizeOfSpells. arrayofspells have been populated by readspells method
 * effects: builds the graph of possibilities for the game, based on rules of the game 
 *
 */
struct Graph* buildGraph(){
    struct Graph* Graph = createGraph(numberOfSpells);
    char currentWord[sizeOfSpells];

    int i,j,k;
    for(i=0; i<numberOfSpells; ++i){
        Graph->adjList[i] = createWord(ArrayOfSpells[i]);
    }

    for(i=0; i<numberOfSpells; ++i){
        for(j=0; j<numberOfSpells; ++j){
            if(i != j){
                if(condition(ArrayOfSpells[i], ArrayOfSpells[j])){
                    addEdge(Graph, ArrayOfSpells[i], ArrayOfSpells[j]);
                    Graph->adjList[i]->branchLength++;
                }
            }
        }
    }
    return Graph;
}
/* @param: Graph*
 * @return: void
 * requires: populated graph
 * effects: prints every word and the possibilities that might be chosen from that word
 *
 */
void printGraph(struct Graph* Graph){
    int i;
    for(i=0; i<numberOfSpells; ++i){
        struct Word* current= Graph->adjList[i];
        while(current != NULL){
            if(current->used){
                printf("[%s][%d](used)-> ", current->word, current->branchLength);
                current = current->nextWord;
                
            }
            else{
                printf("[%s][%d](unused)-> ", current->word, current->branchLength);
                current = current->nextWord;

            }
        }
        printf("\n\n");
    }
}
/* @param: Graph*
 * @return: void
 * requires: populated graph
 * effect: frees the allocated memory
 *
 */
void freeGraph(struct Graph* Graph){
    int i;
    for(i=0; i<numberOfSpells; ++i){
        struct Word* current = Graph->adjList[i];
        struct Word* previous = NULL;
        if(current->nextWord != NULL){
            current = current->nextWord;
            while(current != NULL){
                previous = current;
                current = current->nextWord;
                free(previous);
            }
        }
    }
    free(Graph->adjList);
    free(Graph);
}
/* @param: Graph*, char[]
 * @return: int
 * requires: populated graph, non empty char[]
 * effects: -1 if not in adjacency list, returns its index otherwise
 *
 */
int isInAdjList(struct Graph* Graph, char choice[]){
    int i;
    for(i=0; i<numberOfSpells; ++i){
        if(areEqual(choice, Graph->adjList[i]->word)){
            return i;
        }
    }
    return -1;
}
/* @param: Graph*, char[]
 * @return: void
 * requires: populated graph, non empty char[]
 * effects: when a word is used, mark it as used by changing the used attribute. update branch length accordingly
 *
 */
void updateGraph(struct Graph* Graph, char choice[]){
    int i;
    for(i=0; i<numberOfSpells; ++i){
        struct Word* current = Graph->adjList[i];
        if(areEqual(current->word, choice)){
            current->used = true;
            current->branchLength--;
        }
        if(condition(current->word, choice) && current->branchLength != 0){
            current = current->nextWord;
            while(current != NULL){
                if(areEqual(current->word,choice)){
                    current->used = true;
                    current->branchLength--;
                    break;
                }
                current = current->nextWord;
            }
        }
    }
}
/* @param: int[], int
 * @return: void
 * requires: nonempty array, positive size
 * effects: prints the array
 *
 */
void printArray(int A[], int size){
    int i;
    for(i=0; i<size; ++i){
        printf("%d ", A[i]);
    }
    printf("\n");
}
/* @param: Graph*, char[]
 * @return: int
 * requires: non empty graph, nonempty char[]
 * effects: gets the length of the branch where Spell[] is located
 *
 */
int getBranchLength(struct Graph* Graph, char Spell[]){
    int i;
    for(i=0; i<numberOfSpells; ++i){
        if(areEqual(Graph->adjList[i]->word, Spell)){
            return Graph->adjList[i]->branchLength;
        }
    }
    return -1;
}
/* @param: char[], char[]
 * @return: void
 * requires: nonempty char arrays
 * effects: copies the first to the second
 *
 */
void copyArray(char Spell1[], char Spell2[]){
    int j = 0;
    while(Spell1[j] != '\0'){
        Spell2[j] = Spell1[j];
        ++j;
    }
    Spell2[j] = '\0';
}
/*
 * @param: Graph*
 * @return: void
 * requires: populated graph
 * effects: sets all used flags for nodes in the graph to false
 */
void setFlagsToFalse(struct Graph* Graph){
    int i;
    for(i=0; i<numberOfSpells; ++i){
        struct Word* current = Graph->adjList[i];
        while(current != NULL){
            current->used = false;
            current = current->nextWord;
        }
    }
}
/* @param: Graph*, Word*
 * @return: Word*
 * requires: populated graph, non-null Word*
 * effects: returns whether there is a possibility in the graph after our current position, i.e if we can keep playing. if not return null
 *
 */
struct Word* hasNextPossibility(struct Graph* Graph, struct Word* Word){
    struct Word* current = Graph->adjList[(isInAdjList(Graph, Word->word))];
    if(current->branchLength == 0) {
        return NULL;
    }
    else {
        current = current->nextWord;
        while(current != NULL){
            if(!current->used){
                return current;
            }
            current = current->nextWord;
        }
        return NULL;
    }
}
/* @param: Graph*, Word*, bool, char[]
 * @return: void
 * requires: populated graph, nonempty word, nonempty char[]
 * effect: goes through possibilities on a stack, and determines whether just a chain of possibilities gives a win or not, and stores that choice in myChoice to be used later
 *
 */

void IAmThinking(struct Graph* Graph, struct Word* Word, bool win, char myChoice[]){
    struct StringStack possibilities;
    initializeStringStack(&possibilities);

    bool initialState = win;

    struct Word* current = Graph->adjList[isInAdjList(Graph, Word->word)];
    current->used = true;
    struct Word* possGetter = NULL;
    while(current != NULL){
        possGetter = hasNextPossibility(Graph,current);
        possGetter->used = true;
        while(possGetter != NULL){
            pushString(&possibilities, possGetter->word);
            possGetter->used = true;
            win = !(win);
            possGetter = hasNextPossibility(Graph, possGetter);
        }
        if(win){
            char temp[150];
            while(!isStringStackEmpty(&possibilities)){
                pop(&possibilities, myChoice);
            }
            copyArray(temp, myChoice);
            return;
        }
        setFlagsToFalse(Graph);
        win = initialState;
        current = current->nextWord;
    }
}
/* @param: Graph*, bool
 * @return: int
 * reqires: populated graph
 * effect: checks what valid choices there are, and picks one without much thought or analysis
 *
 */
int Kazdoora(struct Graph* Graph, bool myTurn){

    char oppChoice[150];
    char myChoice[150];
    int i;
    bool win;
    if(!myTurn){
        printf("Enter your choice: ");
        scanf("%s", oppChoice);

        // Check if the openents choice is in the list
        i = isInAdjList(Graph, oppChoice);
        if(i == -1){
            printf("Your choice is not in the list. \n");
            printf("I win :)");
            return 1;
        }
        struct Word* current = Graph->adjList[i];
        int j = 0;

        // If there is no where to go we lose
        if(current->branchLength == 0){
            printf("Congrats you win \n");
            return 0;
        }
        else // We may have a choice
        {
            current = current->nextWord;
            while(current!= NULL){
                if(!current->used){
                    break;
                }
                else current = current->nextWord;
            }
            if(current == NULL){
                printf("Congrats you win \n");
                return 0;
            }
        }
        while(current->word[j] != '\0'){
            myChoice[j] = current->word[j];
            ++j;
        }
        myChoice[j] = '\0';

        printf("My choice is: %s\n", myChoice);

        for(i=0; i<numberOfSpells; ++i){
            if(condition(myChoice, ArrayOfSpells[i]) && Graph->adjList[i]->branchLength != 0){
                win = false;
                break;
            }
            else win = true;
        }
        if(win){
            printf("I win, you ran out of options! Ha Ha");
            return 1;
        }
        updateGraph(Graph, myChoice);
        updateGraph(Graph, oppChoice);
    }
    else{
        i = 0;
        while(Graph->adjList[numberOfSpells/ 2]->word[i] != '\0'){
            myChoice[i] = Graph->adjList[numberOfSpells / 2]->word[i];
            ++i;
        }
        myChoice[i] = '\0';
        printf("My Choice: %s", myChoice);
        updateGraph(Graph, myChoice);
    }
    while(true){
            printf("Enter your choice: ");
            scanf("%s", oppChoice);
            i = isInAdjList(Graph, oppChoice);
            if(i == -1){
                printf("Your choice is not int he list. \n");
                printf("I win :)");
                return 1;
            }
            struct Word* current = Graph->adjList[i];
            if(current->used){
                printf("You chose a previously chosen world. \n");
                printf("I win :)");
                return 1;
            }
            if(!condition(myChoice, oppChoice)){
                printf("You didn't choose a word whose first character is the last character of my previous choice \n");
                printf("Oops I win \n");
                return 1;
            }
            int j = 0;
            if(current->branchLength == 0){
                printf("Congrats you win \n");
                return 0;
            }
            else {
                current = current->nextWord;
                while(current!= NULL){
                    if(!current->used){
                        break;
                    }
                    else current = current->nextWord;
                }
                if(current == NULL){
                    printf("Congrats you win \n");
                    return 0;
                }
            }
            while(current->word[j] != '\0'){
                myChoice[j] = current->word[j];
                ++j;
            }
            myChoice[j] = '\0';

            printf("My choice is: %s\n", myChoice);

            for(i=0; i<numberOfSpells; ++i){
            if(condition(myChoice, ArrayOfSpells[i]) && Graph->adjList[i]->branchLength != 0){
                win = false;
                break;
            }
            else win = true;
            }
            if(win){
                printf("I win, you ran out of options! Ha Ha");
                return 1;
            }

            updateGraph(Graph, myChoice);
            updateGraph(Graph, oppChoice);
        }   
}

// Medium Mode
/* @param: Graph*, bool
 * @return: int
 * requires: populated graph
 * effect: 
 *
 */
int Rehle(struct Graph* Graph, bool myTurn){
    char oppChoice[150];
    char myChoice[150];
    int i;
    bool win;
    struct Graph* myCopy = buildGraph();
    if(!myTurn){
        printf("Enter your choice: ");
        scanf("%s", oppChoice);

        // Check if the openents choice is in the list
        i = isInAdjList(Graph, oppChoice);
        if(i == -1){
            printf("Your choice is not in the list. \n");
            printf("I win :)");
            return 1;
        }


        struct Word* current = Graph->adjList[i];

        // If there is no where to go we lose
        if(current->branchLength == 0){
            printf("Congrats you win \n");
            return 0;
        }
        else // We may have a choice
        {
            current = current->nextWord;
            while(current!= NULL){
                if(!current->used){
                    break;
                }
                else current = current->nextWord;
            }
            if(current == NULL){
                printf("Congrats you win \n");
                return 0;
            }
        }
        myChoice[0] = '\0';
        IAmThinking(myCopy,createWord(oppChoice),false,myChoice);

        if(myChoice[0] == '\0'){
            int maxBranchSize = -1;
            struct Word* current = Graph->adjList[i]->nextWord;
            struct Word* temp = NULL;
            while(current != NULL){
                if(current->branchLength >= maxBranchSize){
                    maxBranchSize = current->branchLength;
                    temp = current;
                }
            }
            copyArray(temp->word, myChoice);
        }

        printf("My Choice: %s\n", myChoice);


        for(i=0; i<numberOfSpells; ++i){
            if(condition(myChoice, ArrayOfSpells[i]) && Graph->adjList[i]->branchLength != 0){
                win = false;
                break;
            }
            else win = true;
        }
        if(win){
            printf("I win, you ran out of options! Ha Ha");
            return 1;
        }
        updateGraph(Graph, myChoice);
        updateGraph(Graph, oppChoice);
    }
    else{
        int j;
        int maxBranchSize = -__INT_MAX__;
        for(i=0; i<numberOfSpells; ++i){
            int current = Graph->adjList[i]->branchLength;
            if(current >= maxBranchSize){
                maxBranchSize = current;
                j=i;
            }
        }

         myChoice[0] = '\0';
        IAmThinking(Graph,createWord(oppChoice),true,myChoice);

        if(myChoice[0] == '\0'){
            int maxBranchSize = -1;
            struct Word* current = Graph->adjList[i]->nextWord;
            struct Word* temp = NULL;
            while(current != NULL){
                if(current->branchLength >= maxBranchSize){
                    maxBranchSize = current->branchLength;
                    temp = current;
                }
            }
            copyArray(temp->word, myChoice);
        }



        printf("My choice: %s", myChoice);
        
        for(i=0; i<numberOfSpells; ++i){
            if(condition(myChoice, ArrayOfSpells[i]) && Graph->adjList[i]->branchLength != 0){
                win = false;
                break;
            }
            else win = true;
        }
        if(win){
            printf("I win, you ran out of options! Ha Ha");
            return 1;
        }
        updateGraph(Graph, myChoice);
    }
    while(true){
            printf("Enter your choice: ");
            scanf("%s", oppChoice);
            i = isInAdjList(Graph, oppChoice);
            if(i == -1){
                printf("Your choice is not int he list. \n");
                printf("I win :)");
                return 1;
            }
            struct Word* current = Graph->adjList[i];
            if(current->used){
                printf("You chose a previously chosen world. \n");
                printf("I win :)");
                return 1;
            }
            if(!condition(myChoice, oppChoice)){
                printf("You didn't choose a word whose first character is the last character of my previous choice \n");
                printf("Oops I win \n");
                return 1;
            }
            if(current->branchLength == 0){
                printf("Congrats you win \n");
                return 0;
            }

            current = Graph->adjList[i];
            struct Word* temp1 = NULL;

            // If there is no where to go we lose
            if(current->branchLength == 0){
                printf("Congrats you win \n");
                return 0;
            }
            else // We may have a choice
            {
                current = current->nextWord;
                while(current!= NULL){
                    if(!current->used){
                        temp1 = current;
                        break;
                    }
                    else current = current->nextWord;
                }
                if(current == NULL){
                    printf("Congrats you win \n");
                    return 0;
                }
            }
             myChoice[0] = '\0';
            IAmThinking(Graph,oppChoice,false,myChoice);

            if(myChoice[0] == '\0'){
                int maxBranchSize = -1;
                struct Word* current = Graph->adjList[i]->nextWord;
                struct Word* temp = NULL;
                while(current != NULL){
                    if(current->branchLength >= maxBranchSize){
                        maxBranchSize = current->branchLength;
                        temp = current;
                    }
                }
                copyArray(temp->word, myChoice);
            }


            printf("My choice: %s", myChoice);

            for(i=0; i<numberOfSpells; ++i){
                if(condition(myChoice, ArrayOfSpells[i]) && Graph->adjList[i]->branchLength != 0){
                    win = false;
                    break;
                }
                else win = true;
                }
            if(win){
                printf("I win, you ran out of options! Ha Ha");
                return 1;
            }

            updateGraph(Graph, myChoice);
            updateGraph(Graph, oppChoice);
        }   
}

/*
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
*/

int main(){
    if(readSpells()!=-1){
        struct Graph* Graph = buildGraph();
        printGraph(Graph);

        /*
            //randomize who starts first
        srand(time(NULL));
        bool myturn;
        if(rand() % 2 ==0) myturn=false;
        else myturn=true;
        */


        Rehle(Graph, true);
        freeGraph(Graph);

    }
    
}
    
#define MAX_SIZE 1000
#define MAX_STRING_SIZE 150

// Define a stack structure for string arrays
typedef struct {
    char data[MAX_SIZE][MAX_STRING_SIZE];
    int top;
} StringArrayStack;

// Function to initialize the string array stack
void initialize(StringArrayStack* stack) {
    stack->top = -1;
}

// Function to check if the string array stack is empty
bool isEmpty(StringArrayStack* stack) {
    return stack->top == -1;
}

// Function to check if the string array stack is full
bool isFull(StringArrayStack* stack) {
    return stack->top == MAX_SIZE - 1;
}

// Function to push a string array onto the stack
void push(StringArrayStack* stack, const char str[]) {
    if (isFull(stack)) {
        printf("Stack overflow\n");
        return;
    }

    stack->top++;
    strncpy(stack->data[stack->top], str, MAX_STRING_SIZE - 1);
    stack->data[stack->top][MAX_STRING_SIZE - 1] = '\0'; // Ensure null-termination
}

void pop(StringArrayStack* stack, char result[]) {
    if (isEmpty(stack)) {
        printf("Stack underflow\n");
        return;
    }

    strncpy(result, stack->data[stack->top], MAX_STRING_SIZE);
    stack->top--;
}






void IAmThinking(struct Graph* Graph, char oppChoice[], char myChoice[], bool win){
    StringArrayStack possibilites;
    initialize(&possibilites);
    bool initialState = win;
    int i = isInAdjList(Graph,oppChoice);
    struct Word* current = Graph->adjList[i];
    updateGraph(Graph, current->word);
    int j;
    for(j=0; j<current->branchLength; ++j){
        struct Word* possGetter = hasNextPossibility(Graph, current);
        push(&possibilites, possGetter->word);
        win = !win;
        updateGraph(Graph, possGetter->word);
        while(possGetter != NULL){
            push(&possibilites, possGetter->word);
            printf("%s", possGetter->word);
            win = !win;
            possGetter = hasNextPossibility(Graph, possGetter->nextWord);
        }
        if(win){
            while(!isEmpty(&possibilites)){
                pop(&possibilites, myChoice);
                break;
            }
        }
        setFlagsToFalse(Graph);
        win = initialState;
        current = current->nextWord;
    }
}
