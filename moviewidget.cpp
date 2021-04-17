#include "moviewidget.h"
#include <QtMultimedia/QMediaContent>
#include <QtMultimedia/QMediaPlayer>
#include <QStyle>
#include <QFileDialog>
#include <QInputDialog>

//******************************************************************************
// MovieWidget()
//******************************************************************************
MovieWidget::MovieWidget(QWidget *parent) : QWidget(parent)
{
    mini = true;
    this->isVideoAvailable=false;
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mediaPlayer = new QMediaPlayer(this, QMediaPlayer::VideoSurface);
    connect(mediaPlayer, SIGNAL(videoAvailableChanged(bool)), this, SLOT(videoAvailableChanged(bool)));

    videoWidget = new QVideoWidget();
    videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //**************************************************************************
    // Context Menu
    //**************************************************************************
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(openContextMenu(QPoint)));
    QList<QWidget *> windowChildren=this->findChildren<QWidget *>();

    foreach (QWidget *child, windowChildren)
    {
        child->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(child,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(openContextMenu(QPoint)));
    }
    contextMenu = new QMenu(videoWidget);

    openAction = new QAction(tr("Open file"), this);
    openAction->setIcon(QIcon(style()->standardIcon(QStyle::SP_DialogOpenButton)));
    contextMenu->addAction(openAction);

    urlAction = new QAction(tr("Open stream"), this);
    urlAction->setIcon(QIcon(style()->standardIcon(QStyle::SP_DirLinkIcon)));
    contextMenu->addAction(urlAction);

    playAction = new QAction(tr("Play/Pause"), this);
    playAction->setIcon(QIcon(style()->standardIcon(QStyle::SP_MediaPlay)));
    playAction->setEnabled(false);
    contextMenu->addAction(playAction);

    closeAction = new QAction(tr("Close"), this);
    closeAction->setIcon(QIcon(style()->standardIcon(QStyle::SP_DialogCloseButton)));
    closeAction->setEnabled(false);
    contextMenu->addAction(closeAction);

    posSlider = new QSlider(Qt::Horizontal);
    posSlider->setRange(0,0);
    connect(posSlider, &QSlider::sliderMoved, this, &MovieWidget::setPosition);
    wac = new QWidgetAction(contextMenu);
    wac->setDefaultWidget(posSlider);
    contextMenu->addAction(wac);

    lblFile = new QLabel();
    wal = new QWidgetAction(contextMenu);
    wal->setDefaultWidget(lblFile);
    contextMenu->addAction(wal);

    //**************************************************************************
    // Control Box
    //**************************************************************************
    openButton = new QPushButton();
    openButton->setIcon(QIcon(style()->standardIcon(QStyle::SP_DialogOpenButton)));
    connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));

    urlButton = new QPushButton();
    urlButton->setIcon(QIcon(style()->standardIcon(QStyle::SP_DirLinkIcon)));
    connect(urlButton, SIGNAL(clicked()), this, SLOT(openURL()));

    playButton = new QPushButton();
    playButton->setIcon(QIcon(style()->standardIcon(QStyle::SP_MediaPlay)));
    playButton->setEnabled(false);
    connect(playButton, SIGNAL(clicked()), this, SLOT(play()));

    positionSlider = new QSlider(Qt::Horizontal);
    positionSlider->setRange(0,0);
    connect(positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(setPosition(int)));

    errorLabel = new QLabel();
    errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    //**************************************************************************
    // Create layouts to place inside widget
    //**************************************************************************
    controlLayout = new QHBoxLayout();
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->addWidget(openButton);
    controlLayout->addWidget(urlButton);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(positionSlider);

    vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(videoWidget);
    if (mini != true) {
        vLayout->addLayout(controlLayout);
        vLayout->addWidget(errorLabel);
    }
    setLayout(vLayout);

    //**************************************************************************
    // Set widget to contain window contents
    //**************************************************************************
    mediaPlayer->setVideoOutput(videoWidget);
    connect(mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(mediaStateChanged(QMediaPlayer::State)));
    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    connect(mediaPlayer, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(handleError()));

    resize(180, 100);
}

//******************************************************************************
// videoAvailableChanged()
//******************************************************************************
void MovieWidget::videoAvailableChanged(bool available) {
    this->isVideoAvailable = available;
}

//******************************************************************************
// openContextMenu()
//******************************************************************************
void MovieWidget::openContextMenu(QPoint position) {
    QAction *action = contextMenu->exec(mapToGlobal(position));
    if (action == playAction) {
        play();
    } else if (action == openAction) {
        openFile();
    } else if (action == urlAction) {
        openURL();
    } else if (action == closeAction) {
        closeFile();
    }
}

