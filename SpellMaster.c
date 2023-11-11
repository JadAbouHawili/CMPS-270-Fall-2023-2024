

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Global Variables
char **ArrayOfSpells; 
int numberOfSpells;
int const sizeOfSpells = 150;
bool startGame = true;

void readSpells(){
    int i;


    FILE * fpointer;

    fpointer = fopen("spells.txt", "r");


    if(fpointer == NULL){
        printf("Could Not Open File! \n");
        numberOfSpells = 0;
        startGame = false;
        return;
    }




    if(fscanf(fpointer, "%d", &numberOfSpells) != 1){
        printf("Please make sure the file starts with an integer to denote the number of Spells.\n");
        numberOfSpells = 0;
        startGame = false;
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
void printArray(){
    int i;
    int j;
    i=0;
    printf("The Following is the list of Spells you can choose from: \n");
    for(i=0; i<numberOfSpells; i = i+5){
        printf("[");
        for(j=i; j<5+i && j<numberOfSpells; ++j){
            if(j<4+i && j+1 < numberOfSpells) printf("\'%s\' ,", ArrayOfSpells[j]);
            else printf("\'%s\'", ArrayOfSpells[j]);
        }
        printf("]\n");
    }
}

void freeArrayOfSpells(){
    int i;

    for(i=0; i<numberOfSpells; ++i){
        free(ArrayOfSpells[i]);
    }
    free(ArrayOfSpells);
}
char lastCharOfSpell(char choice[]){
    int i = 0;
    char lastChar;
    while(choice[i] != '\0'){
        lastChar = choice[i];
        ++i;
    }
    return lastChar;
}

bool isValid(char choice[], int usedWords[]){
    int i;
    int j;
    bool valid;
    for(i=0; i<numberOfSpells; ++i){
        j=0;
        valid = true;
        while(choice[j] != '\0'){
            if(ArrayOfSpells[i][j] != choice[j]){
                valid = false;
                break;
            }
            ++j;
        }
        if(valid){
            ++usedWords[i];
            return true;
        }
    }
    return false;
}
bool repeatedSpells(int usedWords[]){
    int i;

    for(i=0; i<numberOfSpells; ++i){
        if(usedWords[i] == 2) return true;
    }
    return false;
}
bool spellExhaustion(char choice[], int usedWords[]){
    char lastChar = lastCharOfSpell(choice);
    int i;

    for(i=0; i<numberOfSpells; ++i){
        if(usedWords[i] == 0 && ArrayOfSpells[i][0] == lastChar){
            return false;
        }
    }
    return true;
}

bool testGameConditions(bool p1Turn, char p1Name[], char p2Name[], char choice[], int usedWords[], char lastCharOfPrevious, char firstCharOfCurrent){
        if(!isValid(choice,usedWords)){
            if(p1Turn) printf("%s wins because %s chose an invalid spell.", p2Name, p1Name);
            else printf("%s wins because %s chose an invalid spell.", p1Name, p2Name);
            return false;
        }
        if(repeatedSpells(usedWords)){
            if(p1Turn) printf("%s wins because %s chose a previously chosen spell!", p2Name, p1Name);
            else printf("%s wins because %s chose a previously chosen spell!", p1Name, p2Name);
            return false;
        }
        if(firstCharOfCurrent != lastCharOfPrevious){
            if(p1Turn) printf("%s wins because %s has chosen a forbidden spell." , p2Name, p1Name);
            else printf("%s wins because %s has chosen a forbidden spell" ,p1Name, p2Name);
            return false;
        }
        if(spellExhaustion(choice,usedWords)){
            if(p1Turn) printf("%s wins because %s has ran out of spells to cast!", p1Name, p2Name);
            else printf("%s wins because %s has ran out of spells to cast!", p2Name, p1Name);
            return false;
        }

        return true;
}
int determineWhoStarts(){
    srand(time(NULL));
    return rand() % 2;
}
char* botChooseSpell(int usedWords[]){
    int randomIndex;
    /*
    do {
        randomIndex=rand() % numberOfSpells;
    }
    while(usedWords[randomIndex]>=2);
    */
    //usedWords[randomIndex]++;

    randomIndex=rand() % numberOfSpells;
    return ArrayOfSpells[randomIndex];

}

void playGame(int turn, char p1Name[], char p2Name []){
    int i;
    bool p1Turn;
    if(turn == 1) p1Turn = true;
    else p1Turn = false;

    if(p1Turn) printf("%s, will start.\n", p1Name);
    else printf("%s, will start.\n", p2Name);


    char choice[150];

    int usedWords[numberOfSpells];
    for(i=0; i<numberOfSpells; ++i){
        usedWords[i] = 0;
    }

    char lastCharOfPrevious;
    char firstCharOfCurrent;


    if(p1Turn){
         printf("%s, choose a spell: ", p1Name);
         scanf("%s", choice);
    }
    else{
        printf("Bot is choosing");
        strcpy(choice, botChooseSpell(usedWords)); // Implement the bot's spell selection logic here.
        printf("(Bot) chose: %s\n",choice);

    }

    

    if(!isValid(choice,usedWords)){
            if(p1Turn) printf("%s wins because %s chose an invalid spell.", p2Name, p1Name);
            else printf("%s wins because %s chose an invalid spell.", p1Name, p2Name);
            return;
        }
    if(spellExhaustion(choice,usedWords)){
            if(p1Turn) printf("%s wins because %s has ran out of spells to cast!", p1Name, p2Name);
            else printf("%s wins because %s has ran out of spells to cast!", p2Name, p1Name);
            return;
        }
    lastCharOfPrevious = lastCharOfSpell(choice);
    p1Turn = !p1Turn;

    
/*
    while(true){
        if(p1Turn) printf("%s, choose a spell: ", p1Name);
        else printf("%s, choose a spell: ", p2Name);

        scanf("%s", choice);

        firstCharOfCurrent = choice[0];

        if(!testGameConditions(p1Turn,p1Name,p2Name,choice, usedWords, lastCharOfPrevious, firstCharOfCurrent)){
            break;
        }

        lastCharOfPrevious = lastCharOfSpell(choice);
        p1Turn = !p1Turn;
    }
    */

    while (true) {
        if (p1Turn) {
            printf("%s, choose a spell: ", p1Name);
            scanf("%s", choice);
        } else {
            printf("%s (Bot) is choosing a spell...\n");
            strcpy(choice, botChooseSpell(usedWords)); // Implement the bot's spell selection logic here.
            printf("(Bot) chose: %s\n",choice);
        }
        firstCharOfCurrent = choice[0];

        if (!testGameConditions(p1Turn, p1Name, p2Name, choice, usedWords, lastCharOfPrevious, firstCharOfCurrent)) {
            break;
        }
        lastCharOfPrevious = lastCharOfSpell(choice);
        p1Turn = !p1Turn;
    }


}

int main(){
    // 1. The method readSpells reads the input from a file and puts them in a 2D array. It assumes the first line of the file is an integer.
    // Since the number of rows of the Matrix "ArrayOfSpells" is determined during running time we allocate memory for it accordinly.
    // 2. The method printArray prints the array as required from the instructions.
    // 3. The method freeArrayOfSpells frees the allocated memory after the game is finished.
    // 4. The method playGame is where most of the action is happening.
    // 5. The method isValid checks if the input of the user makes sense, for instance if the user inputs "Fourier" but this word does not
    // exist in the ArraysOfSpells it returns false. When the user of the input is valid, it marks the row of that spell in a counter array.
    // 6. The method repeatedSpells checks if one of the users has chose a previously chosen  spell. It does that by checking the counter array.
    // 7. The method spellExhaustion checks if one of the users has ran out of choices, again it makes use of the counter array.
    // 8. The method testGameConditions makes use of previous three methods to determine whether or not the game should continue.
    // it returns true if the game should proceed or false otherwise and it declares why the game has stopped and who is the winner.
    // 9. The method last char of Spell returns the last character of a 1D array.
    // 10. The method determineWhoStarts generates random numbers.
    printf("Welcome to the Spell Master Game! \n");

    char p1Name [50];
    char p2Name [50];

    printf("Player 1 enter your name: ");
    scanf("%s", &p1Name);

    printf("Player 2 enter your name: ");
    scanf("%s", &p2Name);

    readSpells();

    if(startGame){
         int turn = determineWhoStarts();
         playGame(turn, p1Name, p2Name);
    }
    else printf("Failed to Start Game.");
    freeArrayOfSpells();
    return 0;
}