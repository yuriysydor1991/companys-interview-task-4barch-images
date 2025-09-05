#ifndef THE_BMP_2_BARCH_CODER_PROJECT_ERRORSINGLEMODEL_STRUCT_H
#define THE_BMP_2_BARCH_CODER_PROJECT_ERRORSINGLEMODEL_STRUCT_H

#include <QObject>
#include <memory>
#include <mutex>
#include <thread>

namespace Qt6i::models
{

/**
 * @brief The Model for a current directory image files.
 */
class ErrorSingleModel : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString error_description READ getError NOTIFY errorChanged)
 public:
  QString getError();
  void setError(const QString& msg);

  static ErrorSingleModel& instance();

 signals:
  void errorChanged(const QString& e);

 private:
  virtual ~ErrorSingleModel() = default;
  ErrorSingleModel() = default;
  ErrorSingleModel(const ErrorSingleModel&) = delete;
  ErrorSingleModel(ErrorSingleModel&&) = delete;
  ErrorSingleModel& operator=(const ErrorSingleModel&) = delete;
  ErrorSingleModel& operator=(const ErrorSingleModel&&) = delete;

  void emitError();

  QString lastError;
  std::mutex emutex;
};

}  // namespace Qt6i::models

#endif  // THE_BMP_2_BARCH_CODER_PROJECT_ERRORSINGLEMODEL_STRUCT_H
