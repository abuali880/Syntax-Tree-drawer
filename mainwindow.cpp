#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    in=0;
    x=0;
    y=0;
    level = -5;
    scale = 1;
    ERROR = false;
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
}

QVector<QPair<QString,QString>> MainWindow::Scanner(QString Input)
{
    QVector<QPair<QString, QString> > out;
    enum state{START,INNUM,INID,INSIGN,DONE};
    state CurrentState;
    int i = 0 ;
    bool ID=false,NUM=false;
    QString outStr="";
    CurrentState = START;
    while(true)
    {
        if(Input[i]=='{')
        {
            i++;
            while(Input[i]!='}')i++;
            i++;
        }
         switch (CurrentState) {
         case START:
             if(Input[i].isDigit())
             {
                 CurrentState = INNUM;
                 outStr+=Input[i];
                 i++;
                 NUM=true;
             }
             else if(Input[i].isLetter())
             {
                 CurrentState = INID;
                 outStr+=Input[i];
                 i++;
                 ID=true;
             }
             else if (Input[i]==' ' || Input[i]=='\n')
             {
                 i++;
             }
             else if(Input[i]==':')
             {
                 i++;
                 if(Input[i]=='=')
                 {
                     CurrentState = DONE;
                     QString str = ":=",str2="ASSIGN";
                     out.push_back(qMakePair(str,str2));
                     i++;
                 }
             }
             else
             {
                 CurrentState = DONE;
                 outStr=Input[i];
                 i++;
             }
             break;
         case INNUM :
             if(Input[i].isDigit())
             {
                 outStr+=Input[i];
                 i++;
             }
             else
             {
                 CurrentState = DONE;
             }
             break;
         case INID :
             if(Input[i].isLetter())
             {
                 outStr+=Input[i];
                 i++;
             }
             else
             {
                 CurrentState = DONE;
             }
             break;
         case DONE :
             if(NUM)
             {
                 QString str2="NUM";
                 out.push_back(qMakePair(outStr,str2));
                 NUM=false;
                 outStr="";
             }
             else if(ID)
             {
                  QString str2;
                  if (outStr == "if") str2="IF";
                  else if (outStr == "then") str2="THEN";
                  else if (outStr == "else") str2="ELSE";
                  else if (outStr == "until") str2="UNTIL";
                  else if (outStr == "repeat") str2="REPEAT";
                  else if (outStr == "write") str2="WRITE";
                  else if (outStr == "read") str2="READ";
                  else if (outStr == "end") str2="END";
                  else str2="IDENTIFIER";
                  out.push_back(qMakePair(outStr,str2));
                  ID=false;
                  outStr="";
             }
             else if(outStr=="+")
             {
                 QString str2="PLUS";
                 out.push_back(qMakePair(outStr,str2));
                 outStr="";
             }
             else if(outStr=="/")
             {
                 QString str2="DEVIDE";
                 out.push_back(qMakePair(outStr,str2));
                 outStr="";
             }
             else if(outStr=="-")
             {
                 QString str2="MINUS";
                 out.push_back(qMakePair(outStr,str2));
                 outStr="";
             }
             else if(outStr==";")
             {
                 QString str2="SEMI";
                 out.push_back(qMakePair(outStr,str2));
                 outStr="";
             }
             else if(outStr=="*")
             {
                 QString str2="MULTI";
                 out.push_back(qMakePair(outStr,str2));
                 outStr="";
             }
             else if(outStr==">")
             {
                 QString str2="BIGGER THAN";
                 out.push_back(qMakePair(outStr,str2));
                 outStr="";
             }
             else if(outStr=="<")
             {
                 QString str2="SMALLER THAN";
                 out.push_back(qMakePair(outStr,str2));
                 outStr="";
             }
             else if(outStr=="=")
             {
                 QString str2="EQUAL";
                 out.push_back(qMakePair(outStr,str2));
                 outStr="";
             }
             else if(outStr==" ") outStr = "";
             CurrentState = START;
             break;
        default:
             break;
         }
         if(i>Input.length()) return out;
    }
}

void MainWindow::match(QString to)
{
    if(token == to) advance();

    else error();
}

node * MainWindow::program()
{
    return stm_seq();
}

node * MainWindow::stm_seq()
{
    node * stmSeqNode = new node();
    stmSeqNode = statment();
    while(token=="SEMI")
    {
        advance();
        stmSeqNode->adj.push_back(statment());
    }
    return stmSeqNode;
}

node * MainWindow::statment()
{
    node * StatmNode = new node();
    if(token=="IF")
    {
        StatmNode = if_stm();
    }
    else if(token=="REPEAT")
    {
        StatmNode = repeat_stmt();
    }
    else if(token=="IDENTIFIER")
    {
        StatmNode = assign_stmt();
    }
    else if(token=="READ")
    {
        StatmNode = read_stmt();
    }
    else if(token=="WRITE")
    {
        StatmNode = write_stmt();
    }
    else  error();
    return StatmNode;
}

