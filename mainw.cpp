﻿#include "mainw.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  m_pUi(new Ui::MainWindow)
{
  m_pUi->setupUi(this);
  m_pPriceListWindow = new CPriceList();
  m_pOrderAdd = new COrderAdd();
  m_pEditor = new CEditor();
  m_pEmployees = new CEmployees();
  m_pStatistic = new CStatistic();
  m_pCustomers = new CCustomers();

#pragma region Signals/Slots
  connect(this, &MainWindow::showPriceList,
          m_pPriceListWindow, [=]() { m_pPriceListWindow->show(); });

  connect(this, &MainWindow::tbSendPriceList,
          m_pPriceListWindow, &CPriceList::setTablePriceList);

  connect(this, &MainWindow::showAddOrder,
          m_pOrderAdd, &COrderAdd::showWindow);

  connect(this, &MainWindow::showEditor,
          m_pEditor, &CEditor::showWindow);

  connect(this, &MainWindow::showStatistic,
          m_pStatistic, [=]() { m_pStatistic->show(); });

  connect(this, &MainWindow::showEmployees,
          m_pEmployees, &CEmployees::showWindow);

  connect(this, &MainWindow::showCustomers,
          m_pCustomers, &CCustomers::showWindow);


#pragma enregion
  /////
#pragma region Initialize fields of class
  m_hDb = &CODBCW::getInstance();

#ifdef QT_DEBUG
  qDebug() << "Database handle set at: " << m_hDb;
#endif

  if (m_hDb->getStatus()) {
    m_pUi->tb_orders->horizontalHeader()
                    ->setSectionResizeMode(QHeaderView::Stretch);
    m_hModel = new QSqlQueryModel();
    m_hQuery = new CQueryController(*m_hDb);
    m_hFilterModel = new QSortFilterProxyModel();
    initTbOrders();
  }
#pragma endregion

}

MainWindow::~MainWindow() {
  delete m_pUi;
  delete m_pEmployees;
  delete m_pPriceListWindow;
  delete m_pEditor;
  delete m_pOrderAdd;
  delete m_pStatistic;
  delete m_hQuery;
  delete m_hModel;
  delete m_hFilterModel;
}

void MainWindow::initTbOrders() {
  m_hFilterModel->setDynamicSortFilter(true);
  m_hQuery->executeSqlQuery("SELECT C.title AS 'Title', O.comp_count AS 'Count of components', "
                            "O.acceptance_date AS 'Acceptance date', O.price AS 'Price'"
                            " FROM orders O"
                            " JOIN components C"
                            " ON O.FK_component_id = C.PK_component_id;");
  m_hModel->setQuery(m_hQuery->getQuery());
  m_hFilterModel->setSourceModel(m_hModel);
  m_pUi->tb_orders->setModel(m_hFilterModel);
  m_pUi->tb_orders->setSortingEnabled(true);
}

void MainWindow::on_price_btn_clicked() {
  try {
    if (!m_hQuery->executeSqlQuery("SELECT title AS 'Title', component_type AS 'Component type',"
                                   " specifications AS 'Specifications', price AS 'Price', release_date AS 'Release date' "
                                   "FROM components "
                                   "JOIN components_type "
                                   "ON FK_type_code = PK_component_type_id;"))
      throw std::invalid_argument("Error, query for pricelist not executed!");
    //TODO: refactor
    QSqlQueryModel* hCompQModel = new QSqlQueryModel();
    QSqlQueryModel* hCompTypeQModel = new QSqlQueryModel();
    QSortFilterProxyModel* hCompFilterModel = new QSortFilterProxyModel();
    QSortFilterProxyModel* hCompTyFilterModel = new QSortFilterProxyModel();
    hCompQModel->setQuery(m_hQuery->getQuery());
    hCompFilterModel->setDynamicSortFilter(true);
    hCompFilterModel->setSourceModel(hCompQModel);

    m_hQuery->clear();

    if (!m_hQuery->executeSqlQuery("SELECT component_type AS 'Component type' FROM components_type;"))
      throw std::invalid_argument("Error, query for pricelist not executed!");

    hCompTypeQModel->setQuery(m_hQuery->getQuery());
    hCompTyFilterModel->setDynamicSortFilter(true);
    hCompTyFilterModel->setSourceModel(hCompTypeQModel);

    emit tbSendPriceList(hCompFilterModel, hCompTyFilterModel);
    emit showPriceList();
  }
  catch(std::invalid_argument& e) {
    QMessageBox::critical(this, e.what(), e.what());
    return;
  }
  catch(...) {
    QMessageBox::critical(this, "Error!", "MainWindow::on_price_btn_clicked : Unexpected error!");
  }
}

