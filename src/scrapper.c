#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

#include "headers/structures.h"



size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int checkSymboleOption(const char* line) {
    return (line[0] == '(' || line[0] == '{') ? 1 : 0;
}

int checkSymbolParent(const char* line) {
    if(strlen(line) > 0) {
        if (line[0] == '#') {
            return COMMENT;
        } else if (line[0] == '=' && line[1] == '=') {
            return TASK;
        } else if (line[0] == '=') {
            return ACTION;
        } else if (line[0] == '+') {
            return OPTION;
        } 
    }
    return -1;
}

int main(int argc, char** argv) {
    enum symbol s = 0;
    const char* fullPathNameOfConfigFile = "config/configuration.sconf\0";
    ssize_t read = 0;
    char * line = NULL;
    size_t length = 0;
    size_t numberOfTasks = 0;
    TaskList* tasksList = NULL;
    unsigned int numberOfIterationInConfigFile = 0;
    int symbolParent;

    tasksList = malloc(sizeof(TaskList));
    if(tasksList == NULL) return -1;
    tasksList->firstTask = initializeTask();
    if(tasksList->firstTask == NULL) return -1;
    tasksList->numberOfTasks++;
    setNameOfTask(tasksList->firstTask, "My first task\0");
    setAction(tasksList->firstTask->firstAction, "My first action\0", "https://www.google.fr\0");
    setOption(tasksList->firstTask->firstAction->firstOption, "max-iter\0", "2\0");
    addNewOptionInList(tasksList->firstTask->firstAction, "versioning\0", "on\0");
    addNewActionInList(tasksList->firstTask, "My second action\0", "https://www.google.com\0");
    setOption(tasksList->firstTask->firstAction->firstOption, "versioning\0", "off\0");
    addNewOptionInList(tasksList->firstTask->firstAction, "max-iter\0", "0\0");
    addNewTaskInList(tasksList, "My second task\0", 1, 25);
    setAction(tasksList->firstTask->firstAction, "first action of second task", "https://www.google.es\0");
    setOption(tasksList->firstTask->firstAction->firstOption, "versioning\0", "off\0");
    addNewOptionInList(tasksList->firstTask->firstAction, "max-iter\0", "5\0");
    addNewOptionInList(tasksList->firstTask->firstAction, "min-iter\0", "1\0");
    displayTasksList(tasksList);
    printf("==============================================\n");
    freeFirstOptionFromList(tasksList->firstTask->firstAction);
    displayTasksList(tasksList);
    // displayActionsList(tasksList->firstTask);

    // STEP 1: OPEN CONFIG FILE IN READ MODE
    // FILE* configFile = fopen(fullPathNameOfConfigFile, "r");
    // if(configFile == NULL) {
    //     printf("Config file could not have been opened.\n");
    //     return -1;
    // }

    // // STEP 2: READ CONFIG FILE
    // while ((read = getline(&line, &length, configFile)) != -1) {
    //     // STEP 3: CHECK IF THE CHARACTER IS A FLAG
    //     int symboleTmp = checkSymbolParent(line);
    //     if(symboleTmp != -1 && symboleTmp != symbolParent && symbolParent != COMMENT) {
    //         symbolParent = symboleTmp;
            
    //     }
        
    //     //  {
    //     //     printf("\nthere is data to set:\n%s\n", line);
    //     // }
    //     // if (line[0] == '#') {
    //     //     printf("%d ", COMMENT);
    //     //     continue;
    //     // } else if (line[0] == '+') {
    //     //     printf("%d ", OPTION);
    //     //     continue;
    //     //     //set option structure
    //     // } else if (line[0] == '=') {
    //     //     printf("%d ", ACTION);
    //     //     continue;
    //     //     //set action structure
    //     // } else if (line[0] == '=' && line[1] == '=') {
    //     //     printf("%d ", TASK);
    //     //     continue;
    //     //     //set task structure
    //     // }
    //     line = NULL;       
    // }

    // // CURLcode res;
    // // CURL* curl = NULL;
    // // FILE *fp = fopen("tests_curl/test.html", "wb");
    // // if(fp == NULL) return 1;
    // // curl = curl_easy_init();
    // // if(curl) {
    // //     curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.fr/");
    // //     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 
	// //     curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    // //     res = curl_easy_perform(curl);
    // //     curl_easy_cleanup(curl);
    // // }
    // // fclose(fp);
    printf("nb length: %lu.\ntest end \n", length);
    
    
    return 0;
}