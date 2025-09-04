#include "src/qt6/models/FileListModel.h"

#include <QAbstractListModel>
#include <QStringList>
#include <QString>

#include <memory>

#include "src/log/log.h"

namespace Qt6i::models
{

FileListModel::FileListModel(QObject *parent) : QAbstractListModel(parent) {}

int FileListModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);

 return imagesSet.size();
}

QVariant FileListModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() < 0 || index.row() >= imagesSet.size()) {
    LOGE("Invalid index provided: " << index.row());
    return {};
  }

  const ImageFileModelPtr image = imagesSet.at(index.row());
  
  if (image == nullptr) {
    LOGE("Retrieved invalid object pointer");
    return {};
  }
  
  if (role == ImagePathRole || role == Qt::DisplayRole) {
    return QString::fromStdString(image->filepath().string());
  }
  
  if (role == ImageSizeRole) {
    return QString::number(image->bytes_total());
  }

  return {};
}

QHash<int, QByteArray> FileListModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  
  roles[ImagePathRole] = "path";
  roles[ImageSizeRole] = "size";
  
  return roles;
}

void FileListModel::addItem(const QString &item) {}

}  // namespace Qt6i::models
