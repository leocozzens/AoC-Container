// Standard headers
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
// Local headers
#include <challenge.h>
#include <loader.h>

static ErrorData load_data(FILE *inputFile, RawInput *raw);
static bool find_first_text(RawInput *raw, size_t *index);
static size_t count_lines(size_t index, RawInput *raw);
static void add_lines(size_t index, InputData *input);

ErrorData load_raw(RawInput *raw, const char *name) {
    FILE *inputFile = fopen(name, "rb");
    if(inputFile == NULL)
        return CONSTRUCT_ERROR(CHALLENGE_NO_READ, strerror(errno));
    ErrorData error = load_data(inputFile, raw);
    fclose(inputFile);
    return error;
}

ErrorData find_lines(InputData *input) {
    size_t first;
    if(find_first_text(&input->raw, &first))
        return CONSTRUCT_ERROR(
            CHALLENGE_NO_READ,
            "Failed to find any non-delimiter characters"
        );
    LineGrid newGrid;
    newGrid.height = count_lines(first, &input->raw);
    newGrid.lines = malloc(sizeof(char*) * newGrid.height);
    if(newGrid.lines == NULL)
        return CONSTRUCT_ERROR(CHALLENGE_NO_MEMORY, "Failed to allocate input data memory");
    input->grid = newGrid;
    add_lines(first, input);
    return emptySuccess;
}

static ErrorData load_data(FILE *inputFile, RawInput *raw) {
    int status = fseek(inputFile, 0, SEEK_END);
    if(status < 0)
        return CONSTRUCT_ERROR(CHALLENGE_NO_READ, strerror(errno));

    long fileSize = ftell(inputFile);
    if(fileSize < 0)
        return CONSTRUCT_ERROR(CHALLENGE_NO_READ, strerror(errno));
    if(fileSize == 0)
        return CONSTRUCT_ERROR(
            CHALLENGE_NO_READ, "Cannot read input file, because size is 0"
        );

    raw->size = (size_t) fileSize;
    status = fseek(inputFile, 0, SEEK_SET);
    if(status < 0)
        return CONSTRUCT_ERROR(CHALLENGE_NO_READ, strerror(errno));

    raw->data = malloc(raw->size + 1);
    if(raw->data == NULL)
        return CONSTRUCT_ERROR(CHALLENGE_NO_MEMORY, "Failed to allocate input data memory");
    size_t bytesRead = fread(raw->data, sizeof(char), raw->size, inputFile);
    if(bytesRead != raw->size)
        return CONSTRUCT_ERROR(CHALLENGE_NO_READ, "Failed to read entirety of input file");
    raw->data[raw->size] = '\0';
    return emptySuccess;
}
// TODO: Combine all 3 functions
static bool find_first_text(RawInput *raw, size_t *index) {
    for(size_t i = 0; i < raw->size - 1; i++) {
        if(raw->data[i] != delimiter) {
            if(delimiter == '\n' && raw->data[i] == '\r')
                continue;
            *index = i;
            return false;
        }
    }
    return true;
}

static size_t count_lines(size_t first, RawInput *raw) {
    size_t height = 0;
    bool separated = true;
    for(size_t index = first; index < raw->size; index++) {
        if(delimiter == '\n' && raw->data[index] == '\r') {
            raw->data[index] = '\0';
            continue;
        }
        if(raw->data[index] == delimiter) {
            separated = true;
            raw->data[index] = '\0';
            continue;
        }
        if(separated) {
            height++;
            separated = false;
        }
    }
    return height;
}

static void add_lines(size_t first, InputData *input) {
    bool separated = true;
    for(
        size_t index = first, gridPosition = 0;
        index < input->raw.size && gridPosition < input->grid.height;
        index++
    ) {
        if(input->raw.data[index] == '\0') {
            separated = true;
            continue;
        }
        if(separated) {
            input->grid.lines[gridPosition++] = input->raw.data + index;
            separated = false;
        }
    }
}