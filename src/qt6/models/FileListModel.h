#ifndef THE_BMP_2_BARCH_CODER_PROJECT_FILELISTMODEL_STRUCT_H
#define THE_BMP_2_BARCH_CODER_PROJECT_FILELISTMODEL_STRUCT_H

#include <QAbstractListModel>
#include <QStringList>
#include <memory>

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
    NameRole = Qt::UserRole + 1
  };

  virtual ~FileListModel() = default;
  explicit FileListModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void addItem(const QString &item);

 private:
};

}  // namespace Qt6i::models

#endif  // THE_BMP_2_BARCH_CODER_PROJECT_FILELISTMODEL_STRUCT_H
