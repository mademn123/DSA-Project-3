#include "scheduleHelper.h"

// Constructor for the TaskScheduler class
TaskScheduler::TaskScheduler(QWidget *parent) : QMainWindow(parent) {
    // Set the window title and size
    setWindowTitle("Priority Task Scheduler");
    setGeometry(100, 100, 800, 600);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Set dark theme for the main window
    this->setStyleSheet("background-color: #2b2b2b; color: #ffffff;");

    // Style input fields
    QString inputStyle = "QLineEdit, QDateTimeEdit, QSpinBox, QTextEdit { "
                         "background-color: #3a3a3a; "
                         "color: #ffffff; "
                         "border: 1px solid #555555; "
                         "border-radius: 4px; "
                         "padding: 2px; }";

    // Style buttons
    QString buttonStyle = "QPushButton { "
                          "background-color: #4CAF50; "
                          "color: white; "
                          "border: none; "
                          "padding: 5px 10px; "
                          "border-radius: 4px; }"
                          "QPushButton:hover { background-color: #45a049; }";

    // Apply button style globally
    qApp->setStyleSheet(buttonStyle);

    QGroupBox *inputGroup = new QGroupBox("Add New Task");
    QGridLayout *inputLayout = new QGridLayout(inputGroup);

    // Initialize task input fields
    nameInput = new QLineEdit();
    dueDateInput = new QDateTimeEdit(QDateTime::currentDateTime());
    dueDateInput->setCalendarPopup(true);
    QCalendarWidget *calendar = new QCalendarWidget();
    calendar->setGridVisible(true);
    dueDateInput->setCalendarWidget(calendar);
    priorityInput = new QLineEdit();
    descriptionInput = new QTextEdit();
    durationInput = new QSpinBox();
    durationInput->setRange(1, 10080); // 1 minute to 1 week (7 × 24 × 60)
    durationInput->setSuffix(" minutes");

    // Add input widgets to the layout
    inputLayout->addWidget(new QLabel("Name:"), 0, 0);
    inputLayout->addWidget(nameInput, 0, 1);
    inputLayout->addWidget(new QLabel("Due Date:"), 1, 0);
    inputLayout->addWidget(dueDateInput, 1, 1);
    inputLayout->addWidget(new QLabel("Priority:"), 2, 0);
    inputLayout->addWidget(priorityInput, 2, 1);
    inputLayout->addWidget(new QLabel("Description:"), 3, 0);
    inputLayout->addWidget(descriptionInput, 3, 1);
    inputLayout->addWidget(new QLabel("Duration:"), 4, 0);
    inputLayout->addWidget(durationInput, 4, 1);

    QPushButton *addButton = new QPushButton("Add Task");
    inputLayout->addWidget(addButton, 5, 0, 1, 2);

    mainLayout->addWidget(inputGroup);

    // Set up the group for managing task dependencies
    QGroupBox *depGroup = new QGroupBox("Manage Dependencies");
    QGridLayout *depLayout = new QGridLayout(depGroup);

    taskCombo1 = new QComboBox();
    taskCombo2 = new QComboBox();

    depLayout->addWidget(new QLabel("Task:"), 0, 0);
    depLayout->addWidget(taskCombo1, 0, 1);
    depLayout->addWidget(new QLabel("Depends On:"), 1, 0);
    depLayout->addWidget(taskCombo2, 1, 1);

    QHBoxLayout *depButtonLayout = new QHBoxLayout();
    QPushButton *addDepButton = new QPushButton("Add Dependency");
    QPushButton *removeDepButton = new QPushButton("Remove Dependency");
    depButtonLayout->addWidget(addDepButton);
    depButtonLayout->addWidget(removeDepButton);

    depLayout->addLayout(depButtonLayout, 2, 0, 1, 2);

    addDepButton->setStyleSheet(buttonStyle);
    removeDepButton->setStyleSheet(buttonStyle);

    mainLayout->addWidget(depGroup);

    taskList = new QTreeWidget();
    taskList->setHeaderLabels({"Name", "Due Date", "Priority", "Duration", "Dependencies", "Description"});
    mainLayout->addWidget(taskList);

    taskList->installEventFilter(this);

    // Set up action buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *scheduleButton = new QPushButton("Generate Schedule");
    QPushButton *saveButton = new QPushButton("Save Tasks");
    QPushButton *loadButton = new QPushButton("Load Tasks");
    QPushButton *visualizeButton = new QPushButton("Visualize Tasks");
    QPushButton *removeButton = new QPushButton("Remove Task");
    QPushButton *adjacencyListButton = new QPushButton("Show Adjacency List");
    QPushButton *sortButton = new QPushButton("Sort by Due Date");
    QPushButton *showGraphButton = new QPushButton("Show Priority Graph");
    QPushButton *exitButton = new QPushButton("Exit");

    buttonLayout->addWidget(scheduleButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(visualizeButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(adjacencyListButton);
    buttonLayout->addWidget(sortButton);
    buttonLayout->addWidget(showGraphButton);
    buttonLayout->addWidget(exitButton);

    mainLayout->addLayout(buttonLayout);

    connect(addButton, &QPushButton::clicked, this, &TaskScheduler::addTaskFromUI);
    connect(addDepButton, &QPushButton::clicked, this, &TaskScheduler::addDependencyFromUI);
    connect(removeDepButton, &QPushButton::clicked, this, &TaskScheduler::removeDependency);
    connect(scheduleButton, &QPushButton::clicked, this, &TaskScheduler::generateSchedule);
    connect(saveButton, &QPushButton::clicked, this, &TaskScheduler::saveTasks);
    connect(loadButton, &QPushButton::clicked, this, &TaskScheduler::loadTasks);
    connect(visualizeButton, &QPushButton::clicked, this, &TaskScheduler::visualizeTasks);
    connect(removeButton, &QPushButton::clicked, this, &TaskScheduler::removeTask);
    connect(taskList, &QTreeWidget::itemSelectionChanged, this, &TaskScheduler::displayTaskDetails);
    connect(adjacencyListButton, &QPushButton::clicked, this, &TaskScheduler::printAdjacencyList);
    connect(sortButton, &QPushButton::clicked, this, &TaskScheduler::sortTasksByDueDate);
    connect(showGraphButton, &QPushButton::clicked, this, &TaskScheduler::showPriorityGraph);
    connect(exitButton, &QPushButton::clicked, this, &TaskScheduler::exitApplication);

    nameInput->setStyleSheet(inputStyle);
    dueDateInput->setStyleSheet(inputStyle);
    priorityInput->setStyleSheet(inputStyle);
    descriptionInput->setStyleSheet(inputStyle);
    durationInput->setStyleSheet(inputStyle);

    addButton->setStyleSheet(buttonStyle);
    scheduleButton->setStyleSheet(buttonStyle);
    saveButton->setStyleSheet(buttonStyle);
    loadButton->setStyleSheet(buttonStyle);
    visualizeButton->setStyleSheet(buttonStyle);
    removeButton->setStyleSheet(buttonStyle);
    adjacencyListButton->setStyleSheet(buttonStyle);
    sortButton->setStyleSheet(buttonStyle);
    showGraphButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);

    // Style the QTreeWidget
    taskList->setAlternatingRowColors(true);
    taskList->setStyleSheet("QTreeWidget { "
                            "background-color: #1a1a1a; "
                            "color: #ffffff; "
                            "alternate-background-color: #2a2a2a; }"
                            "QTreeWidget::item:selected { "
                            "background-color: #3a3a3a; "
                            "color: #ffffff; }"
                            "QTreeWidget::item { "
                            "border-bottom: 1px solid #3a3a3a; "
                            "padding: 2px 0; }");

    // Set up QTimer for periodic updates
    QTimer *colorUpdateTimer = new QTimer(this);
    connect(colorUpdateTimer, &QTimer::timeout, this, &TaskScheduler::updateTaskColors);
    colorUpdateTimer->start(60000); // Update colors every minute
}

// Full implementation of all functions

// Add task from UI inputs
void TaskScheduler::addTaskFromUI() {
    QString name = nameInput->text();
    QDateTime dueDate = dueDateInput->dateTime();
    int priority = priorityInput->text().toInt();
    QString description = descriptionInput->toPlainText();
    int duration = durationInput->value();

    Task *task = new Task{name, dueDate, 0, description, duration};
    calculateAndAssignPriority(task);
    addTask(task);
}

// Add task to list and UI
void TaskScheduler::addTask(Task *task) {
    tasks.push_back(task);
    addTaskToList(task);
}

// Add a task to the tree widget
void TaskScheduler::addTaskToList(Task *task) {
    QTreeWidgetItem *item = new QTreeWidgetItem(taskList);
    item->setText(0, task->name);
    item->setData(1, Qt::UserRole, task->dueDate);
    item->setText(1, task->dueDate.toString());
    item->setText(2, QString::number(task->priority));
    item->setText(3, formatDuration(task->duration));
    item->setText(5, task->description);
    taskCombo1->addItem(task->name);
    taskCombo2->addItem(task->name);
    updateTaskColors();
}

// Add dependency from UI
void TaskScheduler::addDependencyFromUI() {
    QString taskName = taskCombo1->currentText();
    QString depName = taskCombo2->currentText();

    // Find the corresponding Task objects by their names
    Task *task = findTaskByName(taskName);
    Task *dep = findTaskByName(depName);

    if (task && dep && task != dep) {
         // Check if the dependency does not already exist
        if (std::find(task->dependencies.begin(), task->dependencies.end(), dep) == task->dependencies.end()) {
            if (!isCircularDependency(task, dep)) {
                // Add the dependency and update the adjacency list
                addDependency(task, dep);
                addEdge(task, dep); // Add edge to adjacency list
            } else {
                QMessageBox::warning(this, "Error", "Circular dependency detected!");
            }
        } else {
            QMessageBox::warning(this, "Error", "Dependency already exists!");
        }
    } else {
        QMessageBox::warning(this, "Error", "Invalid task selection!");
    }
}

// Add dependency between tasks
void TaskScheduler::addDependency(Task *task, Task *dependency) {
    task->dependencies.push_back(dependency);
    updateDependenciesDisplay();
}

// Generate schedule for tasks
void TaskScheduler::generateSchedule() {
    std::vector<Task *> schedule; // topological order of tasks
    std::unordered_map<Task *, bool> visited; // Map to track visited tasks during DFS

    std::function<void(Task *)> dfs = [&](Task *task) {
        visited[task] = true;
        for (auto dep: task->dependencies) { // Recursively visit all dependent tasks
            if (!visited[dep]) {
                dfs(dep);
            }
        }
        schedule.push_back(task);
    };

    // Perform DFS for each task to generate the schedule
    for (auto task: tasks) {
        if (!visited[task]) {
            dfs(task);
        }
    }

    std::reverse(schedule.begin(), schedule.end());

    // Create a string representation of the schedule
    QString result = "Schedule:\n";
    for (auto task: schedule) {
        result += QString("%1 - Due: %2, Duration: %3\n")
                .arg(task->name)
                .arg(task->dueDate.toString())
                .arg(formatDuration(task->duration));
    }

    QMessageBox::information(this, "Generated Schedule", result);
}

// Visualize tasks and dependencies in a graph
void TaskScheduler::visualizeTasks() {
    scene = new QGraphicsScene(this);
    QGraphicsView *view = new QGraphicsView(scene); // Create a QGraphicsView to display the scene

    std::unordered_map<Task *, TaskItem *> taskItems;

     // Add tasks to the scene as nodes
    for (const auto &task: tasks) {
        TaskItem *item = new TaskItem(task);
        scene->addItem(item);

        qreal x = QRandomGenerator::global()->bounded(400);
        qreal y = QRandomGenerator::global()->bounded(400);
        item->setPos(x, y);

        // Add a text label with the task's name
        QGraphicsTextItem *textItem = scene->addText(task->name);
        textItem->setParentItem(item);
        textItem->setPos(40 - textItem->boundingRect().width() / 2,
                         40 - textItem->boundingRect().height() / 2);

        taskItems[task] = item; // Store the graphical item associated with the task
    }

    // Add edges to represent dependencies between tasks
    for (const auto &task: tasks) {
        TaskItem *startItem = taskItems[task];
        for (const auto &dep: task->dependencies) {
            TaskItem *endItem = taskItems[dep];
            startItem->addEdge(endItem);
        }
    }

    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::ScrollHandDrag); // Set the view to allow dragging the scene
    view->setMinimumSize(500, 500); // Set the minimum size of the view
    view->show();
}

