// Here we avoid loading the header multiple times
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// KDE
#include <KDE/KMainWindow>

// Forward declaration
// Qt
class QWidget;

class MainWindow: public KMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

    ~MainWindow();
};

#endif  //  CHANNEL-JOINER_WINDOW_H

