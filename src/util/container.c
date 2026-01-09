// Standard headers
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
// Local headers
#include <loader.h>
#include <challenge.h>

static ErrorData load_container(InputData *input, Answer *result);
static void clean_container(InputData *input);
static void print_challenge_result(Answer *result);
static void print_challenge_failure(ErrorData *error, const char *region);

ErrorData emptySuccess = { CHALLENGE_SUCCESS, NULL };
ErrorData emptyFailure = { CHALLENGE_FAILURE, NULL };
static InputData emptyInput = { { NULL, 0 }, { NULL, 0 } };
static Answer emptyResult = { "NO TYPE", 0 };

int main(void) {
    InputData input;
    Answer result;
    ErrorData error;
    error = load_container(&input, &result);
    if(IS_FAILURE(error))
        print_challenge_failure(&error, "CONTAINER");
    else {
        error = evaluate(&input, &result);
        if(IS_SUCCESS(error))
            print_challenge_result(&result);
        else
            print_challenge_failure(&error, "CHALLENGE");
    }
    clean_container(&input);
    return error.code;
}

static ErrorData load_container(InputData *input, Answer *result) {
    *input = emptyInput;
    *result = emptyResult;
    ErrorData error;
    error = load_raw(&input->raw, dataFileName);
    if(IS_SUCCESS(error))
        error = find_lines(input);
    return error;
}

static void clean_container(InputData *input) {
    free(input->grid.lines);        
    free(input->raw.data);
    *input = emptyInput;
}

static void print_challenge_result(Answer *result) {
    printf("%s: %" PRId64 "\n", result->name, result->output);
}

static void print_challenge_failure(ErrorData *error, const char *region) {
    printf(
        "%s ERROR #%d: %s.\n",
        region,
        error->code,
        (error->info == NULL) ? "No error information" : error->info
    );
}