// Save tasks to file
void TaskScheduler::saveTasks() {
    QSaveFile file("tasks.dat");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_6_0);

        out << quint32(tasks.size());
        for (const auto &task: tasks) {
            out << task->name << task->dueDate << task->priority << task->description << task->duration;

            out << quint32(task->dependencies.size());
            for (const auto &dep: task->dependencies) {
                out << dep->name;
            }
        }

        file.commit();
    }
}

// Load tasks from file
void TaskScheduler::loadTasks() {
    QFile file("tasks.dat");
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_0);

        quint32 taskCount;
        in >> taskCount; // Read the number of tasks stored in the file

        tasks.clear();
        taskCombo1->clear();
        taskCombo2->clear();
        taskList->clear();

        // Map to temporarily store dependencies as task names
        QMap<QString, QStringList> dependencyMap;

        // Loop to load each task and its basic properties
        for (quint32 i = 0; i < taskCount; ++i) {
            Task *task = new Task;
            in >> task->name >> task->dueDate >> task->priority >> task->description >> task->duration;

            // Read the number of dependencies for the task
            quint32 depCount;
            in >> depCount; 
            
            QStringList deps;
            for (quint32 j = 0; j < depCount; ++j) {
                QString depName;
                in >> depName;
                deps.append(depName);
            }
            dependencyMap[task->name] = deps;

            // Add the task to the list of tasks
            tasks.push_back(task);

            // Add the task to the UI task list
            addTaskToList(task);
        }

        for (auto &task: tasks) {
            const QStringList &deps = dependencyMap[task->name];
            for (const QString &depName: deps) {
                Task *depTask = findTaskByName(depName);
                if (depTask) {
                    task->dependencies.push_back(depTask);
                }
            }
        }

        updateDependenciesDisplay();
    }
}

