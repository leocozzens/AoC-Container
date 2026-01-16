#ifndef LOADER_H
#define LOADER_H

#include <container.h>

ErrorData load_raw(const char *name, RawInput *raw_out);
ErrorData find_lines(RawInput *raw, LineGrid *grid_out);

#endif