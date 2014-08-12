#include "playlistitem.h"

PlaylistItem::PlaylistItem(Mlt::Producer &producer, QString name, QString in, QString out)
{
    this->producer      = &producer;
    this->name          = name;
    this->in            = in;
    this->out           = out;
}

Mlt::Producer *PlaylistItem::getProducer() {
    return producer;
}

QString PlaylistItem::getName() {
    return name;
}

QString PlaylistItem::getIn() {
    return in;
}

QString PlaylistItem::getOut() {
    return out;
}
