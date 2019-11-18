#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <ctype.h>

typedef struct Option Option;
typedef struct Action Action;
typedef struct Task Task;
typedef struct TaskList TaskList;

enum symbol
{
    COMMENT,
    OPTION,
    ACTION,
    TASK,
    DATA,
    DATA_TASK,
    DATA_ACTION,
    NEW_OPTION,
    NEW_ACTION,
    NEW_TASK
};

struct Option
{
    char *key;
    char *value;
    Option *next;
};

struct Action
{
    char *name;
    char *url;
    Option *firstOption;
    unsigned int numberOfOptions;
    Action *next;
};

struct Task
{
    char *name;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    Action *firstAction;
    unsigned int numberOfActions;
    Task *next;
};

struct TaskList
{
    Task *firstTask;
    unsigned numberOfTasks;
};

void setAllActionsInAllTasks(TaskList *, Task *, int, char ***, int *);

int prepareDataFromConfigFile(FILE *, TaskList **, Task **, char ***, int *);

void prepareActionData(Task *, char **);

void prepareTaskData(TaskList *, char **);

void initializeTaskOrAction(TaskList **, Task **, int *, const char *);

void setOption(Option *, const char *, const char *);

void setKeyOfOption(Option *, const char *);

void setValueOfOption(Option *, const char *);

void setAction(Task *, const char *, const char *);

void setNameOfAction(Task *, const char *);

void setUrlTargetOfAction(Task *, const char *);

void setTask(Task *, const char *, unsigned int, unsigned int);

void setNameOfTask(Task *, const char *);

void setTimeOfTask(Task *, unsigned int, unsigned int);

void setHourOfTask(Task *, const char *);

void setMinuteOfTask(Task *, const char *);

void setSecondOfTask(Task *, const char *);

void freeFirstOptionFromList(Action *);

void freeFirstActionFromList(Task *);

void freeTask(Task **);

void freeTaskFromList(TaskList *);

void addNewOptionInList(Action *, const char *, const char *);

void addNewActionInList(Task *, const char *, const char *);

void addNewEmptyOptionInList(Task *);

void addNewEmptyActionInList(Task *);

void addNewEmptyTaskInList(TaskList *);

void addNewTaskInList(TaskList *, const char *, unsigned int, unsigned int);

void displayOptionsList(Action *);

void displayActionsList(Task *);

void displayTasksList(TaskList *);

char *extractDataFromConfigFile(const char *, int);

char **splitOption(char *, int, int *);

char *trim(const char *);

void addCopyActionToList(Task *, Action *);

Action *unreferencedCopyAction(Action *);

int isAlpha(const char);

Action *findActionByNameInList(Task *, const char *);

Option *initializeOption();

Action *initializeAction();

Task *initializeTask();

TaskList *initializeTaskList();

int initialize(TaskList **, Task **, int);

TaskList *getTasksListFromConfigFile(const char *);

void checkSymbolParent(const char *, int *);

int checkSymbolOption(const char *, int);

#endif