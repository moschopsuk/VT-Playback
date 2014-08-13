#include "vtcontrolwindow.h"
#include "ui_vtcontrolwindow.h"
#include "mediaitemwidget.h"
#include "playlistmodel.h"
#include "playlistitem.h"
#include <mlt++/Mlt.h>
#include <QDebug>
#include <QtGui>

VTControlWindow::VTControlWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VTControlWindow) {
    ui->setupUi(this);

    //Create
    profile = new Mlt::Profile("hdv_720_50p");
    consumer = new Mlt::Consumer(*profile, "multi");
    //Bug fix black screen on decklink pause
    consumer->set("terminate_on_pause", 0);
    consumer->set("rescale", "bilinear");

    createConsumers();
    createPlaylistTable();

    //Create internal timmer for
    //tracking video progress
    timer = new QTimer();
    timer->start(200);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateSeekBar()));

    setupPlaylist();
    reset();
}

VTControlWindow::~VTControlWindow() {
    delete ui;
    reset();
    Mlt::Factory::close();
}

Mlt::Profile* VTControlWindow::activeProfile() {
    return profile;
}

void VTControlWindow::PlayItem(Mlt::Producer &p) {
    producer->seek(0);
    this->producer = &p;

    reset();

    if(producer->is_valid()) {
        ui->totaltimeLabel->setText(QString(producer->get_length_time()));
        consumer->connect(*producer);
        consumer->start();
        isPaused = false;
    }

    if(producer->get_speed() == 0) {
        play();
    }
}

void VTControlWindow::createConsumers() {
    //create the decklink consumer
    consumer->set("0", "decklink:0");

    //create the sdl_preview window
    consumer->set("1", "sdl_preview");
    consumer->set("1.real_time", "0");
    consumer->set("1.resize", "1");
    consumer->set("1.window_id", (int) ui->videoWidget->winId());
    ui->videoWidget->setStyleSheet("background-color:black;");

    //Create a dummy producer and connect the two
    //to avoid any errors
    producer = new Mlt::Producer(*profile, "color:red");
    consumer->connect(*producer);
    producer->set_speed(0.0);
}

void VTControlWindow::setupPlaylist() {
    //Use palettes to change background to white
    //Using stylesheets will reset everything
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    ui->playlistScrollArea->setAutoFillBackground(true);
    ui->playlistScrollArea->setPalette(Pal);

    //create a child widget to hold the
    //items in the scroll area
    techArea = new QWidget(this);
    techArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    //create an layout with the aligmnet at the top.
    QVBoxLayout *layout = new QVBoxLayout(techArea);
    layout->setAlignment(Qt::AlignTop);

    techArea->setLayout(layout);
    ui->playlistScrollArea->setWidget(techArea);
}

void VTControlWindow::on_actionAddMedia_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this);

    if(fileName.isNull()) {
        return;
    }

    QLayout *lay = techArea->layout();
    MediaItemWidget *widget = new MediaItemWidget(this, fileName, techArea);
    lay->addWidget(widget);
}

void VTControlWindow::createPlaylistTable() {
    model = new PlayListModel(this);

    ui->playlistTable->setModel(model);
    ui->playlistTable->verticalHeader()->setVisible(false);
    ui->playlistTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->playlistTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->playlistTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void VTControlWindow::QueueItem(PlaylistItem item) {
    model->addItem(item);
}

void VTControlWindow::updateSeekBar() {
    if(!consumer->is_stopped()){
        int sliderValue = 1.0 * ((int)producer->position()) / producer->get_length() * ui->seekbar->maximum();
        ui->seekbar->setValue(sliderValue);

        ui->currentTimeLabel->setText(QString(producer->frames_to_time(producer->position())));

        int remaining = producer->get_length() - producer->position();
        ui->remaingTimeLabel->setText(QString(producer->frames_to_time(remaining)));

        //qDebug() << producer->position() << "#" << producer->get_length();

        if(producer->position() >= (producer->get_length() - 1)) {
            reset();
            emit vtComplete();
        }
    }
}

void VTControlWindow::on_stopButton_clicked() {
    reset();
}

void VTControlWindow::reset() {
    QString nullTimeStamp = QString("00:00:00:00");
    ui->currentTimeLabel->setText(nullTimeStamp);
    ui->remaingTimeLabel->setText(nullTimeStamp);
    ui->totaltimeLabel->setText(nullTimeStamp);
    ui->seekbar->setValue(0);

    if(!consumer->is_stopped()) {
        consumer->stop();
    }

    isPaused = true;
}

void VTControlWindow::on_playButton_clicked() {
    if(producer->get_speed() == 0) {
        play();
    } else {
        pause();
    }
}

void VTControlWindow::play() {
    isPaused = false;
    ui->playButton->setText("Pause");

    producer->set_speed(1.0);
    producer->set("1.refresh", 1);
}

void VTControlWindow::pause() {
    if (producer && producer->get_speed() != 0) {
        producer->set_speed(0);

        if (consumer->is_valid()) {
            consumer->purge();
            producer->seek(consumer->position() + 1);
        }
    }

    isPaused = true;
    ui->playButton->setText("Play");
}

void VTControlWindow::on_clearButton_clicked() {
    model->reset();
    listIndex = 0;
}

/*
 * Begin Playlist Methods
 */
void VTControlWindow::on_StartPlaylistButton_clicked() {
    //clear onstage
    reset();

    //Connect the two.
    connect(this, SIGNAL(vtComplete()), this, SLOT(playlistNext()));

    startPlaylist();
}

void VTControlWindow::startPlaylist() {
    listIndex = 0;
    playoutVT(0);
}

void VTControlWindow::playlistNext() {
    listIndex++;
    playoutVT(listIndex);
}

void VTControlWindow::playoutVT(int index) {
    QList<PlaylistItem> playlist = model->getModelList();

    qDebug() << listIndex << playlist.count();

    //We are now past the playlist
    if(playlist.count() <= listIndex) {

        if(ui->loopPlaylist->isChecked()) {
            startPlaylist();
            return;
        }

        //unlink the reciver;
        disconnect(this, SLOT(playlistNext()));
        ui->playlistTable->selectRow(0);
        reset();
        return;
    }

    ui->playlistTable->selectRow(index);
    PlaylistItem item = playlist.at(index);
    item.getProducer()->seek(0);
    PlayItem(*item.getProducer());
}
