#include "pricelist.h"
#include "ui_pricelist.h"


CPriceList::CPriceList(QWidget *parent) :
  QWidget(parent),
  m_pUi(new Ui::PriceListWindow)
{
  m_pUi->setupUi(this);
  m_comp_add_ptr = new CCompAdd();
  m_CompTyAdd_ptr = new CComp_TyAdd();
  m_pServiceDeleter = new CServiceDeleter();
  m_menu_ptr = new QMenu();
  m_menu_ptr->addAction("Add component");
  m_menu_ptr->addAction("Add component type");
  m_pUi->add_price_btn->setMenu(m_menu_ptr);

#pragma region Signals/Slots
  connect(this, &CPriceList::showCompAdd,
          m_comp_add_ptr, &CCompAdd::showWindow);

  connect(this, &CPriceList::showServiceDeleter,
          m_pServiceDeleter, &CServiceDeleter::showWindow);

  connect(m_menu_ptr, &QMenu::triggered, [this] (QAction* action_ptr) {
            auto action_name_qstr = action_ptr->text();
            if (action_name_qstr.compare("Add component", Qt::CaseInsensitive) == 0) {
              emit showCompAdd(m_pUi->pricelist_tbv);
            } else if (action_name_qstr.compare("Add component type", Qt::CaseInsensitive) == 0) {
              m_CompTyAdd_ptr->show();
            }
          });
#pragma endregion

  m_pUi->pricelist_tbv->horizontalHeader()
                   ->setSectionResizeMode(QHeaderView::Stretch);
  m_pUi->comp_Ty_tbv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

CPriceList::~CPriceList() {
  delete m_pUi;
  delete m_comp_add_ptr;
  delete m_pServiceDeleter;
}

void CPriceList::setTablePriceList(QSortFilterProxyModel* model) {
  m_pUi->pricelist_tbv->setModel(model);
  m_pUi->pricelist_tbv->setSortingEnabled(true);
}

void CPriceList::on_add_price_btn_clicked() {
  emit showCompAdd(m_pUi->pricelist_tbv);
}

void CPriceList::on_delete_btn_clicked() {
  emit showServiceDeleter(m_pUi->pricelist_tbv);
}

void CPriceList::on_up_btn_clicked() {
  QModelIndex curr_idx = m_pUi->pricelist_tbv->selectionModel()->currentIndex();
  qDebug() << "CPriceList::on_up_btn_clicked: [curr_row]: " << curr_idx.row();
  if (curr_idx.row() > 0)
  {
    QModelIndex upd_idx = m_pUi->pricelist_tbv->model()->index(curr_idx.row() - 1, curr_idx.column());
    m_pUi->pricelist_tbv->selectionModel()->clearSelection();
    qDebug() << m_pUi->pricelist_tbv->selectionModel()->currentIndex();
    m_pUi->pricelist_tbv->selectionModel()->select(upd_idx, QItemSelectionModel::Select);
    m_pUi->pricelist_tbv->selectionModel()->setCurrentIndex(upd_idx, QItemSelectionModel::Select);
  }
}

void CPriceList::on_down_btn_clicked() {
  QModelIndex curr_idx = m_pUi->pricelist_tbv->selectionModel()->currentIndex();
  auto nRow_cnt = m_pUi->pricelist_tbv->selectionModel()->currentIndex().model()->rowCount();
  qDebug() << "CPriceList::on_down_btn_clicked: [curr_row]: " << curr_idx.row();
  if (curr_idx.row() + 1 != nRow_cnt) {
    QModelIndex upd_idx = m_pUi->pricelist_tbv->model()->index(curr_idx.row() + 1, curr_idx.column());
    m_pUi->pricelist_tbv->selectionModel()->clearSelection();
    qDebug() << m_pUi->pricelist_tbv->selectionModel()->currentIndex();
    m_pUi->pricelist_tbv->selectionModel()->select(upd_idx, QItemSelectionModel::Select);
    m_pUi->pricelist_tbv->selectionModel()->setCurrentIndex(upd_idx, QItemSelectionModel::Select);
  }
}

void CPriceList::on_left_btn_clicked() {
  QModelIndex curr_idx = m_pUi->pricelist_tbv->selectionModel()->currentIndex();
  qDebug() << "CPriceList::on_left_btn_clicked: [curr_col]: " << curr_idx.column();
  if (curr_idx.column() > 0) {
    QModelIndex upd_idx = m_pUi->pricelist_tbv->model()->index(curr_idx.row(), curr_idx.column() - 1);
    m_pUi->pricelist_tbv->selectionModel()->clearSelection();
    qDebug() << m_pUi->pricelist_tbv->selectionModel()->currentIndex();
    m_pUi->pricelist_tbv->selectionModel()->select(upd_idx, QItemSelectionModel::Select);
    m_pUi->pricelist_tbv->selectionModel()->setCurrentIndex(upd_idx, QItemSelectionModel::Select);
  }
}

void CPriceList::on_right_btn_clicked() {
  QModelIndex curr_idx = m_pUi->pricelist_tbv->selectionModel()->currentIndex();
  auto nCol_cnt = m_pUi->pricelist_tbv->selectionModel()->currentIndex().model()->columnCount();
  qDebug() << "CPriceList::on_right_btn_clicked: [curr_col]: " << curr_idx.column();
  if (curr_idx.column() + 1 != nCol_cnt) {
    QModelIndex upd_idx = m_pUi->pricelist_tbv->model()->index(curr_idx.row(), curr_idx.column() + 1);
    m_pUi->pricelist_tbv->selectionModel()->clearSelection();
    qDebug() << m_pUi->pricelist_tbv->selectionModel()->currentIndex();
    m_pUi->pricelist_tbv->selectionModel()->select(upd_idx, QItemSelectionModel::Select);
    m_pUi->pricelist_tbv->selectionModel()->setCurrentIndex(upd_idx, QItemSelectionModel::Select);
  }
}
