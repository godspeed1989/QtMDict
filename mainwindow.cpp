#include "mainwindow.h"
#include <QLabel>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
     QLabel label("fsdfs");
     QHBoxLayout layout(this);
     layout.addWidget(&label);
     this->setLayout(&layout);
}

MainWindow::~MainWindow()
{

}
