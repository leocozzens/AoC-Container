// C standard headers
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// Local headers
#include <challenge.h>
#include <feed.h>

#define NUMBER_DIGITS 2

const char delimiter = '\n';
const char *dataFileName = "./data/day_03.txt";

static char *find_highest_digit(char *line, size_t length, bool first);

ErrorData evaluate(InputData *input, Answer *result) {
    *result = (Answer) { "Total output joltage", 0 };

    LineFeed feed = create_linefeed(input);
    for(char *line = get_linefeed(&feed); line != NULL; line = get_linefeed(&feed)) {
        char numberText[NUMBER_DIGITS + 1];
        for(int i = 0; i < NUMBER_DIGITS; i++) {
            line = find_highest_digit(line, strlen(line), i == 0);
            if(line == NULL)
                return CONSTRUCT_ERROR(
                    CHALLENGE_ENCODING_FAILURE,
                    "Invalid input string"
                );
            numberText[i] = *line;
            if(!isdigit(*line))
                return CONSTRUCT_ERROR(
                    CHALLENGE_ENCODING_FAILURE,
                    "Invalid input string"
                );
            line++;
        }
        result->output += atoi(numberText);
    }
    return emptySuccess;
}

static char *find_highest_digit(char *line, size_t length, bool first) {
    if(length == 0) return NULL;
    if(first) length--;
    char *highest = line;
    if(length == 1)
        return highest;
    for(size_t i = 1; i < length; i++) {
        if(line[i] > *highest)
            highest = line + i;
    }
    return highest;
}