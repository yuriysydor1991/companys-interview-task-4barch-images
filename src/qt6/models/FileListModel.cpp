#include "src/qt6/models/FileListModel.h"

#include <QAbstractListModel>
#include <QStringList>
#include <memory>

namespace Qt6i::models
{

FileListModel::FileListModel(QObject *parent) : QAbstractListModel(parent) {}

int FileListModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);

  return 0;  // m_data.size();
}

QVariant FileListModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() < 0 /* || index.row() >= m_data.size() */)
    return {};

  //    if (role == NameRole || role == Qt::DisplayRole)
  //        return m_data.at(index.row());

  return {};
}

QHash<int, QByteArray> FileListModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[NameRole] = "name";
  return roles;
}

void FileListModel::addItem(const QString &item) {}

}  // namespace Qt6i::models
