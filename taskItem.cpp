
#include "taskItem.h"

// Set flags to make the item movable and notify when its geometry changes
TaskItem::TaskItem(Task *task, QGraphicsItem *parent)
        : QGraphicsEllipseItem(0, 0, 80, 80, parent), task(task) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

// If the item's position changes, update the edges connected to it
QVariant TaskItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged && scene()) {
        updateEdges();
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}

// Add an edge to the list of connected TaskItems
void TaskItem::addEdge(TaskItem *endItem) {
    edges.append(endItem);
}

void TaskItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsEllipseItem::paint(painter, option, widget);

        // Draw edges connecting this TaskItem to other TaskItems
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

            // Draw the arrowhead at the end of the edge line
        QPolygonF arrowHead;
        arrowHead << mapFromScene(endPoint) << mapFromScene(arrowP1) << mapFromScene(arrowP2);
        painter->setBrush(Qt::black);
        painter->drawPolygon(arrowHead);
    }
}

// Update this TaskItem and all connected edges
void TaskItem::updateEdges() {
    update();
    for (TaskItem *endItem: edges) {
        endItem->update();
    }
}

