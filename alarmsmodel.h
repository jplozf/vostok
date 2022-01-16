#ifndef ALARMSMODEL_H
#define ALARMSMODEL_H

#include <QAbstractTableModel>

class AlarmsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    AlarmsModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // ALARMSMODEL_H
