#include "myfilesystemmodel.h"

//******************************************************************************
// MyFileSystemModel()
//******************************************************************************
MyFileSystemModel::MyFileSystemModel(QObject* parent, Settings *appSettings):
    QFileSystemModel(parent)
{
    // Needed ?
    this->appSettings = appSettings;
}

//******************************************************************************
// data()
//******************************************************************************
QVariant MyFileSystemModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DecorationRole) {
        QFileInfo info = MyFileSystemModel::fileInfo(index);
        if (info.isFile()) {
            int iconSize = appSettings->get("LAUNCHER_ICON_SIZE").toInt();
            int size = 48;
            if (iconSize == Settings::LAUNCHER_ICON_SIZE_SMALL) {
                size = 16;
            } else {
                if (iconSize == Settings::LAUNCHER_ICON_SIZE_MEDIUM) {
                    size = 32;
                } else {
                    if (iconSize == Settings::LAUNCHER_ICON_SIZE_LARGE) {
                        size = 48;
                    }
                }
            }
            if (info.suffix() == "avi") {
                return QPixmap(":/png/16x16/Computer.png").scaled(size, size, Qt::KeepAspectRatio);
            } else if (info.suffix() == "txt") {
                return QPixmap(":/png/16x16/Folder.png").scaled(size, size, Qt::KeepAspectRatio);
            } else if (info.suffix() == "vostok") {
                Shortcut *sh = new Shortcut(info.absoluteFilePath());
                return QPixmap(sh->icon).scaled(size, size, Qt::KeepAspectRatio);
            }
        }
    } else if (role == Qt::DisplayRole) {
        QFileInfo info = MyFileSystemModel::fileInfo(index);
        return "[" + info.baseName() + "]";
    }
    return QFileSystemModel::data(index, role);
}

//******************************************************************************
// dropEvent()
//******************************************************************************
void MyFileSystemModel::dropEvent(QDropEvent *ev) {
    qDebug("dropEvent");
}

//******************************************************************************
// dragEnterEvent()
//******************************************************************************
void MyFileSystemModel::dragEnterEvent(QDragEnterEvent *ev) {
    // Set the drop action to be the proposed action.
    // ev->acceptProposedAction();
    qDebug("dragEnterEvent");
}

void MyFileSystemModel::dragMoveEvent(QDragMoveEvent *ev)
{
    // The event needs to be accepted here
    // ev->accept();
    qDebug("dragMoveEvent");
}


Qt::ItemFlags MyFileSystemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QFileSystemModel::flags(index);

    if (!index.isValid())
        return defaultFlags;

    const QFileInfo& fileInfo = this->fileInfo(index);

    // The target
    if (fileInfo.isDir())
    {
        // allowed drop
        return Qt::ItemIsDropEnabled | defaultFlags;
    }
    // The source: should be directory (in that case)
    else if (fileInfo.isFile())
    {
        // allowed drag
        return Qt::ItemIsDragEnabled | defaultFlags;
    }

    return defaultFlags;
}
