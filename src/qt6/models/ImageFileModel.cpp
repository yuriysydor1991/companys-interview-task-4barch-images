#include "src/qt6/models/ImageFileModel.h"

#include <QAbstractListModel>
#include <QStringList>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>

#include "src/log/log.h"

namespace Qt6i::models
{

ImageFileModel::ImageFileModel(const std::string& gpath) : mpath{gpath}
{
  if (!read_file()) {
    LOGE("Fail to read file stat");
  }
}

const std::filesystem::path& ImageFileModel::filepath() const { return mpath; }

bool ImageFileModel::filepath(const std::filesystem::path& npath)
{
  mpath = npath;

  if (!read_file()) {
    LOGE("Fail to read file stat");
    return false;
  }

  return true;
}

const size_t& ImageFileModel::bytes_total() const { return m_size; }

ImageFileModelPtr ImageFileModel::create(const std::filesystem::path& npath)
{
  return std::make_shared<ImageFileModel>(npath);
}

bool ImageFileModel::read_file()
{
  if (mpath.empty()) {
    LOGT("No file provided");
    return true;
  }

  try {
    if (!std::filesystem::exists(mpath)) {
      LOGE("File " << mpath << " does not exists");
      return false;
    }

    if (!std::filesystem::is_regular_file(mpath)) {
      LOGE("Path " << mpath << " is not a regular file");
      return false;
    }

    m_size = std::filesystem::file_size(mpath);

    LOGT("File " << mpath << " size is " << m_size);
  }
  catch (const std::exception& e) {
    LOGE("Failure during file read: " << e.what());
    return false;
  }

  return true;
}

std::string ImageFileModel::current_operation() const
{
  // std::lock_guard<std::mutex> lk (opmutex);

  return mop;
}

void ImageFileModel::current_operation(const std::string& opname)
{
  std::lock_guard<std::mutex> lk(opmutex);

  mop = opname;
}

int ImageFileModel::index() { return mindex; }

void ImageFileModel::index(const int& nindex) { mindex = nindex; }

}  // namespace Qt6i::models
