#include "settings.h"

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

const int Settings::LAUNCHER_VIEW_ICON=0;
const int Settings::LAUNCHER_VIEW_LIST=1;
const int Settings::LAUNCHER_ICON_SIZE_SMALL=0;
const int Settings::LAUNCHER_ICON_SIZE_MEDIUM=1;
const int Settings::LAUNCHER_ICON_SIZE_LARGE=2;

//******************************************************************************
// Settings()
//******************************************************************************
Settings::Settings()
{
    // Set  the defaults values...
    defaults["OSD_FONT_SIZE"] = QVariant(16);
    defaults["OSD_COLOR"] = QVariant(QObject::tr("yellow"));
    defaults["OSD_SHADOW_COLOR"] = QVariant(QObject::tr("black"));
    defaults["OSD_TIMEOUT"] = QVariant(3000);
    defaults["MESSAGE_TIMEOUT"] = QVariant(3000);
    defaults["PLAYER_VIDEO_SMART_PAUSE"] = QVariant(true);
    defaults["PLAYER_AUDIO_SMART_PAUSE"] = QVariant(false);
    defaults["TITLE_BAR_TIME_FORMAT"] = QVariant(QObject::tr(" hh:mm:ss - dd/MM/yyyy"));
    defaults["TITLE_BAR_POSITION_TOP"] = QVariant(true);
    defaults["LAUNCHER_ICON_SIZE"] = QVariant(Settings::LAUNCHER_ICON_SIZE_MEDIUM);
    defaults["LAUNCHER_VIEW"] = QVariant(Settings::LAUNCHER_VIEW_ICON);
    defaults["PROMPT_WINDOW_MODE"] = QVariant(QObject::tr("> "));
    defaults["PROMPT_BAR_MODE"] = QVariant(QObject::tr("> "));
    defaults["CONSOLE_TEXT_COLOR"] = QVariant(QObject::tr("green"));
    defaults["CONSOLE_BACKGROUND_COLOR"] = QVariant(QObject::tr("black"));
    defaults["CONSOLE_FONT_SIZE"] = QVariant(16);
    defaults["CONSOLE_HISTORY"] = QVariant(1000);

    // Read the settings from user's settings
    read();

    // Check if all defaults settings are set and set them if any
    foreach (const auto& defaultKey, defaults.keys()) {
        if (!settings.contains(defaultKey)) {
            settings[defaultKey] = defaults[defaultKey];
        }
    }

    // Delete all keys that are no more in the defaults settings values
    foreach (const auto& settingKey, settings.keys()) {
        if (!defaults.contains(settingKey)) {
            settings.erase(settings.find(settingKey));
        }
    }

    // Save the settings to user's settings
    write();
}

//******************************************************************************
// get()
//******************************************************************************
QVariant Settings::get(QString param) {
    return this->settings[param];
}

//******************************************************************************
// write()
//******************************************************************************
void Settings::write() {
    // TODO : Apply new settings in real time
    Constants *appConstants = new Constants();
    QDir appDir = QDir(QDir::homePath()).filePath(appConstants->getQString("APP_FOLDER"));
    QFile fSettings(QDir(appDir).filePath(appConstants->getQString("SETTINGS_FILE")));
    if (fSettings.open(QIODevice::WriteOnly)) {
        QDataStream out(&fSettings);
        out.setVersion(QDataStream::Qt_5_3);
        out << settings;
    }
}

//******************************************************************************
// read()
//******************************************************************************
void Settings::read() {
    Constants *appConstants = new Constants();
    QDir appDir = QDir(QDir::homePath()).filePath(appConstants->getQString("APP_FOLDER"));
    QFile fSettings(QDir(appDir).filePath(appConstants->getQString("SETTINGS_FILE")));
    if (fSettings.open(QIODevice::ReadOnly)) {
        QDataStream in(&fSettings);
        in.setVersion(QDataStream::Qt_5_3);
        in >> settings;
    }
}

//******************************************************************************
// form()
//******************************************************************************
void Settings::form(QWidget *w) {
    QFormLayout *form = new QFormLayout(w);
    form->setLabelAlignment(Qt::AlignRight);

    form->addRow(new QLabel("<b>ᐅ</b>"), new QLabel("<b>VOSTOK'S SETTINGS</b>"));
    form->addRow(new QLabel(""), new QLabel(""));

    for(auto e : settings.keys())
    {
        QLabel *lblSetting = new QLabel(e);
        QLineEdit *txtSetting = new QLineEdit(settings.value(e).toString());
        connect(txtSetting, &QLineEdit::textChanged, [=]{handleTextChanged(lblSetting, txtSetting);});
        form->addRow(lblSetting, txtSetting);
    }

    form->addRow(new QLabel(""), new QLabel(""));
    form->addRow(new QLabel("<b>ᐅ</b>"), new QLabel("<b>RPN HELP</b>"));
    form->addRow(new QLabel(""), new QLabel(""));

    RPN *rpn = new RPN(nullptr);
    for(auto e : rpn->help.keys())
    {
        QLabel *lbl = new QLabel(" " + e + " ");
        // QLabel { color: black; font: 24pt  'Oswald'; }
        lbl->setStyleSheet("QLabel { font: 10pt 'monospace'; background-color : lightgrey; }");
        QLabel *txtHelp = new QLabel(rpn->help.value(e));
        form->addRow(lbl, txtHelp);
    }

    w->setLayout(form);
}

//******************************************************************************
// handleTextChanged()
//******************************************************************************
void Settings::handleTextChanged(QLabel *lbl, QLineEdit *txt) {
    // qDebug(lbl->text().toLatin1());
    settings[lbl->text().toLatin1()] = QVariant(txt->text().toLatin1());
    write();
}
