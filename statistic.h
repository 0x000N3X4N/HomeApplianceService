#ifndef STATISTIC_H
#define STATISTIC_H


#include <QDialog>
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class StatisticWindow;
}

class CStatistic : public QDialog {
  Q_OBJECT

public:
  explicit CStatistic(QWidget *parent = nullptr);
  ~CStatistic();

private slots:
  void on_done_btn_clicked();
  void on_StatisticWindow_finished(int result);

private:
  Ui::StatisticWindow* m_pUi;
  CQueryController* m_hQuery;
  QSqlQueryModel* m_hModel;
  QSortFilterProxyModel* m_hFilterModel;
};


#endif // STATISTIC_H
