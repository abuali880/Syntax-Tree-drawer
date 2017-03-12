#include "node.h"

node::node()
{
    FirChild = NULL;
    SecChild = NULL;
    AltChild = NULL;
    adj.clear();
    s = circl;
    data="";
}

void node::SetData(QString s)
{
    data = s;
}
