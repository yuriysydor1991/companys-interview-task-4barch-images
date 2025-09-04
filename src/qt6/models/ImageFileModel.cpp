#include "src/qt6/models/ImageFileModel.h"

#include <QAbstractListModel>
#include <QStringList>

#include <memory>
#include <filesystem>
#include <cstddef>
#include <string>
#include <exception>

#include "src/log/log.h"

namespace Qt6i::models
{

ImageFileModel::ImageFileModel(const std::string& gpath) : mpath{gpath} 
{
  if (!read_file()) {
    LOGE("Fail to read file stat");
  }
}

const std::filesystem::path& ImageFileModel::filepath() const { return mpath ; }

bool ImageFileModel::filepath(const std::filesystem::path& npath) 
{ 
  mpath = npath ; 
  
  if (!read_file()) {
    LOGE("Fail to read file stat");
    return false;
  }
  
  return true;
}

const size_t& ImageFileModel::bytes_total() const
{
  return m_size;
}

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
    if (std::filesystem::exists(mpath)) {
      LOGE("File " << mpath << " does not exists");
      return false;
    }
    
    if (std::filesystem::is_regular_file(mpath)) {
      LOGE("Path " << mpath << " is not a regular file");
      return false;
    }
    
    m_size = std::filesystem::file_size(mpath);
  } catch (const std::exception& e) {
    LOGE("Failure during file read: " << e.what());
    return false;
  }
  
  return m_size > 0U;
}

}  // namespace Qt6i::models
