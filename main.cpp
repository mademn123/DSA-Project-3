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
