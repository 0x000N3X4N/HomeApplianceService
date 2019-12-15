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
                           std::vector<std::tuple<size_t, QString, double>> vCompsV,
                           std::map<QString, size_t> employees_map, std::map<QString, size_t> cust_map)
{
  m_pTbOrders = ptb_orders;
  m_vCompsV = vCompsV;
  m_employees_map = employees_map;
  m_cust_map = cust_map;

  fillInParams();

  show();
}


void COrderAdd::fillInParams() {
  for (auto it = m_vCompsV.begin(); it != m_vCompsV.end(); it++)
    m_pUi->comp_cBox->addItem(std::get<1>(*it));

  for (auto it = m_employees_map.begin(); it != m_employees_map.end(); it++)
    m_pUi->employee_cBox->addItem(it->first);

  for (auto it = m_cust_map.begin(); it != m_cust_map.end(); it++)
    m_pUi->cust_cBox->addItem(it->first);
}

void COrderAdd::on_accept_btn_clicked() {
  CQueryController query_ctrl(CODBCW::getInstance());

  try {

    QString comp_curr_text = m_pUi->comp_cBox->currentText();
    size_t comp_id = 0;

    for (auto o : m_vCompsV)
      if (std::get<1>(o) == comp_curr_text)
        comp_id = std::get<0>(o);

    auto cust_it = m_cust_map.find(m_pUi->cust_cBox->currentText()),
         empl_it = m_employees_map.find(m_pUi->employee_cBox->currentText());

    QString query_qstr = QString("INSERT INTO orders"
                         " VALUES ('%1', '%2', '%3', '%4', '%5', '%6');").arg(QString::number(comp_id),
                                                                QString::number(empl_it->second),
                                                                QString::number(cust_it->second),
                                                                m_pUi->accept_dateEdit->date().toString("yyyy-MM-dd"),
                                                                QString::number(m_pUi->comp_cnt_dial->value()),
                                                                QString::number(m_total));

    if (query_ctrl.executeSqlQuery(query_qstr)) {
      QMessageBox::information(this, "Success!", "Order was succesfully added!");

      query_ctrl.clear();

      if (query_ctrl.executeSqlQuery("SELECT C.title AS 'Title', E.fullname AS 'Employee ', O.comp_count AS 'Count of components', "
                                     "O.acceptance_date AS 'Acceptance date', O.price AS 'Price' "
                                     "FROM orders O "
                                     "JOIN components C "
                                     "ON O.FK_component_id = C.PK_component_id "
                                     "JOIN employees E "
                                     "ON o.FK_employee_id = e.PK_employee_id;"))
      {
        QSqlQueryModel* hOrdersQModel = new QSqlQueryModel();
        QSortFilterProxyModel* hOrdersFilterModel = new QSortFilterProxyModel();
        hOrdersQModel->setQuery(query_ctrl.getQuery());
        hOrdersFilterModel->setDynamicSortFilter(true);
        hOrdersFilterModel->setSourceModel(hOrdersQModel);
        m_pTbOrders->setModel(hOrdersFilterModel);
      }
      else
        throw std::invalid_argument("Error on SELECT statement from `orders` table from COrderAdd::on_accept_btn_clicked."
                                    "LastError: [" + query_ctrl.getQuery().lastError().text().toStdString() + "]");
    }
    else
      throw std::invalid_argument("Error on INSERT statement from `orders` table from COrderAdd::on_accept_btn_clicked."
                                  "LastError: [" + query_ctrl.getQuery().lastError().text().toStdString() + "]");
  }
  catch(std::invalid_argument& e) {
    QMessageBox::critical(this, e.what(), e.what());
    return;
  }
  catch(...) {
    QMessageBox::critical(this, "Error!", "COrderAdd::on_accept_btn_clicked : Unexpected error! LastError: [" +
                          query_ctrl.getQuery().lastError().text() + "]");
  }
}

void COrderAdd::clearForm() {
  m_vCompsV.clear();
  m_pUi->comp_cBox->clear();
  m_pUi->comp_cnt_dial->setValue(0);
  m_pUi->total_val_l->setText("");
}

void COrderAdd::on_comp_cBox_currentIndexChanged(int index) {
  if (!m_vCompsV.empty()) {
    m_total = fetch_price(m_pUi->comp_cBox->itemText(index)) * m_pUi->comp_cnt_dial->value();
    m_pUi->total_val_l->setText(QString::number(m_total));
  }
}

void COrderAdd::on_comp_cnt_dial_valueChanged(int value) {
  m_pUi->cnt_val_l->setText(QString::number(value));
  if (!m_vCompsV.empty()) {
    m_total = fetch_price(m_pUi->comp_cBox->currentText()) * value;
    m_pUi->total_val_l->setText(QString::number(m_total));
  }
}

double COrderAdd::fetch_price(QString key_qstr) {
  for (auto o : m_vCompsV)
    if (std::get<1>(o) == key_qstr)
      return std::get<2>(o);

  return 0;
}

