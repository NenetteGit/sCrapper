#include "headers/sniff.h"
#include "headers/structures.h"
#include <stdlib.h>
#include <curl/curl.h>

void test()
{

    struct tm *now = NULL;
    char *time = actualTimeToString(&now, "%d/%m/%Y à %X");
    if (time == NULL) 
    {
        printf("An error occured to transform actual time to string.\n");
        return;
    }
    printf("%s\n", time);
    char *timeModified = timeToString(now, "%d-%m-%Y %X");
    if (timeModified == NULL) 
    {
        printf("An error occured to modify time an transform to string.\n");
        return;
    }
    printf("%s\n", timeModified);

    free(time);
    free(timeModified);
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