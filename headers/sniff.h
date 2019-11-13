#ifndef SNIFF_H
#define SNIFF_H

#include "structures.h"
#include <time.h>

void test();

size_t write_data(void *, size_t, size_t, FILE *);

char *actualTimeToString(struct tm **, const char *);

char *timeToString(struct tm *, const char *);

void insertContentInFile(const char *, const char *, const char *);

void runAllTasks(TaskList *tasksList);

#endif