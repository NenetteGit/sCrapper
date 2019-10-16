#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int main(int argc, char** argv) {
    CURLcode res;
    CURL* curl = NULL;
    FILE *fp = fopen("tests_curl/test.html", "wb");
    if(fp == NULL) return 1;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.fr/");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    fclose(fp);
    printf("test file created and filled ! houra !!\n");

    return 0;
}