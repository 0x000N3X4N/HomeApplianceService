#ifndef EXPORT_H
#define EXPORT_H


#include <QDialog>
#include <QTableView>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QDebug>


namespace Ui {
  class ExportWindow;
}


class CExport : public QDialog {
  Q_OBJECT

public:
  explicit CExport(QWidget *parent = nullptr);
  ~CExport();

public slots:
  void showWindow(QTableView* orders_tb_ptr);

private slots:
  void on_export_btn_clicked();

private:
  void saveAsCSV();

  Ui::ExportWindow* m_pUi;
  QTableView* m_orders_tb;
};


#endif // EXPORT_H
