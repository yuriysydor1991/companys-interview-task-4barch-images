#include "src/qt6/models/ErrorSingleModel.h"

#include <memory>
#include <thread>

#include "src/log/log.h"

namespace Qt6i::models
{

QString ErrorSingleModel::getError()
{
  QString cp;
  
  {
    LOGI("Locking error");
    std::lock_guard<std::mutex> lk{emutex};
    
    cp = lastError;
  }
  LOGI("Unlocking error");
  
  return cp;
}

void ErrorSingleModel::setError(const QString& msg)
{
  {
    LOGI("Locking set error");
    std::lock_guard<std::mutex> lk{emutex};
  
    lastError = msg;
  }
  LOGI("Unlocking set error");
  
  emitError();
}

ErrorSingleModel& ErrorSingleModel::instance()
{
  static ErrorSingleModel i;
  
  return i;
}

void ErrorSingleModel::emitError()
{
  emit errorChanged(lastError);
}

}  // namespace Qt6i::models

