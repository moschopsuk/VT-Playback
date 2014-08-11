#include "mediaitemwidget.h"
#include "ui_mediaitemwidget.h"
#include "vtcontrolwindow.h"
#include <mlt++/Mlt.h>

MediaItemWidget::MediaItemWidget(VTControlWindow *player, QString fileName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MediaItemWidget)
{
    ui->setupUi(this);
    ui->holderWidget->setStyleSheet("QWidget#holderWidget { border: 1px solid black; background-color:#ddd }");
    ui->nameLabel->setText(fileName.section("/",-1,-1));

    this->player = player;
    producer = new Mlt::Producer(*player->activeProfile(), fileName.toUtf8().constData());

    ui->labelTotalTime->setText(producer->get_length_time());
    ui->codecLabel->setText(producer->get("meta.media.0.codec.name"));

    //generate thumbnail
    Mlt::Frame *frame = producer->get_frame(( producer->get_length() /2 ));
    ui->pictureLabel->setPixmap(QPixmap::fromImage(frameToImage(frame, 200, 150)));
    delete frame;
}

MediaItemWidget::~MediaItemWidget()
{
    delete ui;
}

void MediaItemWidget::on_quickPlayButton_clicked()
{
    player->PlayItem(*producer);
}

QImage MediaItemWidget::frameToImage(Mlt::Frame *frame, int width, int height) {
    //create holder graphic
    QImage result(width, height, QImage::Format_ARGB32_Premultiplied);

    if (frame && frame->is_valid()) {
        if (width > 0 && height > 0) {
            frame->set("rescale.interp", "bilinear");
            frame->set("deinterlace_method", "onefield");
            frame->set("top_field_first", -1);
        }

        mlt_image_format format = mlt_image_rgb24a;
        const uchar *image = frame->get_image(format, width, height);

        if (image) {
            QImage temp(width, height, QImage::Format_ARGB32_Premultiplied);
            memcpy(temp.scanLine(0), image, width * height * 4);
            result = temp.rgbSwapped();
        }
    } else {
        //Should it not exist we return red
        result.fill(QColor(Qt::red).rgb());
    }
    return result;
}
