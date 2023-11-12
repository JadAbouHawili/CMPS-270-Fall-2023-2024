#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

char** ArrayOfSpells;
int numberOfSpells;
int const sizeOfSpells = 150;
#define MAX_SIZE 1000

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

// Define a stack structure for strings (char pointers)
typedef struct {
    char* data[MAX_SIZE];
    int top;
} StringStack;

// Function to initialize the string stack
void initialize(StringStack* stack) {
    stack->top = -1;
}

// Function to check if the string stack is empty
bool isEmpty(StringStack* stack) {
    return stack->top == -1;
}

// Function to check if the string stack is full
bool isFull(StringStack* stack) {
    return stack->top == MAX_SIZE - 1;
}

// Function to push a string onto the string stack
void push(StringStack* stack, const char* str) {
    if (isFull(stack)) {
        printf("Stack overflow\n");
        return;
    }
    stack->top++;
    stack->data[stack->top] = strdup(str); // Duplicate the string
    if (stack->data[stack->top] == NULL) {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
}

// Function to pop a string from the string stack
void pop(StringStack* stack, char* result) {
    if (isEmpty(stack)) {
        printf("Stack underflow\n");
        return;
    }
    strcpy(result, stack->data[stack->top]);
    free(stack->data[stack->top]);
    stack->top--;
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
    int smallestBranch = __INT_MAX__;
    int currentBranchSize;
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
        current = Graph->adjList[i];
        struct Word* temp2 = NULL;
        current = current->nextWord;
        while(current != NULL){
            if(!current->used){
                currentBranchSize = getBranchLength(Graph, current->word);
                if(currentBranchSize != 0 && currentBranchSize < smallestBranch && currentBranchSize % 2 == 0){
                    smallestBranch = currentBranchSize;
                    temp2 = current;
                }
                current = current->nextWord;
            }

        }
        if(temp2 != NULL){
            copyArray(temp2->word, myChoice);
            temp2 = NULL;
        }
        else{
            copyArray(temp1->word, myChoice);
            temp1 = NULL;
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
        struct Word* current = Graph->adjList[0];
        struct Word* temp = NULL;   
        current = current->nextWord;
        while(current != NULL){
            currentBranchSize = getBranchLength(Graph, current->word);
                if(currentBranchSize != 0 && currentBranchSize < smallestBranch && currentBranchSize % 2 == 1){
                    smallestBranch = currentBranchSize;
                    temp = current;
                }
                current = current->nextWord;
            }
            if(temp != NULL){
                copyArray(temp->word, myChoice);
            }
            else{
                copyArray(Graph->adjList[numberOfSpells/2]->word,myChoice);
            }        
        printf("My choice: %s\n", myChoice);
        
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
            current = Graph->adjList[i];
            struct Word* temp2 = NULL;
            current = current->nextWord;
            while(current != NULL){
                if(!current->used){
                    currentBranchSize = getBranchLength(Graph, current->word);
                    if(currentBranchSize != 0 && currentBranchSize < smallestBranch && currentBranchSize % 2 == 0){
                        smallestBranch = currentBranchSize;
                        temp2 = current;
                    }
                current = current->nextWord;
            }
        }
        if(temp2 != NULL){
            copyArray(temp2->word, myChoice);
            temp2 = NULL;
        }
        else{
            copyArray(temp1->word, myChoice);
            temp1 = NULL;
        }

        printf("My choice: %s\n", myChoice);

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
