#ifndef CHALLENGE_H
#define CHALLENGE_H

#include <stdbool.h>
#include <container.h>

ErrorData evaluate(InputData *input, Answer *result);

typedef struct {
    char delimiter;
    const char *dataFileName;
    bool benchmark;
} EvalOptions;

extern EvalOptions opts;

#endif