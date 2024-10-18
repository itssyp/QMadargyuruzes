#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "birds.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadBirds();
private slots:
    void on_searchButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void filterList(const QString &text);
    void filterRingedList(const QString &text);
    void on_pushButton_clicked();

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_saveButton_clicked();

    void on_loadButton_clicked();

    void on_listView_2_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QMap<QString,Birds> loadedBirds;
    QMap<QString,Birds> ringedBirds;
};
#endif // MAINWINDOW_H
