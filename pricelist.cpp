﻿#include "pricelist.h"
#include "ui_pricelist.h"


CPriceList::CPriceList(QWidget *parent) :
  QWidget(parent),
  m_pUi(new Ui::PriceListWindow)
{
  m_pUi->setupUi(this);
  size_t i = 0;
  m_hQuery = new CQueryController(CQueryController(CODBCW::getInstance("", nullptr, &i)));
  m_comp_add_ptr = new CCompAdd();
  m_CompTyAdd_ptr = new CComp_TyAdd();
  m_pCompDeleter = new CCompDeleter();
  m_pCompAddMenu = new QMenu();
  m_pCompDelMenu = new QMenu();
  m_CompTyDel_ptr = new CComp_TyDeleter();
  m_pCompAddMenu->addAction("Add component");
  m_pCompAddMenu->addAction("Add component type");
  m_pCompDelMenu->addAction("Delete component");
  m_pCompDelMenu->addAction("Delete component type");
  m_pUi->add_price_btn->setMenu(m_pCompAddMenu);
  m_pUi->delete_btn->setMenu(m_pCompDelMenu);

#pragma region Signals/Slots
  connect(this, &CPriceList::showCompAdd,
          m_comp_add_ptr, &CCompAdd::showWindow);

  connect(this, &CPriceList::showCompDeleter,
          m_pCompDeleter, &CCompDeleter::showWindow);

  connect(this, &CPriceList::showCompTypeAdd,
          m_CompTyAdd_ptr, &CComp_TyAdd::showWindow);

  connect(this, &CPriceList::showCompTypeDeleter,
          m_CompTyDel_ptr, &CComp_TyDeleter::showWindow);

  connect(m_pCompAddMenu, &QMenu::triggered, [this] (QAction* action_ptr) {
            auto action_name_qstr = action_ptr->text();
            if (action_name_qstr.compare("Add component", Qt::CaseInsensitive) == 0) {
               try {
                 m_hQuery->clear();
                 if (m_hQuery->executeSqlQuery("SELECT * FROM components_type;")) {
                   while (m_hQuery->next()) {
                     auto comp = std::make_shared<CCompsTraits::comp<>>(m_hQuery->parse_value(1).toString(),
                                                                        m_hQuery->parse_value(0).toUInt());
                     CCompsTraits::get_comps().get()->push_back(comp);
                   }

                     emit showCompAdd(m_pUi->pricelist_tbv);
                 } else throw std::invalid_argument("CPriceList::CPriceList : Error execute query!");
               }
               catch(std::invalid_argument& e) {
                 CMsgBox::show(QMessageBox::Critical, this, "Error", e.what());
               }

            } else if (action_name_qstr.compare("Add component type", Qt::CaseInsensitive) == 0) {
              emit showCompTypeAdd(m_pUi->comp_Ty_tbv);
            }
          });

  connect(m_pCompDelMenu, &QMenu::triggered, [this] (QAction* action_ptr) {
    auto action_name_qstr = action_ptr->text();
    if (action_name_qstr.compare("Delete component", Qt::CaseInsensitive) == 0) {
      try {
        m_hQuery->clear();
        if (m_hQuery->executeSqlQuery("SELECT PK_component_id, title FROM components;"))
        {
          while (m_hQuery->next()) {
            auto comp = std::make_shared<PCOM::comp<>>(m_hQuery->parse_value(1).toString(),
                                                       m_hQuery->parse_value(0).toUInt());
            CCompsTraits::get_comps().get()->push_back(comp);
          }

          emit showCompDeleter(m_pUi->pricelist_tbv);
        }
        //TODO: improve err handling
        else throw std::invalid_argument("CPriceList::CPriceList : Error execute query!");
      }
      catch(...) {
        CMsgBox::show(QMessageBox::Critical, this, "Error!", "CPriceList::CPriceList : Unexpected error!");
      }
    } else if (action_name_qstr.compare("Delete component type", Qt::CaseInsensitive) == 0) {
      try {
        m_hQuery->clear();
        if (m_hQuery->executeSqlQuery("SELECT * FROM components_type;"))
        {
          while (m_hQuery->next()) {
            auto comp = std::make_shared<PCOM::comp<>>(m_hQuery->parse_value(1).toString(),
                                                       m_hQuery->parse_value(0).toUInt());
            CCompsTraits::get_comps().get()->push_back(comp);
          }

          emit showCompTypeDeleter(m_pUi->comp_Ty_tbv, m_pUi->pricelist_tbv);
        }
        else throw std::invalid_argument("CPriceList::CPriceList : Error execute query!");
      }
      catch(...) {
        CMsgBox::show(QMessageBox::Critical, this, "Error!", "CPriceList::CPriceList : Unexpected error!");
      }
    }
  });
#pragma endregion

  m_pUi->pricelist_tbv->horizontalHeader()
                   ->setSectionResizeMode(QHeaderView::Stretch);

  m_pUi->comp_Ty_tbv->horizontalHeader()
                   ->setSectionResizeMode(QHeaderView::Stretch);
}

