#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


// Global Constants
char** ArrayOfSpells;
int numberOfSpells;
int const sizeOfSpells = 150;


struct Word{
    char * word;
    struct Word * nextWord;
    bool used;
    int branchLength;
};
struct Graph{
    struct Word ** adjList;
};
// @param: Char Array
// Requires: A non-empty character array.
// Effects: Constructs a new word structurs. The word attribute of the of the structure is initialized to the inputed array.
//          The nextWord pointer attribute is initialized to NULL. The used boolean is initialized to false. 
//          The length of the branch is initialized to 0. The newly constructed Word structure is returned.

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
// @param: Nothing.
// Requires: Nothing.
// Effects: Loops through the file and stores each word in the 2D array of characters, allocating memory as needed. Assumes the file begins
//          with the number of words. Returns 1 if the file is found, -1 otherwise.
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
// @param: Nothing.
// Requires: The existence of createWord and an execution of readSpells.
// Effects: Constructs a new Graph object. Each word structure in the 2D array of Words is initialized to NULL, allocating memory as needed.
//          The number of rows in the  adjaceny list is exactly the number of spells. Returns a newly constructed Graph Structure.

struct Graph * createGraph(){
    struct Graph * Graph = (struct Graph* ) malloc(sizeof(struct Graph));

    Graph->adjList = (struct Word**)malloc(sizeof(struct Word*) * numberOfSpells);

    int i;
    for(i=0; i<numberOfSpells; ++i){
        Graph->adjList[i] = NULL;
    }

