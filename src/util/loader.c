// Standard headers
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
// Local headers
#include <challenge.h>
#include <loader.h>

#define INITIAL_LINES_CAPACITY 256
static_assert(INITIAL_LINES_CAPACITY > 0, "Initial lines capacity cannot be 0 or less");

static ErrorData load_data(FILE *inputFile, RawInput *raw_out);
static ErrorData realloc_lines(LineGrid *grid, size_t newCapacity);

ErrorData load_raw(const char *name, RawInput *raw_out) {
    FILE *inputFile = fopen(name, "rb");
    if(inputFile == NULL)
        return CONSTRUCT_ERROR(CHALLENGE_NO_READ, strerror(errno));
    ErrorData error = load_data(inputFile, raw_out);
    fclose(inputFile);
    return error;
}

ErrorData find_lines(RawInput *raw, LineGrid *grid_out) {
    size_t capacity = INITIAL_LINES_CAPACITY;
    LineGrid newGrid = { malloc(sizeof(char*) * capacity ), 0 };
    if(newGrid.lines == NULL)
        return CONSTRUCT_ERROR(CHALLENGE_NO_MEMORY, "Failed to allocate input data memory");

    bool separated = true;
    for(size_t index = 0; index < raw->size; index++) {
        if(
            (opts.delimiter == '\n' && raw->data[index] == '\r') ||
            raw->data[index] == opts.delimiter
        ) {
            separated = true;
            raw->data[index] = '\0';
            continue;
        }
        if(separated) {
            separated = false;
            while(newGrid.height >= capacity) {
                capacity *= 2;
                ErrorData result = realloc_lines(&newGrid, capacity);
                if(IS_FAILURE(result)) return result;
            }
            newGrid.lines[newGrid.height++] = raw->data + index;
        }
    }
    if(newGrid.height == 0) {
        free(newGrid.lines);
        return CONSTRUCT_ERROR(
            CHALLENGE_NO_READ,
            "Failed to find any non-delimiter characters"
        );
    }
    ErrorData result = realloc_lines(&newGrid, newGrid.height);
    if(IS_FAILURE(result)) return result;

    *grid_out = newGrid;
    return emptySuccess;
}

static ErrorData load_data(FILE *inputFile, RawInput *raw_out) {
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

    RawInput newRaw;
    newRaw.size = (size_t) fileSize;
    status = fseek(inputFile, 0, SEEK_SET);
    if(status < 0)
        return CONSTRUCT_ERROR(CHALLENGE_NO_READ, strerror(errno));

    newRaw.data = malloc(newRaw.size + 1);
    if(newRaw.data == NULL)
        return CONSTRUCT_ERROR(CHALLENGE_NO_MEMORY, "Failed to allocate input data memory");
    size_t bytesRead = fread(newRaw.data, sizeof(char), newRaw.size, inputFile);
    if(bytesRead != newRaw.size)
        return CONSTRUCT_ERROR(CHALLENGE_NO_READ, "Failed to read entirety of input file");
    newRaw.data[newRaw.size] = '\0';

    *raw_out = newRaw;
    return emptySuccess;
}

static ErrorData realloc_lines(LineGrid *grid, size_t newCapacity) {
    char **oldLines = grid->lines;
    grid->lines = realloc(grid->lines, sizeof(char*) * newCapacity);
    if(grid->lines == NULL) {
        free(oldLines);
        return CONSTRUCT_ERROR(
            CHALLENGE_NO_MEMORY,
            "Failed to allocate input data memory"
        );
    }
    return emptySuccess;
}