#include <stdio.h>
#include <string.h>

/*
 * Assumptions:
 * Max number of words in a paragraph = 85
 * Max length of a word = 18
 */
#define MAX_NUM_WORDS 85


/*
 * Counts each letter in the parameter paragraph and
 * returns the total number of words in the paragraph.
 */
int totalWordCount(char* paragraph) {
    int totalWords = 0;
    size_t parSize = strlen(paragraph);

    // Loop goes through each letter in the array
    for (int letter = 0; letter < parSize; letter++) {

        // If there's a space, or it reaches the last letter, increment total words count
        if (paragraph[letter] == ' ' || (letter + 1) == parSize) {
            totalWords++;
        }
    }

    // If word count exceeds max limit
    if (totalWords > MAX_NUM_WORDS) {
        totalWords = MAX_NUM_WORDS;
    }

    return totalWords;
}


/*
 * Prints the number of spaces needed after a word.
 */
void printSpaces(int currWord, int spaces, int extraSpace) {

    // If it's the first word that just printed, print extra spaces after
    if (currWord == 1) {
        for (int extra = 0; extra < extraSpace; extra++) {
            printf(" ");
        }
    }

    // Print the number of spaces needed after the word
    for (int space = 0; space < spaces; space++) {
        printf(" ");
    }
}


/*
 * Formats and prints words and spaces equal to lineLength that should be printed on the next line.
 * Loops until all words and spaces in the line have printed.
 * Checks condition when index is a space (starting a new word) or the end of the line is reached.
 */
void printWordAndSpaces(char* paragraph, int lineLength, int numWords, int startPos, int spaces, int extraSpace) {
    char newArray[strlen(paragraph)];
    strcpy(newArray, paragraph);

    int index = startPos;
    int letterCount = 0;
    int wordsLeft = numWords;
    int currWord = 0;

    while (letterCount <= lineLength && wordsLeft > 0) {
        char currLetter = newArray[index];

        if (currLetter == ' ') {
            currWord++;
            printSpaces(currWord, spaces, extraSpace);
            wordsLeft--;
        } else if (currLetter == '\000') {
            wordsLeft--;
        } else {
            printf("%c", currLetter);
        }
        letterCount++;
        index++;
    }
    printf("\n");
}


/*
 * Calculates number of spaces between words and extra spaces needed on one line.
 */
void calculateSpaces(int* spaces, int* extraSpace, int totalSpaces, int spaceCount, int lineLength) {

    if (totalSpaces == 0 || spaceCount == 0) {
        // Only one word can fit on the line, so only one space is needed
        *spaces = 1;
        *extraSpace = 0;
    } else if (totalSpaces == lineLength) {
        // If there's only one word that's the length of the line, no spaces are needed
        *spaces = 0;
        *extraSpace = 0;
    } else {
        // Calculate spaces needed by dividing the total number of spaces by the
        // minimum number of spaces between each word
        *spaces = totalSpaces / spaceCount;

        // Calculate extra spaces needed by getting the remainder of the division result
        *extraSpace = totalSpaces % spaceCount;
    }
}


/*
 * Keeps a sum of how many letters can fit in the line.
 * Increments the word and position count in the loop.
 * Calculates the minimum number of spaces needed (number of words minus 1).
 * Calculates the current character count (total letters + space count).
 */
void incrementLetterSum(int* letterSum, int currWordLength, int* currWordPos, int* wordCount, int* currPos, int* spaceCount, int* charCount) {
    *letterSum += currWordLength;
    *currWordPos += currWordLength + 1;

    *wordCount = *(wordCount) + 1;
    *currPos = *(currPos) + 1;

    *spaceCount = *wordCount - 1;
    *charCount = *letterSum + *spaceCount;
}


/*
 * Called in calculateWordsPrinted() method.
 * Resets counter variables and sets the new starting position in the paragraph after a line is printed.
 */
void reset(int* startPos, const int* currWordPos, int* letterSum, int* charCount, int* wordCount) {
    *startPos = *currWordPos;
    *letterSum = 0;
    *charCount = 0;
    *wordCount = 0;
}


/*
 * Called in calculateWordsPrinted() method to check the condition when the current word being looped through
 * is the last word in the paragraph.
 */
