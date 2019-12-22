#include "search.h"
#include "ui_search.h"


CSearch::CSearch(QWidget *parent) :
  QWidget(parent),
  m_pUi(new Ui::SearchWindow)
{
  m_pUi->setupUi(this);
  m_pUi->search_res_tbv->horizontalHeader()
      ->setSectionResizeMode(QHeaderView::Stretch);
  //TODO: investigate why sorting doesn't work
  m_pUi->search_res_tbv->setSortingEnabled(true);
}

CSearch::~CSearch()
{
  delete m_pUi;
}

void CSearch::on_search_btn_clicked() {
  auto query = m_pUi->query_le->text();
  CQueryController query_ctrl(CODBCW::getInstance());

  try {
    QueryTyper query_typer;
    auto query_parse_struct = query_typer.fetch(query);
    QT query_type = query_parse_struct.second;
    auto params_container = query_parse_struct.first;
    QString sql_query;

    switch(query_type) {
    case NotFound:
      throw std::invalid_argument("Error, query type not found!");
    break;
    case NParam:
    {
      auto query_title = query;

      sql_query = parse_search_qeury(query_title, QStringList(), QStringList());

#ifdef QT_DEBUG
      qDebug() << "[Search query:" << sql_query << "][QT:" << query_type << "]";
#endif

      if (query_ctrl.executeSqlQuery(sql_query)) {
        QSqlQueryModel* hResultQModel = new QSqlQueryModel();
        QSortFilterProxyModel* hResultFilterModel = new QSortFilterProxyModel();
        hResultQModel->setQuery(query_ctrl.getQuery());
        hResultFilterModel->setDynamicSortFilter(true);
        hResultFilterModel->setSourceModel(hResultQModel);

        m_pUi->search_res_tbv->setModel(hResultQModel);
      }
      else
        throw std::invalid_argument("Error, can't execute SQL query for search!  LastError: [" +
                                    query_ctrl.getQuery().lastError().text().toStdString() + "]");

    break;
    }
    case ParamWildcard:
    {
      auto query_title = query;
      int off = query_title.indexOf('[');
      query_title.remove(off, query_title.length() - off);
      query_title.replace('*', '%');
      auto params_map = std::get<0>(params_container);
      auto price_it = params_map.find(QPK::PRICE), year_it = params_map.find(QPK::YEAR);
      QStringList price = price_it != params_map.end() ? price_it->second.split('-') : QStringList { },
          year = year_it != params_map.end() ? year_it->second.split('-') : QStringList { };

      if ((year.count() == 1 && year[0] == year_it->second) || (price.count() == 1 && price[0] == price_it->second)
          || (year.count() > 2) || (price.count() > 2))
        throw std::invalid_argument("Error, invalid params!");

      sql_query = parse_search_qeury(query_title, price, year);

#ifdef QT_DEBUG
      qDebug() << "Search query: " << sql_query;
#endif

      if (query_ctrl.executeSqlQuery(sql_query)) {
        QSqlQueryModel* hResultQModel = new QSqlQueryModel();
        QSortFilterProxyModel* hResultFilterModel = new QSortFilterProxyModel();
        hResultQModel->setQuery(query_ctrl.getQuery());
        hResultFilterModel->setDynamicSortFilter(true);
        hResultFilterModel->setSourceModel(hResultQModel);

        m_pUi->search_res_tbv->setModel(hResultQModel);
      }
      else
        throw std::invalid_argument("Error, can't execute SQL query for search!  LastError: [" +
                                    query_ctrl.getQuery().lastError().text().toStdString() + "]");
    break;
    }
    case NParamWildcard:
    {
      auto query_title = query;
      query_title.replace('*', '%');
      sql_query = parse_search_qeury(query_title, QStringList(), QStringList());

#ifdef QT_DEBUG
      qDebug() << "[Search query:" << sql_query << "][QT:" << query_type << "]";
#endif

      if (query_ctrl.executeSqlQuery(sql_query)) {
        QSqlQueryModel* hResultQModel = new QSqlQueryModel();
        QSortFilterProxyModel* hResultFilterModel = new QSortFilterProxyModel();
        hResultQModel->setQuery(query_ctrl.getQuery());
        hResultFilterModel->setDynamicSortFilter(true);
        hResultFilterModel->setSourceModel(hResultQModel);

        m_pUi->search_res_tbv->setModel(hResultQModel);
      }
      else
        throw std::invalid_argument("Error, can't execute SQL query for search!  LastError: [" +
                                    query_ctrl.getQuery().lastError().text().toStdString() + "]");
    }
    break;
    case QT::Param:
      auto query_title = query;
      int off = query_title.indexOf('[');
      query_title.remove(off, query_title.length() - off);
      auto params_map = std::get<0>(params_container);
      auto price_it = params_map.find(QPK::PRICE), year_it = params_map.find(QPK::YEAR);
      QStringList price = price_it != params_map.end() ? price_it->second.split('-') : QStringList { },
          year = year_it != params_map.end() ? year_it->second.split('-') : QStringList { };

      if ((year.count() == 1 && year[0] == year_it->second) || (price.count() == 1 && price[0] == price_it->second)
          || (year.count() > 2) || (price.count() > 2))
        throw std::invalid_argument("Error, invalid params!");

      sql_query = parse_search_qeury(query_title, price, year);

#ifdef QT_DEBUG
      qDebug() << "[Search query:" << sql_query << "][QT:" << query_type << "]";
#endif

      if (query_ctrl.executeSqlQuery(sql_query)) {
        QSqlQueryModel* hResultQModel = new QSqlQueryModel();
        QSortFilterProxyModel* hResultFilterModel = new QSortFilterProxyModel();
        hResultQModel->setQuery(query_ctrl.getQuery());
        hResultFilterModel->setDynamicSortFilter(true);
        hResultFilterModel->setSourceModel(hResultQModel);

        m_pUi->search_res_tbv->setModel(hResultQModel);
      }
      else
        throw std::invalid_argument("Error, can't execute SQL query for search!  LastError: [" +
                                    query_ctrl.getQuery().lastError().text().toStdString() + "]");


    break;
    }
  }
  catch(std::invalid_argument& e) {
      QMessageBox::critical(this, e.what(), e.what());
      return;
    }
  catch(...) {
    QMessageBox::critical(this, "Error!", "CSearch::on_search_btn_clicked : Unexpected error! LastError: [" +
                          query_ctrl.getQuery().lastError().text() + "]");
    return;
  }
}

