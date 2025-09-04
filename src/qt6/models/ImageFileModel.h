#ifndef THE_BMP_2_BARCH_CODER_PROJECT_IMAGEFILEMODEL_STRUCT_H
#define THE_BMP_2_BARCH_CODER_PROJECT_IMAGEFILEMODEL_STRUCT_H

#include <QAbstractListModel>
#include <QStringList>

#include <memory>
#include <filesystem>
#include <cstddef>
#include <string>

namespace Qt6i::models
{

/**
 * @brief The Model for an image file.
 */
class ImageFileModel
{
 public:
  using ImageFileModelPtr = std::shared_ptr<ImageFileModel>;
  
  virtual ~ImageFileModel() = default;
  ImageFileModel(const std::string& gpath);
  
  const std::filesystem::path& filepath() const;
  bool filepath(const std::filesystem::path& npath) ;
  
  const size_t& bytes_total() const ;
  
  static ImageFileModelPtr create(const std::filesystem::path& npath);

private:
  bool read_file();
  
  std::filesystem::path mpath;
  size_t m_size{0U};
};

using ImageFileModelPtr = ImageFileModel::ImageFileModelPtr;

}  // namespace Qt6i::models

#endif  // THE_BMP_2_BARCH_CODER_PROJECT_IMAGEFILEMODEL_STRUCT_H
