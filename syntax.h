#ifndef SYNTAX_H
#define SYNTAX_H

#include <QWidget>
#include <QMainWindow>
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSimpleTextItem>
#include <QMessageBox>

namespace Ui {
class Syntax;
}

class Syntax : public QWidget
{
    Q_OBJECT

public:
    explicit Syntax(QWidget *parent = 0);
    int x;
    ~Syntax();

private:
    Ui::Syntax *ui;
    QGraphicsScene *scene;
};

#endif // SYNTAX_H
