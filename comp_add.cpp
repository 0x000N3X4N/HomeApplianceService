#include "comp_add.h"
#include "ui_comp_add.h"


CCompAdd::CCompAdd(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::ServiceAddWindow)
{
  m_pUi->setupUi(this);
  QWidget::connect(this, &CCompAdd::finished,
          this, &CCompAdd::clearUi);
  m_hQuery = new CQueryController(CODBCW::getInstance());
}

CCompAdd::~CCompAdd() {
  delete m_pUi;
  delete m_hQuery;
}

void CCompAdd::clearUi() {
  m_pUi->spec_lEdit->clear();
  m_pUi->component_name_lEdit->clear();
  m_pUi->component_type_comboBox->clear();
  m_pUi->price_dSpinBox->clear();
}

void CCompAdd::showWindow(QTableView* hTbPriceList) {
  m_hTbPriceList = hTbPriceList;
  update_comp_items();
  this->show();
}

void CCompAdd::on_add_service_btn_clicked() {
//  auto it = CComponents::comps.find(m_pUi->component_type_comboBox->currentText());

  CCompsTraits::comps.
  QString sQuery = QString("INSERT INTO components "
                           "VALUES ('%1', '%2', '%3', '%4', '2019')")
                           .arg(QString::number(it->second),
                                m_pUi->component_name_lEdit->text(),
                                m_pUi->spec_lEdit->text(),
                                QString::number(m_pUi->price_dSpinBox->value()));

  try {
    if (m_hQuery->executeSqlQuery(sQuery)) {
      QMessageBox::information(this, "Success!", "Service was successfully added!");
      m_hQuery->clear();
      sQuery = "SELECT PK_component_id AS 'ID', component_type AS 'Component type', title AS 'Title',"
               " specifications AS 'Specifications', price AS 'Price', release_date AS 'Release date' "
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
    } else
      throw std::invalid_argument("CCompAdd::on_add_service_btn_clicked : " +
                                  m_hQuery->getQuery().lastError().text().toStdString());
  }
  catch(std::invalid_argument& e) {
    QMessageBox::critical(this, "Error!", e.what());
  }
  catch(...) {
    QMessageBox::critical(this, "Error!", "CCompAdd::on_add_service_btn_clicked : Unexpected error!");
  }
}

void CCompAdd::update_comp_items() {
  m_pUi->component_type_comboBox->clear();

  for (auto it = m_map_comp_items.begin(); it != m_map_comp_items.end(); it++)
    m_pUi->component_type_comboBox->addItem(it->first);
}
