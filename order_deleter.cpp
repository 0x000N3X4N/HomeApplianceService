#include "order_deleter.h"
#include "ui_order_deleter.h"


COrderDel::COrderDel(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::OrderWindow)
{
  m_pUi->setupUi(this);
}

COrderDel::~COrderDel()
{
  delete m_pUi;
}
