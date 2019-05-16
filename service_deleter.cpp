#include "service_deleter.h"
#include "ui_service_deleter.h"

CServiceDeleter::CServiceDeleter(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::ServiceDeleterWindow)
{
  m_pUi->setupUi(this);
  m_hQuery = new Handle(XSqlDatabase::getInstance());
}

CServiceDeleter::~CServiceDeleter() {
  delete m_pUi;
  delete m_hQuery;
}

void CServiceDeleter::showWindow(QTableView* hTbPriceList) {
  m_hTbPriceList = hTbPriceList;
  this->show();
}

void CServiceDeleter::on_accept_deleter_btn_clicked() {
  QString sQuery = QString("DELETE FROM pricelist "
                           "WHERE id_pricelist = '%1'")
                           .arg(m_pUi->id_spinBox->text());

  if (m_hQuery->executeSqlQuery(sQuery)) {
    QMessageBox::information(this, "Success",
                             "Service was successfully removed!");

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
    QMessageBox::critical(this, "Error!", "Please check your connection to database!");
}
