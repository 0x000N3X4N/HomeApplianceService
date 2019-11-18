#include "comp_add.h"
#include "ui_comp_add.h"


CCompAdd::CCompAdd(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::ServiceAddWindow)
{
  m_pUi->setupUi(this);
  m_hQuery = new CQueryController(CODBCW::getInstance());
}

CCompAdd::~CCompAdd() {
  delete m_pUi;
  delete m_hQuery;
}

void CCompAdd::showWindow(QTableView* hTbPriceList) {
  try {
    m_hTbPriceList = hTbPriceList;
    m_hQuery->clear();
    if (m_hQuery->executeSqlQuery("SELECT * FROM components_type;")) {
      while (m_hQuery->next())
        m_map_comp_items[m_hQuery->parse_value(1).toString()] = m_hQuery->parse_value(0).toUInt();

      update_comp_items();
      this->show();
    } else throw std::invalid_argument("CCompAdd::showWindow(..) : Error execute query!");
  }
  catch(std::invalid_argument& e) {
    QMessageBox::critical(this, "Error", e.what());
  }
}

void CCompAdd::on_add_service_btn_clicked() {
  auto it = m_map_comp_items.find(m_pUi->component_type_comboBox->currentText());

  QString sQuery = QString("INSERT INTO components "
                           "VALUES ('%1', '%2', '%3', '%4', '2019')")
                           .arg(QString::number(it->second),
                                m_pUi->component_name_lEdit->text(),
                                m_pUi->spec_lEdit->text(),
                                QString::number(m_pUi->price_dSpinBox->value()));

  if (m_hQuery->executeSqlQuery(sQuery)) {
    QMessageBox::information(this, "Success!", "Service was successfully added!");
    m_hQuery->clear();
    sQuery = "SELECT PK_component_id AS 'ID', product_type AS 'Component type', title AS 'Title',"
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
    QMessageBox::critical(this, "Error", "Error to add service into your pricelist!\r\n"
                                         "Please, check your connection to database!");
}

void CCompAdd::update_comp_items() {
  m_pUi->component_type_comboBox->clear();

  for (auto it = m_map_comp_items.begin(); it != m_map_comp_items.end(); it++)
    m_pUi->component_type_comboBox->addItem(it->first);
}