void checkIfLastWord(int* currPos, const int* totalWords, int* charCount, const int* lineLength) {
    // If the current word is the last word, the if condition checking the line length will be false
    // so print paragraph method will be called
    if (*currPos == *totalWords) {
        *charCount = *lineLength;
        *currPos = *currPos + 1;
    }
}


/*
 * Calculates the number of words that need to be printed for each line.
 * Loop runs while the current number of words printed is <= the total number of words in the paragraph.
 * Else statement runs when the max number of words that can fit on the line have been added up.
 */
void calculateWordsPrinted(char* paragraph, const int wordLengths[], int totalWords, int lineLength) {
    char newArray[strlen(paragraph)];
    strcpy(newArray, paragraph);
    int wordCount = 0;
    int letterSum = 0;
    int startPos = 0;       // Starting letter position in the paragraph
    int currPos = 0;
    int currWordPos = 0;    // First letter of the current word's index
    int spaces;
    int extraSpace;
    int spaceCount = 0;     // Minimum spaces needed based on how many words can fit in the line
    int charCount = 0;      // Stores total characters in the line

    do {
        int currWordLength = wordLengths[currPos];
        checkIfLastWord(&currPos, &totalWords, &charCount, &lineLength);

        // If the current word and a space plus the current number of characters can fit on the line
        if ((currWordLength + charCount + 1) <= lineLength) {
            incrementLetterSum(&letterSum, currWordLength, &currWordPos, &wordCount, &currPos, &spaceCount, &charCount);
        } else {
            int totalSpaces = lineLength - letterSum;
            calculateSpaces(&spaces, &extraSpace, totalSpaces, spaceCount, lineLength);

            // If one word's length equals the length of the entire line
            if (currWordLength == lineLength && wordCount == 0) {
                currWordPos += currWordLength + 1;
                wordCount++;
                currPos++;
            }

            // Call method to print the formatted line
            printWordAndSpaces(newArray, lineLength, wordCount, startPos, spaces, extraSpace);
            // Move next starting position to position of the next word's first letter
            reset(&startPos, &currWordPos, &letterSum, &charCount, &wordCount);
        }
    } while (currPos <= totalWords);
}


/*
 * Goes through the paragraph array and counts how many letters are in each word.
 * Each word's letter count is stored in an integer array,
 * which is passed to a method for calculating word's that can fit on a line.
 */
void justifyAndPrintParagraph(char* paragraph, int lineLength) {
    char newArray[strlen(paragraph)];
    strcpy(newArray, paragraph);
    size_t parLength = strlen(paragraph);
    int letterCount = 0;    // Counts current number of letters that have been looped through
    int arrSize = totalWordCount(paragraph);
    int wordLengths[arrSize];   // Stores the total letter count of each word
    int lengthIndex = 0;    // Current index of the word lengths array

    // Loop through each letter in the paragraph array
    for (int i = 0; i < parLength; i++) {
        char currChar = newArray[i];

        if (currChar == ' ') {
            // If character is a space, assign the letter count total to the current index of the word lengths array
            wordLengths[lengthIndex] = letterCount;
            lengthIndex++;
            letterCount = 0;
        } else {
            // If character is a letter, increment letter counter
            letterCount++;
        }

        // If current character is the last character in the paragraph
        if ((i + 1) == parLength) {
            // Assign letter count to the current index of the word lengths array
            wordLengths[lengthIndex] = letterCount;
        }
    }
    // Call method to calculate the number of words that will be printed in each line
    calculateWordsPrinted(newArray, wordLengths, arrSize, lineLength);
}


int main() {
    // 3 Test Cases
    char testcase1[] = "Hi. I hope you have a great day!";
    printf("Line length: 30\n");
    justifyAndPrintParagraph(testcase1, 30);

    char testcase2[] = "I am here to learn C programming. This is my first assignment. Wish me luck!";
    printf("\nLine length: 50\n");
    justifyAndPrintParagraph(testcase2, 50);

    char testcase3[] = "Thank you for using my program. Enjoy the rest of your day!";
    printf("\nLine length: 25\n");
    justifyAndPrintParagraph(testcase3, 25);

    return 0;
}
