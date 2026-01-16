// C standard headers
#include <stdlib.h>
// Local headers
#include <challenge.h>
#include <feed.h>

EvalOptions opts = { 
    .delimiter = '\n',
    .dataFileName = "./data/day_01.txt",
    .benchmark = false
};

ErrorData evaluate(InputData *input, Answer *result) {
    *result = (Answer) { "Password", 0 };
    int position = 50;

    LineFeed feed = create_linefeed(input);
    for(char *line = get_linefeed(&feed); line != NULL; line = get_linefeed(&feed)) {
        int rotations;
        switch(*line) {
            case 'L':
                rotations = atoi(line + 1);
                position -= rotations;
                position = (position % 100 + 100) % 100;
                break;
            case 'R':
                rotations = atoi(line + 1);
                position += rotations;
                position %= 100;
                break;
            default:
                continue;
        }
        if(position == 0) result->output++;
    }
    return emptySuccess;
}