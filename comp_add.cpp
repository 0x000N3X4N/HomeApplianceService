#include "comp_add.h"
#include "ui_comp_add.h"


CCompAdd::CCompAdd(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::ServiceAddWindow)
{
  m_pUi->setupUi(this);
  m_hQuery = new CQueryController(CODBCW::getInstance());

//  std::for_each(m_listOfEquipment.begin(), m_listOfEquipment.end(),
//                [=](QString& name) { m_pUi->component_type_comboBox->addItem(name); });
}

CCompAdd::~CCompAdd() {
  delete m_pUi;
  delete m_hQuery;
}

void CCompAdd::showWindow(QTableView* hTbPriceList) {
  m_hTbPriceList = hTbPriceList;
  m_hQuery->clear();
  m_hQuery->executeSqlQuery(QString("SELECT * "
                                    "FROM components_type;"));
  this->show();
}

void CCompAdd::on_add_service_btn_clicked() {
  QString sQuery = QString("INSERT INTO components "
                           "VALUES ('%1', '%2', '%3', '%4', '2019')")
                           .arg(m_pUi->component_type_comboBox->currentText(),
                                m_pUi->component_name_lEdit->text(),
                                m_pUi->spec_lEdit->text(),
                                m_pUi->price_dSpinBox->text());

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
