#ifndef NODE_H
#define NODE_H

#include <QMainWindow>
#include <QVector>

enum shape{circl,recti};
class node
{

public:
    QString data;
    node * FirChild;
    node * SecChild;
    node * AltChild;
    QVector<node *> adj;
    shape s;
    node();
    void SetData(QString s);
};

#endif // NODE_H
