#ifndef TESTUSER_H
#define TESTUSER_H

#include <QMainWindow>

namespace Ui {
class testuser;
}

class testuser : public QMainWindow
{
    Q_OBJECT

public:
    explicit testuser(QWidget *parent = nullptr);
    ~testuser();

private:
    Ui::testuser *ui;
};

#endif // TESTUSER_H
