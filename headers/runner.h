#ifndef RUNNER_H
#define RUNNER_H

#include "parser.h"
#include <time.h>

typedef struct http_response_t HttpResponse;
typedef struct config_t Config;

struct config_t
{
    int max_depth;
    int versioning;
    char **mimes;
    char *url;
    size_t timer;
    char *main_folder;
    char *task_name;
    char *action_name;
};

struct buffer_t
{
    char *bytes;
    size_t len;
    size_t capacity;
};

struct http_response_t
{
    int status;
    char *content_type;
    char *buffer;
};

size_t get_timer_of_task(Task *);

char **parse_urls(char *, unsigned int *);

HttpResponse http_get(const char *, char **);

void create_all_files_from_url(Config);

size_t write_data(void *, size_t, size_t, FILE *);

char *actualTimeToString(struct tm **, const char *);

char *timeToString(struct tm *, const char *);

void insertContentInFile(const char *, const char *, const char *);

void runAllTasks(TaskList *);

void createNewDirectory(const char *);

void writeContentInFileFromUrl(const char *, const char *);

size_t searchInFile(const char *, const char *);

void *runAction(void *);

char *prepare_directory_path(Config);

char *prepare_directories_from_url(const char *, const char *);

void save_file(const char *, const char *, const char *);

char *get_domain_name(char *);

char **parse_url(const char *, size_t *);

void test();

#endif