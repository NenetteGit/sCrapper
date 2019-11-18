#include "headers/runner.h"
#include "headers/parser.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <pthread.h>
#include <stddef.h>
#include <unistd.h>
#include <curl/curl.h>

static struct buffer_t init_buffer()
{
    struct buffer_t buffer;

    buffer.bytes = calloc(sizeof(char), 0 + 1);
    buffer.len = 0;

    return buffer;
}

static size_t write_buffer(void *ptr, size_t size, size_t nmemb, struct buffer_t *buffer)
{
    size_t nbytes = size * nmemb;

    buffer->bytes = realloc(buffer->bytes, sizeof(char) * (buffer->len + nbytes + 1));

    memcpy(buffer->bytes + buffer->len, ptr, nbytes);
    buffer->len += nbytes;

    return nbytes;
}

HttpResponse http_get(const char *url, char **content_type)
{
    CURL *curl = curl_easy_init();

    HttpResponse response = (HttpResponse){
        .content_type = NULL,
        .buffer = NULL,
    };

    struct buffer_t buffer = init_buffer();

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    curl_easy_setopt(curl, CURLINFO_RESPONSE_CODE, &response.status);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_buffer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "Request failed for '%s': %s\n", url, curl_easy_strerror(res));
        return response;
    }
    else
    {
        curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &response.content_type);
        if (response.content_type != NULL)
        {
            *content_type = calloc(sizeof(char), strlen(response.content_type) + 1);
            strcpy(*content_type, response.content_type);
        }
    }
    curl_easy_cleanup(curl);

    response.buffer = buffer.bytes;
    return response;
}

char *get_domain_name(char *str)
{
    char *domain = NULL;

    char *start_domain = strstr(str, "www");

    start_domain = (start_domain != NULL) ? start_domain + 4 : str;

    char *end_domain = strchr(start_domain, '.');

    if (end_domain != NULL)
    {
        ptrdiff_t length = end_domain - start_domain;
        domain = calloc(sizeof(char), length + 1);
        if (domain == NULL)
            return NULL;

        strncpy(domain, start_domain, length);
    }

    return domain;
}

char **parse_url(const char *url, size_t *length)
{
    size_t capacity = 2;
    char **parsed_url = malloc(sizeof(char *) * capacity);
    if (parsed_url == NULL)
        return NULL;

    char *str = calloc(sizeof(char), strlen(url) + 1);
    if (str == NULL)
        return NULL;

    strcpy(str, url);
    char *token = strtok(str, "/");

    if (token != NULL)
        token = strtok(NULL, "/");

    while (token != NULL)
    {
        parsed_url[(*length)++] = token;
        if ((*length) == capacity)
        {
            capacity *= 2;
            parsed_url = realloc(parsed_url, sizeof(char *) * capacity);
        }
        token = strtok(NULL, "/");
    }
    parsed_url[0] = get_domain_name(parsed_url[0]);

    return parsed_url;
}

