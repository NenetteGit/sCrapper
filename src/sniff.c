#include "headers/sniff.h"
#include "headers/structures.h"
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

void test()
{
    const char *filename = "./src/versioning/tasks_history.log";
    Task *task = initializeTask();
    setNameOfTask(task, "My task name");
    setNameOfAction(task, "My action name");
    insertContentInFile(filename, task->firstAction->name, task->name);
    printf("content inserted success.\n");
    // struct tm *now = NULL;
    // char *time = actualTimeToString(&now, "%d/%m/%Y à %X");
    // if (time == NULL)
    // {
    //     printf("An error occured to transform actual time to string.\n");
    //     return;
    // }
    // printf("%s\n", time);
    // char *timeModified = timeToString(now, "%d-%m-%Y %X");
    // if (timeModified == NULL)
    // {
    //     printf("An error occured to modify time an transform to string.\n");
    //     return;
    // }
    // printf("%s\n", timeModified);

    // free(time);
    // free(timeModified);
    // EXAMPLE OF CURL REQUEST
    //
    // CURLcode res;

    // CURL *curl = NULL;
    // FILE *fp = fopen("src/tests_curl/test.html", "wb");

    // if (fp == NULL)
    //     return;
    // curl = curl_easy_init();
    // if (curl != NULL)
    // {
    //     curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.fr/");
    //     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    //     curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    //     curl_easy_perform(curl);
    //     curl_easy_cleanup(curl);
    //     printf("\nnew file created\n");
    // }
    // fclose(fp);
}

void runAllTasks(TaskList *tasksList)
{
    Task *startTask = tasksList->firstTask;
    while (startTask != NULL)
    {
        Action *startAction = startTask->firstAction;
        while (startAction != NULL)
        {
            Option *startOption = startAction->firstOption;
            while(startOption != NULL)
            {
                if(strcmp(startOption->key, "versioning") == 0 &&
                strcmp(startOption->value, "on") == 0)
                {
                    insertContentInFile("./src/versioning/tasks_history.log", startAction->name, startTask->name);
                    printf("\ncontent inserted success.\n");
                    break;
                }
                startOption = startOption->next;
            }
            startAction = startAction->next;
        }
        startTask = startTask->next;
    }
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