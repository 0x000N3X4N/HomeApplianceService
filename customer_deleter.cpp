#include "customer_deleter.h"
#include "ui_customer_deleter.h"


CCustomerDel::CCustomerDel(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::CustomerDeleterWIndow)
{
  connect(this, &CCustomerDel::finished,
          this, &CCustomerDel::clearUi);

  m_pUi->setupUi(this);
}

CCustomerDel::~CCustomerDel()
{
  delete m_pUi;
  delete m_pCustTb;
}

void CCustomerDel::showWindow(QTableView* cust_tb_ptr,
                              std::vector<QString> vCust)
{
  m_vCust = vCust;
  std::for_each(m_vCust.begin(), m_vCust.end(), [this](QString& cust_qstr) -> void {
                                                  m_pUi->fullname_cBox->addItem(cust_qstr);
                                                });
  m_pCustTb = cust_tb_ptr;
  show();
}

void CCustomerDel::clearUi() {
  m_pUi->fullname_cBox->clear();
}

void CCustomerDel::on_submit_btn_clicked() {
  QMessageBox::StandardButton reply;
  reply = QMessageBox::warning(this, "Warning!", "This action may delete order, are you sure? "
                                                 "More information is available in the help.",
                               QMessageBox::Yes | QMessageBox::No);

  if (reply == QMessageBox::Yes) {
    size_t i = 0;
    CQueryController query(CQueryController(CODBCW::getInstance("", nullptr, &i)));

    try {
      auto customer_name = m_pUi->fullname_cBox->currentText();
      QString del_custQ = QString("DELETE FROM customers WHERE [name] = '%1';")
                          .arg(customer_name);

      if (query.executeSqlQuery(del_custQ)) {
        query.clear();

        if (query.executeSqlQuery("SELECT name AS 'Name', phone AS 'Phone' "
                                  "FROM customers;"))
        {
          CMsgBox::show(QMessageBox::Information, this, "Success!", "Customer '" + customer_name + "' was succesfully deleted!");

          QSqlQueryModel* hCustQModel = new QSqlQueryModel();
          QSortFilterProxyModel* hCustFilterModel = new QSortFilterProxyModel();
          hCustQModel->setQuery(query.getQuery());
          hCustFilterModel->setDynamicSortFilter(true);
          hCustFilterModel->setSourceModel(hCustQModel);

          m_pCustTb->setModel(hCustFilterModel);

          emit updOrders();
        }
        else
          throw std::invalid_argument("Error, query for SELECT customers not executed! LastError: [" +
                                      query.getQuery().lastError().text().toStdString() + "]");
      }
      else
        throw std::invalid_argument("Error, query for DELETE customer not executed! LastError: [" +
                                    query.getQuery().lastError().text().toStdString() + "]");

      updateUi();
    }
    catch(std::invalid_argument& e) {
      CMsgBox::show(QMessageBox::Critical, this, "Error!", e.what());
      return;
    }
    catch(...) {
      CMsgBox::show(QMessageBox::Critical, this, "Error!", "CCustomerDel::on_submit_btn_clicked : Unexpected error! LastError: [" +
                    query.getQuery().lastError().text() + "]");
    }
  }
}

void CCustomerDel::updateUi() {
  m_vCust.clear();
  clearUi();
  fillFN();
  std::for_each(m_vCust.begin(), m_vCust.end(), [this](QString& cust_qstr) -> void {
                                                  m_pUi->fullname_cBox->addItem(cust_qstr);
                                                });
}

void CCustomerDel::fillFN() {
  size_t i = 0;
  CQueryController query(CQueryController(CODBCW::getInstance("", nullptr, &i)));

  try {
    if (query.executeSqlQuery("SELECT [name] FROM customers;"))
      while (query.next())
        m_vCust.push_back(query.parse_value(0).toString());
    else
      throw std::invalid_argument("Error, query for SELECT customers not executed! LastError: [" +
                                  query.getQuery().lastError().text().toStdString() + "]");
  }
  catch(std::invalid_argument& e) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", e.what());
  }
  catch(...) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", "CCustomerDel::fillFN : Unexpected error! LastError: [" +
                  query.getQuery().lastError().text() + "]");
  }
}
