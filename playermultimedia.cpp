 #include "playermultimedia.h"
#include <QPushButton>
#include <QStyle>
#include <QBoxLayout>
#include <QMediaPlaylist>
#include <QDir>
PlayerMultimedia::PlayerMultimedia(const QString &path,QWidget *parent) :
    QWidget(parent)
  , playButton(0)
  , positionSlider(0)
   ,m_appPath(path)  ,

      mediaPlayer(0)
{

playlist = new QMediaPlaylist;
mediaPlayer=new QMediaPlayer;
   createThumbnailToolBar();
mediaPlayer->setPlaylist(playlist);
    playButton = new QToolButton;
//    playButton->setEnabled(false);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(playButton, SIGNAL(clicked()),
            this, SLOT(playPause()));

    positionSlider = new QSlider(Qt::Horizontal);
    positionSlider->setRange(0, 0);

    connect(positionSlider, SIGNAL(sliderMoved(int)),
            this, SLOT(setPosition(int)));

//    errorLabel = new QLabel;
//    errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);

    controlLayout->addWidget(playButton);
    controlLayout->addWidget(positionSlider);

//    QBoxLayout *layout = new QVBoxLayout;

//    layout->addLayout(controlLayout);
//    layout->addWidget(errorLabel);

    setLayout(controlLayout);


    connect(mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(mediaStateChanged(QMediaPlayer::State)));
    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    connect(mediaPlayer, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(handleError()));
    connect(mediaPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));
    connect(playlist, SIGNAL(currentIndexChanged(int)), SLOT(playlistPositionChanged(int)));

    //البحث عن اي ملف   m_MediaObject.clear();استعاذة باي ملحقة كانت
    QStringList filters("audhubillah.*");
    QDir dir(m_appPath);
    QStringList list=dir.entryList(filters,QDir::Files,QDir::Name);
    QString aodho ;
    if (list.count()>0)
        aodho = list.at(0);
 playOne=false;
 isPlay=false;
//    playlist->addMedia(QUrl::fromLocalFile(m_appPath+"/"+aodho));

//    mediaPlayer-> play();


//#if defined(Q_OS_WIN)
//                m_besmala=m_appPath+"/bismillah.mp3";
//#else
                m_besmala=m_appPath+"/bismillah.ogg"   ;
//#endif

           //    createTaskbar();

}


   PlayerMultimedia::~PlayerMultimedia()
   {
deleteLater();
   }

void PlayerMultimedia::play(QMediaContent url,bool besmala)
{



    if(url==mediaPlayer->currentMedia())
        return;
   if(playOne)return;

       playlist->clear();

    if(besmala){
       playOne=true;
       mediaPlayer->stop();
       playlist->addMedia(QUrl::fromLocalFile(m_besmala));
       playlist->addMedia(url);
       playlist->setCurrentIndex(0);
    }else{
         mediaPlayer->stop();
       playlist->addMedia(url);
       playlist->setCurrentIndex(0);
    }



   if(isPlay)
    mediaPlayer->play();

}

void PlayerMultimedia::playPause()
{
   // playOne=false;
    switch(mediaPlayer->state()) {
    case QMediaPlayer::PlayingState:
 isPlay=false;
        mediaPlayer->pause();
        break;


    default:
         isPlay=true;
        mediaPlayer->play();

        break;
    }
}

void PlayerMultimedia::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        //  isPlay=true;
        break;
    default:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
       // isPlay=false;
        break;
    }
}

void PlayerMultimedia::positionChanged(qint64 position)
{
    positionSlider->setValue(position);


}

void PlayerMultimedia::durationChanged(qint64 duration)
{
    positionSlider->setRange(0, duration);
}

void PlayerMultimedia::setPosition(int position)
{
    mediaPlayer->setPosition(position);
}

