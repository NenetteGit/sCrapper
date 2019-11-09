#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include "headers/structures.h"

void setOption(Option *option, const char *key, const char *value)
{
    setKeyOfOption(option, key);
    setValueOfOption(option, value);
}

void setKeyOfOption(Option *option, const char *key)
{
    if (strlen(key) > 0)
    {
        option->key = calloc(sizeof(char), (strlen(key) + 1));
        if (option->key == NULL)
            return;
        strcpy(option->key, key);
    }
}

void setValueOfOption(Option *option, const char *value)
{
    if (strlen(value) > 0)
    {
        option->value = calloc(sizeof(char), (strlen(value) + 1));
        if (option->value == NULL)
            return;
        strcpy(option->value, value);
    }
}

void setAction(Task *actionList, const char *name, const char *url)
{
    setNameOfAction(actionList, name);
    setUrlTargetOfAction(actionList, url);
}

void setUrlTargetOfAction(Task *actionList, const char *url)
{
    if (strlen(url) > 0)
    {
        actionList->firstAction->url = calloc(sizeof(char), (strlen(url) + 1));
        if (actionList->firstAction->url == NULL)
            return;
        strcpy(actionList->firstAction->url, url);
    }
}

void setNameOfAction(Task *actionsList, const char *name)
{
    if (strlen(name) > 0)
    {
        actionsList->firstAction->name = calloc(sizeof(char), (strlen(name) + 1));
        if (actionsList->firstAction->name == NULL)
            return;

        strcpy(actionsList->firstAction->name, name);
    }
}

void setTask(Task *task, const char *name, unsigned int hour,
             unsigned int minute)
{
    setNameOfTask(task, name);
    setTimeOfTask(task, hour, minute);
}

void setNameOfTask(Task *task, const char *name)
{
    if (strlen(name) > 0)
    {
        task->name = calloc(sizeof(char), (strlen(name) + 1));
        if (task->name == NULL)
            return;
        strcpy(task->name, name);
    }
}

void setTimeOfTask(Task *task, unsigned int hour,
                   unsigned int minute)
{
    task->hour = hour;
    task->minute = minute;
    task->second = 0;
}

void setHourOfTask(Task *task, const char *hour)
{
    task->hour = atoi(hour);
}

void setMinuteOfTask(Task *task, const char *minute)
{
    task->minute = atoi(minute);
}

void setSecondOfTask(Task *task, const char *second)
{
    task->second = atoi(second);
}

void freeFirstOptionFromList(Action *action)
{
    if (action->firstOption != NULL)
    {
        Option *optionToDelete = action->firstOption;
        if (optionToDelete->key != NULL && optionToDelete->value != NULL)
        {
            free(optionToDelete->key);
            free(optionToDelete->value);
        }
        action->firstOption = action->firstOption->next;
        free(optionToDelete);
        action->numberOfOptions--;
    }
}

void freeFirstActionFromList(Task *task)
{
    if (task->firstAction != NULL)
    {
        Action *actionToDelete = task->firstAction;
        if (actionToDelete->name != NULL && actionToDelete->url)
        {
            free(actionToDelete->name);
            free(actionToDelete->url);
        }
        while (actionToDelete->firstOption != NULL)
        {
            freeFirstOptionFromList(actionToDelete);
        }
        task->firstAction = task->firstAction->next;
        free(actionToDelete);
        task->numberOfActions--;
    }
}

void freeTaskFromList(TaskList *tasklist)
{
    if (tasklist != NULL)
    {
        while (tasklist->firstTask != NULL)
        {
            Task *taskToDelete = tasklist->firstTask;
            if (taskToDelete->name != NULL)
                free(taskToDelete->name);
            while (taskToDelete->firstAction != NULL)
            {
                freeFirstActionFromList(taskToDelete);
            }
            tasklist->firstTask = tasklist->firstTask->next;
            free(taskToDelete);
            tasklist->numberOfTasks--;
        }
    }
}

