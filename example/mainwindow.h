
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSvg>
#include "texengine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void runMathJax ();

private:
    Ui::MainWindow *ui;
    QSvgWidget* svg;
    TeXEngine engine;
};

#endif // MAINWINDOW_H
