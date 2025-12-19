// C standard headers
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
// Local headers
#include <challenge.h>
#include <feed.h>

const char delimiter = ',';
const char *dataFileName = "./data/day_02.txt";

#define TEXT_LEN 256

static bool get_range(const char *line, int64_t *start, int64_t *end);
static bool is_duplicate(const char *number, size_t length);

ErrorData evaluate(InputData *input, Answer *result) {
    *result = (Answer) { "Invalid ID's", 0 };

    LineFeed feed = create_linefeed(input);
    for(char *line = get_linefeed(&feed); line != NULL; line = get_linefeed(&feed)) {
        int64_t start, end;
        if(get_range(line, &start, &end)) continue;
        char numberText[TEXT_LEN];
        for(int64_t i = start; i <= end; i++) {
            snprintf(numberText, TEXT_LEN, "%lld", i);
            if(is_duplicate(numberText, strlen(numberText)))
                result->output += i;
        }
    }
    return emptySuccess;
}

static bool get_range(const char *line, int64_t *start, int64_t *end) {
    char *middle = strchr(line, '-');
    if(middle == NULL) return true;
    *start = atoll(line);
    *end = atoll(middle + 1);
    return false;
}

static bool is_duplicate(const char *numberText, size_t length) {
    if(length % 2 != 0) return false;
    size_t half = length / 2;
    for(size_t i = 0; i < half; i++) {
        if(numberText[i] != numberText[i + half])
            return false;
    }
    return true;
}