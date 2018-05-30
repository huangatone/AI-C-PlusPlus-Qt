#ifndef SHARUI_GLOBAL_H
#define SHARUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SHARUI_LIBRARY)
#  define SHARUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define SHARUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SHARUI_GLOBAL_H
