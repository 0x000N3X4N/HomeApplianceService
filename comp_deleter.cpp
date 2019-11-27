#include "comp_deleter.h"
#include "ui_comp_deleter.h"


CCompDeleter::CCompDeleter(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::CompDeleterWindow)
{
  m_pUi->setupUi(this);
  m_hQuery = new CQueryController(CODBCW::getInstance());
  connect(this, &CCompDeleter::finished,
          this, &CCompDeleter::CCompDeleter::clearUi);
}

CCompDeleter::~CCompDeleter() {
  delete m_pUi;
  delete m_hQuery;
}

void CCompDeleter::showWindow(QTableView* hTbPriceList,
                              component_itemT& _map_comp_items) {
  m_hTbPriceList = hTbPriceList;
  m_map_comp_items = _map_comp_items;
  //TODO: @arg: std::move
  // pass as rvalue ref
  // FIXME
  _map_comp_items.clear();
  upd_comp_items();
  this->show();
}

void CCompDeleter::on_accept_deleter_btn_clicked() {
  QString sQuery = QString("DELETE FROM components "
                           "WHERE title = '%1'")
                           .arg(m_pUi->comp_type_cmbBox->currentText());

  if (m_hQuery->executeSqlQuery(sQuery)) {
    m_map_comp_items.erase(m_pUi->comp_type_cmbBox->currentText());

    QMessageBox::information(this, "Success",
                             "Component was successfully removed!");

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
    upd_comp_items();
  } else
    QMessageBox::critical(this, "Error!", "Please check your connection to database!");
}

void CCompDeleter::upd_comp_items() {
  m_pUi->comp_type_cmbBox->clear();

  for (auto it = m_map_comp_items.begin(); it != m_map_comp_items.end(); it++)
    m_pUi->comp_type_cmbBox->addItem(it->first);
}

void CCompDeleter::clearUi() {
  m_pUi->comp_type_cmbBox->clear();
}
