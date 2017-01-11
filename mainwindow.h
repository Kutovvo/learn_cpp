#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QDebug>
#include <QRegExp>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString line_exp;
    QString whoSender;
    int curPos;
private:
    Ui::MainWindow *ui;
    void connecting_buttons();
    bool checkPr(int &pos, int &pos_2);
    void sum(QRegExp *find, QString symbol, int &pos);
private slots:
    void addValue();
    void clearFromButton();
    void verifyExp(QString str);
    void showRes();
    void fromKeyboard(QString str);
};


#endif // MAINWINDOW_H
