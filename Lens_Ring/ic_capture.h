#ifndef IC_CAPTURE_H
#define IC_CAPTURE_H
#include <QThread>

class IC_Capture:public QThread
{
    Q_OBJECT
public:
    IC_Capture();
    ~IC_Capture();
    void run();
};

#endif // IC_CAPTURE_H
