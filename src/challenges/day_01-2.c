// C standard headers
#include <stdlib.h>
#include <stdbool.h>
// Local headers
#include <challenge.h>
#include <feed.h>

const char *dataFileName = "./data/day_01.txt";

ErrorData evaluate(InputData *input, Answer *result) {
    *result = (Answer) { "Password", 0 };
    int position = 50;

    LineFeed feed = create_linefeed(input);
    for(char *line = get_linefeed(&feed); line != NULL; line = get_linefeed(&feed)) {
        int rotations, overflows = 0;
        switch(*line) {
            case 'L':
                rotations = atoi(line + 1);
                int start = position;
                position -= rotations;
                if(position <= 0) {
                    overflows = (-position / 100) + 1;
                    if(start == 0) overflows--;
                }
                position = (position % 100 + 100) % 100;
                break;
            case 'R':
                rotations = atoi(line + 1);
                position += rotations;
                overflows = position / 100;
                position %= 100;
                break;
            default:
                continue;
        }
        result->output += overflows;
    }
    return emptySuccess;
}