#include "comp_type_deleter.h"
#include "ui_comp_type_deleter.h"


CComp_TyDeleter::CComp_TyDeleter(QWidget* parent) :
  QDialog(parent),
  m_pUi(new Ui::comp_type_deleter)
{
  m_pUi->setupUi(this);
  m_pTbCompType = new QTableView();
  m_hQuery = new CQueryController(CODBCW::getInstance());

  connect(this, &CComp_TyDeleter::finished,
          this, &CComp_TyDeleter::clearUi);
}

CComp_TyDeleter::~CComp_TyDeleter() {
  delete m_pUi;
}

void CComp_TyDeleter::showWindow(QTableView* hTbCompType) {
  m_pTbCompType = hTbCompType;
  CCompsTraits::get_comps().upd(*m_pUi->comp_type_cBox);
  show();
}

void CComp_TyDeleter::on_delete_comp_type_btn_clicked() {
  QString sQuery = QString("DELETE FROM components_type "
                           "WHERE component_type = '%1'")
                           .arg(m_pUi->comp_type_cBox->currentText());

  try {
    if (m_hQuery->executeSqlQuery(sQuery)) {
      CCompsTraits::get_comps().rm(m_pUi->comp_type_cBox->currentText());

      QMessageBox::information(this, "Success",
                               "Component '" + m_pUi->comp_type_cBox->currentText()
                               + "' was successfully removed!");

      sQuery = "SELECT component_type AS 'Component type' FROM components_type;";

      m_hQuery->clear();
      m_hQuery->executeSqlQuery(sQuery);

      QSqlQueryModel* hModel = new QSqlQueryModel();
      QSortFilterProxyModel* hFilterModel = new QSortFilterProxyModel();
      hModel->setQuery(m_hQuery->getQuery());
      hFilterModel->setDynamicSortFilter(true);
      hFilterModel->setSourceModel(hModel);
      m_pTbCompType->setModel(hFilterModel);
      CCompsTraits::get_comps().upd(*m_pUi->comp_type_cBox);
    } else
      throw std::invalid_argument("CComp_TyDeleter::on_delete_comp_type_btn_clicked : " +
                                  m_hQuery->getQuery().lastError().text().toStdString());
  }
  catch(const  std::invalid_argument& e) {
    QMessageBox::critical(this, "Error!", e.what());
  }
  catch(...) {
    QMessageBox::critical(this, "Error!", "CComp_TyDeleter::on_delete_comp_type_btn_clicked : Unexpected error!");
  }
}

void CComp_TyDeleter::clearUi() {
  m_pUi->comp_type_cBox->clear();
  CCompsTraits::get_comps().get()->clear();
}
