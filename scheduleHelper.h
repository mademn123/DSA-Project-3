
#include <QMainWindow>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QTreeWidget>
#include <QComboBox>
#include <QGraphicsScene>
#include <QGroupBox>
#include <QCalendarWidget>
#include <QTimer>
#include <QSaveFile>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <QtCharts>
#include "taskItem.h"
#ifndef PRIORITYTASKSCHEDULER_SCHEDULEHELPER_H
#define PRIORITYTASKSCHEDULER_SCHEDULEHELPER_H

#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

class TaskScheduler : public QMainWindow {
Q_OBJECT

public:
    explicit TaskScheduler(QWidget *parent = nullptr);

private slots:
    void addTaskFromUI();
    void addDependencyFromUI();
    void addTask(Task *task);
    void addDependency(Task *task, Task *dependency);
    void generateSchedule();
    void visualizeTasks();
    void saveTasks();
    void loadTasks();
    void removeTask();
    void addTaskToList(Task *task);
    void updateTaskColors();
    void displayTaskDetails();
    void calculateAndAssignPriority(Task *task);
    void checkFileContents();
    void sortTasksByDueDate();
    void sortTasksByPriority();
    void addEdge(Task *task1, Task *task2);
    void removeEdge(Task *task1, Task *task2);
    void printAdjacencyList();
    void showPriorityGraph();
    void handleBarHover(bool status, int index, QBarSet *barSet);
    void updateDependenciesDisplay();
    void addDependency();
    bool isCircularDependency(Task *task, Task *newDep);
    void removeDependency();
    void exitApplication();
    Task *findTaskByName(const QString &name);
    QString formatDuration(int minutes);
    void updateTaskColor(QTreeWidgetItem *item, int priority);

private:
    bool eventFilter(QObject *obj, QEvent *event) override;

    QLineEdit *nameInput;
    QDateTimeEdit *dueDateInput;
    QLineEdit *priorityInput;
    QTextEdit *descriptionInput;
    QSpinBox *durationInput;
    QTreeWidget *taskList;
    QComboBox *taskCombo1;
    QComboBox *taskCombo2;
    std::vector<Task *> tasks;
    QGraphicsScene *scene{};
    QMainWindow *priorityGraphWindow = nullptr;
};

#endif // TASKSCHEDULER_H

#endif //PRIORITYTASKSCHEDULER_SCHEDULEHELPER_H
#include "scheduleHelper.moc"