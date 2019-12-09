#ifndef CUSTOMER_ADD_H
#define CUSTOMER_ADD_H


#include <QDialog>
#include <QTableView>
#include <map>


namespace Ui {
  class CustomerAddWindow;
}


class CCustomerAdd : public QDialog {
  Q_OBJECT

public:
  explicit CCustomerAdd(QWidget *parent = nullptr);
  ~CCustomerAdd();

public slots:
  void showWindow(QTableView* cust_tb_ptr, std::map<QString, size_t>* city_map);

private:
  Ui::CustomerAddWindow* m_pUi;
  std::map<QString, size_t>* m_city_map;
  QTableView* m_pCust_tb;
};


#endif // CUSTOMER_ADD_H
