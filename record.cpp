#include "record.h"
#include "ui_record.h"


struct Record::priceListHandler {
  std::array<QString, 8> m_nameList {
                                    "Стиральная машина",
                                    "Посудомоечная машина",
                                    "СВЧ",
                                    "Телевизора",
                                    "ЖК монитора",
                                    "Плазменной панели",
                                    "Холодильника",
                                    "Газовой плиты"
                                  };
  std::shared_ptr<double[]> m_pPriceList;
  size_t m_sizeOfPriceList;
};

Record::Record(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::RecordWindow)
{
  m_pUi->setupUi(this);
  m_pPriceListHandler = new priceListHandler();
  m_total = 0;
#pragma region Init UI elemnts
  m_pUi->complete_dateEdit->setDate(QDate::currentDate());
  m_pUi->accept_dateEdit->setDate(QDate::currentDate());
  m_pUi->accept_dateEdit->setCalendarPopup(true);
  m_pUi->complete_dateEdit->setCalendarPopup(true);
  m_pUi->status_comboBox->addItem("Closed");
  m_pUi->status_comboBox->addItem("Open");
  m_pUi->status_comboBox->addItem("Started");
  m_pUi->type_repair_comboBox->addItem("Minor repair");
  m_pUi->type_repair_comboBox->addItem("Overhaul");
  std::for_each(m_pPriceListHandler->m_nameList.begin(), m_pPriceListHandler->m_nameList.end(),
                [&](QString& name) { m_pUi->name_comboBox->addItem(name); });
#pragma endregion
}

Record::~Record() {
  delete m_pUi;
  delete m_pPriceListHandler;
}

void Record::showWindow() { this->show(); }

void Record::priceListChanged(QTableView* pTbOrders, Handle* hQuery,
                              std::shared_ptr<double[]> priceList,
                              size_t sizeOfPriceList) {
  m_hQuery = hQuery;
  m_pTbOrders = pTbOrders;
  m_pPriceListHandler->m_pPriceList = priceList;
  m_pPriceListHandler->m_sizeOfPriceList = sizeOfPriceList;
  m_priceListStructure[m_pPriceListHandler->m_nameList] = m_pPriceListHandler->m_pPriceList;
  auto it = m_priceListStructure.begin();
  m_pUi->total_value_label->setText(QString::number(it->second[0]));
  m_total = it->second[0];
#ifdef QT_DEBUG
  qDebug() << "pricelist from price Dialog contains: ";
  for (size_t ix = 0; ix < m_pPriceListHandler->m_sizeOfPriceList; ix++)
    qDebug() << m_pPriceListHandler->m_pPriceList[ix];
#endif
}

void Record::on_name_comboBox_currentIndexChanged(int index) {
  if (!m_priceListStructure.empty()) {
    m_map_it = m_priceListStructure.begin();
    (bIsOverhaul) ? m_total = m_map_it->second[index] + OVERHAUL
                               : m_total = m_map_it->second[index];
    m_pUi->total_value_label->setText(QString::number(m_total));
  }
}

void Record::on_type_repair_comboBox_currentIndexChanged(int index) {
  if (index > 0) {
    m_total += OVERHAUL;
    m_pUi->total_value_label->setText(QString::number(m_total));
    bIsOverhaul = true;
  } else {
    m_total -= OVERHAUL;
    m_pUi->total_value_label->setText(QString::number(m_total));
    bIsOverhaul = false;
  }
}

void Record::on_accept_btn_clicked() {
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
                     "name, type, acceptance_date, total, status)"
                     "VALUES ('%1', '%2', '%3', '%4', '%5')")
                     .arg(m_pUi->name_comboBox->currentText(),
                          m_pUi->type_repair_comboBox->currentText(),
                          m_pUi->accept_dateEdit->date().toString("yyyy-MM-dd"),
                          m_pUi->total_value_label->text(),
                          m_pUi->status_comboBox->currentText());

  m_hQuery->executeSqlQuery(sQuery);
  m_hQuery->clear();

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
}

void Record::on_status_comboBox_currentIndexChanged(int index) {
  if (index == 2) {
    m_pUi->completed_order_name_lEdit->setEnabled(true);
    m_pUi->complete_dateEdit->setEnabled(true);
  }
  else {
    m_pUi->completed_order_name_lEdit->clear();
    m_pUi->completed_order_name_lEdit->setEnabled(false);
    m_pUi->complete_dateEdit->setEnabled(false);
  }
}
