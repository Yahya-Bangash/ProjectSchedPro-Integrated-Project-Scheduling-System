// Assignment #2
// Yahya Hassan

#include <iostream>
using namespace std;

class Skill
{
public:
    int id;
    string name;
    float proficiencyLvl;

    Skill()
    {
        id = -1;
        name = "";
        proficiencyLvl = -1.0;
    }

    Skill(int id, string name, float proficiencyLvl)
    {
        this->id = id;
        this->name = name;
        this->proficiencyLvl = proficiencyLvl;
    }
};

class Task;

class Dependency
{
public:
    Task *taskInfo;
    Dependency *next;
    Dependency()
    {
        taskInfo = nullptr;
        next = nullptr;
    }
    Dependency(Task *val)
    {
        taskInfo = val;
        next = nullptr;
    }
};

class Task
{
public:
    int duration;
    int id;
    int resourceTime;
    int resourceId;
    int slack;
    int earlyStart;
    int earlyFinish;
    int lateStart;
    int lateFinish;
    Task *next;
    Dependency *taskDependencies;
    // Resource* recources;
    // Skill* skill dependencies;

    Task()
    {
        duration = 0;
        earlyStart = -1;
        earlyFinish = -1;
        resourceTime = 0;
        resourceId = -1;
        slack = -1;
        lateStart = INT32_MAX;
        lateFinish = INT32_MAX;
        next = nullptr;
        taskDependencies = nullptr;
    }

    // add in the dependency LinkedList of task
    void addTaskDependency(Task *dependencyTask)
    {
        Dependency *newDependency = new Dependency;
        newDependency->taskInfo = dependencyTask;
        newDependency->next = nullptr;
        if (!this->taskDependencies)
        {
            this->taskDependencies = newDependency;
        }
        else
        {
            Dependency *temp = this->taskDependencies;
            while (temp->next)
            {
                temp = temp->next;
            }
            temp->next = newDependency;
        }
    }

    // Print the dependency LinkedList in a task
    void printTaskDependencyList()
    {
        if (!taskDependencies)
        {
            cout << " None";
            return;
        }

        Dependency *temp = taskDependencies;
        while (temp)
        {
            cout << "T" << temp->taskInfo->id + 1 << " ";
            temp = temp->next;
        }
    }

    // Display resource ID if there is any
    void displayResourceId()
    {
        if (this->resourceId == -1)
        {
            cout << "-";
        }
        else
        {
            cout << this->resourceId + 1;
        }
    }
    // Checks if the Early Start and Early Finish assigned
    bool isEarlyTimeAssigned()
    {
        Dependency *temp = this->taskDependencies;
        while (temp)
        {
            if (temp->taskInfo->earlyStart == -1 || temp->taskInfo->earlyFinish == -1)
                return 0;

            temp = temp->next;
        }
        return 1;
    }

    // Return greatest Early Finish in Dependencies of the calling task
    int greatestEarlyFinishInDependencies()
    {
        int max = -1;
        Dependency *temp = this->taskDependencies;
        while (temp)
        {
            if (temp->taskInfo->earlyFinish > max)
                max = temp->taskInfo->earlyFinish;

            temp = temp->next;
        }
        return max;
    }

    // Checks if the parameter Task is present in the dependency list of calling Task
    bool isDependencyPresent(Task *currentTask)
    {
        Dependency *temp = this->taskDependencies;
        while (temp)
        {
            if (temp->taskInfo->id == currentTask->id)
                return 1;

            temp = temp->next;
        }
        return 0;
    }

    // Assign the late start and late finish for dependencies
    void assignLateForDependencies()
    {

        Dependency *temp = this->taskDependencies;
        while (temp)
        {
            // if the late start of the dependency is smaller than its dependent task
            if (this->lateStart < temp->taskInfo->lateFinish)
            {

                temp->taskInfo->lateFinish = this->lateStart;

                temp->taskInfo->lateStart = temp->taskInfo->lateFinish - temp->taskInfo->duration;

                temp->taskInfo->slack = temp->taskInfo->lateFinish - temp->taskInfo->earlyFinish;

                // cout << "Parent Id: T" << this->id << endl;
                // cout << "Child id: T" << temp->taskInfo->id << endl;
                // cout << "late Start=late Finish-duration  : " << temp->taskInfo->lateStart << "=" << temp->taskInfo->lateFinish << " - " << temp->taskInfo->duration << endl;
                // cout << "Late Finish=this.lateStart  : " << temp->taskInfo->lateFinish << "=" << this->lateStart << endl;
            }

            temp = temp->next;
        }
    }
};

class Resources
{
public:
    int id;
    Skill *skill;
    Resources *next;
    Dependency *dependentTasks;

