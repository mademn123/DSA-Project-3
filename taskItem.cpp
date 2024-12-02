
#include "taskItem.h"

TaskItem::TaskItem(Task *task, QGraphicsItem *parent)
        : QGraphicsEllipseItem(0, 0, 80, 80, parent), task(task) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

QVariant TaskItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged && scene()) {
        updateEdges();
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}

void TaskItem::addEdge(TaskItem *endItem) {
    edges.append(endItem);
}

void TaskItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
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

void TaskItem::updateEdges() {
    update();
    for (TaskItem *endItem: edges) {
        endItem->update();
    }
}

