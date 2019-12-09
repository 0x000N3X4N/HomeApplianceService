#include "customer_add.h"
#include "ui_customer_add.h"


CCustomerAdd::CCustomerAdd(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::CustomerAddWindow)
{
  m_pUi->setupUi(this);
  m_pUi->phone_le->setValidator(new QRegExpValidator(QRegExp("^[+][0-9]*")));
}

CCustomerAdd::~CCustomerAdd()
{
  delete m_pUi;
  delete m_city_map;
  delete m_pCust_tb;
}

void CCustomerAdd::showWindow(QTableView* cust_tb_ptr,
                              std::map<QString, size_t>* city_map) {
  m_city_map = city_map;

  for (auto it = city_map->begin(); it != city_map->end(); ++it)
    m_pUi->city_cBox->addItem(it->first);

  m_pCust_tb = cust_tb_ptr;
  show();
}
