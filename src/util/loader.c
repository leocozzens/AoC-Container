// Standard headers
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
// Local headers
#include <challenge.h>
#include <loader.h>

static bool find_first_text(char *data, size_t length, size_t *index);

ErrorData load_raw(RawInput *raw, const char *name) {
    FILE *inputFile = fopen(name, "rb");
    if(inputFile == NULL)
        return CONSTRUCT_ERROR(CHALLENGE_NO_READ, strerror(errno));

    int status;
    status = fseek(inputFile, 0, SEEK_END);
    if(status < 0)
        return CONSTRUCT_ERROR(CHALLENGE_NO_READ, strerror(errno));

    status = ftell(inputFile);
    if(status < 0)
        return CONSTRUCT_ERROR(CHALLENGE_NO_READ, strerror(errno));
    if(status == 0)
        return CONSTRUCT_ERROR(
            CHALLENGE_NO_READ, "Cannot read input file, because size is 0"
        );

    raw->size = (size_t) status;
    status = fseek(inputFile, 0, SEEK_SET);
    if(status < 0)
        return CONSTRUCT_ERROR(CHALLENGE_NO_READ, strerror(errno));

    raw->data = malloc(raw->size + 1);
    if(raw->data == NULL)
        return CONSTRUCT_ERROR(CHALLENGE_NO_MEMORY, "Failed to allocate input data memory");
    size_t bytesRead = fread(raw->data, sizeof(char), raw->size, inputFile);
    if(bytesRead != raw->size)
        return CONSTRUCT_ERROR(CHALLENGE_NO_READ, "Failed to read entirety of input file");
    fclose(inputFile);
    raw->data[raw->size] = '\0';
    return emptySuccess;
}

ErrorData find_lines(InputData *input) {
    size_t index;
    if(find_first_text(input->raw.data, input->raw.size, &index))
        return CONSTRUCT_ERROR(
            CHALLENGE_NO_READ,
            "Failed to find any non-delimiter characters"
        );
    for(size_t index = 1; index < input->raw.size - 1; index++) {

        if(delimiter == '\n' && input->raw.data[index] == '\r') {
            input->raw.data[index] = '\0';
            continue;
        }
        if(input->raw.data[index] == delimiter) {
            input->raw.data[index] = '\0';
            input->grid.height++;
            while(input->raw.data[index + 1] == delimiter) {
                index++;
                input->raw.data[index] = '\0';
            }
        }
    }

    input->grid.lines = malloc(sizeof(char*) * input->grid.height);
    if(input->grid.lines == NULL)
        return CONSTRUCT_ERROR(CHALLENGE_NO_MEMORY, "Failed to allocate input data memory");
    size_t gridPosition = 0;
    input->grid.lines[gridPosition++] = (input->raw.data == delimiter) ? input;
    for(
        size_t index = 0;
        index < input->raw.size - 1 && gridPosition < input->grid.height;
        index++
    ) {
        if(input->raw.data[index] == '\0') {
            input->grid.lines[gridPosition++] = input->raw.data + index + 1;
            while(input->raw.data[index + 1] == delimiter)
                index++;
        }
    }
    return emptySuccess;
}

static bool find_first_text(char *data, size_t length, size_t *index) {
    for(size_t i = 0; i < length; i++) {
        if(data[i] != '\0') {
            *index = i;
            return false;
        }
    }
    return true;
}