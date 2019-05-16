#include "record.h"
#include "ui_record.h"


struct CRecord::priceListHandler {
  std::vector<QString> m_nameList;
  std::shared_ptr<double[]> m_pPriceList;
};

CRecord::CRecord(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::RecordWindow)
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

CRecord::~CRecord() {
  delete m_pUi;
  delete m_pPriceListHandler;
}

void CRecord::showWindow() { this->show(); }

void CRecord::priceListChanged(QTableView* pTbOrders, Handle* hQuery,
                              std::shared_ptr<double[]> priceList,
                              std::vector<QString> aEquipmentName) {
  m_hQuery = hQuery;
  m_pTbOrders = pTbOrders;
  m_pPriceListHandler->m_pPriceList = priceList;
  m_pPriceListHandler->m_nameList = aEquipmentName;
  m_priceListStructure[m_pPriceListHandler->m_nameList] = m_pPriceListHandler->m_pPriceList;

#pragma region
  auto it = m_priceListStructure.begin();
  std::for_each(m_pPriceListHandler->m_nameList.begin(), m_pPriceListHandler->m_nameList.end(),
                [&](QString& name) { m_pUi->name_comboBox->addItem(name); });
  m_pUi->total_value_label->setText(QString::number(it->second[0]));
#pragma comment endregion

  m_total = it->second[0];
  m_prevTotal = m_total;
}

void CRecord::on_name_comboBox_currentIndexChanged(int index) {
  if (!m_priceListStructure.empty()) {
    m_map_it = m_priceListStructure.begin();
    (bIsOverhaul) ? m_total = m_map_it->second[index] + OVERHAUL
                              : m_total = m_map_it->second[index];
    m_pUi->total_value_label->setText(QString::number(m_total));
    m_pUi->guarantee_dial->setValue(0);
    m_prevTotal = m_total;
  }
}

void CRecord::on_type_repair_comboBox_currentIndexChanged(int index) {
  if (index > 0) {
    m_total += OVERHAUL;
    m_prevTotal += OVERHAUL;
    m_pUi->total_value_label->setText(QString::number(m_total));
    bIsOverhaul = true;
  } else {
    m_total -= OVERHAUL;
    m_prevTotal -= OVERHAUL;
    m_pUi->total_value_label->setText(QString::number(m_total));
    bIsOverhaul = false;
  }
}

void CRecord::on_accept_btn_clicked() {
  QString sQuery;
  if  (m_pUi->complete_dateEdit->isEnabled())
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

void CRecord::on_status_comboBox_currentIndexChanged(int index) {
  (index == 2) ? m_pUi->complete_dateEdit->setEnabled(true)
               : m_pUi->complete_dateEdit->setEnabled(false);
}

void CRecord::on_guarantee_dial_valueChanged(int value) {
  m_pUi->year_guarantee_count_label->setText(QString::number(value));
  m_guarantee = value * 10;
#ifdef QT_DEBUG
  qDebug() << "[on_guarantee_dial_valueChanged] Guarantee value changed to: " << m_guarantee;
#endif
}

void CRecord::on_guarantee_dial_sliderReleased() {
  m_total = m_prevTotal + m_guarantee;
  m_pUi->total_value_label->setText(QString::number(m_total));
#ifdef QT_DEBUG
  qDebug() << "[on_guarantee_dial_sliderReleased] m_total: " << m_total;
#endif
}

void CRecord::on_RecordWindow_finished(int result) {
  m_pUi->name_comboBox->clear();
  m_pUi->type_repair_comboBox->setCurrentIndex(0);
  m_pUi->completed_order_name_lEdit->clear();
  m_pUi->status_comboBox->setCurrentIndex(0);
  m_priceListStructure.clear();
}
