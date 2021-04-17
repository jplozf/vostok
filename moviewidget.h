#ifndef MOVIEWIDGET_H
#define MOVIEWIDGET_H

#include <QWidget>
#include <QtMultimedia/QMediaContent>
#include <QtMultimedia/QMediaPlayer>
#include <QVideoWidget>
#include <QMenu>
#include <QStyle>
#include <QSlider>
#include <QWidgetAction>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>

class MovieWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MovieWidget(QWidget *parent = nullptr);    
    QMediaPlayer *mediaPlayer;
    void playForce();
    void pauseForce();
    void restoreSession(QString, int, qint64, bool);
    bool isPlaying = false;
    int file_1_url_2 = 0;
    QString currentMedia;
    qint64 currentPosition;
    bool isVideoAvailable = false;

private:
    bool mini;
    QVideoWidget *videoWidget;
    QMenu *contextMenu;
    QAction *openAction;
    QAction *urlAction;
    QAction *playAction;
    QAction *closeAction;
    QSlider *posSlider;
    QWidgetAction *wac;
    QWidgetAction *wal;
    QLabel *lblFile;
    QAction *posAction;
    QPushButton *openButton;
    QPushButton *urlButton;
    QPushButton *playButton;
    QLabel *errorLabel;
    QSlider *positionSlider;
    QHBoxLayout *controlLayout;
    QVBoxLayout *vLayout;
    void closeFile();

public slots:
    void openContextMenu(QPoint);

private slots:
    void openFile();
    void openURL();
    void play();
    void mediaStateChanged(QMediaPlayer::State);
    void positionChanged(qint64);
    void durationChanged(qint64);
    void setPosition(int);
    void handleError();
    void videoAvailableChanged(bool);


signals:

};

#endif // MOVIEWIDGET_H
