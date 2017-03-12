#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QPair>
#include <QStandardItemModel>
#include <tree.h>
#include <QMainWindow>
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSimpleTextItem>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QVector<QPair<QString,QString> > Scanner(QString Input);
    QString NoSpaces(QString s);
    void match(QString to);
    void advance();
    void error();
    node *program();
    node *stm_seq();
    node *statment();
    node *if_stm();
    node *repeat_stmt();
    node *assign_stmt();
    node *read_stmt();
    node *write_stmt();
    node *exp();
    node *comparsion();
    node *simple_exp();
    node *addop();
    node *term();
    node *multop();
    node *factor();
    void DrawTree(node *no);
    void DrawNode(node *no);
    ~MainWindow();

private slots:

    void on_Parse_clicked();

    void on_InputString_textChanged();

private:
    Ui::MainWindow *ui;
    QString token;
    int in;
    QString inputPa;
    QVector<QPair<QString,QString> > pars;
    QGraphicsScene *scene;
    int x;
    int y;
    int level;
    bool ERROR;
    int scale;
};

#endif // MAINWINDOW_H
