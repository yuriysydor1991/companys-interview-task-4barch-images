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
    LOGT("Locking error");
    std::lock_guard<std::mutex> lk{emutex};

    cp = lastError;
  }
  LOGT("Unlocking error");

  return cp;
}

void ErrorSingleModel::setError(const QString& msg)
{
  {
    LOGT("Locking set error");
    std::lock_guard<std::mutex> lk{emutex};

    lastError = msg;
  }
  LOGT("Unlocking set error");

  emitError();
}

ErrorSingleModel& ErrorSingleModel::instance()
{
  static ErrorSingleModel i;

  return i;
}

void ErrorSingleModel::emitError() { emit errorChanged(lastError); }

}  // namespace Qt6i::models
