#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractTableModel>
#include "playlistitem.h"

class PlayListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PlayListModel(QObject *parent = 0);

    //General QTableView functions
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    //Our data model functions
    void addItem(PlaylistItem item);
    QList<PlaylistItem> getModelList();

signals:

public slots:

private:
    QList<PlaylistItem> playlist;

};

#endif // PLAYLISTMODEL_H
