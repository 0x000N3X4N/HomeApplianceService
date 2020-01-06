#include "search.h"
#include "ui_search.h"


CSearch::CSearch(QWidget *parent) :
  QWidget(parent),
  m_pUi(new Ui::SearchWindow)
{
  m_hCmp = new CCompare();

  m_pUi->setupUi(this);
  m_pUi->search_res_tbv->horizontalHeader()
      ->setSectionResizeMode(QHeaderView::Stretch);
  //TODO: investigate why sorting doesn't work
  m_pUi->search_res_tbv->setSortingEnabled(true);

  connect(m_pUi->query_le, &QLineEdit::returnPressed,
          this, &CSearch::on_search_btn_clicked);
  connect(this, &CSearch::showCompareWindow,
          m_hCmp, &CCompare::showWindow);
}

CSearch::~CSearch()
{
  delete m_pUi;
}

void CSearch::on_search_btn_clicked() {
  auto query = m_pUi->query_le->text();
  size_t i = 0;
  CQueryController query_ctrl(CQueryController(CODBCW::getInstance("", nullptr, &i)));

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
    CMsgBox::show(QMessageBox::Critical, this, "Error!", e.what());
    return;
  }
  catch(...) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", "CSearch::on_search_btn_clicked : Unexpected error! LastError: [" +
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
    return QString("SELECT title AS 'Title', component_type AS 'Component type', specifications AS 'Specifications', "
                   "price AS 'Price', release_date AS 'Release date' "
                   "FROM components "
                   "JOIN components_type "
                   "ON FK_type_code = PK_component_type_id "
                   "WHERE title LIKE '%1' AND price BETWEEN %2 AND %3 "
                   "AND release_date BETWEEN %4 AND %5;")
                   .arg(query_title, price[0], price[1], year[0], year[1]);
  else if (isPreValidQTitle && isPreValidPrice && !isPreValidYear)
    return QString("SELECT title AS 'Title', component_type AS 'Component type', "
                   "specifications AS 'Specifications', "
                   "price AS 'Price', release_date AS 'Release date' "
                   "FROM components "
                   "JOIN components_type "
                   "ON FK_type_code = PK_component_type_id "
                   "WHERE title LIKE '%1' AND price "
                   "BETWEEN %2 AND %3;")
                   .arg(query_title, price[0], price[1]);
  else if (isPreValidPrice && !isPreValidQTitle && !isPreValidYear)
    return QString("SELECT title AS 'Title', component_type AS 'Component type', "
                   "specifications AS 'Specifications', price AS 'Price', "
                   "release_date AS 'Release date' "
                   "FROM components "
                   "JOIN components_type "
                   "ON FK_type_code = PK_component_type_id "
                   "WHERE price BETWEEN %1 AND %2;")
                   .arg(price[0], price[1]);
  else if(!isPreValidPrice && !isPreValidQTitle && !isPreValidYear)
    return QString("SELECT title AS 'Title', component_type AS 'Component type', specifications AS 'Specifications',"
                   " price AS 'Price', release_date AS 'Release date' "
                   "FROM components "
                   "JOIN components_type "
                   "ON FK_type_code = PK_component_type_id;");
  else if(isPreValidQTitle && !isPreValidPrice && !isPreValidYear)
    return QString("SELECT title AS 'Title', component_type AS 'Component type', specifications AS 'Specifications', "
                   "price AS 'Price', release_date AS 'Release date' "
                   "FROM components "
                   "JOIN components_type "
                   "ON FK_type_code = PK_component_type_id "
                   "WHERE title LIKE '%1';")
                   .arg(query_title);
  else if(isPreValidYear && !isPreValidQTitle && !isPreValidPrice)
    return QString("SELECT title AS 'Title', component_type AS 'Component type', specifications AS 'Specifications', "
                   "price AS 'Price', release_date AS 'Release date' "
                   "FROM components "
                   "JOIN components_type "
                   "ON FK_type_code = PK_component_type_id "
                   "WHERE release_date BETWEEN %1 AND %2;")
                   .arg(year[0], year[1]);
  else if(isPreValidYear && isPreValidPrice && !isPreValidQTitle)
    return QString("SELECT title AS 'Title', component_type AS 'Component type', specifications AS 'Specifications', "
                   "price AS 'Price', release_date AS 'Release date' "
                   "FROM components "
                   "JOIN components_type "
                   "ON FK_type_code = PK_component_type_id "
                   "WHERE price BETWEEN %1 AND %2 AND "
                   "release_date BETWEEN %3 AND %4;")
                   .arg(price[0], price[1], year[0], year[1]);
  else if(isPreValidYear && isPreValidQTitle && !isPreValidPrice)
    return QString("SELECT title AS 'Title', component_type AS 'Component type', specifications AS 'Specifications', "
                   "price AS 'Price', release_date AS 'Release date' "
                   "FROM components "
                   "JOIN components_type "
                   "ON FK_type_code = PK_component_type_id "
                   "WHERE title LIKE '%1' AND release_date BETWEEN "
                   "%2 AND %3;")
                   .arg(query_title, year[0], year[1]);

  return QString();
}

void CSearch::on_cmp_btn_clicked() {
  try {
    QItemSelectionModel* select = m_pUi->search_res_tbv->selectionModel();
    QAbstractItemModel* search_res_AIM = m_pUi->search_res_tbv->model();
    std::map<QString, QStringList> cmp_item_map;

    if (select->hasSelection()) {
      QModelIndexList idx_list = select->selectedIndexes();
      if (idx_list.count() > CMP::MAX && idx_list.count() == 0)
        throw std::invalid_argument("Error, too many rows are selected for compare or it's 0!");

      QString comp_type = search_res_AIM->data(search_res_AIM->
                                               index(idx_list[0].row(), idx_list[0].column() + 1)).toString();
      //validate selected model
      for (auto i = 0; i < idx_list.count(); i++) {
        if (idx_list[i].column() > 0)
          throw std::invalid_argument("Error, invalid column selected!");
        else if (comp_type != search_res_AIM->data(search_res_AIM->
                                      index(idx_list[i].row(), idx_list[i].column() + 1)).toString())
          throw std::invalid_argument("Error, component types are different!");

        auto title_qstr = idx_list[i].data().toString();
        auto spec_qstr = search_res_AIM->data(search_res_AIM->index(idx_list[i].row(), idx_list[i].column() + 2)).toString();
        qDebug() << "[" << idx_list[i].row() << ";" << idx_list[i].column() << "]"
                 << "data: " << title_qstr << "specifications: " <<
                    spec_qstr;

        auto spec_qstr_list = spec_qstr.split(';');
        spec_qstr_list.sort();
        cmp_item_map[title_qstr] = spec_qstr_list;
      }

      emit showCompareWindow(cmp_item_map);
    }
  }
  catch(const std::invalid_argument& e) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", e.what());
    return;
  }
  catch(...) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", "CSearch::on_cmp_btn_clicked : Unexpected error!");
  }
}
