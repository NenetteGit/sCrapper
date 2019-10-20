#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/structures.h"

void setOption(Option* option, const char* key, const char* value) {
    setKeyOfOption(option, key);
    setValueOfOption(option, value);
}

void setKeyOfOption(Option* option, const char* key) {
    if(strlen(key) > 0) {
        option->key = malloc(sizeof(char) * (strlen(key) + 1));
        if(option->key == NULL) return;
        strcpy(option->key, key);
        option->key[strlen(key)] = '\0';
    }
}

void setValueOfOption(Option* option, const char* value) {
    if(strlen(value) > 0) {
        option->value = malloc(sizeof(char) * (strlen(value) + 1));
        if(option->value == NULL) return;
        strcpy(option->value, value);
        option->value[strlen(value)] = '\0';
    }
}

void setAction(Action* action, const char* name, const char* url) {
    setNameOfAction(action, name);
    setUrlTargetOfAction(action, url);
}

void setUrlTargetOfAction(Action* action, const char* url) {
    if(strlen(url) > 0) {
        action->url = malloc(sizeof(char) * (strlen(url) + 1));
        if(action->url == NULL) return;
        strcpy(action->url, url);
        action->url[strlen(url)] = '\0';
    }
}

void setNameOfAction(Action* action, const char* name) {
    if(strlen(name) > 0) {
        action->name = malloc(sizeof(char) * (strlen(name) + 1));
        if(action->name == NULL) return;
        strcpy(action->name, name);
        action->name[strlen(name)] = '\0';
    }
}

void setTask(Task* task, const char* name, unsigned int hour,
unsigned int minute) {
    setNameOfTask(task, name);
    setTimeOfTask(task, hour, minute);
}

void setNameOfTask(Task* task, const char* name) {
    if(strlen(name) > 0) {
        task->name = malloc(sizeof(char) * (strlen(name) + 1));
        if(task->name == NULL) return;
        strcpy(task->name, name);
        task->name[strlen(name)] = '\0';
    }
}

void setTimeOfTask(Task* task, unsigned int hour,
unsigned int minute) {
    task->hour = hour;
    task->minute = minute;
    task->second = 0;
}

void setHourOfTask(Task* task, unsigned int hour) {
    task->hour = hour;
}

void setMinuteOfTask(Task* task, unsigned int minute) {
    task->minute = minute;
}

void freeFirstOptionFromList(Action* action) {
    if(action->firstOption != NULL) {
        Option* optionToDelete = action->firstOption;
        if(optionToDelete->key != NULL && optionToDelete->value != NULL) {
            free(optionToDelete->key);
            free(optionToDelete->value);
        }
        action->firstOption = action->firstOption->next;
        free(optionToDelete);
        action->numberOfOptions--;
    }
}

void freeFirstActionFromList(Task* task) {
    if(task->firstAction != NULL) {
        Action* actionToDelete = task->firstAction;
        if (actionToDelete->name != NULL && actionToDelete->url) {
            free(actionToDelete->name);
            free(actionToDelete->url);
        }
        while (actionToDelete->firstOption != NULL) {
            freeFirstOptionFromList(actionToDelete);
        }
        task->firstAction = task->firstAction->next;
        free(actionToDelete);
        task->numberOfActions--;
    }
}

void freeTask(Task*);

void addNewOptionInList(Action* action, const char* key, const char* value) {
    Option* newOption = malloc(sizeof(Option));
    if(action->firstOption == NULL || newOption == NULL) {
        printf("\nerror\n");
        return;
    }
    setOption(newOption, key, value);
    newOption->next = action->firstOption;
    action->firstOption = newOption;
    action->numberOfOptions++;
}

void addNewActionInList(Task* task, const char* name, const char* url) {
    Action* newAction = initializeAction();
    if(task->firstAction == NULL || newAction == NULL) return;
    setAction(newAction, name, url);
    newAction->next = task->firstAction;
    task->firstAction = newAction;
    task->numberOfActions++;
}

void addNewTaskInList(TaskList* taskList, const char* name, unsigned int hour, unsigned int minute) {
    Task* newTask = initializeTask();
    if(taskList->firstTask == NULL || newTask == NULL) return;
    setTask(newTask, name, hour, minute);
    newTask->next = taskList->firstTask;
    taskList->firstTask = newTask;
    taskList->numberOfTasks++;
}

void displayOptionsList(Action* action) {
    Option* optionToDisplay = action->firstOption;
    int i = 0;
    while (optionToDisplay != NULL) {
        printf("Option %d:\nkey: %s\nvalue: %s\n\n", i + 1, optionToDisplay->key, optionToDisplay->value);
        i++;
        optionToDisplay = optionToDisplay->next;
    }
    printf("\n");
}

void displayActionsList(Task* task) {
    Action* actionToDisplay = task->firstAction;
    int i = 0;
    while (actionToDisplay != NULL) {
        printf("Action %d:\nname: %s\nurl: %s\n", i + 1, actionToDisplay->name, actionToDisplay->url);
        displayOptionsList(actionToDisplay);
        i++;
        actionToDisplay = actionToDisplay->next;
    }
    printf("\n");
}

void displayTasksList(TaskList* taskList) {
    Task* taskToDisplay = taskList->firstTask;
    int i = 0;
    while (taskToDisplay != NULL) {
        printf("Task %d:\nname: %s\nhour: %u\nminute: %u\nsecond: %u\n", i + 1, taskToDisplay->name, taskToDisplay->hour, taskToDisplay->minute, taskToDisplay->second);
        displayActionsList(taskToDisplay);
        i++;
        taskToDisplay = taskToDisplay->next;
    }
    printf("\n");
}

Action* initializeAction() {
    Action* action = malloc(sizeof(Action));
    Option* option = malloc(sizeof(Option));
    if(action == NULL || option == NULL) return NULL;
    option->next = NULL;
    action->numberOfOptions = 0;
    action->firstOption = option;
    action->next = NULL;
    return action;
}

Task* initializeTask() {
    Task* task = malloc(sizeof(Task));
    Action* action = initializeAction();
    if(task == NULL || action == NULL) return NULL;
    task->hour = 0;
    task->minute = 0;
    task->second = 0;
    task->numberOfActions = 0;
    task->firstAction = action;
    task->next = NULL;
    return task;
}