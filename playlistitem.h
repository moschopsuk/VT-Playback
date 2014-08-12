#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include <mlt++/Mlt.h>
#include <QString>

class PlaylistItem
{
public:
    PlaylistItem(Mlt::Producer &producer, QString name, QString in, QString out);
    Mlt::Producer *getProducer();
    QString getName();
    QString getIn();
    QString getOut();

private:
    Mlt::Producer *producer;
    QString name;
    QString in;
    QString out;
};

#endif // PLAYLISTITEM_H
