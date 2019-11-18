#ifndef COMPADD_H
#define COMPADD_H


#include <QDialog>
#include <QTableView>
#include <QMessageBox>
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class ServiceAddWindow;
}

typedef std::map<QString, size_t> component_item_Ty_T;

class CCompAdd : public QDialog {
  Q_OBJECT

public:
  explicit CCompAdd(QWidget *parent = nullptr);
  ~CCompAdd();

public slots:
  void showWindow(QTableView* hTbPriceList);

private slots:
  void on_add_service_btn_clicked();

private:
  void update_comp_items();

  Ui::ServiceAddWindow* m_pUi;
  CQueryController* m_hQuery;
  QTableView* m_hTbPriceList;
  component_item_Ty_T m_map_comp_items;
};


#endif // COMPADD_H
