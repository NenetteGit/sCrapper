#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "headers/structures.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int main(int argc, char** argv) {
    const char* configFullPathName = "config/configuration.sconf\0";
    ssize_t read = 0;
    char * line = NULL;
    size_t length = 0;
    size_t numberOfTasks = 1;
    Task* tasks = malloc(sizeof(Task));
    
    if(tasks == NULL) {
        printf("Tasks array could not be allocated.\n");
        return -1;
    }
    FILE* config = fopen(configFullPathName, "r");
    if(config == NULL) {
        printf("Config file could not be opened.\n");
        return -1;
    }
    while ((read = getline(&line, &length, config)) != -1) {
        if (line[0] == '#') {
            continue;
        } else if (line[0] == '+') {
            //set option structure
        } else if (line[0] == '=') {
            //set action structure
        } else if (line[0] == '=' && line[1] == '=') {
            //set task structure
        }       
    }

    // CURLcode res;
    // CURL* curl = NULL;
    // FILE *fp = fopen("tests_curl/test.html", "wb");
    // if(fp == NULL) return 1;
    // curl = curl_easy_init();
    // if(curl) {
    //     curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.fr/");
    //     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 
	//     curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    //     res = curl_easy_perform(curl);
    //     curl_easy_cleanup(curl);
    // }
    // fclose(fp);
    printf("test file created and filled ! houraaaaa !!\n");
    
    
    return 0;
}