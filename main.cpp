// Assignment #2
// Yahya Hassan
#include <iostream>
#include <unistd.h>
#include "header.h"
using namespace std;

void printTasks(int numTask, int exception)
{
    for (int i = 0; i < numTask; i++)
    {
        if (i == exception)
            cout << i + 1 << "- Current Task (Can't be chosen)" << endl;
        else
            cout << i + 1 << "- T" << i + 1 << endl;
    }
}

int validateInput(int uperBound, int lowerBound, int input)
{
    while (input < lowerBound || input > uperBound)
    {
        if (input < lowerBound)
        {
            cout << "Input should be Positive" << endl;
        }
        cout << "Invalid Input, Enter again: ";
        cin >> input;
        cout << endl;
    }
    return input;
}

int validateInputFloat(float uperBound, float lowerBound, float input)
{
    while (input < lowerBound || input > uperBound)
    {
        cout << "Invalid Input, Input should be between 0.1 and 1.0 Enter again: ";
        cin >> input;
    }
    return input;
}

int validateWithException(int upperBound, int lowerBound, int exception, int input)
{
    while (input == exception || input > upperBound || input < lowerBound)
    {
        if (input == exception)
        {
            cout << "T" << input << " cannot be its own dependency, Enter again: ";
        }
        else
            cout << "Invalid Input, Enter again: ";

        cin >> input;
    }
    return input;
}

