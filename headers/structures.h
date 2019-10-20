enum symbol { COMMENT, OPTION, ACTION, TASK, DATA };

typedef struct Option Option;
typedef struct Action Action;
typedef struct Task Task;
typedef struct TaskList TaskList;

struct Option {
    char* key;
    char* value;
    Option* next;
};

struct Action {
    char* name;
    char* url;
    Option* firstOption;
    unsigned int numberOfOptions;
    Action* next;
};

struct Task {
    char* name;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    Action* firstAction;
    unsigned int numberOfActions;
    Task* next;
};

struct TaskList {
    Task* firstTask;
    unsigned numberOfTasks;
};

void setOption(Option*, const char*, const char*);

void setKeyOfOption(Option*, const char*);

void setValueOfOption(Option*, const char*);

void setAction(Action*, const char*, const char*);

void setNameOfAction(Action*, const char*);

void setUrlTargetOfAction(Action*, const char*);

void setTask(Task*, const char*, unsigned int, unsigned int);

void setNameOfTask(Task*, const char*);

void setTimeOfTask(Task*, unsigned int, unsigned int);

void setHourOfTask(Task*, unsigned int);

void setMinuteOfTask(Task*, unsigned int);

void freeFirstOptionFromList(Action*);

void freeFirstActionFromList(Task*);

void freeTask(Task*);

void addNewOptionInList(Action*, const char*, const char*);

void addNewActionInList(Task*, const char*, const char*);

void addNewTaskInList(TaskList*, const char*, unsigned int, unsigned int);

void displayOptionsList(Action*);

void displayActionsList(Task*);

void displayTasksList(TaskList*);

//void setSecondOfTask(Task*, unsigned int); // to do when subject fixed with teachers

Action* initializeAction();

Task* initializeTask();

