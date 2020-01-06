#include "mainw.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  m_pUi(new Ui::MainWindow)
{
  m_pUi->setupUi(this);
  m_pPriceListWindow = new CPriceList();
  m_pOrderAdd = new COrderAdd();
  m_pEmployees = new CEmployees();
  m_pStatistic = new CStatistic();
  m_pCustomers = new CCustomers();
  m_pOrderDeleter = new COrderDel();
  m_pExport = new CExport();
  m_pSearch = new CSearch();

#pragma region Signals/Slots
  connect(this, &MainWindow::showPriceList,
          m_pPriceListWindow, [=]() { m_pPriceListWindow->show(); });

  connect(this, &MainWindow::tbSendPriceList,
          m_pPriceListWindow, &CPriceList::setTablePriceList);

  connect(this, &MainWindow::showAddOrder,
          m_pOrderAdd, &COrderAdd::showWindow);

  connect(this, &MainWindow::showDelOrder,
          m_pOrderDeleter, &COrderDel::showWindow);

  connect(m_pPriceListWindow->getCompTypeDel(), &CComp_TyDeleter::updOrders,
          this, &MainWindow::initTbOrders);

  connect(m_pPriceListWindow->getCompDel(), &CCompDeleter::updOrders,
          this, &MainWindow::initTbOrders);

  connect(m_pCustomers->getCustDel(), &CCustomerDel::updOrders,
          this, &MainWindow::initTbOrders);

  connect(m_pEmployees->getEmplDel(), &CEmployeeDel::updOrders,
          this, &MainWindow::initTbOrders);

  connect(this, &MainWindow::showStatistic,
          m_pStatistic, [=]() { m_pStatistic->show(); });

  connect(this, &MainWindow::showEmployees,
          m_pEmployees, &CEmployees::showWindow);

  connect(this, &MainWindow::showCustomers,
          m_pCustomers, &CCustomers::showWindow);

  connect(this, &MainWindow::showExport,
          m_pExport, &CExport::showWindow);

  connect(m_pUi->search_btn, &QPushButton::clicked,
          m_pSearch, [this](){m_pSearch->show();});
#pragma enregion
  /////
#pragma region Initialize fields of class
  size_t i = 0;
  m_hDb = &CODBCW::getInstance("", nullptr, &i);

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
  delete m_pOrderAdd;
  delete m_pStatistic;
  delete m_hQuery;
  delete m_hModel;
  delete m_hFilterModel;
  delete m_pCustomers;
}

void MainWindow::initTbOrders() {
  m_hFilterModel->setDynamicSortFilter(true);

  //TODO: err handling
  m_hQuery->executeSqlQuery("SELECT C.title AS 'Title', E.fullname AS 'Employee ', O.comp_count AS 'Count of components', "
                            "O.acceptance_date AS 'Acceptance date', O.price AS 'Price' "
                            "FROM orders O "
                            "JOIN components C "
                            "ON O.FK_component_id = C.PK_component_id "
                            "JOIN employees E "
                            "ON o.FK_employee_id = e.PK_employee_id;");
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
      throw std::invalid_argument("Error, query for pricelist not executed!  LastError: [" +
                                  m_hQuery->getQuery().lastError().text().toStdString() + "]");

    hCompTypeQModel->setQuery(m_hQuery->getQuery());
    hCompTyFilterModel->setDynamicSortFilter(true);
    hCompTyFilterModel->setSourceModel(hCompTypeQModel);

    emit tbSendPriceList(hCompFilterModel, hCompTyFilterModel);
    emit showPriceList();
  }
  catch(std::invalid_argument& e) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", e.what());
    return;
  }
  catch(...) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", "MainWindow::on_price_btn_clicked : Unexpected error!");
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
      std::map<QString, size_t> empl_map, cust_map;

      while (m_hQuery->next())
        vCompsV.push_back(std::make_tuple(m_hQuery->parse_value(0).toUInt(),
                                          m_hQuery->parse_value(1).toString(),
                                          m_hQuery->parse_value(2).toDouble()));
      m_hQuery->clear();

      if (m_hQuery->executeSqlQuery("SELECT PK_employee_id, "
                                    "fullname FROM employees;")) {
        while (m_hQuery->next())
          empl_map[m_hQuery->parse_value(1).toString()] = m_hQuery->parse_value(0).toUInt();

        m_hQuery->clear();

        if (m_hQuery->executeSqlQuery("SELECT PK_customer_id, "
                                      "[name] FROM customers;")) {
          while (m_hQuery->next())
            cust_map[m_hQuery->parse_value(1).toString()] = m_hQuery->parse_value(0).toUInt();

          emit showAddOrder(m_pUi->tb_orders, vCompsV, empl_map, cust_map);
        }
        else
          throw std::invalid_argument("Error from: MainWindow::on_add_order_btn_clicked. LastError: [" +
                                      m_hQuery->getQuery().lastError().text().toStdString() + "]");
      }
      else
        throw std::invalid_argument("Error from: MainWindow::on_add_order_btn_clicked. LastError: [" +
                                    m_hQuery->getQuery().lastError().text().toStdString() + "]");


    }
    else
      throw std::invalid_argument("Error from: MainWindow::on_add_order_btn_clicked. LastError: [" +
                                  m_hQuery->getQuery().lastError().text().toStdString() + "]");
  } catch(std::invalid_argument& e) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", e.what());
    return;
  }
  catch(...) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", "MainWindow::on_add_order_btn_clicked : Unexpected error! LastError: [" +
                  m_hQuery->getQuery().lastError().text() + "]");
    return;
  }
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
      throw std::invalid_argument("Error, can't execute employee query! LastError: [" +
                                  m_hQuery->getQuery().lastError().text().toStdString() + "]");

    QSqlQueryModel* hEmpQModel = new QSqlQueryModel();
    QSortFilterProxyModel* hEmpFilterModel = new QSortFilterProxyModel();
    hEmpQModel->setQuery(m_hQuery->getQuery());
    hEmpFilterModel->setDynamicSortFilter(true);
    hEmpFilterModel->setSourceModel(hEmpQModel);

    emit showEmployees(hEmpFilterModel);
  } catch(std::invalid_argument& e) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", e.what());
    return;
  }
  catch(...) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", "MainWindow::on_employees_btn_clicked : Unexpected error! LastError: [" +
                  m_hQuery->getQuery().lastError().text() + "]");
    return;
  }
}

