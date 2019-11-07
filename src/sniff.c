#include "headers/sniff.h"
#include "headers/structures.h"
#include <curl/curl.h>
void test()
{
    // EXAMPLE OF CURL REQUEST
    //
    // CURLcode res;

    CURL *curl = NULL;
    FILE *fp = fopen("src/tests_curl/test.html", "wb");

    if (fp == NULL)
        return;
    curl = curl_easy_init();
    if (curl != NULL)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.fr/");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        printf("\nnew file created\n");
    }
    fclose(fp);
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}