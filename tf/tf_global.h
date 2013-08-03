#ifndef TF_GLOBAL_H
#define TF_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TF_LIBRARY)
#  define TFSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TFSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TF_GLOBAL_H
