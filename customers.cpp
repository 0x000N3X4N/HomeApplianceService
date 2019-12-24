#include "customers.h"
#include "ui_customers.h"


CCustomers::CCustomers(QWidget *parent) :
  QWidget(parent),
  m_pUi(new Ui::CustomersWindow)
{

  m_pCust_add = new CCustomerAdd();
  m_pCust_del = new CCustomerDel();

  connect(this, &CCustomers::showCustomerAdd,
          m_pCust_add, &CCustomerAdd::showWindow);

  connect(this, &CCustomers::showCustomerDel,
          m_pCust_del, &CCustomerDel::showWindow);

  m_pUi->setupUi(this);
  m_pUi->customers_tb->horizontalHeader()
                     ->setSectionResizeMode(QHeaderView::Stretch);
  m_pUi->customers_tb->setSortingEnabled(true);
}

CCustomers::~CCustomers()
{
  delete m_pUi;
}

void CCustomers::showWindow(QSortFilterProxyModel* res_qsfpm) {
  m_pUi->customers_tb->setModel(res_qsfpm);
  show();
}

void CCustomers::on_add_customer_btn_clicked() {
  size_t i = 0;
  CQueryController query(CQueryController(CODBCW::getInstance("", nullptr, &i)));

  try {
    std::map<QString, size_t> city_map;

    if (query.executeSqlQuery("SELECT * FROM city;")) {
      while(query.next())
        city_map[query.parse_value(1).toString()] = query.parse_value(0).toUInt();

      emit showCustomerAdd(m_pUi->customers_tb, city_map);
    }
    else
      throw std::invalid_argument("Error, query for SELECT city not executed! LastError: [" +
                                  query.getQuery().lastError().text().toStdString() + "]");
  }
  catch(std::invalid_argument& e) {
    QMessageBox::critical(this, e.what(), e.what());
    return;
  }
  catch(...) {
    QMessageBox::critical(this, "Error!", "CCustomers::on_add_customer_btn_clicked : Unexpected error! LastError: [" +
                          query.getQuery().lastError().text() + "]");
  }
}

void CCustomers::on_delete_customer_btn_clicked() {
  size_t i = 0;
  CQueryController query(CQueryController(CODBCW::getInstance("", nullptr, &i)));

  try {
    std::vector<QString> vCust;

    QString custQ = "SELECT [name] FROM customers;";

    if (query.executeSqlQuery(custQ)) {
      while(query.next())
        vCust.push_back(query.parse_value(0).toString());

      emit showCustomerDel(m_pUi->customers_tb, vCust);
    }
    else
      throw std::invalid_argument("Error, query for SELECT customers not executed! LastError: [" +
                                  query.getQuery().lastError().text().toStdString() + "]");
  }
  catch(std::invalid_argument& e) {
    QMessageBox::critical(this, e.what(), e.what());
    return;
  }
  catch(...) {
    QMessageBox::critical(this, "Error!", "CCustomers::on_delete_customer_btn_clicked : Unexpected error! LastError: [" +
                          query.getQuery().lastError().text() + "]");
  }
}
