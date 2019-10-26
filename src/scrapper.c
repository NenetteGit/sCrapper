#include <pthread.h>

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

// void* testPthread() {
//     int i = 0;
//     while (i < 2000000)
//     {
//         printf("%d ", i);
//         i++;
//     }
//     printf("\n");
//     return NULL;
// }

void* testPthread(void* test) {
    int* i = (int*) test;
    while (*i < 5)
    {
        *i++;
    }
    return NULL;
}

void checkSymbolParent(const char* line, int* parentSymbol) {
    if(strlen(line) > 0) {
        if (line[0] == '#') return;
        else if (line[0] == '=' && line[1] == '=' && TASK != *parentSymbol) {
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

    pthread_t pthread;
    int p = 0;
    volatile int het = pthread_create(&pthread, NULL, testPthread, &p);
    if (het != 0);
    {
        fprintf(stderr, "Error creating thread: %d\n", het);
        return EXIT_FAILURE;
    }
    
    // STEP 1: OPEN CONFIG FILE IN READ MODE
    FILE* configFile = fopen(fullPathNameOfConfigFile, "r");
    if(configFile == NULL) {
        printf("Config file could not have been opened.\n");
        return EXIT_FAILURE;
    }
    // STEP 2: READ CONFIG FILE
    while ((read = getline(&line, &length, configFile)) != -1) {
        if(line[0] == '#') continue;
        // STEP 3: CHECK IF THE CHARACTER IS A PARENT FLAG
        if(line[0] != '{' && line[0] != '(') {
            checkSymbolParent(line, &symbolParent);
            if (symbolParent == TASK || symbolParent == ACTION) {
                int isInitialized = initialize(&tasksList, &actionsList, symbolParent);
                if (isInitialized != 0) {
                    printf("\nelement not initialized\n");
                    return EXIT_FAILURE;
                }
            } 
        } 
        else {
            int optionType = checkSymbolOption(line, symbolParent);
            if (optionType != -1) {
                char* substr = extractDataFromConfigFile(line, optionType);
                if(substr == NULL) return EXIT_FAILURE;
                int numberOfOptions = 0;
                char** options = splitOption(substr, optionType, &numberOfOptions);
                for(int i = 0; i < numberOfOptions; i++) {
                    options[i] = trim(options[i]);
                }
                if(optionType == DATA_ACTION) {
                    if(strcmp(options[0], "name\0") == 0) {
                        setNameOfAction(actionsList, options[1]);
                    }
                    if(strcmp(options[0], "url\0") == 0) {
                        setUrlTargetOfAction(actionsList, options[1]);
                    }                    
                } else if(optionType == DATA_TASK) {
                    if(strcmp(options[0], "name\0") == 0) setNameOfTask(tasksList->firstTask, options[1]);
                } else if(optionType == NEW_OPTION) {
                    if (actionsList->firstAction->numberOfOptions == 0) {
                        setOption(actionsList->firstAction->firstOption, options[0], options[1]);
                        actionsList->firstAction->numberOfOptions += 1;
                    } else {
                        addNewOptionInList(actionsList->firstAction, options[0], options[1]);
                    }
                } 
                else if(optionType == NEW_ACTION) {
                    for (int i = 0; i < numberOfOptions; i++) {
                        Action* action = findActionByNameInList(actionsList, options[i]);
                        addCopyActionToList(tasksList->firstTask, unreferencedCopyAction(action));
                    }
                }
            }
        }
    }
    displayActionsList(actionsList);
    displayTasksList(tasksList);

    if(pthread_join(pthread, NULL)) {
        fprintf(stderr, "error joining thread\n");
        return 2;
    }
    // pthread_exit(NULL);
    printf("length: %lu.\ntest end \n", length);
    
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