char *prepare_directories_from_url(const char *url, const char *directory)
{
    size_t dir_number = 0;
    size_t length = strlen(directory);
    
    char **directories_and_file = parse_url(url, &dir_number);

    for (size_t i = 0; i < dir_number; i++)
        length += strlen(directories_and_file[i]) + 1;
    
    char *full_file_path = calloc(sizeof(char), length + 1);
    if (full_file_path == NULL) 
        return NULL;

    strcpy(full_file_path, directory);

    if (dir_number == 0)
        return NULL;
    
    char *token = NULL;
    for (size_t j = 0; j < dir_number; j++)
    {
        token = strchr(directories_and_file[j], '.');
        if (token == NULL)
        {
            strcat(full_file_path, directories_and_file[j]);
            strcat(full_file_path, "/");
            createNewDirectory(full_file_path);
        }
        else
        {
            char *token2 = strtok(directories_and_file[j], "?");
            if (token2 == NULL)
            {
                strcat(full_file_path, directories_and_file[j]);
            }
            else
            {
                strcat(full_file_path, token2);
            }
            
        }   
    }
    if (token == NULL)
    {
        size_t size_boost = strlen(directories_and_file[dir_number - 1]) + 6;
        full_file_path = realloc(full_file_path, sizeof(char) * (strlen(full_file_path) + size_boost));

        memset(full_file_path + strlen(full_file_path), '\0', size_boost);

        strcat(full_file_path, directories_and_file[dir_number - 1]);
        strcat(full_file_path, ".html");
    }

    return full_file_path;
}

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void save_file(const char *url, const char *directory, const char *buffer)
{
    pthread_mutex_lock(&lock);
    char *file_name = prepare_directories_from_url(url, directory);
    printf("filename: %s\n", file_name);
    FILE *new_file = fopen(file_name, "wb");
    if (new_file == NULL)
        return;
    fprintf(new_file, "%s\n", buffer);
    fclose(new_file);
    pthread_mutex_unlock(&lock);
}

void create_all_files_from_url(Config config)
{
    char *content_type = NULL;
    HttpResponse response = http_get(config.url, &content_type);
    
    if (response.buffer == NULL || content_type == NULL)
        return;
    
    response.content_type = calloc(sizeof(char), strlen(content_type) + 1);
    strcpy(response.content_type, content_type);
    free(content_type);
    
    save_file(config.url, config.main_folder, response.buffer);

    if (config.max_depth == 0)
        return;

    unsigned int length = 0;
    char **urls = parse_urls(response.buffer, &length);
    free(response.buffer);
    config.max_depth--;
    for (size_t i = 0; i < length; i++)
    {
        config.url = urls[i];
        create_all_files_from_url(config);
    }
}

void writeContentInFileFromUrl(const char *filename, const char *url)
{
    CURL *curl = NULL;
    FILE *file = fopen(filename, "wb");

    if (file == NULL)
        return;

    curl = curl_easy_init();
    if (curl != NULL)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        printf("\nnew file created\n");
    }
    fclose(file);
}

char **parse_urls(char *buffer, unsigned int *length)
{
    if (strlen(buffer) == 0)
    {
        return NULL;
    }

    unsigned int capacity = 10;
    char **urls = malloc(sizeof(char *) * capacity);
    char *start = buffer;
    char *end = NULL;

    while (start != NULL)
    {
        start = strstr(start, "http");
        if (start != NULL)
        {
            if ((start - 1)[0] == '"')
                end = strchr(start, '"');

            if ((start - 1)[0] == '\'')
                end = strchr(start, '\'');

            if (end != NULL)
            {
                if (*length == capacity)
                {
                    capacity *= 2;
                    urls = realloc(urls, sizeof(char *) * capacity);
                }
                urls[*length] = calloc(sizeof(char), (end - start) + 1);
                if (urls[*length] != NULL)
                {
                    strncpy(urls[(*length)++], start, end - start);
                    start = end;
                }
            }
            start++;
        }
    }
    return urls;
}

