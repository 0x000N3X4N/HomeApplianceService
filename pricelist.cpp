#include "pricelist.h"
#include "ui_pricelist.h"


CPriceList::CPriceList(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::PriceListWindow)
{
  m_pUi->setupUi(this);
  m_pServiceAdd = new CServiceAdd();
  m_pServiceDeleter = new CServiceDeleter();

#pragma region Signals/Slots
  connect(this, &CPriceList::showServiceAdd,
          m_pServiceAdd, &CServiceAdd::showWindow);

  connect(this, &CPriceList::showServiceDeleter,
          m_pServiceDeleter, &CServiceDeleter::showWindow);
#pragma endregion

  m_pUi->tb_price_list->horizontalHeader()
                   ->setSectionResizeMode(QHeaderView::Stretch);
}

CPriceList::~CPriceList() {
  delete m_pUi;
  delete m_pServiceAdd;
  delete m_pServiceDeleter;
}

void CPriceList::setTablePriceList(QSortFilterProxyModel* model) {
  m_pUi->tb_price_list->setModel(model);
  m_pUi->tb_price_list->setSortingEnabled(true);
}

void CPriceList::on_add_price_btn_clicked() {
  emit showServiceAdd(m_pUi->tb_price_list);
}

void CPriceList::on_delete_btn_clicked() {
  emit showServiceDeleter(m_pUi->tb_price_list);
}
