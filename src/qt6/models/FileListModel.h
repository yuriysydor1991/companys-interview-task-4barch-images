#ifndef THE_BMP_2_BARCH_CODER_PROJECT_FILELISTMODEL_STRUCT_H
#define THE_BMP_2_BARCH_CODER_PROJECT_FILELISTMODEL_STRUCT_H

#include <QAbstractListModel>
#include <QStringList>
#include <filesystem>
#include <memory>

#include "src/qt6/models/ImageFileModel.h"

namespace Qt6i::models
{

/**
 * @brief The Model for a current directory image files.
 */
class FileListModel : public QAbstractListModel
{
  Q_OBJECT
 public:
  enum Roles
  {
    ImagePathRole = Qt::UserRole + 1,
    ImageSizeRole = Qt::UserRole + 2,
    ImageOperationRole = Qt::UserRole + 3,
  };

  using ImageFileModelSet = std::vector<ImageFileModelPtr>;
  using FileListModelPtr = std::shared_ptr<FileListModel>;

  virtual ~FileListModel() = default;
  explicit FileListModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void addItem(const QString &item);

  /// @brief init object with a current directory path
  bool init();

  /// @brief init object with directory path
  bool init(const std::filesystem::path &dpath);

  static FileListModelPtr create(QObject *parent = nullptr);

 private:
  ImageFileModelSet imagesSet;
};

using FileListModelPtr = FileListModel::FileListModelPtr;

}  // namespace Qt6i::models

#endif  // THE_BMP_2_BARCH_CODER_PROJECT_FILELISTMODEL_STRUCT_H
