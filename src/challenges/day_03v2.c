// C standard headers
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// Local headers
#include <challenge.h>
#include <feed.h>

// TMP
#include <stdio.h>
#include <time.h>

#define NUMBER_DIGITS 2

const char delimiter = '\n';
const char *dataFileName = "./data/sample.txt";

static char *find_highest_digit(char *line, size_t length, bool first);

ErrorData evaluate(InputData *input, Answer *result) {
    *result = (Answer) { "Total output joltage", 0 };
    struct timespec start, end;

    LineFeed feed = create_linefeed(input);
    timespec_get(&start, TIME_UTC);
    for(char *line = get_linefeed(&feed); line != NULL; line = get_linefeed(&feed)) {
        printf("%s\n", line);
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
    timespec_get(&end, TIME_UTC);
    printf("sec: %ld, nsec: %ld\n", end.tv_sec - start.tv_sec, end.tv_nsec - start.tv_nsec);
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