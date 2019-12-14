#include "order_add.h"
#include "ui_order_add.h"


COrderAdd::COrderAdd(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::OrderAddWindow)
{
  m_pUi->setupUi(this);
  m_total = 0;

  connect(this, &COrderAdd::finished,
          this, &COrderAdd::clearForm);

#pragma region Init UI elemnts
  m_pUi->accept_dateEdit->setDate(QDate::currentDate());
  m_pUi->accept_dateEdit->setCalendarPopup(true);
#pragma endregion

}

COrderAdd::~COrderAdd() {
  delete m_pUi;
}

void COrderAdd::showWindow(QTableView* ptb_orders,
                           std::vector<std::tuple<size_t, QString, double>> vCompsV)
{
  m_pTbOrders = ptb_orders;
  m_vCompsV = vCompsV;

  for (auto it = vCompsV.begin(); it != vCompsV.end(); it++)
    m_pUi->name_cBox->addItem(std::get<1>(*it));

  show();
}

void COrderAdd::on_accept_btn_clicked() {
//  QString sQuery;
//  if (m_pUi->complete_dateEdit->isEnabled())
//    sQuery = QString("INSERT INTO `home_appliance_service`.`order`"
//                     "(name, type, acceptance_date, completion_date, total, status, completed_surname)"
//                     "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7')")
//                     .arg(m_pUi->name_comboBox->currentText(), m_pUi->type_repair_comboBox->currentText(),
//                          m_pUi->accept_dateEdit->date().toString("yyyy-MM-dd"),
//                          m_pUi->complete_dateEdit->date().toString("yyyy-MM-dd"),
//                          QString::number(m_total), m_pUi->status_comboBox->currentText(),
//                          m_pUi->completed_order_name_lEdit->text());

//  else
//    sQuery = QString("INSERT INTO `home_appliance_service`.`order`("
//                     "name, type, acceptance_date, total, status, completed_surname)"
//                     "VALUES ('%1', '%2', '%3', '%4', '%5', '%6')")
//                     .arg(m_pUi->name_comboBox->currentText(),
//                          m_pUi->type_repair_comboBox->currentText(),
//                          m_pUi->accept_dateEdit->date().toString("yyyy-MM-dd"),
//                          m_pUi->total_value_label->text(),
//                          m_pUi->status_comboBox->currentText(),
//                          m_pUi->completed_order_name_lEdit->text());

//  if (m_hQuery->executeSqlQuery(sQuery)) {
//    m_hQuery->clear();
//    QMessageBox::information(this, "Successfully!", "Order has been success added!");

//    m_hQuery->executeSqlQuery(
//            "SELECT id_order AS 'Id', name AS 'Name', type AS 'Type', "
//            "acceptance_date AS 'Accept_date', completion_date AS 'Complete_date', "
//            "total AS 'Total', status AS 'Status', completed_surname AS 'Surname' "
//            "FROM home_appliance_service.order"
//          );

//    //TODO: refactor
//    QSqlQueryModel* hModel = new QSqlQueryModel();
//    QSortFilterProxyModel* hFilterModel = new QSortFilterProxyModel();
//    hModel->setQuery(m_hQuery->getQuery());
//    hFilterModel->setSourceModel(hModel);
//    m_pTbOrders->setModel(hFilterModel);
//  } else
//    QMessageBox::critical(this, "Error", "Cannnot add order to database\r\n"
//                                         "Please check your connection to database!");
}

void COrderAdd::clearForm() {
  m_vCompsV.clear();
  m_pUi->name_cBox->clear();
  m_pUi->comp_cnt_dial->setValue(0);
  m_pUi->total_val_l->setText("");
}

void COrderAdd::on_name_cBox_currentIndexChanged(int index) {
  if (!m_vCompsV.empty()) {
    m_total = fetch_price(m_pUi->name_cBox->itemText(index)) * m_pUi->comp_cnt_dial->value();
    m_pUi->total_val_l->setText(QString::number(m_total));
  }
}

void COrderAdd::on_comp_cnt_dial_valueChanged(int value) {
  m_pUi->cnt_val_l->setText(QString::number(value));
  if (!m_vCompsV.empty()) {
    m_total = fetch_price(m_pUi->name_cBox->currentText()) * value;
    m_pUi->total_val_l->setText(QString::number(m_total));
  }
}

double COrderAdd::fetch_price(QString key_qstr) {
  for (auto o : m_vCompsV)
    if (std::get<1>(o) == key_qstr)
      return std::get<2>(o);

  return 0;
}
