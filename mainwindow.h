#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::shared_ptr<QStandardItemModel> model;
    std::shared_ptr<QStandardItem> rootItem;
    std::shared_ptr<QStandardItem> item1;
    std::shared_ptr<QStandardItem> item2;

    void on_close_curve();


private:
    Ui::MainWindow *ui;

protected:
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
