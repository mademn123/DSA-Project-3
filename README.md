# DSA-Project-3

This project is a Task Scheduler application built using C++, Qt, and CLion. It allows users to manage tasks, add dependencies, visualize tasks in a graph, and generate schedules based on task priorities and dependencies. The application features a user-friendly GUI with support for task management, dependency handling, and interactive visualization.

Features

Add New Tasks: Input task details including name, due date, priority, description, and duration.

Manage Dependencies: Add or remove dependencies between tasks.

Generate Task Schedule: Automatically generate a schedule that respects task dependencies.

Task Visualization: Visualize tasks as nodes with edges representing dependencies.

Priority Graph: View the number of tasks by priority in a bar graph.

Task Sorting: Sort tasks by due date for better management.

Requirements:

Qt 5 or 6 

CMake 

CLion IDE 

C++14

Usage:

Add Tasks: Enter the task name, due date, priority, description, and duration. Click Add Task.

Add Dependencies: Select two tasks and create dependencies between them.

Visualize Tasks: Click Visualize Tasks to see a graphical representation of the tasks and their dependencies.

Generate Schedule: Click Generate Schedule to see a suggested order of task completion.

Save and Load Tasks: Save your tasks to a file or load existing tasks.

View Priority Graph: Click Show Priority Graph to visualize the number of tasks by priority.

File Structure:

main.cpp: Entry point of the application.

Task.h/Task.cpp: Defines the Task structure and operations for adding/removing dependencies.

TaskItem.h/TaskItem.cpp: Represents the graphical item used for visualizing tasks.

TaskScheduler.h/TaskScheduler.cpp: Contains the main logic and GUI for the application.

Requirements for Running the Code:

Ensure that Qt is properly installed and the environment variable Qt_DIR is set to the correct path.

Use a compatible compiler (e.g., MSVC for Windows, GCC for Linux/macOS) that matches the Qt version.

Make sure CMakeLists.txt is correctly configured with the appropriate paths.

Installation and Setup:

Install Qt:

Download and install Qt from https://www.qt.io/download.

Make a note of the installation path, as it will be needed later.

Install CLion:

Download and install CLion from https://www.jetbrains.com/clion/.

Ensure that the Qt plugin for CLion is installed via File -> Settings -> Plugins.

Clone the Repository:

Clone the project repository to your local machine:

git clone <repository_url>

Configure CMake:

Open the project in CLion.

Modify the CMakeLists.txt file to point to your Qt installation:

set(CMAKE_PREFIX_PATH "<Qt_installation_path>/lib/cmake")
find_package(Qt5 COMPONENTS Widgets REQUIRED)

Replace <Qt_installation_path> with the actual path where Qt is installed on your system.

Build the Project:

Click Build in CLion to compile the project. Ensure that the CMake settings are correctly configured.

Run the Application:

Click the Run button in CLion to start the Task Scheduler application.
