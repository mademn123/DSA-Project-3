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

QElapsedTimer timer;

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
    TaskItem(Task *task, QGraphicsItem *parent = nullptr)
            : QGraphicsEllipseItem(0, 0, 80, 80, parent), task(task) {
        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    }

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override {
        if (change == ItemPositionHasChanged && scene()) {
            updateEdges();
        }
        return QGraphicsEllipseItem::itemChange(change, value);
    }

    void addEdge(TaskItem *endItem) {
        edges.append(endItem);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        QGraphicsEllipseItem::paint(painter, option, widget);

        for (TaskItem *endItem: edges) {
            QPointF startPoint = mapToScene(rect().center());
            QPointF endPoint = endItem->mapToScene(endItem->rect().center());

            painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->drawLine(mapFromScene(startPoint), mapFromScene(endPoint));

            double angle = std::atan2(endPoint.y() - startPoint.y(), endPoint.x() - startPoint.x());
            QPointF arrowP1 = endPoint - QPointF(15 * std::cos(angle - M_PI / 6),
                                                 15 * std::sin(angle - M_PI / 6));
            QPointF arrowP2 = endPoint - QPointF(15 * std::cos(angle + M_PI / 6),
                                                 15 * std::sin(angle + M_PI / 6));

            QPolygonF arrowHead;
            arrowHead << mapFromScene(endPoint) << mapFromScene(arrowP1) << mapFromScene(arrowP2);
            painter->setBrush(Qt::black);
            painter->drawPolygon(arrowHead);
        }
    }

    void updateEdges() {
        update();
        for (TaskItem *endItem: edges) {
            endItem->update();
        }
    }

    Task *task;
    QList<TaskItem *> edges;
};

int main() {

    return 0;
}
