#include "testuser.h"
#include "ui_testuser.h"

testuser::testuser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::testuser)
{
    ui->setupUi(this);
}

testuser::~testuser()
{
    delete ui;
}