void runAllTasks(TaskList *tasksList)
{
    Task *startTask = tasksList->firstTask;
    size_t thread_capacity = 10;
    pthread_t *threads = malloc(sizeof(pthread_t) * thread_capacity);
    size_t thread_index = 0;
    
    while (startTask != NULL)
    {
        Action *startAction = startTask->firstAction;
        while (startAction != NULL)
        {
            Config *config = malloc(sizeof(Config));
            *config = (Config){
                .max_depth = 0,
                .versioning = 0,
                .timer = get_timer_of_task(startTask),
                .main_folder = "../downloads",
                .task_name = startTask->name,
                .action_name = startAction->name,
                .url = startAction->url,
            };
            
            Option *startOption = startAction->firstOption;
            
            while (startOption != NULL)
            {
                if (strcmp(startOption->key, "max-depth") == 0 && atoi(startOption->value) >= 0)
                    config->max_depth = atoi(startOption->value);

                if (strcmp(startOption->key, "versioning") == 0 && strcmp(startOption->value, "on") == 0)
                    config->versioning = 1;

                startOption = startOption->next;
            }
            if (thread_index == thread_capacity)
            {
                thread_capacity *= 2;
                threads = realloc(threads, sizeof(pthread_t) * thread_capacity);
            }

            pthread_create(&threads[thread_index++], NULL, runAction, config);

            startAction = startAction->next;
        }
        startTask = startTask->next;
    }
    for (size_t i = 0; i < thread_index; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

size_t get_timer_of_task(Task *task)
{
    return (task->hour * 3600) + (task->minute * 60) + task->second;
}

char *prepare_directory_path(Config config)
{
    struct tm *actualTime = NULL;
    char *taskFolder = calloc(sizeof(char), strlen(config.main_folder) + strlen(config.task_name) + 3);
    if (taskFolder == NULL)
        return NULL;

    sprintf(taskFolder, "%s/%s/", config.main_folder, config.task_name);
    createNewDirectory(taskFolder);
    char *actionFolder = actionFolder = calloc(sizeof(char), strlen(taskFolder) + strlen(config.action_name) + 20);
    if (actionFolder == NULL)
        return NULL;

    sprintf(actionFolder, "%s%s", taskFolder, config.action_name);
    free(taskFolder);

    if (config.versioning == 1)
    {
        char *now = actualTimeToString(&actualTime, " %Y-%m-%d");
        if (now == NULL)
            return NULL;

        strcat(actionFolder, now);
        strcat(actionFolder, "/");
        free(now);
        insertContentInFile("../versioning/tasks.log", config.action_name, config.task_name);
    }
    else
        strcat(actionFolder, "/");

    createNewDirectory(actionFolder);

    return actionFolder;
}

void *runAction(void *ptr)
{
    Config *config = (Config *)ptr;
    config->main_folder = prepare_directory_path(*config);
    create_all_files_from_url(*config);
    return NULL;
}

size_t searchInFile(const char *filename, const char *needle)
{
    ssize_t read = 0;
    char *line = NULL;
    size_t length = 0;
    char *found = NULL;
    int flag = 0;
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        return -1;
    while ((read = getline(&line, &length, file)) != -1)
    {
        found = strstr(line, needle);
        if (found != NULL)
            flag = 1;
        if (flag == 1)
        {
            return length;
        }
    }
    return -1;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

char *actualTimeToString(struct tm **now, const char *format)
{
    time_t rawtime;
    char *buffer = calloc(sizeof(char), 50);
    if (buffer == NULL)
        return NULL;
    time(&rawtime);
    *now = localtime(&rawtime);
    size_t error = strftime(buffer, 50, format, *now);
    if (error == 0)
        return NULL;
    return buffer;
}

char *timeToString(struct tm *time, const char *format)
{
    char *buffer = calloc(sizeof(char), 50);
    if (buffer == NULL)
        return NULL;
    size_t error = strftime(buffer, 50, format, time);
    if (error == 0)
        return NULL;
    return buffer;
}

void insertContentInFile(const char *filename, const char *actionName, const char *taskName)
{
    FILE *file = fopen(filename, "a+");
    if (file == NULL)
    {
        printf("File does not exit in path given.\n");
        return;
    }
    struct tm *now = NULL;
    char *nowDateTime = actualTimeToString(&now, "%d-%m-%Y %X");
    fprintf(file, "Action: %s\nExecuted by: %s\nDate: %s\n\n", actionName, taskName, nowDateTime);
    free(nowDateTime);
    fclose(file);
}

void createNewDirectory(const char *dirname)
{
    struct stat newStat = {0};
    if (stat(dirname, &newStat) == -1)
    {
        mkdir(dirname, 0700);
    }
}