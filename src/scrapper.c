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
        printf("Configuration path name missing.\nPlease retry the command giving the configuration file name as parameter.\n");
        return EXIT_FAILURE;
    }
    if (strstr(argv[1], ".sconf") == NULL)
    {
        printf("Wrong type of configuration file.\n");
        printf("Please retry the commande giving a file as parameter with '.sconf' as extension.\n");
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
