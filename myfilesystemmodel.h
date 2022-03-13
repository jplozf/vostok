#ifndef MYFILESYSTEMMODEL_H
#define MYFILESYSTEMMODEL_H

#include <QFileSystemModel>
#include "shortcuts.h"
#include "settings.h"
// #include <QSet>
// #include <QHash>

class Settings;

class MyFileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    MyFileSystemModel(QObject* parent=0, Settings* appSettings=0);
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    Settings *appSettings;

protected:
    void dropEvent(QDropEvent *ev);
    void dragEnterEvent(QDragEnterEvent *ev);
    void dragMoveEvent(QDragMoveEvent *ev);
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // MYFILESYSTEMMODEL_H