CPriceList::~CPriceList() {
  delete m_pUi;
  delete m_hQuery;
  delete m_comp_add_ptr;
  delete m_pCompDeleter;
  delete m_CompTyAdd_ptr;
}

void CPriceList::setTablePriceList(QSortFilterProxyModel* hCompFModel,
                                   QSortFilterProxyModel* hCompTypeFModel) {
  m_pUi->pricelist_tbv->setModel(hCompFModel);
  m_pUi->pricelist_tbv->setSortingEnabled(true);
  m_pUi->comp_Ty_tbv->setModel(hCompTypeFModel);
  m_pUi->comp_Ty_tbv->setSortingEnabled(true);
}

void CPriceList::on_up_btn_clicked() {
  QModelIndex curr_idx = m_pUi->pricelist_tbv->selectionModel()->currentIndex();
  qDebug() << "CPriceList::on_up_btn_clicked: [curr_row]: " << curr_idx.row();
  if (curr_idx.isValid() && curr_idx.row() > 0)
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

  qDebug() << "CPriceList::on_down_btn_clicked: [curr_row]: " << curr_idx.row();
  if (curr_idx.isValid()) {
    auto nRow_cnt = curr_idx.model()->rowCount();
    if (curr_idx.row() + 1 != nRow_cnt) {
      QModelIndex upd_idx = m_pUi->pricelist_tbv->model()->index(curr_idx.row() + 1, curr_idx.column());
      m_pUi->pricelist_tbv->selectionModel()->clearSelection();
      qDebug() << m_pUi->pricelist_tbv->selectionModel()->currentIndex();
      m_pUi->pricelist_tbv->selectionModel()->select(upd_idx, QItemSelectionModel::Select);
      m_pUi->pricelist_tbv->selectionModel()->setCurrentIndex(upd_idx, QItemSelectionModel::Select);
    }
  }
}

void CPriceList::on_left_btn_clicked() {
  QModelIndex curr_idx = m_pUi->pricelist_tbv->selectionModel()->currentIndex();
  qDebug() << "CPriceList::on_left_btn_clicked: [curr_col]: " << curr_idx.column();

  if (curr_idx.isValid()) {
    if (curr_idx.column() > 0) {
      QModelIndex upd_idx = m_pUi->pricelist_tbv->model()->index(curr_idx.row(), curr_idx.column() - 1);
      m_pUi->pricelist_tbv->selectionModel()->clearSelection();
      qDebug() << m_pUi->pricelist_tbv->selectionModel()->currentIndex();
      m_pUi->pricelist_tbv->selectionModel()->select(upd_idx, QItemSelectionModel::Select);
      m_pUi->pricelist_tbv->selectionModel()->setCurrentIndex(upd_idx, QItemSelectionModel::Select);
    }
  }
}

void CPriceList::on_right_btn_clicked() {
  QModelIndex curr_idx = m_pUi->pricelist_tbv->selectionModel()->currentIndex();
  qDebug() << "CPriceList::on_right_btn_clicked: [curr_col]: " << curr_idx.column();

  if (curr_idx.isValid()) {
    auto nCol_cnt = m_pUi->pricelist_tbv->selectionModel()->currentIndex().model()->columnCount();
    if (curr_idx.column() + 1 != nCol_cnt) {
      QModelIndex upd_idx = m_pUi->pricelist_tbv->model()->index(curr_idx.row(), curr_idx.column() + 1);
      m_pUi->pricelist_tbv->selectionModel()->clearSelection();
      qDebug() << m_pUi->pricelist_tbv->selectionModel()->currentIndex();
      m_pUi->pricelist_tbv->selectionModel()->select(upd_idx, QItemSelectionModel::Select);
      m_pUi->pricelist_tbv->selectionModel()->setCurrentIndex(upd_idx, QItemSelectionModel::Select);
    }
  }
}

CComp_TyDeleter* CPriceList::getCompTypeDel() const { return m_CompTyDel_ptr; }

CCompDeleter* CPriceList::getCompDel() const { return m_pCompDeleter; }