void PlayerMultimedia::handleError()
{
//    playButton->setEnabled(false);
qDebug()<<"Error: " << mediaPlayer->errorString();
}
void PlayerMultimedia::statusChanged(QMediaPlayer::MediaStatus status)
{

    bool isvalid=true;
    bool finoshed=false;
    // handle status message
    switch (status) {
    case QMediaPlayer::UnknownMediaStatus:
        qDebug()<<"QMediaPlayer::statusChanged"<<"UnknownMediaStatus";
    case QMediaPlayer::NoMedia:
        qDebug()<<"QMediaPlayer::statusChanged"<<"NoMedia";
    case QMediaPlayer::LoadedMedia:
        qDebug()<<"QMediaPlayer::statusChanged"<<"LoadedMedia";
    case QMediaPlayer::BufferingMedia:
        qDebug()<<"QMediaPlayer::statusChanged"<<"BufferingMedia";
    case QMediaPlayer::BufferedMedia:
        qDebug()<<"QMediaPlayer::statusChanged"<<"BufferedMedia";
        //        setStatusInfo(QString());
        break;
    case QMediaPlayer::LoadingMedia:
        //        setStatusInfo(tr("Loading..."));
        break;
    case QMediaPlayer::StalledMedia:
        qDebug()<<"QMediaPlayer::statusChanged"<<"StalledMedia";
        break;
    case QMediaPlayer::EndOfMedia:
        finoshed=true;

        break;
    case QMediaPlayer::InvalidMedia:
        isvalid=true;
        qDebug()<<"QMediaPlayer::statusChanged"<<"InvalidMedia";
        mediaPlayer->stop();

        //  displayErrorMessage();
        break;
    }

    if(isvalid && finoshed){

        if(playOne)
            playOne=false;
        else
            emit finished();
    }


}
void PlayerMultimedia::clearList()
{
    isPlay=false;mediaPlayer->stop();playlist->clear();
}
void PlayerMultimedia::playlistPositionChanged(int)
{
    if(playOne){
//       playOne=false;
////        mediaPlayer->pause();
        return;
   }

//    if(playlist->currentIndex()==-1)
//        emit finished();
}

void PlayerMultimedia::createTaskbar()
{
//    taskbarButton = new QWinTaskbarButton(this);
//    taskbarButton->setWindow(windowHandle());

//    taskbarProgress = taskbarButton->progress();
//    connect(positionSlider, SIGNAL(valueChanged(int)), taskbarProgress, SLOT(setValue(int)));
//    connect(positionSlider, SIGNAL(rangeChanged(int, int)), taskbarProgress, SLOT(setRange(int, int)));

//    connect(mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(updateTaskbar()));
}
void PlayerMultimedia::createThumbnailToolBar()
{
//    thumbnailToolBar = new QWinThumbnailToolBar(this);
//    thumbnailToolBar->setWindow(windowHandle());

//    playToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
//    playToolButton->setEnabled(true);
//    playToolButton->setToolTip(tr("Play"));
//    playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
//  //connect(playToolButton, SIGNAL(clicked()), this, SLOT(playPause()));

//    forwardToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
//    forwardToolButton->setEnabled(true);
//    forwardToolButton->setToolTip(tr("Fast forward"));
//    forwardToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
//   // connect(forwardToolButton, SIGNAL(clicked()), this, SLOT(seekForward()));

//    backwardToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
//    backwardToolButton->setEnabled(true);
//    backwardToolButton->setToolTip(tr("Rewind"));
//    backwardToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
//    //connect(backwardToolButton, SIGNAL(clicked()), this, SLOT(seekBackward()));

//    thumbnailToolBar->addButton(backwardToolButton);
//    thumbnailToolBar->addButton(playToolButton);
//    thumbnailToolBar->addButton(forwardToolButton);

//    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(updateThumbnailToolBar()));
//    connect(mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(updateThumbnailToolBar()));
//    connect(mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(updateThumbnailToolBar()));
}

void PlayerMultimedia::updateThumbnailToolBar()
{

//    playToolButton->setEnabled(mediaPlayer->duration() > 0);
//    backwardToolButton->setEnabled(mediaPlayer->position() > 0);
//    forwardToolButton->setEnabled(mediaPlayer->position() < mediaPlayer->duration());

//    if (mediaPlayer->state() == QMediaPlayer::PlayingState) {
//        playToolButton->setToolTip(tr("Pause"));
//        playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
//    } else {
//        playToolButton->setToolTip(tr("Play"));
//        playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
//    }

}