// Remove selected task
void TaskScheduler::removeTask() {
    QTreeWidgetItem *selectedItem = taskList->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "No Task Selected", "Please select a task to remove.");
        return;
    }

    QString taskName = selectedItem->text(0);
    Task *taskToRemove = findTaskByName(taskName);

    if (taskToRemove) {
        for (auto &task: tasks) {
            task->dependencies.erase(
                    std::remove(task->dependencies.begin(), task->dependencies.end(), taskToRemove),
                    task->dependencies.end());
        }

        tasks.erase(std::remove(tasks.begin(), tasks.end(), taskToRemove), tasks.end());

        delete selectedItem;

        int index = taskCombo1->findText(taskName);
        if (index != -1) {
            taskCombo1->removeItem(index);
            taskCombo2->removeItem(index);
        }

        delete taskToRemove;
    }

    updateDependenciesDisplay();
}

// Update the colors of tasks based on their due date
void TaskScheduler::updateTaskColors() {
    QDateTime currentDateTime = QDateTime::currentDateTime();
    for (int i = 0; i < taskList->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = taskList->topLevelItem(i);
        QDateTime dueDate = item->data(1, Qt::UserRole).toDateTime();
        int priority = item->text(2).toInt();
        QColor backgroundColor;
        QColor textColor = Qt::white;

        if (dueDate < currentDateTime) {
            backgroundColor = QColor(200, 0, 0);
        } else if (dueDate < currentDateTime.addDays(1)) {
            backgroundColor = QColor(200, 200, 0);
        } else {
            backgroundColor = QColor(0, 150, 0);
        }

        int priorityAdjustment = (10 - priority) * 10;
        backgroundColor = backgroundColor.darker(100 + priorityAdjustment);

        if (backgroundColor.lightness() > 128) {
            textColor = Qt::black;
        }

        for (int j = 0; j < item->columnCount(); ++j) {
            item->setBackground(j, backgroundColor);
            item->setForeground(j, textColor);
        }
    }
}

