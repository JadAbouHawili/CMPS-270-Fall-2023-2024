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

void pop(struct StringStack* stack, char* result) {
    if (isStringStackEmpty(stack)) {
        printf("Stack underflow\n");
        return;
    }
    strcpy(result, stack->items[stack->top]);
    free(stack->items[stack->top]);
    stack->top--;
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
struct Word{
    char * word;
    struct Word * nextWord;
    bool used;
    int branchLength;
};
struct Graph{
    struct Word ** adjList;
};
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

struct Graph * createGraph(int numWords){
    struct Graph * Graph = (struct Graph* ) malloc(sizeof(struct Graph));

    Graph->adjList = (struct Word**)malloc(sizeof(struct Word*) * numberOfSpells);

    int i;
    for(i=0; i<numWords; ++i){
        Graph->adjList[i] = NULL;
    }

    return Graph;
}
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
char lastCharOf(char word[]){
    int i = 0;
    while(word[i] != '\0'){
        ++i;
    }
    return word[i-1];
}
bool condition(char word1[], char word2[]){  
    return lastCharOf(word1) == word2[0];
}
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
int isInAdjList(struct Graph* Graph, char choice[]){
    int i;
    for(i=0; i<numberOfSpells; ++i){
        if(areEqual(choice, Graph->adjList[i]->word)){
            return i;
        }
    }
    return -1;
}
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
void printArray(int A[], int size){
    int i;
    for(i=0; i<size; ++i){
        printf("%d ", A[i]);
    }
    printf("\n");
}
int getBranchLength(struct Graph* Graph, char Spell[]){
    int i;
    for(i=0; i<numberOfSpells; ++i){
        if(areEqual(Graph->adjList[i]->word, Spell)){
            return Graph->adjList[i]->branchLength;
        }
    }
    return -1;
}
void copyArray(char Spell1[], char Spell2[]){
    int j = 0;
    while(Spell1[j] != '\0'){
        Spell2[j] = Spell1[j];
        ++j;
    }
    Spell2[j] = '\0';
}
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
// Easy Mode
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
        IAmThinking(Graph,createWord(oppChoice),false,myChoice);

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
/*
int main(){
    if(readSpells()!=-1){
        struct Graph* Graph = buildGraph();
        printGraph(Graph);

            //randomize who starts first
        srand(time(NULL));
        bool myturn;
        if(rand() % 2 ==0) myturn=false;
        else myturn=true;

        Rehle(Graph, myturn);
        freeGraph(Graph);

    }
    
}
    */
