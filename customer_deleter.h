#ifndef CUSTOMER_DELETER_H
#define CUSTOMER_DELETER_H

#include <QDialog>

namespace Ui {
  class CCustomerDel;
}

class CCustomerDel : public QDialog
{
  Q_OBJECT

public:
  explicit CCustomerDel(QWidget *parent = nullptr);
  ~CCustomerDel();

private:
  Ui::CCustomerDel *ui;
};

#endif // CUSTOMER_DELETER_H