// Display details of selected task
void TaskScheduler::displayTaskDetails() {
    QTreeWidgetItem *selectedItem = taskList->currentItem();
    if (!selectedItem) {
        return;
    }

    QString taskName = selectedItem->text(0);
    Task *selectedTask = findTaskByName(taskName);

    if (selectedTask) {
        QDialog detailsDialog(this);
        detailsDialog.setWindowTitle("Task Details");

        QVBoxLayout *layout = new QVBoxLayout(&detailsDialog);

        QLabel *nameLabel = new QLabel(QString("Name: %1").arg(selectedTask->name));
        QLabel *dueDateLabel = new QLabel(QString("Due Date: %1").arg(selectedTask->dueDate.toString()));
        QLabel *priorityLabel = new QLabel(QString("Priority: %1").arg(selectedTask->priority));
        QLabel *durationLabel = new QLabel(QString("Duration: %1").arg(formatDuration(selectedTask->duration)));
        QLabel *descriptionLabel = new QLabel("Description:");
        QTextEdit *descriptionText = new QTextEdit(selectedTask->description);
        descriptionText->setReadOnly(true);

        QLabel *dependenciesLabel = new QLabel("Dependencies:");
        QListWidget *dependenciesList = new QListWidget();
        for (const auto &dep: selectedTask->dependencies) {
            dependenciesList->addItem(dep->name);
        }

        layout->addWidget(nameLabel);
        layout->addWidget(dueDateLabel);
        layout->addWidget(priorityLabel);
        layout->addWidget(durationLabel);
        layout->addWidget(descriptionLabel);
        layout->addWidget(descriptionText);
        layout->addWidget(dependenciesLabel);
        layout->addWidget(dependenciesList);

        QPushButton *closeButton = new QPushButton("Close");
        connect(closeButton, &QPushButton::clicked, &detailsDialog, &QDialog::accept);
        layout->addWidget(closeButton);

        detailsDialog.exec();
    }
}