    Resources()
    {
        id = -1;
        skill = nullptr;
        next = nullptr;
        dependentTasks = nullptr;
    }

    Resources(int id, Skill *skill)
    {
        this->id = id;
        this->skill = skill;
        next = nullptr;
        dependentTasks = nullptr;
    }

    void addDependentTask(Task *newTask)
    {
        Dependency *newDependentTask = new Dependency(newTask);

        if (!this->dependentTasks)
        {
            this->dependentTasks = newDependentTask;
        }
        else
        {
            Dependency *temp = this->dependentTasks;
            while (temp->next)
            {
                temp = temp->next;
            }

            temp->next = newDependentTask;
        }
    }

    void printResourceDependents()
    {
        if (!this->dependentTasks)
        {
            cout << "Resource Dependent Task is Empty" << endl;
        }
        Dependency *temp = this->dependentTasks;
        while (temp)
        {
            cout << temp->taskInfo->id << " ";
            temp = temp->next;
        }
    }
};

string spaceFormating(int num)
{
    if (num > 9)
        return "         ";
    else
        return "          ";
}

class Project
{
public:
    int id;
    int duration;
    int current_task_id;
    int numTasks;
    int numResources;
    Task *taskCollection;
    Task *tempTaskCollection;
    Task *lastTask;
    Resources *lastRescource;
    Resources *resourceCollection;

    Project()
    {

        taskCollection = nullptr;
        tempTaskCollection = nullptr;
        lastTask = nullptr;
        resourceCollection = nullptr;
        lastRescource = nullptr;
        current_task_id = 0;
        int numResources = 0;
    }

    Project(int numTask, int numResource)
    {
        numTasks = numTask;
        numResources = numResource;
        taskCollection = nullptr;
        tempTaskCollection = nullptr;
        lastTask = nullptr;
        resourceCollection = nullptr;
        lastRescource = nullptr;
        current_task_id = 0;
    }

    // Changes the Duration Of a Specific Task
    void set_nth_TaskDuration(int taskChoice, int durationInput)
    {
        Task *temp = taskCollection;
        for (int i = 0; i < taskChoice; i++)
        {
            temp = temp->next;
        }
        temp->duration = durationInput;
    }

    // Changes Duration of All Tasks
    void setTaskDuration(int duration)
    {
        Task *temp = taskCollection;
        while (temp)
        {
            temp->duration = duration;
            temp = temp->next;
        }
    }

    void addResource(Skill *newSkill)
    {

        Resources *newResource = new Resources(newSkill->id, newSkill);
        newResource->next = nullptr;

        if (!resourceCollection)
        {
            resourceCollection = newResource;
            lastRescource = newResource;
        }
        else
        {
            lastRescource->next = newResource;
            lastRescource = newResource;
        }
    }

    // To insert a Task in taskCollection
    void addTask(int duration)
    {
        // creating new node
        Task *newNode = new Task;
        newNode->duration = duration;
        newNode->next = nullptr;
        // assigning id to new task
        newNode->id = current_task_id;
        current_task_id++;

        // inserting in task collection if it is empty or not
        if (!taskCollection)
        {
            taskCollection = newNode;
            lastTask = newNode;
        }
        else
        {
            lastTask->next = newNode;
            lastTask = newNode;
        }
    }

    // adding taskdependency of a task
    void addTaskDependency(int dependentTaskIndex, int dependencyTaskindex)
    {
        Task *dependentTask = taskCollection;

        for (int i = 0; i < dependentTaskIndex; i++)
        {
            dependentTask = dependentTask->next;
        }

        Task *dependencyTask = taskCollection;
        for (int i = 0; i < dependencyTaskindex; i++)
        {
            dependencyTask = dependencyTask->next;
        }

        dependentTask->addTaskDependency(dependencyTask);
    }

    void forwardPass()
    {
        int count = 0; // count defines how many tasks have es and sf assigned

        while (count <= numTasks + 10)
        {
            Task *temp = taskCollection;
            while (temp)
            {

                // Setting the es and ef of the independent task
                if (!temp->taskDependencies)
                {
                    temp->earlyStart = 0 + temp->resourceTime;
                    temp->earlyFinish = temp->earlyStart + temp->duration;
                    count++;
                }
                // If it have dependencies then it es and ef will depend on the dependencies
                else
                {
                    // if es and ef of the its dependencies is assigned
                    if (temp->isEarlyTimeAssigned())
                    {
                        temp->earlyStart = temp->greatestEarlyFinishInDependencies() + temp->resourceTime;
                        temp->earlyFinish = temp->earlyStart + temp->duration;
                        count++;
                    }
                }

                temp = temp->next;
            }
        }
    }

    int projectTotalTime()
    {
        Task *temp = taskCollection;
        int max = -1;
        while (temp)
        {
            if (temp->earlyFinish > max)
            {
                max = temp->earlyFinish;
            }

            temp = temp->next;
        }
        return max;
    }

