#include "src/qt6/models/FileListModel.h"

#include <QAbstractListModel>
#include <QString>
#include <QStringList>
#include <exception>
#include <memory>

#include "src/log/log.h"

namespace Qt6i::models
{

FileListModel::FileListModel(QObject *parent) : QAbstractListModel{parent}, cfactory{}, converter{cfactory.create()} 
{
  if (converter == nullptr) {
    LOGE("Fail to create converter instance");
  }
}

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

  if (role == ImageOperationRole) {
    return QString::fromStdString(image->current_operation());
  }

  LOGE("Unknown role provided: " << role);

  return {};
}

QHash<int, QByteArray> FileListModel::roleNames() const
{
  QHash<int, QByteArray> roles;

  roles[ImagePathRole] = "path";
  roles[ImageSizeRole] = "size";
  roles[ImageOperationRole] = "current_operation";

  return roles;
}

void FileListModel::convert_file(const int& index)
{
  
}

bool FileListModel::init()
{
  namespace fs = std::filesystem;

  const fs::path current_path = fs::current_path();

  return init(current_path);
}

bool FileListModel::init(const std::filesystem::path &dpath)
{
  namespace fs = std::filesystem;

  if (dpath.empty()) {
    LOGE("Empty path provided");
    return false;
  }

  LOGD("Listing directory: " << dpath);

  try {
    for (const auto &entry : fs::directory_iterator(dpath)) {
      if (!fs::is_regular_file(entry.path())) {
        continue;
      }

      LOGT("Obtained file: " << entry.path().filename().string());

      auto cimage = ImageFileModel::create(entry.path());

      imagesSet.emplace_back(cimage);
    }
  }
  catch (const std::exception &e) {
    LOGE("Error during fs traverse: " << e.what());
    return 1;
  }

  return true;
}

FileListModelPtr FileListModel::create(QObject *parent)
{
  return std::make_shared<FileListModel>(parent);
}

}  // namespace Qt6i::models
