#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = std::make_shared<QStandardItemModel>(0, 0);
    model->setHeaderData(0, Qt::Horizontal, "Project");
    ui->treeView->setModel(model.get());

    rootItem = std::make_shared<QStandardItem>("Scene root");
    model->invisibleRootItem()->appendRow(rootItem.get());

    item1 = std::make_shared<QStandardItem>("Item 1");
    rootItem->appendRow(item1.get());

    item2 = std::make_shared<QStandardItem>("Item 2");
    rootItem->appendRow(item2.get());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event){
    ui->renderWidget->close();
    QWidget::closeEvent(event);
}

