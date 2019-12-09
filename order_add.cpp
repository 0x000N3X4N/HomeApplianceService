#include "order_add.h"
#include "ui_order_add.h"


struct COrderAdd::priceListHandler {
  std::vector<QString> m_nameList;
  std::shared_ptr<double[]> m_pPriceList;
};

COrderAdd::COrderAdd(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::OrderAddWindow)
{
  m_pUi->setupUi(this);
  m_pPriceListHandler = new priceListHandler();
  m_total = 0;
  m_prevTotal = 0;
  m_guarantee = 0;

#pragma region Init UI elemnts
  m_pUi->complete_dateEdit->setDate(QDate::currentDate());
  m_pUi->accept_dateEdit->setDate(QDate::currentDate());
  m_pUi->accept_dateEdit->setCalendarPopup(true);
  m_pUi->complete_dateEdit->setCalendarPopup(true);
  m_pUi->status_comboBox->addItem("Open");
  m_pUi->status_comboBox->addItem("Started");
  m_pUi->status_comboBox->addItem("Closed");
  m_pUi->type_repair_comboBox->addItem("Minor repair");
  m_pUi->type_repair_comboBox->addItem("Overhaul");
#pragma endregion

}

COrderAdd::~COrderAdd() {
  delete m_pUi;
  delete m_pPriceListHandler;
}

void COrderAdd::showWindow() { this->show(); }

void COrderAdd::on_name_comboBox_currentIndexChanged(int index) {
//  if (!m_priceListStructure.empty()) {
//    m_map_it = m_priceListStructure.begin();
//    m_pUi->total_value_label->setText(QString::number(m_total));
//    m_pUi->guarantee_dial->setValue(0);
//    m_prevTotal = m_total;
//  }
}

void COrderAdd::on_accept_btn_clicked() {
  QString sQuery;
  if (m_pUi->complete_dateEdit->isEnabled())
    sQuery = QString("INSERT INTO `home_appliance_service`.`order`"
                     "(name, type, acceptance_date, completion_date, total, status, completed_surname)"
                     "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7')")
                     .arg(m_pUi->name_comboBox->currentText(), m_pUi->type_repair_comboBox->currentText(),
                          m_pUi->accept_dateEdit->date().toString("yyyy-MM-dd"),
                          m_pUi->complete_dateEdit->date().toString("yyyy-MM-dd"),
                          QString::number(m_total), m_pUi->status_comboBox->currentText(),
                          m_pUi->completed_order_name_lEdit->text());

  else
    sQuery = QString("INSERT INTO `home_appliance_service`.`order`("
                     "name, type, acceptance_date, total, status, completed_surname)"
                     "VALUES ('%1', '%2', '%3', '%4', '%5', '%6')")
                     .arg(m_pUi->name_comboBox->currentText(),
                          m_pUi->type_repair_comboBox->currentText(),
                          m_pUi->accept_dateEdit->date().toString("yyyy-MM-dd"),
                          m_pUi->total_value_label->text(),
                          m_pUi->status_comboBox->currentText(),
                          m_pUi->completed_order_name_lEdit->text());

  if (m_hQuery->executeSqlQuery(sQuery)) {
    m_hQuery->clear();
    QMessageBox::information(this, "Successfully!", "Order has been success added!");

    m_hQuery->executeSqlQuery(
            "SELECT id_order AS 'Id', name AS 'Name', type AS 'Type', "
            "acceptance_date AS 'Accept_date', completion_date AS 'Complete_date', "
            "total AS 'Total', status AS 'Status', completed_surname AS 'Surname' "
            "FROM home_appliance_service.order"
          );

    //TODO: refactor
    QSqlQueryModel* hModel = new QSqlQueryModel();
    QSortFilterProxyModel* hFilterModel = new QSortFilterProxyModel();
    hModel->setQuery(m_hQuery->getQuery());
    hFilterModel->setSourceModel(hModel);
    m_pTbOrders->setModel(hFilterModel);
  } else
    QMessageBox::critical(this, "Error", "Cannnot add order to database\r\n"
                                         "Please check your connection to database!");
}

void COrderAdd::on_status_comboBox_currentIndexChanged(int index) {
  (index == 2) ? m_pUi->complete_dateEdit->setEnabled(true)
               : m_pUi->complete_dateEdit->setEnabled(false);
}

//void COrderAdd::on_guarantee_dial_valueChanged(int value) {
//  m_pUi->year_guarantee_count_label->setText(QString::number(value));
//  m_guarantee = value * 10;
//#ifdef QT_DEBUG
//  qDebug() << "[on_guarantee_dial_valueChanged] Guarantee value changed to: " << m_guarantee;
//#endif
//}

//void COrderAdd::on_guarantee_dial_sliderReleased() {
//  m_total = m_prevTotal + m_guarantee;
//  m_pUi->total_value_label->setText(QString::number(m_total));
//#ifdef QT_DEBUG
//  qDebug() << "[on_guarantee_dial_sliderReleased] m_total: " << m_total;
//#endif
//}

//void COrderAdd::on_RecordWindow_finished(int result) {
//  m_pUi->name_comboBox->clear();
//  m_pUi->type_repair_comboBox->setCurrentIndex(0);
//  m_pUi->completed_order_name_lEdit->clear();
//  m_pUi->status_comboBox->setCurrentIndex(0);
//  m_priceListStructure.clear();
//}
