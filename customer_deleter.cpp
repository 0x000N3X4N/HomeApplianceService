#include "customer_deleter.h"
#include "ui_customer_deleter.h"


CCustomerDel::CCustomerDel(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::CustomerDeleterWIndow)
{
  m_pUi->setupUi(this);
}

CCustomerDel::~CCustomerDel()
{
  delete m_pUi;
}
