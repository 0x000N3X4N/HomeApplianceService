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
  m_pUi->from_de->setCalendarPopup(true);
  m_pUi->from_de->setDate(QDate::currentDate());
  m_pUi->to_de->setCalendarPopup(true);
  m_pUi->to_de->setDate(QDate::currentDate());
  m_pUi->tb_stats->horizontalHeader()
                  ->setSectionResizeMode(QHeaderView::Stretch);
}

CStatistic::~CStatistic() {
  delete m_pUi;
  delete m_hQuery;
  delete m_hModel;
  delete m_hFilterModel;
}

void CStatistic::on_done_btn_clicked() {
  QString sQuery = QString("EXEC [stats] '%1','%2'")
                           .arg(m_pUi->from_de->date().toString("yyyy-MM-dd"),
                                m_pUi->to_de->date().toString("yyyy-MM-dd"));

  m_hQuery->executeSqlQuery(sQuery);

  m_hModel->setQuery(m_hQuery->getQuery());
  m_hFilterModel->setDynamicSortFilter(true);
  m_hFilterModel->setSourceModel(m_hModel);
  m_pUi->tb_stats->setModel(m_hFilterModel);
}

void CStatistic::on_StatisticWindow_finished(int result) { m_hModel->clear(); }
