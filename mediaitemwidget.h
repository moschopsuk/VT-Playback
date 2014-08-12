#ifndef MEDIAITEMWIDGET_H
#define MEDIAITEMWIDGET_H

#include <QWidget>
#include <mlt++/Mlt.h>
#include "vtcontrolwindow.h"

namespace Ui {
class MediaItemWidget;
}

class MediaItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MediaItemWidget(VTControlWindow *player, QString fileName, QWidget *parent = 0);
    ~MediaItemWidget();

private slots:
    void on_quickPlayButton_clicked();

    void on_queueButton_clicked();

private:
    QImage frameToImage(Mlt::Frame *frame, int w, int h);
    Ui::MediaItemWidget *ui;
    Mlt::Producer  *producer;
    VTControlWindow *player;
    QString mediaName;
};

#endif // MEDIAITEMWIDGET_H