node * MainWindow::if_stm()
{
    match("IF");
    node * IfNode = new node();
    IfNode->s = recti;
    IfNode->data = pars[in-1].first;
    IfNode->FirChild = exp();
    match("THEN");
    IfNode->SecChild = stm_seq();
    if(token=="ELSE")
    {
        match("ELSE");
        IfNode->AltChild = stm_seq();
    }
    match("END");
    return IfNode;
}

node * MainWindow::repeat_stmt()
{
    node * ReNode = new node();
    match("REPEAT");
    ReNode->data = pars[in-1].first;
    ReNode->FirChild = stm_seq();
    ReNode->s = recti;
    match("UNTIL");
    ReNode->SecChild = exp();
    return ReNode;
}

node * MainWindow::assign_stmt()
{
    node * AssiNode = new node();
    match("IDENTIFIER");
    AssiNode->data = "assign\n("+pars[in-1].first+")";
    AssiNode->s = recti;
    match("ASSIGN");
    AssiNode->FirChild = exp();
    return AssiNode;
}

node * MainWindow::read_stmt()
{
    node * ReadNode= new node();
    ReadNode->s = recti;
    match("READ");
    match("IDENTIFIER");
    ReadNode->data = pars[in-2].first+"\n("+pars[in-1].first+")";
    return ReadNode;
}

node * MainWindow::write_stmt()
{
    node * WriNode = new node();
    match("WRITE");
    WriNode->data = pars[in-1].first;
    WriNode->s = recti;
    WriNode->FirChild = exp();
    return WriNode;
}

node * MainWindow::exp()
{
    node * ExpNode = new node();
    ExpNode= simple_exp();
    if(token=="GREATER THAN" || token=="SMALLER THAN" || token=="EQUAL")
    {
        node * temp = new node();
        temp = ExpNode;
        ExpNode= comparsion();
        ExpNode->FirChild = temp;
        ExpNode->SecChild = simple_exp();
    }
    return ExpNode;
}

node * MainWindow::comparsion()
{
    node * CompNode = new node();
    if(token=="GREATER THAN") match("GREATER THAN");
    else if(token=="SMALLER THAN") match("SMALLER THAN");
    else if(token=="EQUAL") match("EQUAL");
    else  error();
    CompNode->data = "  op\n  ("+pars[in-1].first+")";
    return CompNode;
}

node * MainWindow::simple_exp()
{
    node * SimNod = new node();
    SimNod = term();
    while (token=="PLUS" || token=="MINUS")
    {
        node * temp = new node();
        temp = SimNod;
        SimNod= addop();
        SimNod->FirChild = temp;
        SimNod->SecChild = term();
    }
    return SimNod;
}

node * MainWindow::addop()
{
    node * AddopNode = new node();
    if(token=="PLUS") match("PLUS");
    else if(token=="MINUS") match("MINUS");
    else  error();
    AddopNode->data = "  op\n  ("+pars[in-1].first+")";
    return AddopNode;
}

node * MainWindow::term()
{
    node * TermNode = new node();
    TermNode = factor();
    while (token=="MULTI" || token=="DEVIDE")
    {
        node * temp = new node();
        temp = TermNode;
        TermNode = multop();
        TermNode->FirChild = temp;
        TermNode->SecChild = factor();
    }
    return TermNode;
}

node * MainWindow::multop()
{
    node * MultopNode = new node();
    if(token=="MULTI") match("MULTI");
    else if(token=="DEVIDE") match("DEVIDE");
    else  error();
    MultopNode->data = "  op\n  ("+pars[in-1].first+")";
    return MultopNode;
}

node * MainWindow::factor()
{
    node * FactorNode = new node();
    if(token=="(")
    {
        match("(");
        FactorNode = exp();
        match(")");
    }
    else if(token=="NUM")
    {
        match("NUM");
        FactorNode->data = "const\n("+pars[in-1].first+")";
    }
    else if(token=="IDENTIFIER")
    {
        match("IDENTIFIER");
        FactorNode->data = "id\n("+pars[in-1].first+")";
    }
    else error();
    return FactorNode;
}

void MainWindow::DrawTree(node * no)
{
    QPen pen1(QColor(95, 244, 66));
    int Xadj = x;
    int Yadj = y;
    DrawNode(no);
    if(no->FirChild != NULL && no->SecChild == NULL)
    {
        scene->addLine(x+40,y+50,x+40,y+150,pen1);
        x+=5;
        y+=150;
        DrawTree(no->FirChild);
        y-=150;
    }
    if(no->FirChild != NULL && no->SecChild != NULL && no->AltChild == NULL)
    {
        int Xt = x+35;
        scene->addLine(Xt,y+50,x+5,y+110,pen1);
        x-=30;
        y+=110;
        DrawTree(no->FirChild);
        x+=30;
        y-=110;
        scene->addLine(Xt,y+50,x+115,y+110,pen1);
        x+=80;
        y+=110;
        DrawTree(no->SecChild);
        x-=10;
        y-=110;
    }
    else if(no->AltChild != NULL)
    {
        int Xt = x+35;
        scene->addLine(Xt,y+50,x-85,y+110,pen1);
        x-=120;
        y+=110;
        DrawTree(no->FirChild);
        x+=120;
        y-=110;
        scene->addLine(Xt,y+50,x+35,y+110,pen1);
        y+=110;
        DrawTree(no->SecChild);
        y-=110;
        scene->addLine(Xt,y+50,x+155,y+110,pen1);
        x+=120;
        y+=110;
        DrawTree(no->AltChild);
        x+=30;
        y-=110;
    }
    int i = 130;
    int zyada = 0;
    Xadj = Xadj+70+zyada;
    if(!no->adj.empty())
    {
        for(QVector<node *>::Iterator it = no->adj.begin() ;it != no->adj.end() ; it++)
        {
            scene->addLine(Xadj,Yadj+25,x+i,y+25,pen1);
            x+=i;
            DrawTree(*it);
            x-=200;
            i+=130;
            zyada+=135;
        }
        x+=250;
    }
}

