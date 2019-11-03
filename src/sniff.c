#include "headers/sniff.h"
#include "headers/structures.h"
#include <curl/curl.h>
void test(TaskList* tasklist) {
        // EXAMPLE OF CURL REQUEST
    //
    // CURLcode res;
    CURL* curl = NULL;
    FILE *fp = fopen("tests_curl/test.html", "wb");
    if(fp == NULL) return 1;
    curl = curl_easy_init();
    if(curl != NULL) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.fr/");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        printf("\nnew file created\n");
    }
}