void freeTask(Task **task)
{
    if (*task != NULL)
    {
        // Task* taskToDelete = tasklist->firstTask;
        if ((*task)->name != NULL)
            free((*task)->name);
        while ((*task)->firstAction != NULL)
        {
            freeFirstActionFromList((*task));
        }
        // tasklist->firstTask = tasklist->firstTask->next;
        free((*task));
        // tasklist->numberOfTasks--;
    }
}

void addNewOptionInList(Action *action, const char *key, const char *value)
{
    Option *newOption = malloc(sizeof(Option));
    if (action->firstOption == NULL || newOption == NULL)
        return;
    setOption(newOption, key, value);
    newOption->next = action->firstOption;
    action->firstOption = newOption;
    action->numberOfOptions++;
}

void addNewEmptyActionInList(Task *task)
{
    Action *newAction = initializeAction();
    if (task->firstAction == NULL || newAction == NULL)
        return;
    newAction->next = task->firstAction;
    task->firstAction = newAction;
    task->numberOfActions++;
}

void addNewEmptyTaskInList(TaskList *taskList)
{
    Task *newTask = initializeTask();
    if (taskList->firstTask == NULL || newTask == NULL)
        return;
    newTask->next = taskList->firstTask;
    taskList->firstTask = newTask;
    taskList->numberOfTasks++;
}

void displayOptionsList(Action *action)
{
    Option *optionToDisplay = action->firstOption;
    int i = 0;
    while (optionToDisplay != NULL)
    {
        printf("\nOption %d:\nkey: %s\nvalue: %s\n", i + 1, optionToDisplay->key, optionToDisplay->value);
        i++;
        optionToDisplay = optionToDisplay->next;
    }
}

void displayActionsList(Task *task)
{
    Action *actionToDisplay = task->firstAction;
    int i = 0;
    while (actionToDisplay != NULL)
    {
        printf("\nAction %d:\nname: %s\nurl: %s\n", i + 1, actionToDisplay->name, actionToDisplay->url);
        displayOptionsList(actionToDisplay);
        i++;
        actionToDisplay = actionToDisplay->next;
    }
}

void displayTasksList(TaskList *taskList)
{
    Task *taskToDisplay = taskList->firstTask;
    int i = 0;
    while (taskToDisplay != NULL)
    {
        printf("\nTask %d:\nname: %s\nhour: %u\nminute: %u\nsecond: %u\n", i + 1, taskToDisplay->name, taskToDisplay->hour, taskToDisplay->minute, taskToDisplay->second);
        displayActionsList(taskToDisplay);
        i++;
        taskToDisplay = taskToDisplay->next;
    }
}

char *extractDataFromConfigFile(const char *line, int parentSymbol)
{
    char endOfData = parentSymbol == NEW_ACTION ? ')' : '}';
    int i = 1;
    char *substr;
    int subStringLength = 0;
    while (line[i++] != endOfData)
    {
        subStringLength++;
    }
    substr = calloc(sizeof(char), (subStringLength + 1));
    if (substr == NULL)
        return NULL;
    strncpy(substr, line + 1, subStringLength);
    return substr;
}

char **splitOption(char *optionNotSplit, int parentSymbol, int *numberOfOptions)
{
    char *separator = parentSymbol == NEW_ACTION ? "," : "->";
    char **resultOfSplit = malloc(sizeof(char *) * 5);
    int index = 0;
    char *firstFind;
    char *ptr = optionNotSplit;

    while ((firstFind = strstr(ptr, separator)) != NULL)
    {
        ptrdiff_t size = firstFind - optionNotSplit;
        resultOfSplit[index] = calloc(sizeof(char), size + 1);
        strncpy(resultOfSplit[index], ptr, size);
        ptr += (parentSymbol == NEW_ACTION) ? size + 1 : size + 2;
        index++;
    }

    resultOfSplit[index] = calloc(sizeof(char), (strlen(ptr) + 1));
    strcpy(resultOfSplit[index++], ptr);
    *numberOfOptions = index;

    for (int i = 0; i < *numberOfOptions; i++)
    {
        resultOfSplit[i] = trim(resultOfSplit[i]);
    }

    return resultOfSplit;
}

