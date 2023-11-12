#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char** ArrayOfSpells;
int numberOfSpells;
int const sizeOfSpells = 150;

struct Word{
    char * word;
    struct Word * nextWord;
    bool used;
};

struct Graph{
    int numWords;
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

    return newWord;
}

void readSpells(){
    int i;


    FILE * fpointer;

    fpointer = fopen("spells.txt", "r");


    if(fpointer == NULL){
        printf("Could Not Open File! \n");
        numberOfSpells = 0;
        return;
    }




    if(fscanf(fpointer, "%d", &numberOfSpells) != 1){
        printf("Please make sure the file starts with an integer to denote the number of Spells.\n");
        numberOfSpells = 0;
        fclose(fpointer);
        return;
    }


    ArrayOfSpells = (char **)malloc(numberOfSpells * sizeof(char *));

    for(i=0; i<numberOfSpells; ++i){
        ArrayOfSpells[i] = (char* )malloc((sizeOfSpells+1) * sizeof(char));
        fscanf(fpointer, "%s", ArrayOfSpells[i]);
    }

    fclose(fpointer);
}

struct Graph * createGraph(int numWords){
    struct Graph * Graph = (struct Graph* ) malloc(sizeof(struct Graph));

    Graph->numWords = numWords;

    Graph->adjList = (struct Word**)malloc(sizeof(struct Word*) * numWords);

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
    for(i=0; i<Graph->numWords; ++i){
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
                }
            }
        }
    }
    return Graph;
}

void printGraph(struct Graph* Graph){
    int i;
    for(i=0; i<Graph->numWords; ++i){
        struct Word* current= Graph->adjList[i];
        while(current != NULL){
            printf("[%s]-> ", current->word);
            current = current->nextWord;
        }
        printf("\n");
    }
}
void freeGraph(struct Graph* Graph){
    int i;
    for(i=0; i<Graph->numWords; ++i){
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
    for(i=0; i<Graph->numWords; ++i){
        if(areEqual(choice, Graph->adjList[i]->word)){
            return i;
        }
    }
    return -1;
}
void updateGraph(struct Graph* Graph, char choice[]){
    int i;
    for(i=0; i<Graph->numWords; ++i){
        struct Word* current = Graph->adjList[i];
        if(areEqual(current->word, choice)){
            current->used = true;
        }
        if(condition(current->word, choice) && current->nextWord != NULL){
            current = current->nextWord;
            while(current != NULL){
                if(areEqual(current->word,choice)){
                    current->used = true;
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

// Easy Mode
int Kazdoora(struct Graph* Graph, bool myTurn, int lastCharCount[], int firstCharCount[]){
    char oppChoice[150];
    char myChoice[150];
    int i;
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
        // If it is update the counter arrays
        if(lastCharCount[lastCharOf(oppChoice) - 'a'] > 0) lastCharCount[lastCharOf(oppChoice) - 'a']--;
        if(firstCharCount[oppChoice[0] - 'a'] >0 ) firstCharCount[oppChoice[0] - 'a']--;


        struct Word* current = Graph->adjList[i];
        int j = 0;

        // If there is no where to go we lose
        if(current->nextWord == NULL){
            printf("Congrats you win \n");
            return 0;
        }
        else // We have a choice
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
        // Update the counter arrays
        if(lastCharCount[lastCharOf(oppChoice) - 'a'] > 0) lastCharCount[lastCharOf(oppChoice) - 'a']--;
        if(firstCharCount[oppChoice[0] - 'a'] >0 ) firstCharCount[oppChoice[0] - 'a']--;

        printf("My choice is: %s\n", myChoice);

        if(lastCharCount[lastCharOf(myChoice) - 'a'] ==0 && firstCharCount[lastCharOf(myChoice) - 'a'] != 0){
            printf("I win ha ha");
            return 1;
        }
        updateGraph(Graph, myChoice);
        updateGraph(Graph, oppChoice);
    }
    else{
        i = 0;
        while(Graph->adjList[Graph->numWords / 2]->word[i] != '\0'){
            myChoice[i] = Graph->adjList[Graph->numWords / 2]->word[i];
            ++i;
        }
        myChoice[i] = '\0';
        printf("My Choice: %s", myChoice);
        updateGraph(Graph, myChoice);

        if(lastCharCount[lastCharOf(oppChoice) - 'a'] > 0) lastCharCount[lastCharOf(oppChoice) - 'a']--;
        if(firstCharCount[oppChoice[0] - 'a'] >0 ) firstCharCount[oppChoice[0] - 'a']--;
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
            if(lastCharCount[lastCharOf(oppChoice) - 'a'] > 0) lastCharCount[lastCharOf(oppChoice) - 'a']--;
            if(firstCharCount[oppChoice[0] - 'a'] >0 ) firstCharCount[oppChoice[0] - 'a']--;

            printArray(firstCharCount,26);
            printArray(lastCharCount,26);
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
            if(current->nextWord == NULL){
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
            if(lastCharCount[lastCharOf(oppChoice) - 'a'] > 0) lastCharCount[lastCharOf(oppChoice) - 'a']--;
            if(firstCharCount[oppChoice[0] - 'a'] >0 ) firstCharCount[oppChoice[0] - 'a']--;

            printArray(firstCharCount,26);
            printArray(lastCharCount,26);
            printf("My choice is: %s\n", myChoice);

            if(lastCharCount[lastCharOf(myChoice) - 'a'] ==0 && firstCharCount[lastCharOf(myChoice) - 'a'] != 0){
                printf("I win ha ha");
                return 1;
            }
            updateGraph(Graph, myChoice);
            updateGraph(Graph, oppChoice);
        }   
}
void getCounterArrays(int lastCharcount[], int firstCharCount[]){
    int i;
    for(i=0; i<26; ++i){
        lastCharcount[i] = 0;
        firstCharCount[i] = 0;
    }
    for(i=0; i<numberOfSpells; ++i){
        lastCharcount[lastCharOf(ArrayOfSpells[i]) - 'a']++;
        firstCharCount[ArrayOfSpells[i][0] -'a']++;
    }
}
int main(){
    readSpells();
    struct Graph* Graph = buildGraph();
    printGraph(Graph);
    freeGraph(Graph);
}
