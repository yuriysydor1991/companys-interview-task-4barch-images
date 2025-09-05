#ifndef THE_BMP_2_BARCH_CODER_PROJECT_FILELISTMODEL_STRUCT_H
#define THE_BMP_2_BARCH_CODER_PROJECT_FILELISTMODEL_STRUCT_H

#include <QAbstractListModel>
#include <filesystem>
#include <memory>
#include <set>
#include <thread>

#include "LibraryFacade.h"
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

  using mutexptr = std::shared_ptr<std::mutex>;
  using muteximagepair = std::pair<mutexptr, ImageFileModelPtr>;
  using ImageFileModelSet = std::vector<muteximagepair>;
  using FileListModelPtr = std::shared_ptr<FileListModel>;
  using threadsQueue = std::set<std::shared_ptr<std::thread>>;

  virtual ~FileListModel();
  explicit FileListModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void convert_file(const int &index);

  /// @brief init object with a current directory path
  bool init();

  /// @brief init object with directory path
  bool init(const std::filesystem::path &dpath);

  static FileListModelPtr create(QObject *parent = nullptr);

 private:
  static bool is_bmp(const std::filesystem::path &gpath);
  static bool is_barch(const std::filesystem::path &gpath);
  static bool is_image(const std::filesystem::path &gpath);

  bool thread_perform(barchclib0::ILibPtr converter, ImageFileModelPtr model,
                      QModelIndex idx);
  bool thread_deal_bmp(barchclib0::ILibPtr converter, ImageFileModelPtr model);
  bool thread_deal_barch(barchclib0::ILibPtr converter,
                         ImageFileModelPtr model);

  void emit_row_data_update(QModelIndex idx);

  void clean_threads();

  ImageFileModelSet imagesSet;

  barchclib0::LibraryFacade cfactory;

  threadsQueue mthqueue;
};

using FileListModelPtr = FileListModel::FileListModelPtr;

}  // namespace Qt6i::models

#endif  // THE_BMP_2_BARCH_CODER_PROJECT_FILELISTMODEL_STRUCT_H
