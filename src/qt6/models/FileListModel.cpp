#include "src/qt6/models/FileListModel.h"

#include <QAbstractListModel>
#include <QString>
#include <QStringList>
#include <exception>
#include <memory>

#include "src/log/log.h"

namespace Qt6i::models
{

FileListModel::~FileListModel()
{
  LOGD("Waiting threads");
  for (auto thp : mthqueue) {
    if (thp->joinable()) {
      thp->join();
    }
  }
}

FileListModel::FileListModel(QObject *parent)
    : QAbstractListModel{parent}, cfactory{}
{
}

void FileListModel::clean_threads()
{
  LOGD("Cleaning finished threads");

  for (auto thiter = mthqueue.begin(); thiter != mthqueue.end(); ++thiter) {
    auto thp = *thiter;

    if (!thp->joinable()) {
      thiter = mthqueue.erase(thiter);
    }
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

bool FileListModel::thread_perform(barchclib0::ILibPtr converter,
                                   ImageFileModelPtr model)
{
  if (is_bmp(model->filepath())) {
    if (!thread_deal_bmp(converter, model)) {
      LOGE("Failure while dealing with the BMP " << model->filepath());
      return false;
    }
  } else if (is_barch(model->filepath())) {
    if (!thread_deal_barch(converter, model)) {
      LOGE("Failure while dealing with the Barch " << model->filepath());
      return false;
    }
  } else {
    LOGE("Unknown file type" << model->filepath());
  }

  return true;
}

bool FileListModel::thread_deal_bmp(barchclib0::ILibPtr converter,
                                    ImageFileModelPtr model)
{
  auto img = converter->read(model->filepath());

  if (img == nullptr) {
    LOGE("Fail while reading the image");
    return false;
  }

  // encode here!
  auto barch = converter->bmp_to_barch(img);

  if (barch == nullptr) {
    LOGE("Failure during the barch conversion");
    return false;
  }

  auto nfilepath = model->filepath().parent_path() /
                   (model->filepath().filename().string() + "packed.barch");

  barch->filepath(nfilepath);

  if (!converter->write(barch)) {
    LOGE("Failure during barch save");
    return false;
  }

  return true;
}

bool FileListModel::thread_deal_barch(barchclib0::ILibPtr converter,
                                      ImageFileModelPtr model)
{
  auto img = converter->read(model->filepath());

  if (img == nullptr) {
    LOGE("Fail while reading the image");
    return false;
  }

  auto bmp = converter->barch_to_bmp(img);

  if (bmp == nullptr) {
    LOGE("Fail to unpack the barch: " << model->filepath());
    return false;
  }

  return true;
}

void FileListModel::convert_file(const int &index)
{
  if (index < 0 || index > imagesSet.size()) {
    LOGE("Invalid index provided");
    return;
  }

  auto imgptr = imagesSet.at(index);

  assert(imgptr != nullptr);

  barchclib0::ILibPtr converter = cfactory.create();

  assert(converter != nullptr);

  if (converter == nullptr) {
    LOGE("Fail to create converter instance");
    return;
  }

  LOGD("Trying to create dealing thread");

  mthqueue.insert(std::make_shared<std::thread>([this, converter, imgptr]() {
    if (!thread_perform(converter, imgptr)) {
      LOGE("Failure during task performing");
      return;
    }

    LOGT("Performed successfully");
  }));

  clean_threads();
}

bool FileListModel::is_bmp(const std::filesystem::path &gpath)
{
  static const std::string bmpe = ".bmp";

  return gpath.extension().string() == bmpe;
}

bool FileListModel::is_barch(const std::filesystem::path &gpath)
{
  static const std::string barche = ".barch";
  static const std::string bae = ".barch";

  return gpath.extension().string() == barche ||
         gpath.extension().string() == bae;
}

bool FileListModel::is_image(const std::filesystem::path &gpath) {}

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