    return Graph;
}
// @param: Char Array 1, Char Array2
// Requires: Two non-empty character arrays.
// Effects: Returns true if the two arrays are equal, false otherwise.
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
// @param: Char Array
// Requires: A non-empty Char Array
// Effects: Returns the last character in the given array.
char lastCharOf(char word[]){
    int i = 0;
    while(word[i] != '\0'){
        ++i;
    }
    return word[i-1];
}
// @param: Char Array1, Char Array2
// Requires: Two non-empty Char Arrays and the existence of lastCharOf method.
// Effects: Returns true if the main game condition is satisfied false otherwise. The game conidition being that the last char of word1
//          is the first character of word2
bool condition(char word1[], char word2[]){  
    return lastCharOf(word1) == word2[0];
}
// @param: Graph Structure, Char Array source, Char Array destination
// Requries: A Graph structure, and two non-empty char arrays. In addition, it requires the existence of createWor, the source is already
//           in the adjaceny list, the areEqual method exists, and the game condition is satsified so of course it assumes the condition method
//           exists.
// Effects: Loops through the adjacency list and locates the source. If the source is pointing at null then the source points to the destination
//          otherwise the destination is added at the end of the linked list at source.
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
// @param: Nothing.
// Requires: The existence of createGraph, createWord, addEdge, and condition method.
// Effects: Creates a graph using the createGraph method. Loops through the array of spells and creates a word structure for each word
//          using the createWord method, then it makes each pointer in the adjacenly list points at its corresponding created word.
//          Loops through the array of spells and for each word it checks if there is another word that satififies the game conditions
//          with it. If there is, it adds an edge between them. Returns a new manipulated Graph.
struct Graph* buildGraph(){
    struct Graph* Graph = createGraph();

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
// @param: Graph structure.
// Requires: A non-empty Graph structure.
// Effects: Prints the Graph in the form of adjacencyList and pointers.
void printGraph(struct Graph* Graph){
    int i;
    for(i=0; i<numberOfSpells; ++i){
        struct Word* current= Graph->adjList[i];
        while(current != NULL){
            printf("[%s][%d]-> ", current->word, current->branchLength);
            current = current->nextWord;
        }
        printf("\n");
    }
}
// @param: Graph structure
// Requires: A non-empty Graph structure
// Effects: Traverses the Graph freeing the allocated memory for each word structure. Then it frees the memory allocated for the adjacency
//          list and for the Graph itself. Nothing is returned.
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
// @param: Graph Structure
// Requires: A non-empty Graph structure and a non-empty char array and the existence of areEqual method.
// Effects: Traverses the adjacency list to check if the word is in the adjacney list. If it is then the index at which the word is located 
//          is found. Otherwise -1 is returned.
int isInAdjList(struct Graph* Graph, char choice[]){
    int i;
    for(i=0; i<numberOfSpells; ++i){
        if(areEqual(choice, Graph->adjList[i]->word)){
            return i;
        }
    }
    return -1;
}
// @param: Graph structure, char array
// Requires: A non-empty Graph structure and a non empty char array. The existence of areEqual and condition method.
// EFfects: Makes sure that after each choice is made my the algorithm or the player the word is marked as used. Traverses the Graph and checks
//          and locates the source and marks its used attribute as being used. It also decreases branch lengths as needed. Nothing is returned.
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
// @param: Two non-empty char arrays.
// Requires: Two non-char arrays.
// Effects: Coppies the characters in Spell1 to those in Spell2. Nothing is returned.
void copyArray(char Spell1[], char Spell2[]){
    int j = 0;
    while(Spell1[j] != '\0'){
        Spell2[j] = Spell1[j];
        ++j;
    }
    Spell2[j] = '\0';
}
// @param: Graph structure and boolean to indicate turns
// Requires: A non-empty Graph structure and a boolean
// Effects: Prompts the user for input or chooses a word form the graph depending on the boolean. Analysis is made to choose the words at 
//          each turn. After each choice the update Graph method is called and the conditions of the game are evaluated. If the algorithm
//          wins it will indicate so, if the player wins is will also indicate that. Returns 1 if the algorithm wins, -1 otherwise.
int LetsPlay(struct Graph* Graph, bool myTurn){
    char oppChoice[150];
    char myChoice[150];
    int i;
    int BranchLength;
    int smallestBranch;
    int longestBranch;
    bool win;
    if(!myTurn){
        printf("Enter your choice: ");
        scanf("%s", oppChoice);

        ////////////////////////////////////////////////////
        i = isInAdjList(Graph, oppChoice);
        if(i == -1){
            printf("Your choice is not in the list. \n");
            printf("I win :)");
            return 1;
        }
        ////////////////////////////////////////////////////


        
        struct Word* current = Graph->adjList[i];
        BranchLength = current->branchLength;

        ////////////////////////////////////////////////////
        if(BranchLength == 0){
            printf("Congrats you win \n");
            return 0;
        }
        else
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
        /////////////////////////////////////////////////////

        longestBranch = -1;
        int j;
        int currentBranch;
        current = Graph->adjList[i]->nextWord;
        struct Word* temp = NULL;
        for(j=0; j<BranchLength; ++j){
            currentBranch = current->branchLength;
            if(currentBranch >= longestBranch){
                longestBranch = currentBranch;
                temp = current;
            }
            current = current->nextWord;
        }

        copyArray(temp->word, myChoice);
        printf("My choice is: %s\n", myChoice);


        ///////////////////////////////////////////////////////
        bool gameCondition;
        bool used;
        for(j=0; j<numberOfSpells; ++j){
            gameCondition = condition(myChoice, ArrayOfSpells[j]);
            used = Graph->adjList[j]->used;
            if(gameCondition && !used){
                win = false;
                break;
            }
            else win = true;
        }
        if(win){
            printf("I win, you ran out of options! Ha Ha");
            return 1;
        }
        ////////////////////////////////////////////////////////

        updateGraph(Graph, myChoice);
        updateGraph(Graph, oppChoice);
    }
    else{
        bool found = false;
        for(i=0; i<numberOfSpells; ++i){
            struct Word* current = Graph->adjList[i];
            if(current->branchLength == 0){
                copyArray(current->word, myChoice);
                found = true;
                break;
            }
        }
        if(found){
            printf("My Choice: %s\n", myChoice);
        }
        else{
            longestBranch = -1;
            struct Word* temp1 = NULL;
            int currentLength;
            for(i=0; i<numberOfSpells; ++i){
                struct Word* current = Graph->adjList[i];
                currentLength = current->branchLength;
                if(currentLength >= longestBranch){
                    longestBranch = currentLength;
                    temp1 = current;
                }
            }
            copyArray(temp1->nextWord->word, myChoice);
            printf("My Choice: %s\n", myChoice);
        }
        /////////////////////////////////////////////////////////////
        bool gameCondition;
        bool used;
        for(i=0; i<numberOfSpells; ++i){
            gameCondition = condition(myChoice, ArrayOfSpells[i]);
            used = Graph->adjList[i]->used;
            if(gameCondition && !used){
                win = false;
                break;
            }
            else win = true;
        }
        if(win){
            printf("I win, you ran out of options! Ha Ha");
            return 1;
        }
        //////////////////////////////////////////////////////////////
        updateGraph(Graph, myChoice);
    }
    while(true){
            printf("Enter your choice: ");
            scanf("%s", oppChoice);
            i = isInAdjList(Graph, oppChoice);

            /////////////////////////////////////////////////
            if(i == -1){
                printf("Your choice is not int he list. \n");
                printf("I win :)");
                return 1;
            }
            /////////////////////////////////////////////////

            struct Word* current = Graph->adjList[i];
            BranchLength = current->branchLength;
            if(current->used){
                printf("You chose a previously chosen world. \n");
                printf("I win :)");
                return 1;
            }
            ///////////////////////////////////////////////////
            if(!condition(myChoice, oppChoice)){
                printf("You didn't choose a word whose first character is the last character of my previous choice \n");
                printf("Oops I win \n");
                return 1;
            }
            ////////////////////////////////////////////////////
            if( BranchLength == 0){
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
            bool found = false;
            for(i=0; i<numberOfSpells; ++i){
            struct Word* current = Graph->adjList[i];
                if(current->branchLength == 0 && condition(oppChoice, current->word)){
                    copyArray(current->word, myChoice);
                    found = true;
                    break;
                }
            }
            if(found){
                printf("My choice is: %s \n", myChoice);
            }
            else{
                longestBranch = -1;
                int j;
                int currentBranch;
                // current = Graph->adjList[i]->nextWord;
                struct Word* temp = NULL;
                for(j=0; j<BranchLength; ++j){
                    currentBranch = current->branchLength;
                    if(currentBranch >= longestBranch){
                        longestBranch = currentBranch;
                        temp = current;
                    }
                    current = current->nextWord;
                }
                copyArray(temp->word, myChoice);
                printf("My choice is: %s\n", myChoice);
            }   
            ///////////////////////////////////////////////////////////
            bool gameCondition;
            bool used;
            for(i=0; i<numberOfSpells; ++i){
                gameCondition = condition(myChoice, ArrayOfSpells[i]);
                used = Graph->adjList[i]->used;
                if(gameCondition && !used){
                    win = false;
                    break;
                }
                else win = true;
            }
            if(win){
                printf("I win, you ran out of options! Ha Ha");
                return 1;
            }
            ///////////////////////////////////////////////////////////
            updateGraph(Graph, myChoice);
            updateGraph(Graph, oppChoice);
        }   
}

int main(){
    readSpells();
    struct Graph* Graph = buildGraph();
    LetsPlay(Graph, false);
    freeGraph(Graph);
}
