// C standard headers
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// Local headers
#include <challenge.h>
#include <feed.h>

#define NUMBER_DIGITS 2

EvalOptions opts = { 
    .delimiter = '\n',
    .dataFileName = "./data/day_03.txt",
    .benchmark = false
};

static char *find_highest_digit(char *line, size_t length, bool first);

ErrorData evaluate(InputData *input, Answer *result) {
    *result = (Answer) { "Total output joltage", 0 };

    LineFeed feed = create_linefeed(input);
    for(char *line = get_linefeed(&feed); line != NULL; line = get_linefeed(&feed)) {
        char numberText[NUMBER_DIGITS + 1];
        numberText[NUMBER_DIGITS] = '\0';
        char *data = line;
        size_t length = strlen(line);
        for(int i = 0; i < NUMBER_DIGITS; i++) {
            data = find_highest_digit(data, length - (data - line), i == 0);
            if(data == NULL)
                return CONSTRUCT_ERROR(
                    CHALLENGE_ENCODING_FAILURE,
                    "Invalid input string, length is 0"
                );
            numberText[i] = *data;
            if(!isdigit(*data))
                return CONSTRUCT_ERROR(
                    CHALLENGE_ENCODING_FAILURE,
                    "Invalid input string, non-digit character"
                );
            data++;
        }
        result->output += atoi(numberText);
        result->output += (numberText[0] - '0') * 10 + numberText[1] - '0';
    }
    return emptySuccess;
}

static char *find_highest_digit(char *line, size_t length, bool first) {
    if(length == 0) return NULL;
    if(first) length--;
    char *highest = line;
    for(size_t i = 1; i < length; i++) {
        if(line[i] > *highest)
            highest = line + i;
    }
    return highest;
}