#include "systemService.h"
#include <QString>

void logOutput(QtMsgType p_type, const QMessageLogContext& p_context, const QString& p_msg);

void initSystemService()
{
	qInstallMessageHandler(logOutput);
}