// Calculate and assign priority based on due date and dependencies
void TaskScheduler::calculateAndAssignPriority(Task *task) {
    QDateTime currentDate = QDateTime::currentDateTime();
    int daysUntilDue = currentDate.daysTo(task->dueDate);

    int basePriority;
    if (daysUntilDue <= 1) {
        basePriority = 10;
    } else if (daysUntilDue <= 3) {
        basePriority = 8;
    } else if (daysUntilDue <= 7) {
        basePriority = 6;
    } else if (daysUntilDue <= 14) {
        basePriority = 4;
    } else {
        basePriority = 2;
    }

    int dependencyFactor = task->dependencies.size();
    int finalPriority = basePriority + dependencyFactor;
    task->priority = std::max(1, std::min(10, finalPriority));
}

// Format task duration for display
QString TaskScheduler::formatDuration(int minutes) {
    if (minutes < 60) {
        return QString("%1 minutes").arg(minutes);
    } else if (minutes < 1440) {
        int hours = minutes / 60;
        int mins = minutes % 60;
        return QString("%1 hours %2 minutes").arg(hours).arg(mins);
    } else {
        int days = minutes / 1440;
        int hours = (minutes % 1440) / 60;
        int mins = minutes % 60;
        return QString("%1 days %2 hours %3 minutes").arg(days).arg(hours).arg(mins);
    }
}

