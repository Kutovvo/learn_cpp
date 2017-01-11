#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Calculator");
    connecting_buttons();
}

MainWindow::~MainWindow()
{
    delete ui;
    qDebug() <<"Destructor";
}

void MainWindow  :: connecting_buttons()
{
    // 0-9 = && ввод с клавиатуры
    connect(ui->pushButton_0, SIGNAL(clicked(bool)), SLOT(addValue()));
    connect(ui->pushButton_1, SIGNAL(clicked(bool)), SLOT(addValue()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), SLOT(addValue()));
    connect(ui->pushButton_3, SIGNAL(clicked(bool)), SLOT(addValue()));
    connect(ui->pushButton_4, SIGNAL(clicked(bool)), SLOT(addValue()));
    connect(ui->pushButton_5, SIGNAL(clicked(bool)), SLOT(addValue()));
    connect(ui->pushButton_6, SIGNAL(clicked(bool)), SLOT(addValue()));
    connect(ui->pushButton_7, SIGNAL(clicked(bool)), SLOT(addValue()));
    connect(ui->pushButton_8, SIGNAL(clicked(bool)), SLOT(addValue()));
    connect(ui->pushButton_9, SIGNAL(clicked(bool)), SLOT(addValue()));
    connect(ui->pushButton_point, SIGNAL(clicked(bool)), SLOT(addValue()));
    connect(ui->pushButton_result, SIGNAL(clicked(bool)), SLOT(showRes()));
    connect(ui->lineEdit, SIGNAL(textEdited(QString)), SLOT(fromKeyboard(QString)));
    // clear line && last symbol
    connect(ui->pushButton_clear_all, SIGNAL(clicked(bool)),SLOT(clearFromButton()));
    connect(ui->pushButton_clear_last, SIGNAL(clicked(bool)),SLOT(clearFromButton()));
    // + - * /
    connect(ui->pushButton_plus, SIGNAL(clicked(bool)), SLOT(addValue()));
    connect(ui->pushButton_minus, SIGNAL(clicked(bool)), SLOT(addValue()));
    connect(ui->pushButton_delete, SIGNAL(clicked(bool)), SLOT(addValue()));
    connect(ui->pushButton_multiply, SIGNAL(clicked(bool)), SLOT(addValue()));
    // Проверка на корректность ввода
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), SLOT(verifyExp(QString)));


}



void MainWindow :: addValue()
{
    QRegExp reg("^\\w+_");
    whoSender = sender()->objectName().split(reg).value(1);
    curPos = ui->lineEdit-> cursorPosition();

    if (whoSender == "plus")
    {
        ui->lineEdit->setText(line_exp.insert(curPos,"+"));
        ui->lineEdit->setCursorPosition(++curPos);
    }
    else if (whoSender == "minus")
    {
        ui->lineEdit->setText(line_exp.insert(curPos,"-"));
        ui->lineEdit->setCursorPosition(++curPos);
    }
    else if (whoSender == "delete")
    {
        ui->lineEdit->setText(line_exp.insert(curPos,"÷"));
        ui->lineEdit->setCursorPosition(++curPos);
    }
    else if (whoSender == "multiply")
    {
        ui->lineEdit->setText(line_exp.insert(curPos, "*"));
        ui->lineEdit->setCursorPosition(++curPos);
    }
    else if (whoSender == "point")
    {
        ui->lineEdit->setText(line_exp.insert(curPos, "."));
        ui->lineEdit->setCursorPosition(++curPos);
    }
    else
    {
        ui->lineEdit->setText(line_exp.insert(curPos,whoSender));
        ui->lineEdit->setCursorPosition(++curPos);
    }
}



void MainWindow :: clearFromButton()
{
    if (sender()->objectName() == "pushButton_clear_all")
    {
        line_exp.clear();
        ui->lineEdit->clear();
    }
    else if (sender()->objectName() == "pushButton_clear_last")
    {
        curPos = ui->lineEdit->cursorPosition();
        switch(curPos)
        {
        case 0 :
            return; break;
        default :
            ui->lineEdit->setText(line_exp.remove(curPos-1,1));
            ui->lineEdit->setCursorPosition(--curPos);
        }
    }
}

void MainWindow::fromKeyboard(QString str)
{
    line_exp = str;
    qDebug() << "1";
}


void MainWindow :: verifyExp(QString str)
{
    QRegExp check_1("^\\+|^\\*|^\\÷|^\\--|^\\=|^\\.|^\\/|^[a-zA-Z]+"); //начало выражения
    ui->lineEdit->setText(str.replace(check_1, ""));
    line_exp.replace(check_1, "");

    QRegExp check_2("\\/");
    ui->lineEdit->setText(str.replace(check_2, "÷"));
    line_exp.replace(check_2, "÷");

    QRegExp check_3("\\,");
    ui->lineEdit->setText(str.replace(check_3, "."));
    line_exp.replace(check_3, ".");

    QRegExp check_4("\\-\\+|\\-\\*|\\-\\÷|\\-\\.|\\-{3,}|\\+{2,}|\\+\\*|\\+\\÷|\\+\\-{2,}|\\+\\."); //минус, плюс
    ui->lineEdit->setText(str.replace(check_4, ""));
    line_exp.replace(check_4, "");

    QRegExp check_5("\\s|\\d+\\.{2,}|\\d+\\.\\d+\\.|[a-zA-Z]+"); //точки буквы
    ui->lineEdit->setText(str.replace(check_5, ""));
    line_exp.replace(check_5, "");

}

