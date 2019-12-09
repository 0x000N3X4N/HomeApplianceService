#include "customer_deleter.h"
#include "ui_customer_deleter.h"

CCustomerDel::CCustomerDel(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CCustomerDel)
{
  ui->setupUi(this);
}

CCustomerDel::~CCustomerDel()
{
  delete ui;
}
