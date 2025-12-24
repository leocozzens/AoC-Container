// C standard headers
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
// Local headers
#include <challenge.h>
#include <feed.h>

const char delimiter = ',';
const char *dataFileName = "./data/day_02.txt";

#define TEXT_LEN 256

static bool get_range(const char *line, int64_t *start, int64_t *end);
static bool is_duplicate(const char *numberText, int length);

ErrorData evaluate(InputData *input, Answer *result) {
    *result = (Answer) { "Sum of invalid ID's", 0 };

    LineFeed feed = create_linefeed(input);
    for(char *line = get_linefeed(&feed); line != NULL; line = get_linefeed(&feed)) {
        int64_t start, end;
        if(get_range(line, &start, &end)) continue;
        char numberText[TEXT_LEN];
        for(int64_t i = start; i <= end; i++) {
            int status = snprintf(numberText, TEXT_LEN, "%" PRId64, i);
            if(status < 0)
                return CONSTRUCT_ERROR(
                    CHALLENGE_ENCODING_FAILURE,
                    "Failed to encode number to text conversion"
                );
            if(status >= TEXT_LEN)
                return CONSTRUCT_ERROR(
                    CHALLENGE_ENCODING_FAILURE,
                    "Converted number had length too great for buffer"
                );
            if(is_duplicate(numberText, status))
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

static bool is_duplicate(const char *numberText, int length) {
    if(length <= 1) return false;
    for(int pattern = 1; pattern <= length / 2; pattern++) {
        if(length % pattern != 0) continue;

        bool match = true;
        for(int i = pattern; i < length; i++) {
            if(numberText[i] != numberText[i % pattern]) {
                match = false;
                break;
            }
        }
        if(match) return true;
    }
    return false;
}