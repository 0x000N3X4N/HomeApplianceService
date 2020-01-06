#include "connection.h"
#include <QApplication>
#include <QFile>
#include <QDebug>


int main(int argc, char *argv[]) {
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QApplication a(argc, argv);

  QString pathToStyleSheet = ":/styles/windowStyle.css";
  QFile fStyleSheet(pathToStyleSheet);
  QString contentOfStyleSheet;

  if (!fStyleSheet.open(QFile::ReadOnly))
    qDebug() << "Error to open stylesheet!";
  else {
    contentOfStyleSheet = fStyleSheet.readAll();
    qApp->setStyleSheet(contentOfStyleSheet);
  }

  CConnection w;
  w.show();

  return a.exec();
}
