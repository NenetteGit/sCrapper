#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <unistd.h>
#include "headers/structures.h"
#include "headers/sniff.h"

int main(int argc, char **argv)
{
    TaskList *tasksList = NULL;

    if (argc != 2)
    {
        printf("Configuration path name missing.\nPlease retry the command with the file name as parameter.\n");
        return EXIT_FAILURE;
    }

    tasksList = getTasksListFromConfigFile(argv[1]);
    displayTasksList(tasksList);

    // test();

    runAllTasks(tasksList);

    // freeTaskFromList(tasksList);

    printf("\ntest end \n");

    return EXIT_SUCCESS;
}
