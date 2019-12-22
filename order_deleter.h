#ifndef ORDER_DEL_H
#define ORDER_DEL_H


#include <QDialog>
#include <QTableView>
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class OrderWindow;
}


class COrderDel : public QDialog {
  Q_OBJECT

public:
  explicit COrderDel(QWidget *parent = nullptr);
  ~COrderDel();

public slots:
  void showWindow(QTableView* tb_ptr,
                  std::tuple <
                                 std::vector<QString>, // order title
                                 std::vector<QString>, // order employeer
                                 std::vector<QString>, // order acceptance date
                                 std::vector<QString>  // order price
                             > o_struct);

private slots:
  void clearUi();
  void on_accept_btn_clicked();

private:
  Ui::OrderWindow* m_pUi;
  QTableView* m_tb_ptr;
};

#endif // ORDER_DEL_H