// Helper function to find a task by name
Task *TaskScheduler::findTaskByName(const QString &name) {
    auto it = std::find_if(tasks.begin(), tasks.end(),
                           [&name](const Task *task) { return task->name == name; });
    return (it != tasks.end()) ? *it : nullptr;
}

// Add edge for adjacency list
void TaskScheduler::addEdge(Task *task1, Task *task2) {
    if (std::find(task1->adjacentTasks.begin(), task1->adjacentTasks.end(), task2) == task1->adjacentTasks.end()) {
        task1->adjacentTasks.push_back(task2);
    }
}


// Remove edge from adjacency list
void TaskScheduler::removeEdge(Task *task1, Task *task2) {
    task1->adjacentTasks.erase(std::remove(task1->adjacentTasks.begin(), task1->adjacentTasks.end(), task2),
                               task1->adjacentTasks.end());
    task2->adjacentTasks.erase(std::remove(task2->adjacentTasks.begin(), task2->adjacentTasks.end(), task1),
                               task2->adjacentTasks.end());
}

// Print adjacency list for tasks
void TaskScheduler::printAdjacencyList() {
    QString result = "Adjacency List:\n";
    for (const auto &task : tasks) {
        result += task->name + ": ";
        if (task->adjacentTasks.empty()) {
            result += "(No dependencies)";
        } else {
            for (const auto &adjTask : task->adjacentTasks) {
                result += adjTask->name + " ";
            }
        }
        result += "\n";
    }
    QMessageBox::information(this, "Adjacency List", result);
}

