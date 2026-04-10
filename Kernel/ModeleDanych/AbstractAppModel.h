#ifndef ABSTRACTAPPMODEL_H
#define ABSTRACTAPPMODEL_H

#include <QAbstractListModel>

class AbstractAppModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit AbstractAppModel(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {}

    virtual void initData(const QList<QStringList>&data) =0;
    virtual QList<QStringList> getData() const =0;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const =0;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const =0;

    virtual void dumpData() =0;
};

#endif // ABSTRACTAPPMODEL_H