bool CSearch::isOnlyDigits(std::string str) {
  return (str.find_first_not_of("0123456789") == std::string::npos);
}

QString CSearch::parse_search_qeury(QString query_title, QStringList price, QStringList year) {
  bool isPreValidPrice = price.count() == QPK::COUNT && isOnlyDigits(price[0].toStdString())
                         && isOnlyDigits(price[1].toStdString()) ? true : false;

  bool isPreValidYear = year.count() == QPK::COUNT && isOnlyDigits(year[0].toStdString()) &&
                        isOnlyDigits(year[1].toStdString()) ? true : false;

  bool isPreValidQTitle = query_title != "";

  if (isPreValidPrice && isPreValidYear && isPreValidQTitle)
    return QString("SELECT title AS 'Title', specifications AS 'Specifications', "
                   "price AS 'Price', release_date AS 'Release date' "
                   "FROM components "
                   "WHERE title LIKE '%1' AND price BETWEEN %2 AND %3 "
                   "AND release_date BETWEEN %4 AND %5;")
                   .arg(query_title, price[0], price[1], year[0], year[1]);
  else if (isPreValidQTitle && isPreValidPrice && !isPreValidYear)
    return QString("SELECT title AS 'Title', "
                   "specifications AS 'Specifications', "
                   "price AS 'Price', release_date AS 'Release date' "
                   "FROM components "
                   "WHERE title LIKE '%1' AND price "
                   "BETWEEN %2 AND %3;")
                   .arg(query_title, price[0], price[1]);
  else if (isPreValidPrice && !isPreValidQTitle && !isPreValidYear)
    return QString("SELECT title AS 'Title', "
                   "specifications AS 'Specifications', price AS 'Price', "
                   "release_date AS 'Release date' "
                   "FROM components "
                   "WHERE price BETWEEN %1 AND %2;")
                   .arg(price[0], price[1]);
  else if(!isPreValidPrice && !isPreValidQTitle && !isPreValidYear)
    return QString("SELECT title AS 'Title', specifications AS 'Specifications',"
                   " price AS 'Price', release_date AS 'Release date' "
                   "FROM components;");
  else if(isPreValidQTitle && !isPreValidPrice && !isPreValidYear)
    return QString("SELECT title AS 'Title', specifications AS 'Specifications', "
                   "price AS 'Price', release_date AS 'Release date' "
                   "FROM components "
                   "WHERE title LIKE '%1';")
                   .arg(query_title);
  else if(isPreValidYear && !isPreValidQTitle && !isPreValidPrice)
    return QString("SELECT title AS 'Title', specifications AS 'Specifications', "
                   "price AS 'Price', release_date AS 'Release date' "
                   "FROM components "
                   "WHERE release_date BETWEEN %1 AND %2;")
                   .arg(year[0], year[1]);
  else if(isPreValidYear && isPreValidPrice && !isPreValidQTitle)
    return QString("SELECT title AS 'Title', specifications AS 'Specifications', "
                   "price AS 'Price', release_date AS 'Release date' "
                   "FROM components "
                   "WHERE price BETWEEN %1 AND %2 AND "
                   "release_date BETWEEN %3 AND %4;")
                   .arg(price[0], price[1], year[0], year[1]);
  else if(isPreValidYear && isPreValidQTitle && !isPreValidPrice)
    return QString("SELECT title AS 'Title', specifications AS 'Specifications', "
                   "price AS 'Price', release_date AS 'Release date' "
                   "FROM components "
                   "WHERE title LIKE '%1' AND release_date BETWEEN "
                   "%2 AND %3;")
                   .arg(query_title, year[0], year[1]);

  return QString();
}
