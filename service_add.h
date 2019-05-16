#ifndef PRICEADD_H
#define PRICEADD_H

#include <QDialog>
#include <QTableView>
#include <QMessageBox>
#include "libs/xSql/handle.h"

namespace Ui {
  class ServiceAddWindow;
}

class CServiceAdd : public QDialog
{
  Q_OBJECT

public:
  explicit CServiceAdd(QWidget *parent = nullptr);
  ~CServiceAdd();

public slots:
  void showWindow(QTableView* hTbPriceList);

private slots:
  void on_add_service_btn_clicked();

private:
  Ui::ServiceAddWindow* m_pUi;
  Handle* m_hQuery;
  QTableView* m_hTbPriceList;
  std::array<QString, 20> m_listOfEquipment {
    "Стиральных машин",
    "Посудомоечных машин",
    "СВЧ",
    "Телевизоров",
    "ЖК мониторов",
    "Плазменных панелей",
    "Холодильников",
    "Газовых плит",
    "Сушильных шкафов",
    "Блендеры",
    "Воздухоочистители",
    "Инфракрасные обогреватели ",
    "Котлы ",
    "Утюги",
    "Фены",
    "Степлеры",
    "Бензопилы",
    "Мотоблоки",
    "Хлебопечки",
    "Кофеварки"
  };
};

#endif // PRICEADD_H
