#include "comp_deleter.h"
#include "ui_comp_deleter.h"


CCompDeleter::CCompDeleter(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::CompDeleterWindow)
{
  m_pUi->setupUi(this);
  m_hQuery = new CQueryController(CODBCW::getInstance());
  QWidget::connect(this, &CCompDeleter::finished,
          this, &CCompDeleter::CCompDeleter::clearUi);
}

CCompDeleter::~CCompDeleter() {
  delete m_pUi;
  delete m_hQuery;
}

void CCompDeleter::showWindow(QTableView* hTbPriceList) {
  m_hTbPriceList = hTbPriceList;
  //TODO: @arg: std::move
  // pass as rvalue ref
  // FIXME
  CCompsTraits::get_comps().upd(*m_pUi->comp_name_cmbBox);
  this->show();
}

void CCompDeleter::on_accept_deleter_btn_clicked() {
  QString sQuery = QString("DELETE FROM components "
                           "WHERE title = '%1'")
                           .arg(m_pUi->comp_name_cmbBox->currentText());

  if (m_hQuery->executeSqlQuery(sQuery)) {
    CCompsTraits::get_comps().rm(m_pUi->comp_name_cmbBox->currentText());

    QMessageBox::information(this, "Success",
                             "Component'" + m_pUi->comp_name_cmbBox->currentText() + "' was successfully removed!");

    sQuery = "SELECT title AS 'Title',"
             "specifications AS 'Specifications', price AS 'Price', release_date AS 'Release date' "
             "FROM components "
             "JOIN components_type "
             "ON FK_type_code = PK_component_type_id;";

    m_hQuery->executeSqlQuery(sQuery);

    QSqlQueryModel* hModel = new QSqlQueryModel();
    QSortFilterProxyModel* hFilterModel = new QSortFilterProxyModel();
    hModel->setQuery(m_hQuery->getQuery());
    hFilterModel->setDynamicSortFilter(true);
    hFilterModel->setSourceModel(hModel);
    m_hTbPriceList->setModel(hFilterModel);
    CCompsTraits::get_comps().upd(*m_pUi->comp_name_cmbBox);
  } else
    QMessageBox::critical(this, "Error!", "Please check your connection to database!");
}

void CCompDeleter::clearUi() {
  CCompsTraits::get_comps().get()->clear();
  m_pUi->comp_name_cmbBox->clear();
}
