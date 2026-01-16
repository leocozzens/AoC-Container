// Standard headers
#include <stdio.h>
#include <time.h>
#include <inttypes.h>
// Local headers
#include <challenge.h>

ErrorData ext_bench_eval(InputData *input, Answer *result) {
    struct timespec start, end;
    timespec_get(&start, TIME_UTC);
    ErrorData error = evaluate(input, result);
    timespec_get(&end, TIME_UTC);
    double msDiff =
    (double) (end.tv_sec - start.tv_sec) * 1e3 +
    (double) (end.tv_nsec - start.tv_nsec) / 1e6;

    printf("[benchmark] eval: %.3f ms\n", msDiff);
    return error;
}