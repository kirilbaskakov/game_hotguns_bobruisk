#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->showFullScreen();

    controller = new Controller(size(), this);
    setCentralWidget(controller->GetView());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete controller;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    controller->HandleKeyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    controller->HandleKeyReleaseEvent(event);
}