char *trim(const char *input)
{
    char *output = calloc(sizeof(char), strlen(input) + 1);
    strcpy(output, input + (isspace(input[0]) ? 1 : 0));
    for (int i = strlen(output) - 1; i >= 0 && isspace(output[i]); i--)
    {
        output[i] = '\0';
    }
    return output;
}

// char *trim(const char *str)
// {
//     while(isspace(*str)) str++;

//     int size = strlen(str) - 1;
//     while(str[size] > str && isspace(str[size])) size--;

//     char *out = calloc(sizeof(char), size + 1);
//     strncpy(out, str, size);

//     return out;
// }

Action *unreferencedCopyAction(Action *actionToCopy)
{
    Action *a = malloc(sizeof(Action));
    a->name = actionToCopy->name;
    a->url = actionToCopy->url;
    a->firstOption = actionToCopy->firstOption;
    a->next = NULL;
    return a;
}

void addCopyActionToList(Task *actionsList, Action *action)
{
    if (actionsList->numberOfActions > 0)
    {
        action->next = actionsList->firstAction;
    }
    actionsList->firstAction = action;
    actionsList->numberOfActions++;
}

Action *findActionByNameInList(Task *actionsList, const char *haystack)
{
    Action *a = actionsList->firstAction;
    while (a != NULL)
    {
        if (strcmp(a->name, haystack) == 0)
        {
            return a;
        }
        a = a->next;
    }
    return NULL;
}

Option *initializeOption()
{
    Option *option = malloc(sizeof(Option));
    if (option == NULL)
        return NULL;
    option->next = NULL;
    return option;
}

Action *initializeAction()
{
    Action *action = malloc(sizeof(Action));
    Option *option = initializeOption();
    if (action == NULL || option == NULL)
        return NULL;
    action->numberOfOptions = 0;
    action->firstOption = option;
    action->next = NULL;
    return action;
}

Task *initializeTask()
{
    Task *task = malloc(sizeof(Task));
    Action *action = initializeAction();
    if (task == NULL || action == NULL)
        return NULL;
    task->hour = 0;
    task->minute = 0;
    task->second = 0;
    task->numberOfActions = 0;
    task->firstAction = action;
    task->next = NULL;
    return task;
}

TaskList *initializeTaskList()
{
    TaskList *taskList = malloc(sizeof(TaskList));
    Task *task = initializeTask();
    if (taskList == NULL || task == NULL)
        return NULL;
    taskList->numberOfTasks = 0;
    taskList->firstTask = task;
    return taskList;
}

int initialize(TaskList **tasksList, Task **actionsList, int symbolParent)
{
    switch (symbolParent)
    {
    case TASK:
        if (*tasksList == NULL)
        {
            *tasksList = initializeTaskList();
        }
        else
        {
            addNewEmptyTaskInList(*tasksList);
        }
        break;
    case ACTION:
        if (*actionsList == NULL)
        {
            *actionsList = initializeTask();
        }
        else
        {
            addNewEmptyActionInList(*actionsList);
        }
        break;
    default:
        return 1;
    }
    return 0;
}

int checkSymbolOption(const char *line, int parentSymbol)
{
    if (line[0] == '{' && parentSymbol == TASK)
    {
        return DATA_TASK;
    }
    else if (line[0] == '{' && parentSymbol == ACTION)
    {
        return DATA_ACTION;
    }
    else if (line[0] == '(' && parentSymbol == OPTION)
    {
        return NEW_ACTION;
    }
    else if (line[0] == '{' && parentSymbol == OPTION)
    {
        return NEW_OPTION;
    }
    return -1;
}

