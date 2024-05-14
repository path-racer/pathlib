#include "pathlib.h"

namespace Pathlib {

/**/
Error Core::initiate(const utf8* log_path)
{
  SetConsoleOutputCP(CP_UTF8);
  if (Errors::popup_on_error(log.initiate(log_path))) {
    return false;
  }
  return Errors::ERROR_NONE;
}

/**/
void Core::shutdown()
{
}
}