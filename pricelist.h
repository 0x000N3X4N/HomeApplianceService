#ifndef PRICELISTWINDOW_H
#define PRICELISTWINDOW_H

#include <QDialog>
#include <QtSql>

namespace Ui {
  class PriceListWindow;
}

class PriceList : public QDialog
{
  Q_OBJECT

public:
  explicit PriceList(QWidget *parent = nullptr);
  ~PriceList();

public slots:
  void setTablePriceList(QSortFilterProxyModel* );
  void showWindow();

private:
  Ui::PriceListWindow* ui;
};

#endif // PRICELISTWINDOW_H
