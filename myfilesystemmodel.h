#ifndef MYFILESYSTEMMODEL_H
#define MYFILESYSTEMMODEL_H

#include <QFileSystemModel>
#include "shortcuts.h"
#include "settings.h"
// #include <QSet>
// #include <QHash>

class MyFileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    MyFileSystemModel(QObject* parent=0, Settings *appSettings=0);
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    Settings *appSettings;
};

#endif // MYFILESYSTEMMODEL_H