void MainWindow::on_add_order_btn_clicked() {
  try {
    if (m_hQuery->executeSqlQuery("SELECT PK_component_id, "
                                  "title, price FROM components;"))
    {

      if (!m_hQuery->isSelect())
        throw std::invalid_argument("Error, SELECT comps doesn't contain prices! LastError: [" +
                                    m_hQuery->getQuery().lastError().text().toStdString() + "]");

      std::vector<std::tuple<size_t, QString, double>> vCompsV;

      while (m_hQuery->next())
        vCompsV.push_back(std::make_tuple(m_hQuery->parse_value(0).toUInt(),
                                          m_hQuery->parse_value(1).toString(),
                                          m_hQuery->parse_value(2).toDouble()));
      m_hQuery->clear();

      //TODO: employees, customers
      if (m_hQuery->executeSqlQuery("SELECT PK_component_id, "
                                    "title, price FROM components;"))

      emit showAddOrder(m_pUi->tb_orders, vCompsV);
    }
    else
      throw std::invalid_argument("Error from: MainWindow::on_add_order_btn_clicked. LastError: [" +
                                  m_hQuery->getQuery().lastError().text().toStdString() + "]");
  } catch(std::invalid_argument& e) {
    QMessageBox::critical(this, e.what(), e.what());
    return;
  }
  catch(...) {
    QMessageBox::critical(this, "Error!", "MainWindow::on_add_order_btn_clicked : Unexpected error! LastError: [" +
                          m_hQuery->getQuery().lastError().text() + "]");
    return;
  }
}

void MainWindow::on_edit_btn_clicked() {
  emit showEditor(m_pUi->tb_orders, m_hQuery);
}

void MainWindow::on_statistic_btn_clicked() {
  emit showStatistic();
}

void MainWindow::on_employees_btn_clicked() {
  try {
    m_hQuery->executeSqlQuery("SELECT fullname AS 'Fullname', post AS 'Post', "
                              "salary AS 'Salary', working_hours AS 'Working hours' "
                              "FROM employees;");

    if (!m_hQuery->isSelect())
      throw std::invalid_argument("Error, can't execute employee query!");

    QSqlQueryModel* hEmpQModel = new QSqlQueryModel();
    QSortFilterProxyModel* hEmpFilterModel = new QSortFilterProxyModel();
    hEmpQModel->setQuery(m_hQuery->getQuery());
    hEmpFilterModel->setDynamicSortFilter(true);
    hEmpFilterModel->setSourceModel(hEmpQModel);

    emit showEmployees(hEmpFilterModel);
  } catch(std::invalid_argument& e) {
    QMessageBox::critical(this, e.what(), e.what());
    return;
  }
}

void MainWindow::on_customers_btn_clicked() {
  try {
    m_hQuery->executeSqlQuery("SELECT name AS 'Name', phone AS 'Phone' "
                              "FROM customers;");

    if (!m_hQuery->isSelect())
      throw std::invalid_argument("Error, can't execute customers query!");

    QSqlQueryModel* hCustQModel = new QSqlQueryModel();
    QSortFilterProxyModel* hCustFilterModel = new QSortFilterProxyModel();
    hCustQModel->setQuery(m_hQuery->getQuery());
    hCustFilterModel->setDynamicSortFilter(true);
    hCustFilterModel->setSourceModel(hCustQModel);

    emit showCustomers(hCustFilterModel);
  } catch(std::invalid_argument& e) {
    QMessageBox::critical(this, e.what(), e.what());
    return;
  }
}
