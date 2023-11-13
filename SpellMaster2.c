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
/*
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
*/
/* @param: Graph*, Word*
 * @return: Word*
 * requires: populated graph, non-null Word*
 * effects: returns whether there is a possibility in the graph after our current position, i.e if we can keep playing. if not return null
 *
 */

/*
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
*/
/* @param: Graph*, Word*, bool, char[]
 * @return: void
 * requires: populated graph, nonempty word, nonempty char[]
 * effect: goes through possibilities on a stack, and determines whether just a chain of possibilities gives a win or not, and stores that choice in myChoice to be used later
 *
 */

/*
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
*/
/* @param: Graph*, bool
 * @return: int
 * reqires: populated graph
 * effect: checks what valid choices there are, and picks one without much thought or analysis
 *
 */
int easy(struct Graph* Graph, bool myTurn){

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
        printf("My Choice: %s\n", myChoice);
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

*/
/* @param: Graph*, bool
 * @return: int
 * reqires: populated graph
 * effect: checks what valid choices there are, and picks one through doing more analysis on the branches
 *
 */
int medium(struct Graph* Graph, bool myTurn){

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

        int branchLength = current->bracnhLength;

        // If there is no where to go we lose
        if(current->bracnhLength == 0){
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
        int smallestBracnh = __INT_MAX__;
        struct Word* temp = NULL;
        current = Graph->adjList[i];
        current = current->nextWord;
        for(i=0; i<branchLength; ++i){
            if(current->bracnhLength < smallestBracnh && !current->used){
                smallestBracnh = branchLength;
                temp = current;
            }
            current = current->nextWord;
        }

        copyArray(temp->word, myChoice);

        for(i=0; i<numberOfSpells; ++i){
            if(condition(myChoice, ArrayOfSpells[i]) && Graph->adjList[i]->bracnhLength != 0){
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
        int smallestBranch = __INT_MAX__;
        struct Word* temp = NULL;
        for(i=0; i<numberOfSpells; ++i){
            struct Word* current = Graph->adjList[i];
            for(j=0; j<current->bracnhLength; ++j){
                if(current->bracnhLength < smallestBranch && !current->used){
                smallestBranch = current->bracnhLength;
                temp = current;
                current = current->nextWord;
            }
        }

        copyArray(temp->word, myChoice);

        printf("My Choice: %s", myChoice);

        for(i=0; i<numberOfSpells; ++i){
            if(condition(myChoice, ArrayOfSpells[i]) && Graph->adjList[i]->bracnhLength != 0){
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
            int j = 0;
            int branchLength = current->bracnhLength;
            if(branchLength == 0){
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

            int smallestBracnh = __INT_MAX__;
            struct Word* temp = NULL;
            current = Graph->adjList[i];
            current = current->nextWord;
            for(i=0; i<branchLength; ++i){
                if(current->bracnhLength < smallestBracnh && !current->used){
                    smallestBracnh = branchLength;
                    temp = current;
                }
                current = current->nextWord;
            }

        copyArray(temp->word, myChoice);

            for(i=0; i<numberOfSpells; ++i){
            if(condition(myChoice, ArrayOfSpells[i]) && Graph->adjList[i]->bracnhLength != 0){
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
}

*/
/* @param: Graph*, bool
 * @return: int
 * reqires: populated graph
 * effect: checks what valid choices there are, and picks one through doing stronger analysis on the branches
 *
 */
int hard(struct Graph* Graph, bool myTurn){

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

        int branchLength = current->bracnhLength;

        // If there is no where to go we lose
        if(current->bracnhLength == 0){
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
        int smallestBracnh = __INT_MAX__;
        struct Word* temp = NULL;
        current = Graph->adjList[i];
        current = current->nextWord;
        for(i=0; i<branchLength; ++i){
            if(current->bracnhLength < smallestBracnh && !current->used){
                smallestBracnh = branchLength;
                temp = current;
            }
            current = current->nextWord;
        }

        copyArray(temp->word, myChoice);

        for(i=0; i<numberOfSpells; ++i){
            if(condition(myChoice, ArrayOfSpells[i]) && Graph->adjList[i]->bracnhLength != 0){
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
        bool found = false;
        for(i=0; i<numberOfSpells; ++i){
            struct Word* current = Graph->adjList[i];
            if(current->bracnhLength == 0){
                copyArray(current->word, myChoice);
                found = true;
                break;
            }
        }
        if(!found){
            int j;
            int smallestBranch = __INT_MAX__;
            struct Word* temp = NULL;
            for(i=0; i<numberOfSpells; ++i){
                struct Word* current = Graph->adjList[i];
                for(j=0; j<current->bracnhLength; ++j){
                    if(current->bracnhLength < smallestBranch && !current->used){
                    smallestBranch = current->bracnhLength;
                    temp = current;
                    current = current->nextWord;
                }
            }

        copyArray(temp->word, myChoice);
        }

        printf("My Choice: %s", myChoice);

        for(i=0; i<numberOfSpells; ++i){
            if(condition(myChoice, ArrayOfSpells[i]) && Graph->adjList[i]->bracnhLength != 0){
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
            int j = 0;
            int branchLength = current->bracnhLength;
            if(branchLength == 0){
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

            int smallestBracnh = __INT_MAX__;
            struct Word* temp = NULL;
            current = Graph->adjList[i];
            current = current->nextWord;
            for(i=0; i<branchLength; ++i){
                if(current->bracnhLength < smallestBracnh && !current->used){
                    smallestBracnh = branchLength;
                    temp = current;
                }
                current = current->nextWord;
            }

        copyArray(temp->word, myChoice);

            for(i=0; i<numberOfSpells; ++i){
            if(condition(myChoice, ArrayOfSpells[i]) && Graph->adjList[i]->bracnhLength != 0){
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
        


        easy(Graph, myturn);
        freeGraph(Graph);

    }
    
}

