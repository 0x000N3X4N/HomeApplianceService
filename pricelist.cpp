#include "pricelist.h"
#include "ui_pricelist.h"


CPriceList::CPriceList(QWidget *parent) :
  QWidget(parent),
  m_pUi(new Ui::PriceListWindow)
{
  m_pUi->setupUi(this);
  m_pServiceAdd = new CCompAdd();
  m_pServiceDeleter = new CServiceDeleter();
  m_menu_ptr = new QMenu();

#pragma region Signals/Slots
  connect(this, &CPriceList::showServiceAdd,
          m_pServiceAdd, &CCompAdd::showWindow);

  connect(this, &CPriceList::showServiceDeleter,
          m_pServiceDeleter, &CServiceDeleter::showWindow);
  connect(m_menu_ptr, &QMenu::triggered, [](QAction *action){
          qDebug()<< "triggered: " <<action->text();
      });
#pragma endregion
  m_menu_ptr->addAction("Add component");
  m_menu_ptr->addAction("Add component type");
  m_pUi->add_price_btn->setMenu(m_menu_ptr);

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

void CPriceList::on_up_btn_clicked() {
  QModelIndex curr_idx = m_pUi->tb_price_list->selectionModel()->currentIndex();
  qDebug() << "CPriceList::on_up_btn_clicked: [curr_row]: " << curr_idx.row();
  if (curr_idx.row() > 0)
  {
    QModelIndex upd_idx = m_pUi->tb_price_list->model()->index(curr_idx.row() - 1, curr_idx.column());
    m_pUi->tb_price_list->selectionModel()->clearSelection();
    qDebug() << m_pUi->tb_price_list->selectionModel()->currentIndex();
    m_pUi->tb_price_list->selectionModel()->select(upd_idx, QItemSelectionModel::Select);
    m_pUi->tb_price_list->selectionModel()->setCurrentIndex(upd_idx, QItemSelectionModel::Select);
  }
}

void CPriceList::on_down_btn_clicked() {
  QModelIndex curr_idx = m_pUi->tb_price_list->selectionModel()->currentIndex();
  auto nRow_cnt = m_pUi->tb_price_list->selectionModel()->currentIndex().model()->rowCount();
  qDebug() << "CPriceList::on_down_btn_clicked: [curr_row]: " << curr_idx.row();
  if (curr_idx.row() + 1 != nRow_cnt) {
    QModelIndex upd_idx = m_pUi->tb_price_list->model()->index(curr_idx.row() + 1, curr_idx.column());
    m_pUi->tb_price_list->selectionModel()->clearSelection();
    qDebug() << m_pUi->tb_price_list->selectionModel()->currentIndex();
    m_pUi->tb_price_list->selectionModel()->select(upd_idx, QItemSelectionModel::Select);
    m_pUi->tb_price_list->selectionModel()->setCurrentIndex(upd_idx, QItemSelectionModel::Select);
  }
}

void CPriceList::on_left_btn_clicked() {
  QModelIndex curr_idx = m_pUi->tb_price_list->selectionModel()->currentIndex();
  qDebug() << "CPriceList::on_left_btn_clicked: [curr_col]: " << curr_idx.column();
  if (curr_idx.column() > 0) {
    QModelIndex upd_idx = m_pUi->tb_price_list->model()->index(curr_idx.row(), curr_idx.column() - 1);
    m_pUi->tb_price_list->selectionModel()->clearSelection();
    qDebug() << m_pUi->tb_price_list->selectionModel()->currentIndex();
    m_pUi->tb_price_list->selectionModel()->select(upd_idx, QItemSelectionModel::Select);
    m_pUi->tb_price_list->selectionModel()->setCurrentIndex(upd_idx, QItemSelectionModel::Select);
  }
}

void CPriceList::on_right_btn_clicked() {
  QModelIndex curr_idx = m_pUi->tb_price_list->selectionModel()->currentIndex();
  auto nCol_cnt = m_pUi->tb_price_list->selectionModel()->currentIndex().model()->columnCount();
  qDebug() << "CPriceList::on_right_btn_clicked: [curr_col]: " << curr_idx.column();
  if (curr_idx.column() + 1 != nCol_cnt) {
    QModelIndex upd_idx = m_pUi->tb_price_list->model()->index(curr_idx.row(), curr_idx.column() + 1);
    m_pUi->tb_price_list->selectionModel()->clearSelection();
    qDebug() << m_pUi->tb_price_list->selectionModel()->currentIndex();
    m_pUi->tb_price_list->selectionModel()->select(upd_idx, QItemSelectionModel::Select);
    m_pUi->tb_price_list->selectionModel()->setCurrentIndex(upd_idx, QItemSelectionModel::Select);
  }
}