//******************************************************************************
// restoreSession()
//******************************************************************************
void MovieWidget::restoreSession(QString mediaFile, int mediaType, qint64 mediaPosition, bool mediaRun) {
    switch (mediaType) {
        case 1: {
            currentMedia = mediaFile;
            file_1_url_2 = 1;
            QFileInfo fi(mediaFile);
            lblFile->setText(" " + fi.baseName());
            errorLabel->setText(fi.baseName());
            mediaPlayer->setMedia(QMediaContent(QUrl::fromLocalFile(QFile(mediaFile).fileName())));
            playButton->setEnabled(true);
            playAction->setEnabled(true);
            closeAction->setEnabled(true);
            setPosition(mediaPosition);
            if (mediaRun == true) {
                play();
            }
            break;
        }

        case 2: {
            lblFile->setText(" " + mediaFile);
            errorLabel->setText(mediaFile);
            currentMedia = mediaFile;
            file_1_url_2 = 2;
            mediaPlayer->setMedia(QMediaContent(QUrl(mediaFile)));
            playButton->setEnabled(true);
            playAction->setEnabled(true);
            closeAction->setEnabled(true);
            setPosition(mediaPosition);
            if (mediaRun == true) {
                play();
            }
            break;
        }
    }
}

//******************************************************************************
// openFile()
//******************************************************************************
void MovieWidget::openFile() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open media"), QDir::homePath(),tr("All Files (*);;Movies (*.avi *.mp4);;Music (*.mp3 *.ogg)"), nullptr, QFileDialog::DontUseNativeDialog);
    if (filename != "") {
        // QFile playFile(filename);
        // qDebug("%s", qUtf8Printable(playFile.fileName()));
        currentMedia = filename;
        file_1_url_2 = 1;
        QFileInfo fi(filename);
        qDebug("%s", qUtf8Printable(filename));
        qDebug("%s", qUtf8Printable(fi.baseName()));
        lblFile->setText(" " + fi.baseName());
        errorLabel->setText(fi.baseName());
        // errorLabel->setText("Hello");
        mediaPlayer->setMedia(QMediaContent(QUrl::fromLocalFile(QFile(filename).fileName())));
        // mediaPlayer->setMedia(QUrl::fromLocalFile("/home/jpl/Téléchargements/alc.avi"));
        // control->setMedia(QMediaContent(QUrl::fromLocalFile(file->fileName())), nullptr);
        playButton->setEnabled(true);
        playAction->setEnabled(true);
        closeAction->setEnabled(true);
        play();
    }
}

//******************************************************************************
// openURL()
//******************************************************************************
void MovieWidget::openURL() {
    bool ok;
    QString url = QInputDialog::getText(this, tr("Open Stream"), tr("URL of stream :"), QLineEdit::Normal, "", &ok);
    if (ok && !url.isEmpty()) {
        lblFile->setText(" " + url);
        errorLabel->setText(url);
        currentMedia = url;
        file_1_url_2 = 2;
        // mediaPlayer->setMedia(QMediaContent(QUrl("https://www.rmp-streaming.com/media/big-buck-bunny-360p.mp4")));
        mediaPlayer->setMedia(QMediaContent(QUrl(url)));
        playButton->setEnabled(true);
        playAction->setEnabled(true);
        closeAction->setEnabled(true);
        play();
    }
}

//******************************************************************************
// closeFile()
//******************************************************************************
void MovieWidget::closeFile() {
    pauseForce();
    mediaPlayer->setMedia(QMediaContent());
    playButton->setEnabled(false);
    playAction->setEnabled(false);
    closeAction->setEnabled(false);
    currentMedia = "";
    lblFile->setText("");
}

//******************************************************************************
// play()
//******************************************************************************
void MovieWidget::play() {
    if (mediaPlayer->state() == QMediaPlayer::PlayingState) {
        mediaPlayer->pause();
        isPlaying = false;
    } else {
        mediaPlayer->play();
        isPlaying = true;
    }
}

//******************************************************************************
// playForce()
//******************************************************************************
void MovieWidget::playForce() {
    mediaPlayer->play();
    isPlaying = true;
}

//******************************************************************************
// pauseForce()
//******************************************************************************
void MovieWidget::pauseForce() {
    mediaPlayer->pause();
    // isPlaying = false;
}

//******************************************************************************
// mediaStateChanged()
//******************************************************************************
void MovieWidget::mediaStateChanged(QMediaPlayer::State state) {
    if (mediaPlayer->state() == QMediaPlayer::PlayingState) {
        playButton->setIcon(QIcon(style()->standardIcon(QStyle::SP_MediaPause)));
        playAction->setIcon(QIcon(style()->standardIcon(QStyle::SP_MediaPause)));
    } else {
        playButton->setIcon(QIcon(style()->standardIcon(QStyle::SP_MediaPlay)));
        playAction->setIcon(QIcon(style()->standardIcon(QStyle::SP_MediaPlay)));
    }
}

//******************************************************************************
// positionChanged()
//******************************************************************************
void MovieWidget::positionChanged(qint64 position) {
    posSlider->setValue(position);
    positionSlider->setValue(position);
}

//******************************************************************************
// durationChanged()
//******************************************************************************
void MovieWidget::durationChanged(qint64 duration) {
    posSlider->setRange(0, duration);
    positionSlider->setRange(0, duration);
}

//******************************************************************************
// setPosition()
//******************************************************************************
void MovieWidget::setPosition(int position) {
    mediaPlayer->setPosition(position);
}

//******************************************************************************
// handleError()
//******************************************************************************
void MovieWidget::handleError() {
    playButton->setEnabled(false);
    errorLabel->setText(mediaPlayer->errorString());
}