    void backwardPass()
    {
        for (int i = 0; i < numTasks + 4; i++)
        {
            Task *temp = taskCollection;
            while (temp)
            {
                // If the current task is dependency of no task then this means its last task
                if (!this->isAnyTaskDependency(temp))
                {
                    int projectTime = this->projectTotalTime();
                    temp->lateFinish = projectTime;
                    temp->lateStart = temp->lateFinish - temp->duration;
                    temp->slack = temp->lateFinish - temp->earlyFinish;
                }
                // If the late times of the current task is assigned then we would assigne them to its dependencies
                // if it is smaller then late time of its dependencies
                if (temp->lateStart != INT32_MAX && temp->lateFinish != INT32_MAX)
                {
                    temp->assignLateForDependencies();
                }

                temp = temp->next;
            }
        }
    }

    void createTempTaskCollection()
    {
        Task *temp = taskCollection;
        tempTaskCollection = new Task;
        tempTaskCollection->earlyStart = temp->earlyStart;
        tempTaskCollection->earlyFinish = temp->earlyFinish;
        tempTaskCollection->lateStart = temp->lateStart;
        tempTaskCollection->lateFinish = temp->lateFinish;
        Task *tempTail = tempTaskCollection;
        temp = temp->next;

        while (temp)
        {
            Task *newTask = new Task;
            newTask->earlyStart = temp->earlyStart;
            newTask->earlyFinish = temp->earlyFinish;
            newTask->lateStart = temp->lateStart;
            newTask->lateFinish = temp->lateFinish;
            tempTail->next = newTask;
            tempTail = newTask;

            temp = temp->next;
        }
    }

    // calculate Basic Schedule
    void calculateBasicSchedule()
    {
        this->forwardPass();
        this->backwardPass();
        this->createTempTaskCollection();
        this->Display();
    }

    // Compare dependent tasks of each resource and make changes in resourceTime
    void completionTimeWithResources()
    {
        Resources *temp = resourceCollection;
        for (int i = 0; i < numResources; i++)
        {
            Dependency *currentTask = temp->dependentTasks;
            while (currentTask)
            {
                Dependency *comparingTask = temp->dependentTasks;
                while (comparingTask)
                {
                    // cout << "Entering Comparing loop" << endl;

                    // Problem in resource allocation occurs only when the early start of both the dependent tasks of resource are equal
                    if (currentTask->taskInfo->earlyStart == comparingTask->taskInfo->earlyStart)
                    {
                        if (currentTask->taskInfo->lateStart < comparingTask->taskInfo->lateStart)
                        {
                            // incrementing resource time of the comparing task by late finish of the current task
                            comparingTask->taskInfo->resourceTime += currentTask->taskInfo->duration;
                            // cout << "Incrementing resiurceTime through first if" << endl;
                        }
                        else if ((currentTask->taskInfo->lateStart == comparingTask->taskInfo->lateStart) &&
                                 (currentTask->taskInfo->slack < comparingTask->taskInfo->slack))
                        {
                            // if late start is equal then compare for least slack time
                            comparingTask->taskInfo->resourceTime += currentTask->taskInfo->duration;
                            // cout << "Incrementing resiurceTime through second if" << endl;
                        }
                        else if ((currentTask->taskInfo->lateStart == comparingTask->taskInfo->lateStart) &&
                                 (currentTask->taskInfo->slack == comparingTask->taskInfo->slack) &&
                                 (currentTask->taskInfo->duration < comparingTask->taskInfo->duration))
                        {
                            // if late start and slack are equal then compare for lowest duration
                            comparingTask->taskInfo->resourceTime += currentTask->taskInfo->duration;
                            // cout << "Incrementing resiurceTime through third if" << endl;
                        }
                    }

                    comparingTask = comparingTask->next;
                }

                currentTask = currentTask->next;
            }
            temp = temp->next;
        }

        // calling forward and backward pass to calculate overall time
        this->forwardPass();

        // Reasigning Late Finishs of every task to MAX_Int in order for assignLateForDependencies() to work in backward pass
        this->resetLateFinishes();
        this->backwardPass();
        this->displayWithResources();
    }

