#ifndef PCSM_ERR_H
#define PCSM_ERR_H


#include <QMessageBox>
#include <QApplication>
#include <QDebug>


class CMsgBox {
public:
  static void show(QMessageBox::Icon ico, QWidget* parent,
                    QString const & title, QString const & text)
  {
    switch(ico) {
    case QMessageBox::NoIcon:
    break;
    case QMessageBox::Information:
      qDebug().noquote().nospace() << "INFO: [title:" << title << "][text:" << text << "]";
      QApplication::beep();
      QMessageBox::information(parent, title, text);
    break;
    case QMessageBox::Warning:
      qDebug().noquote().nospace() << "WARNING: [title:" << title << "][text:" << text << "]";
      QApplication::beep();
      QMessageBox::warning(parent, title, text);
    break;
    case QMessageBox::Critical:
      qDebug().noquote().nospace() << "ERROR: [title:" << title << "][text:" << text << "]";
      QApplication::beep();
      QMessageBox::critical(parent, title, text);
    break;
    case QMessageBox::Question:
      QApplication::beep();
      QMessageBox::question(parent, title, text);
    break;
    }
  }
};


#endif // PCSM_ERR_H
