// C standard headers
#include <stdlib.h>
// Local headers
#include <challenge.h>
#include <feed.h>

const char delimiter = ',';
const char *dataFileName = "./data/day_02.txt";

ErrorData evaluate(InputData *input, Answer *result) {
    LineFeed feed = create_linefeed(input);
    for(char *line = get_linefeed(&feed); line != NULL; line = get_linefeed(&feed)) {
        // PROCESS HERE
    }
    return emptySuccess;
}