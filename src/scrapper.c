#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <unistd.h>
#include "headers/structures.h"

int main(int argc, char** argv) {
    TaskList* tasksList = NULL;

    if (argc != 2) {
        printf("Configuration path name missing.\nPlease retry the command with the file name as parameter.\n");
        return EXIT_FAILURE;
    }
    tasksList = getTasksListFromConfigFile(argv[1]);
    displayTasksList(tasksList);

    // freeTaskFromList(tasksList);

    printf("\ntest end \n");
    
    return EXIT_SUCCESS;
}








    // EXAMPLE OF CURL REQUEST
    //
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
    // freeTask(&actionsList);
    // while (tasksList->firstTask != NULL)
    // {
    //     freeTaskFromList(tasksList);
    // }
    // displayActionsList(actionsList);
    // displayTasksList(tasksList);