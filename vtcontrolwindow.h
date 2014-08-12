#ifndef VTCONTROLWINDOW_H
#define VTCONTROLWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <mlt++/Mlt.h>
#include "playlistmodel.h"
#include "playlistitem.h"

namespace Ui {
class VTControlWindow;
}

class VTControlWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VTControlWindow(QWidget *parent = 0);
    void PlayItem(Mlt::Producer &producer);
    void QueueItem(PlaylistItem item);
    Mlt::Profile *activeProfile();
    ~VTControlWindow();

private slots:
    void updateSeekBar();

    //User UI actions
    void on_actionAddMedia_triggered();
    void on_stopButton_clicked();
    void on_playButton_clicked();

private:
    void createConsumers();
    void createPlaylistTable();
    void setupPlaylist();
    void reset();

    Ui::VTControlWindow *ui;

    //Handle the mediabin layout
    QVBoxLayout *playlistLayout;
    QWidget* techArea;

    QTimer *timer;

    //Media fields
    bool isPaused;
    Mlt::Profile   *profile;
    Mlt::Consumer  *consumer;
    Mlt::Producer  *producer;

    //playlist
    PlayListModel *model;
};

#endif // VTCONTROLWINDOW_H
