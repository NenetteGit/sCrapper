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

int checkSymbolOption(const char* line, int parentSymbol) {
    if (line[0] == '{' && parentSymbol == TASK) {
        return DATA_TASK;
    } else if (line[0] == '{' && parentSymbol == ACTION) {
        return DATA_ACTION;
    } else if (line[0] == '(' && parentSymbol == OPTION) {
        return NEW_ACTION;
    } else if (line[0] == '{' && parentSymbol == OPTION) {
        return NEW_OPTION;
    }
    return -1;
}

// int setParentSymbol(const char* line, const char* parentSymbol) {
//     int symbol²;
//     if (strlen(line) > 0 && strlen(parentSymbol) > 0 && strlen(parentSymbol) <= 2) {
//         if
//     }
//     return symbol;
// }

void checkSymbolParent(const char* line, int* parentSymbol) {
    if(strlen(line) > 0) {
        if (line[0] == '#') {
            return;
        } else if (line[0] == '=' && line[1] == '=' && TASK != *parentSymbol) {
            *parentSymbol = TASK;
        } else if (line[0] == '=' && ACTION != *parentSymbol) {
            *parentSymbol = ACTION;
        } else if (line[0] == '+' && OPTION != *parentSymbol) {
            *parentSymbol = OPTION;
        } 
    }
}

int main(int argc, char** argv) {
    enum symbol startEnumeration = 0;
    const char* fullPathNameOfConfigFile = "config/configuration.sconf\0";
    ssize_t read = 0;
    char * line = NULL;
    size_t length = 0;
    size_t numberOfTasks = 0;
    TaskList* tasksList = NULL;
    Task* actionsList = NULL;
    unsigned int numberOfIterationInConfigFile = 0;
    int symbolParent = -1;

    // STEP 1: OPEN CONFIG FILE IN READ MODE
    FILE* configFile = fopen(fullPathNameOfConfigFile, "r");
    if(configFile == NULL) {
        printf("Config file could not have been opened.\n");
        return EXIT_FAILURE;
    }
    tasksList = malloc(sizeof(TaskList));
    actionsList = malloc(sizeof(Task));
    if (tasksList == NULL || actionsList == NULL) return EXIT_FAILURE;
    // STEP 2: READ CONFIG FILE
    while ((read = getline(&line, &length, configFile)) != -1) {
        // STEP 3: CHECK IF THE CHARACTER IS A PARENT FLAG
        checkSymbolParent(line, &symbolParent);
        if (symbolParent == TASK || symbolParent == ACTION) {
            int isInitialized = initialize(tasksList, actionsList, symbolParent);
            if (isInitialized == 0) {
                printf("\nelement not initialized: %s\n", line);
                continue;
            }
        }
        int optionType = checkSymbolOption(line, symbolParent);
        if (optionType != -1) {
            char* substr = extractDataFromConfigFile(line, optionType);
            char** test = splitOption(substr, optionType);
            // get data from config file => substr function to create
            if(optionType == DATA_ACTION) {
                // actionsList->firstAction->name = 
            } else if(optionType == DATA_TASK) {
                // tasksList->firstTask->name = 
            } else if(optionType == NEW_OPTION) {
                //check if option ptr is NULL
                //if null set option
                //else add new option to action list
            } else if(optionType == NEW_ACTION) {
                //check if option list ptr is NULL
                //if null set option
                //else add new option to action list
            }
        }
        // printf("\nsymbol parent: %d\n", symbolParent);
        // checkSymbolOption(line, symbolParent);
        
        // printf("\nelement initialized: %s\n", line);
    }
    printf("\n\nlength: %lu.\ntest end \n", length);
    
    // CURLcode res;
    // CURL* curl = NULL;
    // FILE *fp = fopen("tests_curl/test.html", "wb");
    // if(fp == NULL) return 1;
    // curl = curl_easy_init();
    // if(curl != NULL) {
    //     curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.fr/");
    //     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 
	//     curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    //     res = curl_easy_perform(curl);
    //     curl_easy_cleanup(curl);
    //     printf("\nnew file created\n");
    // }
    // fclose(fp);

    return EXIT_SUCCESS;
}