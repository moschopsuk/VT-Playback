#include "playlistmodel.h"
#include "playlistitem.h"
#include <mlt++/Mlt.h>
#include <QDebug>

PlayListModel::PlayListModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int PlayListModel::rowCount(const QModelIndex & /*parent*/) const {
    return playlist.size();
}

int PlayListModel::columnCount(const QModelIndex & /*parent*/) const {
    return 4;
}

bool PlayListModel::removeRows(int position, int rows, const QModelIndex &index)
 {
     Q_UNUSED(index);
     beginRemoveRows(QModelIndex(), position, position+rows-1);

     for (int row=0; row < rows; ++row) {
         playlist.removeAt(position);
     }

     endRemoveRows();
     return true;
 }


 QVariant PlayListModel::headerData(int section, Qt::Orientation orientation, int role) const
 {
    if (role != Qt::DisplayRole)
        return QVariant();

        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return tr("#");

                case 1:
                    return tr("Name");

                case 2:
                    return tr("In");

                case 3:
                    return tr("Out");

                default:
                    return QVariant();
        }
    }
    return QVariant();
 }

 QVariant PlayListModel::data(const QModelIndex &index, int role) const {
      if (!index.isValid())
          return QVariant();

      if (index.row() >= playlist.size() || index.row() < 0)
          return QVariant();

      if (role == Qt::DisplayRole) {
          PlaylistItem item = playlist.at(index.row());

          if (index.column() == 0)
              return index.row();
          else if (index.column() == 1)
              return item.getName();
          else if (index.column() == 2)
              return item.getIn();
          else if (index.column() == 3)
              return item.getOut();
      }
      return QVariant();
}

Qt::ItemFlags PlayListModel::flags(const QModelIndex &index) const{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

Qt::DropActions PlayListModel::supportedDropActions() const {
    return Qt::CopyAction | Qt::MoveAction;
}


 void PlayListModel::addItem(PlaylistItem item) {
    int row = playlist.count();

    // insert 1 row.
    beginInsertRows(QModelIndex(), row, row);

    playlist.append(item);

    endInsertRows();
 }

 QList<PlaylistItem> PlayListModel::getModelList() {
    return playlist;
 }

 void PlayListModel::reset() {
    qDebug() << playlist.size();
    removeRows(0, playlist.size(), QModelIndex());
 }

