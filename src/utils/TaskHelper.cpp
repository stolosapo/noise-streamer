#include "TaskHelper.h"
#include "StringHelper.h"

string build_full_task_name(Task* task)
{
    string name = task->getTaskName();
    vector<string> params = task->getParams();

    string paramsStr;
    join(params, '&', paramsStr);
    if (paramsStr != "")
    {
        name += "?" + paramsStr;
    }

    return name;
}
