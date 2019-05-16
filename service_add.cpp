#include "service_add.h"
#include "ui_service_add.h"

CServiceAdd::CServiceAdd(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::ServiceAddWindow)
{
  m_pUi->setupUi(this);
  m_hQuery = new Handle(XSqlDatabase::getInstance());
  std::for_each(m_listOfEquipment.begin(), m_listOfEquipment.end(),
                [=](QString& name) { m_pUi->name_equipment_comboBox->addItem(name); });
}

CServiceAdd::~CServiceAdd() {
  delete m_pUi;
  delete m_hQuery;
}

void CServiceAdd::showWindow(QTableView* hTbPriceList) {
  m_hTbPriceList = hTbPriceList;
  this->show();
}

void CServiceAdd::on_add_service_btn_clicked() {
  QString sQuery = QString("INSERT INTO pricelist (service, equipment, price)"
                           "VALUES ('%1', '%2', '%3')")
                           .arg(m_pUi->service_lEdit->text(),
                                m_pUi->name_equipment_comboBox->currentText(),
                                m_pUi->price_dSpinBox->text());

  if (m_hQuery->executeSqlQuery(sQuery)) {
    QMessageBox::information(this, "Success!", "Service was successfully added!");
    m_hQuery->clear();
    sQuery = "SELECT id_pricelist AS 'Id', service AS 'Service', "
             "price AS 'Price' FROM pricelist";
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
