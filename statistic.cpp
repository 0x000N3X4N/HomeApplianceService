#include "statistic.h"
#include "ui_statistic.h"


CStatistic::CStatistic(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::StatisticWindow)
{
  m_pUi->setupUi(this);
  m_hQuery = new CQueryController(CODBCW::getInstance());
  m_hModel = new QSqlQueryModel();
  m_hFilterModel = new QSortFilterProxyModel();
  m_pUi->accept_dateEdit->setCalendarPopup(true);
  m_pUi->accept_dateEdit->setDate(QDate::currentDate());
}

CStatistic::~CStatistic() {
  delete m_pUi;
  delete m_hQuery;
  delete m_hModel;
  delete m_hFilterModel;
}

void CStatistic::on_done_btn_clicked() {
  QString sQuery = QString("SELECT id_order AS 'Id', "
                           "name AS 'Name', type AS 'Type', acceptance_date AS 'Accept date', "
                           "completion_date AS 'Complete date', total AS 'Total', "
                           "status AS 'Status', completed_surname AS 'Surname' "
                           "FROM home_appliance_service.order "
                           "WHERE acceptance_date = '%1'")
                           .arg(m_pUi->accept_dateEdit->date().toString("yyyy-MM-dd"));

  m_hQuery->executeSqlQuery(sQuery);

  m_hModel->setQuery(m_hQuery->getQuery());
  m_hFilterModel->setDynamicSortFilter(true);
  m_hFilterModel->setSourceModel(m_hModel);
  m_pUi->tb_stats->setModel(m_hFilterModel);
}

void CStatistic::on_StatisticWindow_finished(int result) { m_hModel->clear(); }