    void completionTimeWithResourcesProficiency()
    {
        Resources *temp = resourceCollection;

        // Increamenting duration of every Task in resourceDependents according to proficiency
        while (temp)
        {
            Dependency *resourceDependents = temp->dependentTasks;
            while (resourceDependents)
            {
                // Dividing Every Task Dependent on the current Resource with its proficiency Level
                resourceDependents->taskInfo->duration = (resourceDependents->taskInfo->duration / temp->skill->proficiencyLvl);
                resourceDependents = resourceDependents->next;
            }

            temp = temp->next;
        }

        // Reseting the Early & late times and resource time to basic shedule.
        Task *temp1 = taskCollection;
        Task *temp2 = tempTaskCollection;
        while (temp1)
        {
            temp1->earlyStart = temp2->earlyStart;
            temp1->earlyFinish = temp2->earlyFinish;
            temp1->lateStart = temp2->lateStart;
            temp1->lateFinish = temp2->lateFinish;
            temp1->resourceTime = 0;

            temp1 = temp1->next;
            temp2 = temp2->next;
        }

        completionTimeWithResources();
    }

    // To reset Late Finishes of all tasks to zero in order for backward Pass to run correctly
    void resetLateFinishes()
    {
        Task *temp = taskCollection;
        while (temp)
        {
            temp->lateFinish = INT32_MAX;
            temp = temp->next;
        }
    }

    // Check if the currentTask is dependency of any task in the Task Collection
    bool isAnyTaskDependency(Task *currentTask)
    {
        Task *temp = taskCollection;
        while (temp)
        {
            if (temp->isDependencyPresent(currentTask))
            {
                return 1;
            }

            temp = temp->next;
        }
        return 0;
    }

    // add Tasks in the DependentTask of the specific resource
    void addResourceDependents(int resourceIndex, int taskIndex)
    {
        // cout << "Entering addResourceDependents" << endl;
        Resources *resourceTemp = resourceCollection;
        for (int i = 0; i < resourceIndex; i++)
        {
            resourceTemp = resourceTemp->next;
        }

        // cout << "Resource Temp id is " << resourceTemp->id << endl;

        Task *taskTemp = taskCollection;
        for (int i = 0; i < taskIndex; i++)
        {
            taskTemp = taskTemp->next;
        }
        taskTemp->resourceId = resourceTemp->id;
        // cout << "Task temp id is" << taskTemp->id << endl;

        // cout << "Leaving addResourceDependents" << endl;
        resourceTemp->addDependentTask(taskTemp);
    }

    // Print Critical Tasks And there Duration.
    void printCriticalTasks()
    {
        int sum = 0;
        Task *temp = taskCollection;
        cout << "Critical Tasks: ";
        while (temp)
        {
            if (temp->slack == 0)
            {
                sum += temp->duration;
                cout << "T" << temp->id + 1 << " ";
            }
            temp = temp->next;
        }
        cout << endl;
        cout << "Critical Tasks Duration Sum: " << sum << endl;
    }

    // Displaying task in Project
    void Display()
    {
        Task *temp = taskCollection;
        cout << "ID      Duration      ES         EF         LS         LF       Slack     Task Dependency List" << endl;
        while (temp)
        {
            cout << temp->id + 1 << spaceFormating(temp->id + 1) << temp->duration << spaceFormating(temp->duration) << temp->earlyStart << spaceFormating(temp->earlyStart) << temp->earlyFinish << spaceFormating(temp->earlyFinish) << temp->lateStart << spaceFormating(temp->lateStart) << temp->lateFinish << spaceFormating(temp->lateFinish) << temp->slack << spaceFormating(temp->slack);
            temp->printTaskDependencyList();
            cout << endl;
            temp = temp->next;
        }
        cout << endl;
        cout << "Project Completion Time: " << this->projectTotalTime() << endl;
        this->printCriticalTasks();
    }

    void displayWithResources()
    {
        Task *temp = taskCollection;
        cout << "ID      Duration      ES         EF         LS         LF       Slack     Resource ID    Task Dependency List" << endl;
        while (temp)
        {
            cout << temp->id + 1 << spaceFormating(temp->id + 1) << temp->duration << spaceFormating(temp->duration) << temp->earlyStart << spaceFormating(temp->earlyStart) << temp->earlyFinish << spaceFormating(temp->earlyFinish) << temp->lateStart << spaceFormating(temp->lateStart) << temp->lateFinish << spaceFormating(temp->lateFinish) << temp->slack << spaceFormating(temp->slack);
            temp->displayResourceId();
            cout << spaceFormating(temp->resourceId);
            temp->printTaskDependencyList();
            cout << endl;
            temp = temp->next;
        }
        cout << endl;
        cout << "Project Completion Time: " << this->projectTotalTime() << endl;
        this->printCriticalTasks();
    }

    void displayRescources()
    {
        Resources *temp = resourceCollection;
        while (temp)
        {
            cout << endl;
            cout << "Rescource Id: " << temp->id << endl;
            cout << "Resource Name: " << temp->skill->name << endl;
            cout << "SKill Proficiency Level: " << temp->skill->proficiencyLvl << endl;
            cout << "Task Dependents: ";
            temp->printResourceDependents();
            cout << endl;

            temp = temp->next;
        }
    }
};