// Show priority graph for tasks
void TaskScheduler::showPriorityGraph() {
    if (priorityGraphWindow) {
        priorityGraphWindow->close();
        delete priorityGraphWindow;
        priorityGraphWindow = nullptr;
    }

    QMap<int, int> priorityCounts;
    for (const auto &task : tasks) {
        priorityCounts[task->priority]++;
    }

    QBarSet *set = new QBarSet("Tasks");
    QStringList categories;
    int maxCount = 0;

    for (auto it = priorityCounts.begin(); it != priorityCounts.end(); ++it) {
        *set << it.value();
        categories << QString::number(it.key());
        maxCount = qMax(maxCount, it.value());
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Number of Tasks by Priority");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->setBackgroundBrush(QColor(53, 53, 53));
    chart->setTitleBrush(QColor(255, 255, 255));
    chart->setTitleFont(QFont("Arial", 18, QFont::Bold));

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    axisX->setLabelsColor(QColor(255, 255, 255));

    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    axisY->setRange(0, maxCount + 1);
    axisY->setLabelFormat("%d");
    axisY->setLabelsColor(QColor(255, 255, 255));

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setLabelColor(QColor(255, 255, 255));

    QLinearGradient gradient(0, 0, 0, 1);
    gradient.setColorAt(0.0, QColor(255, 0, 0));
    gradient.setColorAt(0.5, QColor(255, 255, 0));
    gradient.setColorAt(1.0, QColor(0, 255, 0));
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

    set->setBrush(gradient);

    series->setLabelsVisible(true);
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    connect(series, &QBarSeries::hovered, this, &TaskScheduler::handleBarHover);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    priorityGraphWindow = new QMainWindow();
    priorityGraphWindow->setCentralWidget(chartView);
    priorityGraphWindow->resize(600, 400);
    priorityGraphWindow->setWindowTitle("Task Priority Distribution");
    priorityGraphWindow->show();
}

// Handle bar hover for priority graph
void TaskScheduler::handleBarHover(bool status, int index, QBarSet *barSet) {
    if (status) {
        QToolTip::showText(QCursor::pos(),
                           QString("Priority %1: %2 task(s)")
                                   .arg(index + 1)
                                   .arg(barSet->at(index)));
    }
}

// Check for circular dependency
bool TaskScheduler::isCircularDependency(Task *task, Task *newDep) {
    std::unordered_set<Task *> visited;
    std::function<bool(Task *)> dfs = [&](Task *current) {
        if (current == task) return true;
        if (visited.find(current) != visited.end()) return false;
        visited.insert(current);
        for (auto dep: current->dependencies) {
            if (dfs(dep)) return true;
        }
        return false;
    };
    return dfs(newDep);
}

// Remove dependency
void TaskScheduler::removeDependency() {
    QString taskName = taskCombo1->currentText();
    QString depName = taskCombo2->currentText();

    Task *task = findTaskByName(taskName);
    Task *dep = findTaskByName(depName);

    if (task && dep) {
        task->dependencies.erase(
                std::remove(task->dependencies.begin(), task->dependencies.end(), dep),
                task->dependencies.end()
        );
        removeEdge(task, dep); // Remove edge from adjacency list
        updateDependenciesDisplay();
    } else {
        QMessageBox::warning(this, "Error", "Could not find specified tasks.");
    }
}
// Update dependencies display
void TaskScheduler::updateDependenciesDisplay() {
    for (int i = 0; i < taskList->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = taskList->topLevelItem(i);
        QString itemName = item->text(0);
        Task *task = findTaskByName(itemName);
        if (task) {
            QString depText;
            for (const auto &dep: task->dependencies) {
                depText += dep->name + ", ";
            }
            item->setText(4, depText);
        }
    }
}

// Sort tasks by due date
void TaskScheduler::sortTasksByDueDate() {
    taskList->sortItems(1, Qt::AscendingOrder);
}

// Sort tasks by priority
void TaskScheduler::sortTasksByPriority() {
    taskList->sortItems(2, Qt::AscendingOrder);
}

// Exit application
void TaskScheduler::exitApplication() {
    QApplication::closeAllWindows();
    QApplication::quit();
}

// Event filter for custom sorting
bool TaskScheduler::eventFilter(QObject *obj, QEvent *event) {
    if (obj == taskList && event->type() == QEvent::ChildAdded) {
        taskList->sortItems(1, Qt::AscendingOrder);
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void TaskScheduler::checkFileContents() {
    QFile file("tasks.dat");
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_0); // Ensure this matches the version used for writing

        quint32 taskCount;
        in >> taskCount; // Read the number of tasks stored in the file
        qDebug() << "Number of tasks:" << taskCount;

        if (in.status() != QDataStream::Ok) { // Check if reading the task count was successful
            qDebug() << "Error reading task count:" << in.status();
            return;
        }

        for (quint32 i = 0; i < taskCount; ++i) {
            QString name;
            QDate dueDate;
            int priority;
            QString description;
            qint64 duration;

            // Debug log for tracking task reading progress
            qDebug() << "Reading task" << i + 1;
            qDebug() << "Stream position before read:" << file.pos();

            
            // Read the task's name
            in >> name;
            if (in.status() != QDataStream::Ok) {
                qDebug() << "Error reading name:" << in.status();
                break;
            }

            // Read the task's due date
            in >> dueDate;
            if (in.status() != QDataStream::Ok) {
                qDebug() << "Error reading dueDate:" << in.status();
                break;
            }

            // Read the task's priority
            in >> priority;
            if (in.status() != QDataStream::Ok) {
                qDebug() << "Error reading priority:" << in.status();
                break;
            }

            // Read the task's description
            in >> description;
            if (in.status() != QDataStream::Ok) {
                qDebug() << "Error reading description:" << in.status();
                break;
            }

            // Read the task's duration
            in >> duration;
            if (in.status() != QDataStream::Ok) {
                qDebug() << "Error reading duration:" << in.status();
                break;
            }

            qDebug() << "Stream position after read:" << file.pos();
            qDebug() << "Task" << i + 1 << ":" << name << dueDate.toString() << priority << description << duration;
        }

        qDebug() << "Final stream position:" << file.pos();
        qDebug() << "File size:" << file.size();
    } else {
        qDebug() << "Failed to open file for checking:" << file.errorString();
    }
}

void TaskScheduler::addDependency() {
    QString taskName = taskCombo1->currentText();
    QString depName = taskCombo2->currentText();

// Find the task item early
    QTreeWidgetItem * item = nullptr;
    QList < QTreeWidgetItem * > items = taskList->findItems(taskName, Qt::MatchExactly, 0);
    if (!items.isEmpty()) {
        item = items[0];
    } else {
        QMessageBox::warning(this, "Error", "Could not find the specified task.");
        return;
    }

    Task *task = nullptr;
    Task *dep = nullptr;

    for (auto t: tasks) {
        if (t->name == taskName) task = t;
        if (t->name == depName) dep = t;
    }

    if (!task || !dep) {
        QMessageBox::warning(this, "Error", "Could not find specified tasks.");
        if (task) updateTaskColor(item, task->priority);
        return;
    }

    if (task && dep) {

// Check if the dependency already exists
        if (std::find(task->dependencies.begin(), task->dependencies.end(), dep) != task->dependencies.end()) {
            QMessageBox::warning(this, "Duplicate Dependency",
                                 QString("The dependency %1 -> %2 already exists.").arg(taskName).arg(depName));
            updateTaskColor(item, task->priority);
            return;
        }

// Check for circular dependency
        if (isCircularDependency(task, dep)) {
            QMessageBox::warning(this, "Circular Dependency",
                                 QString("Adding this dependency would create a circular reference between %1 and %2.").arg(
                                         taskName).arg(depName));
            updateTaskColor(item, task->priority);
            return;
        }

        task->dependencies.push_back(dep);
        addEdge(task, dep); // Add this line
        QTreeWidgetItem * item = taskList->findItems(taskName, Qt::MatchExactly, 0)[0];
        item->setText(4, item->text(4) + depName + ", ");

// Recalculate priority
        calculateAndAssignPriority(task);
        item->setText(2, QString::number(task->priority));

// Update color based on new priority
        QColor backgroundColor;
        QColor itemColor = Qt::white;

        if (task->priority >= 8) {
            itemColor = QColor(200, 0, 0); // Brighter red for high priority
        } else if (task->priority >= 4) {
            itemColor = QColor(200, 200, 0); // Brighter yellow for medium priority
        } else {
            itemColor = QColor(0, 150, 0); // Brighter green for low priority
        }

// Adjust color based on priority
        int priorityAdjustment = (10 - task->priority) * 10;
        backgroundColor = backgroundColor.darker(100 + priorityAdjustment);

// Ensure text is always visible
        if (backgroundColor.lightness() > 128) {
            itemColor = Qt::black;
        }

        for (int i = 0; i < item->columnCount(); ++i) {
            item->setBackground(i, backgroundColor);
            item->setForeground(i, itemColor);
        }
    }
}

void TaskScheduler::updateTaskColor(QTreeWidgetItem *item, int priority) {
    QColor itemColor;
    if (priority >= 8) {
        itemColor = QColor(150, 50, 50); // Darker red for high priority
    } else if (priority >= 4) {
        itemColor = QColor(150, 150, 50); // Darker yellow for medium priority
    } else {
        itemColor = QColor(50, 150, 50); // Darker green for low priority
    }

    for (int i = 0; i < item->columnCount(); ++i) {
        item->setBackground(i, itemColor);
        item->setForeground(i, QColor(255, 255, 255)); // Set text color to white
    }
}
