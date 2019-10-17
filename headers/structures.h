typedef struct Option
{
    unsigned int maxDepth;
    unsigned int versioning;
    char** types;
} Option;


typedef struct Action
{
    char* name;
    char* url;
    Option* options;
    unsigned int numberOfOptions;
} Action;

typedef struct Task 
{
    char* name;
    Action* actions;
    unsigned int numberOfActions;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
} Task;
