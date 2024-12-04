#include "taskItem.h"
#include "scheduleHelper.h"

int main(int argc, char *argv[]) {
/* interactive main */
    QElapsedTimer timer;
    QApplication app(argc, argv);
    TaskScheduler scheduler;
    scheduler.show();
    return app.exec();
}
/*
    QApplication app(argc, argv);

    // Create a TaskScheduler instance
    TaskScheduler scheduler;

    // Load the Amazon product co-purchasing network dataset
    QFile file("amazon0302.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Failed to open file";
    return 1;
    }

    QTextStream in(&file);
    std::unordered_map<int, Task*> tasks;
    int count = 0;
    QElapsedTimer timer;
    timer.start();

    // Skip the first 4 lines (comments)
    for (int i = 0; i < 4; ++i) {
    in.readLine();
    }

    int lines = 0;
    //while (!in.atEnd() && count < 100000) {
    while (!in.atEnd() && lines < 100000) {

    lines++;

    if (lines % 1000 == 0) {
    qDebug() << "Current line:" << lines << "Processed tasks:" << count;
    }

    QString line = in.readLine();
    QStringList parts = line.split("\t");
    if (parts.size() == 2) {
    int source = parts[0].toInt();
    int target = parts[1].toInt();

    // Create tasks if they don't exist
    if (tasks.find(source) == tasks.end()) {
    Task* task = new Task{
    QString("Product_%1").arg(source),
    QDateTime::currentDateTime().addDays(QRandomGenerator::global()->bounded(30)),
    QRandomGenerator::global()->bounded(10) + 1,
    QString("Description for Product %1").arg(source),
    QRandomGenerator::global()->bounded(120) + 1
    };
    tasks[source] = task;
    scheduler.addTask(task);
    count++;
    }

    if (tasks.find(target) == tasks.end() && count < 100000) {
    Task* task = new Task{
    QString("Product_%1").arg(target),
    QDateTime::currentDateTime().addDays(QRandomGenerator::global()->bounded(30)),
    QRandomGenerator::global()->bounded(10) + 1,
    QString("Description for Product %1").arg(target),
    QRandomGenerator::global()->bounded(120) + 1
    };
    tasks[target] = task;
    scheduler.addTask(task);
    count++;
    }

    // Add dependency (representing co-purchasing relationship)
    if (tasks.find(source) != tasks.end() && tasks.find(target) != tasks.end()) {
    scheduler.addDependency(tasks[source], tasks[target]);
    }
    }
    }

    qint64 elapsed = timer.elapsed();
    qDebug() << "Time taken to load and process data:" << elapsed << "ms";
    qDebug() << "Number of tasks created:" << count;

    // Test various operations
    timer.restart();
    //scheduler.generateSchedule();
    qDebug() << "Time taken to generate schedule:" << timer.elapsed() << "ms";

    timer.restart();
    //scheduler.visualizeTasks();
    qDebug() << "Time taken to visualize tasks:" << timer.elapsed() << "ms";

    timer.restart();
    //scheduler.saveTasks();
    qDebug() << "Time taken to save tasks:" << timer.elapsed() << "ms";

    timer.restart();
    //scheduler.loadTasks();
    qDebug() << "Time taken to load tasks:" << timer.elapsed() << "ms";

    timer.restart();
    scheduler.showPriorityGraph(); // graph
    qDebug() << "Time taken to Priority Graph:" << timer.elapsed() << "ms";

    return app.exec();

}
*/
