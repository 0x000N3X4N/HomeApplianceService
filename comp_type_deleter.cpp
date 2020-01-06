#include "comp_type_deleter.h"
#include "ui_comp_type_deleter.h"


CComp_TyDeleter::CComp_TyDeleter(QWidget* parent) :
  QDialog(parent),
  m_pUi(new Ui::comp_type_deleter)
{
  m_pUi->setupUi(this);
  m_pTbCompType = new QTableView();
  m_pTbComps = new QTableView();
  size_t i = 0;
  m_hQuery = new CQueryController(CQueryController(CODBCW::getInstance("", nullptr, &i)));

  connect(this, &CComp_TyDeleter::finished,
          this, &CComp_TyDeleter::clearUi);
}

CComp_TyDeleter::~CComp_TyDeleter() {
  delete m_pUi;
}

void CComp_TyDeleter::showWindow(QTableView* hTbCompType, QTableView* hTbComps) {
  m_pTbCompType = hTbCompType;
  m_pTbComps = hTbComps;
  CCompsTraits::get_comps().upd(*m_pUi->comp_type_cBox);
  show();
}

void CComp_TyDeleter::on_delete_comp_type_btn_clicked() {
  QMessageBox::StandardButton reply;
  reply = QMessageBox::warning(this, "Warning!", "This action may delete order, are you sure? "
                                                 "More information is available in the help.",
                               QMessageBox::Yes | QMessageBox::No);

  if (reply == QMessageBox::Yes) {
    QString sQuery = QString("DELETE FROM components_type "
                             "WHERE component_type = '%1'")
                             .arg(m_pUi->comp_type_cBox->currentText());

    try {
      if (m_hQuery->executeSqlQuery(sQuery)) {
        CCompsTraits::get_comps().rm(m_pUi->comp_type_cBox->currentText());

        CMsgBox::show(QMessageBox::Information, this, "Success",
                      "Component '" + m_pUi->comp_type_cBox->currentText()
                      + "' was successfully removed!");

        sQuery = "SELECT component_type AS 'Component type' FROM components_type;";

        m_hQuery->clear();
        m_hQuery->executeSqlQuery(sQuery);

        QSqlQueryModel* hCompTypeModel = new QSqlQueryModel();
        QSortFilterProxyModel* hCompTypeFilterModel = new QSortFilterProxyModel();

        hCompTypeModel->setQuery(m_hQuery->getQuery());
        hCompTypeFilterModel->setDynamicSortFilter(true);
        hCompTypeFilterModel->setSourceModel(hCompTypeModel);
        m_pTbCompType->setModel(hCompTypeFilterModel);
        CCompsTraits::get_comps().upd(*m_pUi->comp_type_cBox);

        sQuery = "SELECT title AS 'Title', component_type AS 'Component type',"
                 " specifications AS 'Specifications', price AS 'Price', release_date AS 'Release date' "
                 "FROM components "
                 "JOIN components_type "
                 "ON FK_type_code = PK_component_type_id;";

        m_hQuery->clear();
        m_hQuery->executeSqlQuery(sQuery);


        QSqlQueryModel* hCompModel = new QSqlQueryModel();
        QSortFilterProxyModel* hCompFilterModel = new QSortFilterProxyModel();

        hCompModel->setQuery(m_hQuery->getQuery());
        hCompFilterModel->setDynamicSortFilter(true);
        hCompFilterModel->setSourceModel(hCompModel);
        m_pTbComps->setModel(hCompFilterModel);

        emit updOrders();
      } else
        throw std::invalid_argument("CComp_TyDeleter::on_delete_comp_type_btn_clicked : " +
                                    m_hQuery->getQuery().lastError().text().toStdString());
    }
    catch(const  std::invalid_argument& e) {
      CMsgBox::show(QMessageBox::Critical, this, "Error!", e.what());
    }
    catch(...) {
      CMsgBox::show(QMessageBox::Critical, this, "Error!", "CComp_TyDeleter::on_delete_comp_type_btn_clicked : "
                                                           "Unexpected error!");
    }
  }
}

void CComp_TyDeleter::clearUi() {
  m_pUi->comp_type_cBox->clear();
  CCompsTraits::get_comps().get()->clear();
}