void MainWindow::DrawNode(node *no)
{
    if(no == NULL)
    {
        in = 0;
        x=0;
        x=0;
        scene->clear();
    }
    else
    {
        QBrush greenBru(Qt::yellow);
        QBrush redBru(QColor(95, 244, 66));
        QPen pen;
        QFont font;
        font.bold();
        font.setPixelSize(12);
        pen.setWidth(1);
        if(no->s == circl)
        {
            scene->addEllipse(x,y,70,50,pen,greenBru);
            scene->addSimpleText(no->data,font)->setPos(x+20,y+10);
        }
        else if(no->s == recti)
        {
            scene->addRect(x,y,70,50,pen,redBru);
            scene->addSimpleText(no->data,font)->setPos(x+20,y+10);
        }
    }
}





MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::advance()
{
    in++;
    if(in<pars.size()) token = pars[in].second;
}

void MainWindow::error()
{
    ERROR = true;
}


void MainWindow::on_Parse_clicked()
{
    ERROR = false;
    in = 0;
    x=0;
    x=0;
    scene->clear();
    node * tree = new node();
    tree = program();
    if(!pars.empty())
    {
        if(pars[pars.size()-1].first!=";")
        {
            DrawTree(tree);
        }
        else
        {
            error();
        }
    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Please enter your code");
        messageBox.setFixedSize(0,0);
        goto end;
    }
    if(ERROR)
    {
        in = 0;
        x=0;
        x=0;
        scene->clear();
        QMessageBox messageBox;
        messageBox.critical(0,"Error","There's a syntax error in your code, please check it and try again!");
        messageBox.setFixedSize(0,0);
    }
    end:;
    /*node * no = new node();
    node * no1 = new node();
    node * no2 = new node();
    node * no3 = new node();
    node * no4 = new node();
    node * no5 = new node();
    node * no6 = new node();
    node * no7 = new node();
    node * no8 = new node();
    node * no9 = new node();
    node * no10 = new node();
    node * no11 = new node();
    node * no12 = new node();
    node * no13 = new node();
    node * no14 = new node();
    no->s = recti;
    no1->s = recti;
    no2->s = recti;
    no->data = "trial\nx";
    no1->data = "Trial\n  y";
    no2->data = "Trial\n  y";
    no3->data = "Trial\n  y";
    no4->data = "Trial\n  y";
    no5->data = "Trial\n  y";
    no6->data = "Trial\n  y";
    no7->data = "Trial\n  y";
    no8->data = "Trial\n  y";
    no4->s = recti;
    no1->FirChild = no4;
    no1->SecChild = no5;
    no2->FirChild = no6;
    no2->SecChild = no7;
    no3->FirChild = no8;
    no3->SecChild = no9;
    no10->FirChild = no11;
    no10->SecChild = no12;
    no11->FirChild = no13;
    no11->SecChild = no14;
    no->adj.push_back(no10);
    //no->adj.push_back(no1);
    //no->adj.push_back(no2);
    //no->AltChild = no3;
   // no->adj.push_back(no4);
    no->FirChild = no1;
    no->SecChild = no2 ;
    no->AltChild = no3;
    //DrawNode(no,x,y);
    //DrawNode(no1,100,100);
    DrawTree(no);*/
}

void MainWindow::on_InputString_textChanged()
{
    pars.clear();
    QString inp = ui->InputString->toPlainText();
    if(inp.size()!=0)
    {
        QVector<QPair<QString, QString> > tar = Scanner(inp);
        pars = tar;
        token = pars[0].second;
        int index = 0 ;
        QStandardItemModel *model = new QStandardItemModel(tar.size(),2,this); //2 Rows and 3 Columns
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Token value")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Token type")));
        for(QVector<QPair<QString, QString> >::Iterator it=tar.begin(); it != tar.end(); it++)
        {
            QStandardItem *first = new QStandardItem(QString(it->first));
            QStandardItem *second = new QStandardItem(QString(it->second));
            model->setItem(index,0,first);
            model->setItem(index,1,second);
            index++;
        }
        ui->tableView->setModel(model);
    }
}
