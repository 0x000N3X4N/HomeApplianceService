#include "mainw.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  m_pUi(new Ui::MainWindow)
{
  m_pUi->setupUi(this);
  m_pPriceListWindow = new CPriceList();
  m_pRecord = new CRecord();
  m_pEditor = new CEditor();
  m_pStatistic = new CStatistic();

#pragma region Signals/Slots
  connect(this, &MainWindow::showPriceList,
          m_pPriceListWindow, [=]() { m_pPriceListWindow->show(); });

  connect(this, &MainWindow::showAddOrder,
          m_pRecord, &CRecord::showWindow);

  connect(this, &MainWindow::showEditor,
          m_pEditor, &CEditor::showWindow);

  connect(this, &MainWindow::tbSendPriceList,
          m_pPriceListWindow, &CPriceList::setTablePriceList);

  connect(this, &MainWindow::sendPriceList,
          m_pRecord, &CRecord::priceListChanged);

  connect(this, &MainWindow::showStatistic,
          m_pStatistic, [=]() { m_pStatistic->show(); });
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
  delete m_pPriceListWindow;
  delete m_pEditor;
  delete m_pRecord;
  delete m_pStatistic;
  delete m_hQuery;
  delete m_hModel;
  delete m_hFilterModel;
}

void MainWindow::initTbOrders() {
  m_hFilterModel->setDynamicSortFilter(true);
  m_hQuery->executeSqlQuery("SELECT id_order AS 'Id', "
    "name AS 'Name', type AS 'Type', acceptance_date AS 'Accept date', "
    "completion_date AS 'Complete date', total AS 'Total', "
    "status AS 'Status', completed_surname AS 'Surname' "
    "FROM laptop_maintenance_service.order");
  m_hModel->setQuery(m_hQuery->getQuery());
  m_hFilterModel->setSourceModel(m_hModel);
  m_pUi->tb_orders->setModel(m_hFilterModel);
  m_pUi->tb_orders->setSortingEnabled(true);
}

void MainWindow::on_price_btn_clicked() {
  try {
    if (!m_hQuery->executeSqlQuery("SELECT title AS 'Title',"
                                   " specifications AS 'Specifications', price AS 'Price', release_date AS 'Release date' "
                                   "FROM components "
                                   "JOIN components_type "
                                   "ON FK_type_code = PK_component_type_id;"))
      throw std::invalid_argument("Error, query for pricelist not executed!");
    //TODO: refactor
    QSqlQueryModel* hModel = new QSqlQueryModel();
    QSortFilterProxyModel* hFilterModel = new QSortFilterProxyModel();
    hModel->setQuery(m_hQuery->getQuery());
    hFilterModel->setDynamicSortFilter(true);
    hFilterModel->setSourceModel(hModel);

    emit tbSendPriceList(hFilterModel);
    emit showPriceList();
  }
  catch(std::invalid_argument& e) {
    qDebug() << e.what();
    QMessageBox::critical(this, e.what(), e.what());
    return;
  }
}

void MainWindow::on_add_order_btn_clicked() {
  try {
    m_hQuery->executeSqlQuery("SELECT price AS 'Price', name FROM pricelist");

    if (!m_hQuery->isSelect())
      throw std::invalid_argument("Error, price list doesn't contain prices!");

    size_t sizeOfPriceList = m_hQuery->size();
    std::shared_ptr<double[]> pPriceList(new double[sizeOfPriceList],
                                         std::default_delete<double[]>());
    std::vector<QString> equipment_name;
    int ecx = 0; // counter
    while (m_hQuery->next()) {
      pPriceList[ecx] = m_hQuery->parse_value(0).toDouble();
      equipment_name.push_back(m_hQuery->parse_value(1).toString());
      ecx++;
    }

    emit sendPriceList(m_pUi->tb_orders, m_hQuery, pPriceList, equipment_name);
  } catch(std::invalid_argument& e) {
    qDebug() << e.what();
    QMessageBox::critical(this, e.what(), e.what());
    return;
  }

  emit showAddOrder();
}

void MainWindow::on_edit_btn_clicked() {
  emit showEditor(m_pUi->tb_orders, m_hQuery);
}

void MainWindow::on_statistic_btn_clicked() {
  emit showStatistic();
}