void MainWindow::on_customers_btn_clicked() {
  try {
    if (m_hQuery->executeSqlQuery("SELECT name AS 'Name', phone AS 'Phone' "
                                  "FROM customers;")) {
      QSqlQueryModel* hCustQModel = new QSqlQueryModel();
      QSortFilterProxyModel* hCustFilterModel = new QSortFilterProxyModel();
      hCustQModel->setQuery(m_hQuery->getQuery());
      hCustFilterModel->setDynamicSortFilter(true);
      hCustFilterModel->setSourceModel(hCustQModel);

      emit showCustomers(hCustFilterModel);
    }
    else
      throw std::invalid_argument("Error, can't execute customers query!  LastError: [" +
                                  m_hQuery->getQuery().lastError().text().toStdString() + "]");
  } catch(std::invalid_argument& e) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", e.what());
    return;
  }
  catch(...) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", "MainWindow::on_customers_btn_clicked : Unexpected error! LastError: [" +
                  m_hQuery->getQuery().lastError().text() + "]");
    return;
  }
}

void MainWindow::on_del_order_btn_clicked() {
  try {
    std::tuple <
                 std::vector<QString>, // order title
                 std::vector<QString>, // order employeer
                 std::vector<QString>, // order acceptance date
                 std::vector<QString>  // order price
               > o_struct;
    std::vector<QString> vOrders, vEmpl, vAcceptDate, vPrices;

    if (m_hQuery->executeSqlQuery("SELECT C.title AS 'Title', E.fullname AS 'Employee ', "
                                  "O.acceptance_date AS 'Acceptance date', O.price AS 'Price' "
                                  "FROM orders O "
                                  "JOIN components C "
                                  "ON O.FK_component_id = C.PK_component_id "
                                  "JOIN employees E "
                                  "ON o.FK_employee_id = e.PK_employee_id;"))
    {
      while(m_hQuery->next()) {
        vOrders.push_back(m_hQuery->parse_value(0).toString());
        vEmpl.push_back(m_hQuery->parse_value(1).toString());
        vAcceptDate.push_back(m_hQuery->parse_value(2).toString());
        vPrices.push_back(m_hQuery->parse_value(3).toString());
      }

      o_struct = std::make_tuple(vOrders, vEmpl, vAcceptDate, vPrices);

      emit showDelOrder(m_pUi->tb_orders, o_struct);
    }
    else
      throw std::invalid_argument("Error, can't execute orders query!  LastError: [" +
                                  m_hQuery->getQuery().lastError().text().toStdString() + "]");

  } catch(std::invalid_argument& e) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", e.what());
    return;
  }
  catch(...) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", "MainWindow::on_del_order_btn_clicked : Unexpected error! LastError: [" +
                  m_hQuery->getQuery().lastError().text() + "]");
    return;
  }
}

void MainWindow::on_export_btn_clicked() {
  emit showExport(m_pUi->tb_orders);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == 16777264)
    QDesktopServices::openUrl(
          QUrl::fromLocalFile(QDir::currentPath() + "/docs/chm/PC-Sales-Manager-Workstation.chm")
          );
}
