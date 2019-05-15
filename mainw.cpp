#include "mainw.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  m_pUi(new Ui::MainWindow)
{
  m_pUi->setupUi(this);
  m_pPriceListWindow = new PriceList();
  m_pRecord = new Record();
  m_pEditor = new CEditor();

#pragma region Signals/Slots
  connect(this, SIGNAL(showPriceList()),
          m_pPriceListWindow, SLOT(showWindow()));

  connect(this, &MainWindow::showAddOrder,
          m_pRecord, &Record::showWindow);

  connect(this, &MainWindow::showEditor, m_pEditor, &CEditor::showWindow);

  connect(this, SIGNAL(tbSendPriceList(QSortFilterProxyModel*)),
          m_pPriceListWindow, SLOT(setTablePriceList(QSortFilterProxyModel*)));


  connect(this, SIGNAL(sendPriceList(QTableView*, Handle*, std::shared_ptr<double[]>, size_t)),
          m_pRecord, SLOT(priceListChanged(QTableView*, Handle*, std::shared_ptr<double[]>,
                                                            size_t)));

#pragma enregion
  /////
#pragma region Initialize fields of class
  m_hDb = &XSqlDatabase::getInstance();

#ifdef QT_DEBUG
  qDebug() << "Database handle set at: " << m_hDb;
#endif

  if (m_hDb->getStatus()) {
    m_pUi->tb_orders->horizontalHeader()
                    ->setSectionResizeMode(QHeaderView::Stretch);
    m_hModel = new QSqlQueryModel();
    m_hQuery = new Handle(*m_hDb);
    m_hFilterModel = new QSortFilterProxyModel();
    initTbOrders();
  }
#pragma endregion

}

MainWindow::~MainWindow() {
  delete m_pUi;
  delete m_pRecord;
  delete m_pPriceListWindow;
  delete m_hQuery;
  delete m_hModel;
  delete m_hFilterModel;
}

void MainWindow::initTbOrders() {
  m_hFilterModel->setDynamicSortFilter(true);
  m_hQuery->executeSqlQuery("SELECT id_order AS 'Id', "
  "name AS 'Name', type AS 'Type', acceptance_date AS 'Accept_date', "
  "completion_date AS 'Complete_date', total AS 'Total', "
  "status AS 'Status', completed_surname AS 'Surname' "
                            "FROM home_appliance_service.order");
  m_hModel->setQuery(m_hQuery->getQuery());
  m_hFilterModel->setSourceModel(m_hModel);
  m_pUi->tb_orders->setModel(m_hFilterModel);
  m_pUi->tb_orders->setSortingEnabled(true);
}


void MainWindow::on_price_btn_clicked() {
  m_hQuery->executeSqlQuery("SELECT service AS 'Service', "
                            "price AS 'Price' FROM pricelist");
  //TODO: refactor
  QSqlQueryModel* hModel = new QSqlQueryModel();
  QSortFilterProxyModel* hFilterModel = new QSortFilterProxyModel();
  hModel->setQuery(m_hQuery->getQuery());
  hFilterModel->setDynamicSortFilter(true);
  hFilterModel->setSourceModel(hModel);

  emit tbSendPriceList(hFilterModel);
  emit showPriceList();
}

void MainWindow::on_add_order_btn_clicked() {
  try {
    m_hQuery->executeSqlQuery("SELECT price AS 'Price' FROM pricelist");

    if (!m_hQuery->isSelect())
      throw std::invalid_argument("Erorr, price list doesn't contain prices!");

    size_t sizeOfPriceList = m_hQuery->size();
    std::shared_ptr<double[]> pPriceList(new double[sizeOfPriceList],
                                         std::default_delete<double[]>());
    int ecx = 0; // counter
    while (m_hQuery->next()) {
      pPriceList[ecx] = m_hQuery->parse_value(0).toDouble();
      ecx++;
    }

    emit sendPriceList(m_pUi->tb_orders, m_hQuery, pPriceList, sizeOfPriceList);
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
