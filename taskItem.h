#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QTreeWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include <QPainter>
#include <QCalendarWidget>
#include <QTextEdit>
#include <QSpinBox>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <QListWidget>
#include <QTimer>
#include <QColor>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSaveFile>
#include <QFileInfo>
#include <unordered_set>
#include <QtCharts>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <QToolTip>

#include <QElapsedTimer>
#ifndef PRIORITYTASKSCHEDULER_TASKITEM_H
#define PRIORITYTASKSCHEDULER_TASKITEM_H


struct Task {
    QString name;
    QDateTime dueDate;
    int priority;
    QString description;
    int duration; // in minutes
    std::vector<Task *> dependencies;
    std::vector<Task *> adjacentTasks; // New: for adjacency list
};

class TaskItem : public QObject, public QGraphicsEllipseItem {
Q_OBJECT

public:
    TaskItem(Task *task, QGraphicsItem *parent = nullptr);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void addEdge(TaskItem *endItem);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void updateEdges();

private:
    Task *task;
    QList<TaskItem *> edges;
};

#endif //PRIORITYTASKSCHEDULER_TASKITEM_H
#include "taskItem.moc"