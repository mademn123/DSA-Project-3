#include "scheduleHelper.h"

TaskScheduler::TaskScheduler(QWidget *parent) : QMainWindow(parent) {
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

    Task *task = findTaskByName(taskName);
    Task *dep = findTaskByName(depName);

    if (task && dep && task != dep) {
        if (std::find(task->dependencies.begin(), task->dependencies.end(), dep) == task->dependencies.end()) {
            if (!isCircularDependency(task, dep)) {
                addDependency(task, dep);
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
    std::vector<Task *> schedule;
    std::unordered_map<Task *, bool> visited;

    std::function<void(Task *)> dfs = [&](Task *task) {
        visited[task] = true;
        for (auto dep: task->dependencies) {
            if (!visited[dep]) {
                dfs(dep);
            }
        }
        schedule.push_back(task);
    };

    for (auto task: tasks) {
        if (!visited[task]) {
            dfs(task);
        }
    }

    std::reverse(schedule.begin(), schedule.end());

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
    QGraphicsView *view = new QGraphicsView(scene);

    std::unordered_map<Task *, TaskItem *> taskItems;

    for (const auto &task: tasks) {
        TaskItem *item = new TaskItem(task);
        scene->addItem(item);

        qreal x = QRandomGenerator::global()->bounded(400);
        qreal y = QRandomGenerator::global()->bounded(400);
        item->setPos(x, y);

        QGraphicsTextItem *textItem = scene->addText(task->name);
        textItem->setParentItem(item);
        textItem->setPos(40 - textItem->boundingRect().width() / 2,
                         40 - textItem->boundingRect().height() / 2);

        taskItems[task] = item;
    }

    for (const auto &task: tasks) {
        TaskItem *startItem = taskItems[task];
        for (const auto &dep: task->dependencies) {
            TaskItem *endItem = taskItems[dep];
            startItem->addEdge(endItem);
        }
    }

    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setMinimumSize(500, 500);
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
        in >> taskCount;

        tasks.clear();
        taskCombo1->clear();
        taskCombo2->clear();
        taskList->clear();

        QMap<QString, QStringList> dependencyMap;

        for (quint32 i = 0; i < taskCount; ++i) {
            Task *task = new Task;
            in >> task->name >> task->dueDate >> task->priority >> task->description >> task->duration;

            quint32 depCount;
            in >> depCount;
            QStringList deps;
            for (quint32 j = 0; j < depCount; ++j) {
                QString depName;
                in >> depName;
                deps.append(depName);
            }
            dependencyMap[task->name] = deps;

            tasks.push_back(task);
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