void MainWindow :: showRes()
{
    qDebug() << "Строка для фин.подсчета" << line_exp;

    QRegExp rule_1("\\--");
    line_exp.replace(rule_1,"+");

    QRegExp find_1("\\-?\\d+(\\.\\d+)?\\*\\-?\\d+(\\.\\d+)?"); // умножение
    QRegExp find_2("\\-?\\d+(\\.\\d+)?\\÷\\-?\\d+(\\.\\d+)?"); // деление

    QRegExp find_3("\\-?\\d+(\\.\\d+)?\\+\\-?\\d+(\\.\\d+)?"); // плюс
    QRegExp find_4("\\-?\\d+(\\.\\d+)?\\-\\-?\\d+(\\.\\d+)?"); // минус

    bool exit = false;

    int pos = find_1.indexIn(line_exp, 0);
    int pos_2 = find_2.indexIn(line_exp, 0);

    while (exit != true)
    {
        if (checkPr(pos, pos_2)) // умножить?
        {
            sum(&find_1, "*", pos);
            pos = find_1.indexIn(line_exp, 0);
            pos_2 = find_2.indexIn(line_exp, 0);

        }
        else if (checkPr(pos_2, pos)) // делить ?
        {
            sum(&find_2, "÷", pos_2);
            pos = find_1.indexIn(line_exp, 0);
            pos_2 = find_2.indexIn(line_exp, 0);
        }
        else
        {
            ui->lineEdit->setText(line_exp);
            exit = true;
        }
    }
    exit = false;
    pos = find_3.indexIn(line_exp, 0);
    pos_2 = find_4.indexIn(line_exp, 0);

    while (exit != true)
    {
        if (checkPr(pos, pos_2)) // плюс?
        {
            sum(&find_3, "+", pos);
            pos = find_3.indexIn(line_exp, 0);
            pos_2 = find_4.indexIn(line_exp, 0);
            qDebug() << "+" << pos << pos_2;

        }
        else if (checkPr(pos_2, pos)) // минус ?
        {
            sum(&find_4, "-", pos_2);
            pos = find_3.indexIn(line_exp, 0);
            pos_2 = find_4.indexIn(line_exp, 0);
        }
        else
        {
            ui->lineEdit->setText(line_exp);
            exit = true;
        }
    }
}




bool MainWindow :: checkPr(int &pos, int &pos_2) // Проверка приоритета операции
{
    if (pos  >= 0 && pos_2 == -1)
    {
        return 1;
    }
    else if(pos == -1 && pos_2 == -1)
    {
        return 0;
    }
    else if(pos == -1 && pos_2 >= 0)
    {
        return 0;
    }
    else if (pos < pos_2)
    {
        return 1;
    }
    else if(pos  >= 0 && pos_2 == 0)
    {
        return 0;
    }
    else
    {
        return 0;
    }
}

void MainWindow :: sum(QRegExp *find, QString symbol, int &pos) // Подсчет && замена строки
{

    QStringList listForVal;
    QString tmp_val_0, tmp_val_1, final_val;
    listForVal << find->cap(0).split(symbol);
    qDebug() << listForVal;
    if (listForVal.length() == 3)
    {
        tmp_val_0 = listForVal.value(1);
        tmp_val_1 = listForVal.value(2);
    }
    else
    {
        tmp_val_0 = listForVal.value(0);
        tmp_val_1 = listForVal.value(1);
    }
    qDebug() << tmp_val_0 << tmp_val_1;
    if (symbol == "*")
    {
        final_val = QString::number((tmp_val_0.toDouble() * tmp_val_1.toDouble()));
    }
    else if (symbol == "÷")
    {
        final_val = QString::number((tmp_val_0.toDouble() / tmp_val_1.toDouble()));
    }
    else if (symbol == "+")
    {
        qDebug() << tmp_val_0.toDouble() << tmp_val_1.toDouble();
        final_val = QString::number((tmp_val_0.toDouble() + tmp_val_1.toDouble()));
    }
    else if (symbol == "-")
    {
        if (listForVal.length() == 3) final_val = QString::number((-tmp_val_0.toDouble() - tmp_val_1.toDouble()));
        else final_val = QString::number((tmp_val_0.toDouble() - tmp_val_1.toDouble()));
    }
    line_exp.replace(pos, find->matchedLength(), final_val);
    pos += find->matchedLength();
}