int main()
{

    int numTask;
    int taskDuration;
    int choice;
    int taskChoice;
    int durationInput;
    int numDependencies;
    int numResources;
    string resourceName;
    float proficiencyLvl;
    bool independentTask = false;
    char resourceChoice;
    char charChoice;

    Project p1;
    p1.id = 0;

    system("clear");
    cout << "************************************* Project " << p1.id << " *************************************" << endl;
    cout << endl;
    cout << "-------------------------------- Project Resources Input --------------------------------" << endl;
    cout << endl;
    cout << "                    Enter the number of rescources for this Project" << endl;
    cin >> numResources;
    numResources = validateInput(INT32_MAX, 0, numResources);
    system("clear");

    for (int i = 0; i < numResources; i++)
    {
        cout << "-------------------------------- Project Resources Input --------------------------------" << endl;
        cout << endl;
        cout << "Rescource No." << i + 1 << endl;
        cout << endl;

        cout << "Enter Skill Name: " << endl;
        cin >> resourceName;
        cout << endl;

        cout << "Enter the Skill proficiency Level:" << endl;
        cin >> proficiencyLvl;
        // proficiencyLvl = validateInputFloat(1.0, 0.1, proficiencyLvl);
        while (proficiencyLvl > 1.0 || proficiencyLvl < 0.1)
        {
            cout << "Proficiency Level Should be in between 0 and 1.1: ";
            cin >> proficiencyLvl;
        }
        cout << endl;

        system("clear");
        Skill *newSkill = new Skill(i, resourceName, proficiencyLvl);
        p1.addResource(newSkill);
    }

    cout << "-------------------------------- Project Tasks Input --------------------------------" << endl;
    cout << endl;
    cout << "Enter the number of Tasks" << endl;
    cin >> numTask;
    numTask = validateInput(INT32_MAX, 1, numTask);
    p1.numTasks = numTask;
    p1.numResources = numResources;

    system("clear");

    cout << "                                    Tasks Created" << endl
         << endl;
    for (int i = 0; i < numTask; i++)
    {
        cout << "                                       " << i + 1 << "- T" << i + 1 << endl;
    }

    sleep(3);
    system("clear");
    // creating Tasks with their duration and adding them to project collection
    for (int i = 0; i < numTask; i++)
    {
        cout << "-------------------------------- Tasks Duration Input --------------------------------" << endl;
        cout << "Task T" << i + 1 << endl;
        cout << "Enter Duration ";
        cin >> taskDuration;
        taskDuration = validateInput(INT32_MAX, 1, taskDuration);
        p1.addTask(taskDuration);
        system("clear");
    }

    if (numResources != 0)
    {

        for (int i = 0; i < numTask; i++)
        {
            cout << "-------------------------------- Skill Required For a Task --------------------------------" << endl;

            cout << endl;
            cout << "                                        Task T" << i + 1 << endl;
            cout << "Do Task need a Resource? (y/n)" << endl;
            cin >> resourceChoice;
            while (1)
            {
                if (resourceChoice == 'y' || resourceChoice == 'n')
                    break;
                cout << "Input should be 'y' or 'n', Enter again" << endl;
                cin >> resourceChoice;
            }
            if (resourceChoice == 'y')
            {
                Resources *temp = p1.resourceCollection;
                cout << endl;
                cout << "Enter a Skill between 1-" << numResources << endl;
                for (int j = 1; j <= numResources; j++)
                {
                    cout << j << "- " << temp->skill->name << endl;
                    temp = temp->next;
                }

                cin >> choice;
                choice = validateInput(numResources, 1, choice);
                p1.addResourceDependents(choice - 1, i);
            }
            cout << endl;
            system("clear");
        }
    }

    // Adding dependencies of the task

    for (int i = 0; i < numTask - 1; i++)
    {
        cout << "-------------------------------- Task Dependencies On Other Tasks --------------------------------" << endl;
        cout << endl;
        cout << "How many task is Task T" << i + 1 << " dependent on: " << endl
             << endl;
        cin >> numDependencies;
        numDependencies = validateInput(numTask, 0, numDependencies);
        if (numDependencies == 0)
        {
            independentTask = true;
        }
        else
        {
            cout << endl;
            cout << "Enter " << numDependencies << " dependencie(s) from " << endl;
            printTasks(numTask, i);
            for (int j = 0; j < numDependencies; j++)
            {
                cin >> choice;
                choice = validateWithException(numTask, 1, i + 1, choice);
                p1.addTaskDependency(i, choice - 1);
            }
            cout << endl;
        }
        system("clear");
    }
    // We would only ask dependencies of the last task if there is already an independent task else last task would
    // be independent. Last task would be numTask-1
    if (independentTask)
    {
        cout << "-------------------------------- Task Dependencies On Other Tasks --------------------------------" << endl;
        cout << endl;
        int lastTask = numTask;
        cout << "How many task is Task T" << lastTask << " dependent on: ";
        cin >> numDependencies;
        numDependencies = validateInput(numTask, 0, numDependencies);
        if (numDependencies != 0)
        {
            cout << "Enter the " << numDependencies << " dependencies from " << endl;
            printTasks(numTask, lastTask);
            for (int j = 0; j < numDependencies; j++)
            {
                cin >> choice;
                choice = validateWithException(numTask, 1, lastTask, choice);
                p1.addTaskDependency(lastTask - 1, choice - 1);
            }
            cout << endl;
        }

        system("clear");
    }
    else
    {
        cout << "-------------------------------- Task Dependencies On Other Tasks --------------------------------" << endl;
        cout << endl;
        cout << "Task T" << numTask << " is Independent" << endl;
        cout << endl;
        cout << "Reason: As there is no Independent task in this project So far therfore Task T" << numTask << " is independent" << endl;
        sleep(4);
        system("clear");
    }

    // Asking if User Wants to Change duration of any Task
    cout << "-------------------------------- Changing Task Duration --------------------------------" << endl
         << endl;
    cout << "Choose " << endl;
    cout << "1- Specific Task" << endl;
    cout << "2- All Tasks" << endl;
    cout << "3- No Change in Duration" << endl;
    cin >> choice;
    choice = validateInput(3, 1, choice);
    if (choice == 1)
    {
        charChoice = 'y';
        while (charChoice == 'y')
        {
            cout << "-------------------------------- Changing Task Duration --------------------------------" << endl
                 << endl;

            cout << "Which Task Duration You want to change: " << endl;
            printTasks(numTask, numTask + 1);
            cin >> taskChoice;
            cout << "Enter its Duration: " << endl;
            cin >> durationInput;
            p1.set_nth_TaskDuration(taskChoice - 1, durationInput);
            cout << endl;
            cout << "Do u want to change duration of any other task(y/n) " << endl;
            cin >> charChoice;
            while (1)
            {
                if (charChoice == 'y' || charChoice == 'n')
                    break;
                cout << "Input should be 'y' or 'n', Enter again" << endl;
                cin >> charChoice;
            }
            system("clear");
        }
    }
    else if (choice == 2)
    {
        cout << "-------------------------------- Changing Task Duration --------------------------------" << endl
             << endl;
        cout << "Enter Duration" << endl;
        cin >> durationInput;
        durationInput = validateInput(INT32_MAX, 0, durationInput);
        p1.setTaskDuration(durationInput);
        system("clear");
    }
    else if (choice == 3)
    {
        system("clear");
    }

    cout << "Basic Schedule: " << endl
         << endl;
    p1.calculateBasicSchedule();
    cout << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------------------------" << endl
         << endl;

    if (numResources != 0)
    {
        cout << "Schedule With Resources: " << endl
             << endl;

        p1.completionTimeWithResources();
        cout << endl;
        cout << "------------------------------------------------------------------------------------------------------------------------------------------" << endl;

        cout << "Schedule With Resources and Proficiency: " << endl
             << endl;
        p1.completionTimeWithResourcesProficiency();
    }
    return 0;
}
