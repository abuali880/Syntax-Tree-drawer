#include "syntax.h"
#include "ui_syntax.h"

Syntax::Syntax(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Syntax)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
}

Syntax::~Syntax()
{
    delete ui;
}