void checkSymbolParent(const char *line, int *parentSymbol)
{
    if (strlen(line) > 0)
    {
        if (line[0] == '#')
            return;
        else if (line[0] == '=' && line[1] == '=' && TASK != *parentSymbol)
        {
            *parentSymbol = TASK;
        }
        else if (line[0] == '=' && ACTION != *parentSymbol)
        {
            *parentSymbol = ACTION;
        }
        else if (line[0] == '+' && OPTION != *parentSymbol)
        {
            *parentSymbol = OPTION;
        }
    }
}

TaskList *getTasksListFromConfigFile(const char *filename)
{
    ssize_t read = 0;
    char *line = NULL;
    size_t length = 0;
    TaskList *tasksList = NULL;
    Task *actionsList = NULL;
    int symbolParent = -1;
    FILE *configFile;
    int index = 0;
    char ***temporaryListOfActionsNamePerTask = malloc(sizeof(char **) * 10);
    int *temporaryNumberOfActionPerTask = malloc(sizeof(int) * 10);
    if (temporaryListOfActionsNamePerTask == NULL || temporaryNumberOfActionPerTask == NULL)
        return NULL;

    configFile = fopen(filename, "r");
    if (configFile == NULL)
    {
        printf("Config file could not have been opened.\n");
        return NULL;
    }

    while ((read = getline(&line, &length, configFile)) != -1)
    {
        if (line[0] == '#')
        {
            continue;
        }
        else if (line[0] != '{' && line[0] != '(')
        {
            checkSymbolParent(line, &symbolParent);
            if (symbolParent == TASK || symbolParent == ACTION)
            {
                int isInitialized = initialize(&tasksList, &actionsList, symbolParent);
                if (isInitialized != 0)
                {
                    printf("\nelement not initialized\n");
                    return NULL;
                }
            }
        }
        else
        {
            int optionType = checkSymbolOption(line, symbolParent);
            if (optionType != -1)
            {
                char *substr = extractDataFromConfigFile(line, optionType);
                if (substr == NULL)
                    return NULL;
                int numberOfOptions = 0;
                char **options = splitOption(substr, optionType, &numberOfOptions);
                if (optionType == DATA_ACTION)
                {
                    if (strcmp(options[0], "name") == 0)
                        setNameOfAction(actionsList, options[1]);
                    else if (strcmp(options[0], "url") == 0)
                        setUrlTargetOfAction(actionsList, options[1]);
                }
                else if (optionType == DATA_TASK)
                {
                    if (strcmp(options[0], "name") == 0)
                        setNameOfTask(tasksList->firstTask, options[1]);
                    else if (strcmp(options[0], "hour") == 0)
                        setHourOfTask(tasksList->firstTask, options[1]);
                    else if (strcmp(options[0], "minute") == 0)
                        setMinuteOfTask(tasksList->firstTask, options[1]);
                    else if (strcmp(options[0], "second") == 0)
                        setSecondOfTask(tasksList->firstTask, options[1]);
                }
                else if (optionType == NEW_OPTION)
                {
                    if (actionsList->firstAction->numberOfOptions == 0)
                    {
                        setOption(actionsList->firstAction->firstOption, options[0], options[1]);
                        actionsList->firstAction->numberOfOptions += 1;
                    }
                    else
                        addNewOptionInList(actionsList->firstAction, options[0], options[1]);
                }
                else if (optionType == NEW_ACTION)
                {
                    temporaryListOfActionsNamePerTask[index] = options;
                    temporaryNumberOfActionPerTask[index] = numberOfOptions;
                    index++;
                }
            }
        }
    }
    fclose(configFile);

    Task *tmp = tasksList->firstTask;
    for (int i = index - 1; i >= 0; i--)
    {
        for (int j = temporaryNumberOfActionPerTask[i] - 1; j >= 0; j--)
        {
            Action *searchedAction = findActionByNameInList(actionsList, temporaryListOfActionsNamePerTask[i][j]);
            if (searchedAction != NULL)
            {
                addCopyActionToList(tmp, unreferencedCopyAction(searchedAction));
            }
        }
        tmp = tmp->next;
    }

    free(tmp);

    return tasksList;
}