#include "settings.h"

Settings::Settings(QWidget *parent) : QWidget(parent)
{
    mainGrid = new QGridLayout(this);

    cbDark = new QCheckBox(this);
    cbLanguage = new QCheckBox(this);

    lbDark = new QLabel(this);
    lbLanguage = new QLabel(this);

    lbOpen = new QLabel(this);
    leOpen = new QLineEdit(this);
    lbSave = new QLabel(this);
    leSave = new QLineEdit(this);
    lbClear = new QLabel(this);
    leClear = new QLineEdit(this);
    lbExit = new QLabel(this);
    leExit = new QLineEdit(this);

    qss =   "QWidget{""background-color: rgb(239, 239, 239);""}"
                           "QTextEdit{""background-color: rgb(255, 255, 255);""border-color: rgb(0, 179, 255);""border: 5px solid rgb(0, 179, 255);""}"
                           "QPushButton{""border-radius: 00px;""background-color: rgb(239, 239, 239);""qproperty-iconSize: 28px 28px;""}"
                           "QPushButton:pressed{"" border-radius: 10px;""background-color: rgb(0, 179, 255);""}"
                           "QToolButton{""border-radius: 00px;""background-color: rgb(239, 239, 239);""qproperty-iconSize: 28px 28px;""}"
                           "QToolButton:pressed{"" border-radius: 10px;""background-color: rgb(0, 179, 255);""}";


    qssDark = "QWidget{""background-color: rgb(53, 53, 53);""text-color: rgb(255, 255, 255);""}"
                              "QTextEdit{""background-color: rgb(25, 25, 25);""border-color: rgb(0, 179, 255);"
                                  "             ""border: 5px solid rgb(0, 179, 255);""text-color: rgb(255, 255, 255);""}"
                              "QPushButton{""border-radius: 00px;""background-color: rgb(53, 53, 53);""qproperty-iconSize: 28px 28px;""}"
                              "QPushButton:pressed{"" border-radius: 10px;""background-color: rgb(0, 179, 255);""}"
                              "QToolButton{""border-radius: 00px;""background-color: rgb(53, 53, 53);""qproperty-iconSize: 28px 28px;""}"
                              "QToolButton:pressed{"" border-radius: 10px;""background-color: rgb(0, 179, 255);""}";


    // Настраиваем палитру для цветовых ролей элементов интерфейса
//    Active и Normal — компонент активен (окно находится в фокусе ввода);
//    Disabled — компонент недоступен;
//    Inactive — компонент неактивен (окно находится вне фокуса ввода).
//    darkPalette.setColor(QPalette::Normal, QPalette::Window, QColor(53, 53, 53)); //без указания Normal применяется во всех трех режимах
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    palette = style()->standardPalette();

    leOpen->setText("Ctrl+O");
    leOpen->setReadOnly(true);
    leSave->setText("Ctrl+S");
    leSave->setReadOnly(true);
    leClear->setText("Ctrl+N");
    leClear->setReadOnly(true);
    leExit->setText("Ctrl+Q");
    leExit->setReadOnly(true);

    connect(cbDark, SIGNAL(clicked()), this, SLOT(on_cbDark_clicked()));
    connect(cbLanguage, SIGNAL(clicked()), this, SLOT(on_cbLanguage_clicked()));
    connect(leOpen,SIGNAL(selectionChanged()),this,SLOT(leOpen_onTextEdit()));
    connect(leSave,SIGNAL(selectionChanged()),this,SLOT(leSave_onTextEdit()));
    connect(leClear,SIGNAL(selectionChanged()),this,SLOT(leClear_onTextEdit()));
    connect(leExit,SIGNAL(selectionChanged()),this,SLOT(leExit_onTextEdit()));

    mainGrid->addWidget(cbDark, 0, 1);
    mainGrid->addWidget(cbLanguage, 1, 1);
    mainGrid->addWidget(lbDark, 0, 0);
    mainGrid->addWidget(lbLanguage, 1, 0);
    mainGrid->addWidget(lbOpen, 2, 0);
    mainGrid->addWidget(leOpen, 2, 1);
    mainGrid->addWidget(lbSave, 3, 0);
    mainGrid->addWidget(leSave, 3, 1);
    mainGrid->addWidget(lbClear, 4, 0);
    mainGrid->addWidget(leClear, 4, 1);
    mainGrid->addWidget(lbExit, 5, 0);
    mainGrid->addWidget(leExit, 5, 1);

    cbLanguage->setChecked(true); //старт с русского языка
    cbDark->setChecked(true);  //старт в темной теме

    qApp->setPalette(getPalette());
    this->setStyleSheet(getQss());
    if(cbLanguage->isChecked()) on_cbLanguage_clicked();

    this->setLayout(mainGrid);

    open = Qt::Key_O;
    modifyOpen = Qt::ControlModifier;
    save = Qt::Key_S;
    modifySave = Qt::ControlModifier;
    clear = Qt::Key_N;
    modifyClear = Qt::ControlModifier;
    exit = Qt::Key_Q;
    modifyExit = Qt::ControlModifier;

    retranslateUI();
    redrawUI(qss, palette);
}

QString Settings::getQss()
{
    if (cbDark->isChecked()) return qssDark;
    else return qss;
}

QPalette Settings::getPalette()
{
    if (cbDark->isChecked()) return darkPalette;
    else return style()->standardPalette();
}

QVector<unsigned int> Settings::getHotkey()
{
    QVector<unsigned int> hotkeys;
    hotkeys.append(modifyOpen);
    hotkeys.append(static_cast<unsigned int>(open));
    hotkeys.append(modifySave);
    hotkeys.append(static_cast<unsigned int>(save));
    hotkeys.append(modifyClear);
    hotkeys.append(static_cast<unsigned int>(clear));
    hotkeys.append(modifyExit);
    hotkeys.append(static_cast<unsigned int>(exit));
    return hotkeys;
}

QString Settings::getNameKey(int _key)
{
  switch (_key) {
    case Qt::ControlModifier: return "Ctrl";
    case Qt::Key_F1: return "F1";
    case Qt::Key_F2: return "F2";
    case Qt::Key_F3: return "F3";
    case Qt::Key_F4: return "F4";
    case Qt::Key_F5: return "F5";
    case Qt::Key_F6: return "F6";
    case Qt::Key_F7: return "F7";
    case Qt::Key_F8: return "F8";
    case Qt::Key_F9: return "F9";
    case Qt::Key_F10: return "F10";
    case Qt::Key_F11: return "F11";
    case Qt::Key_F12: return "F12";
    case Qt::Key_0: return "0";
    case Qt::Key_1: return "1";
    case Qt::Key_2: return "2";
    case Qt::Key_3: return "3";
    case Qt::Key_4: return "4";
    case Qt::Key_5: return "5";
    case Qt::Key_6: return "6";
    case Qt::Key_7: return "7";
    case Qt::Key_8: return "8";
    case Qt::Key_9: return "9";
    case Qt::Key_A: return "A";
    case Qt::Key_B: return "B";
    case Qt::Key_C: return "C";
    case Qt::Key_D: return "D";
    case Qt::Key_E: return "E";
    case Qt::Key_F: return "F";
    case Qt::Key_G: return "G";
    case Qt::Key_H: return "H";
    case Qt::Key_I: return "I";
    case Qt::Key_J: return "J";
    case Qt::Key_K: return "K";
    case Qt::Key_L: return "L";
    case Qt::Key_M: return "M";
    case Qt::Key_N: return "N";
    case Qt::Key_O: return "O";
    case Qt::Key_P: return "P";
    case Qt::Key_Q: return "Q";
    case Qt::Key_R: return "R";
    case Qt::Key_S: return "S";
    case Qt::Key_T: return "T";
    case Qt::Key_U: return "U";
    case Qt::Key_V: return "V";
    case Qt::Key_W: return "W";
    case Qt::Key_X: return "X";
    case Qt::Key_Y: return "Y";
    case Qt::Key_Z: return "Z";
    default: return "";
    }
}

void Settings::keyPressEvent(QKeyEvent *event)
{
    modify = event->modifiers();
    key = event->key();
    if(changeOpen){ 
        if(modify!= 0 && modifyOpen == 0) {
            leOpen->setText(getNameKey(static_cast<int>(modify))+ "+");
            modifyOpen = modify;
        }
        else {
            open = key;
            changeOpen = false;
            leOpen->setStyleSheet("");
            qDebug() << "Испустили сигнал открыть" << modifyOpen << "___" << open << " от кнопки " << key << " с модификатором " << modify;
            emit signal_changeOpen(modifyOpen,open);
            leOpen->setText((modifyOpen!= 0)?(leOpen->text() + getNameKey(key)):(getNameKey(key)));
        }    
    }


    else if(changeSave){
        if(modify!= 0 && modifySave == 0) {
            modifySave = modify;
            leSave->setText(getNameKey(static_cast<int>(modifySave))+ "+");
        }
        else {
            save = key;
            changeSave = false;
            leSave->setStyleSheet("");
            qDebug() << "Испустили сигнал сохранить" << modifySave << "___" << save << " от кнопки " << key << " с модификатором " << modify;
            emit signal_changeSave(modifySave,save);
            leSave->setText((modifySave!= 0)?(leSave->text() + getNameKey(key)):(getNameKey(key)));
        }
    }


    else if(changeClear){
        if(modify!= 0 && modifyClear == 0) {
            modifyClear = modify;
            leClear->setText(getNameKey(static_cast<int>(modifyClear))+ "+");
        }
        else {
             clear = key;
            changeClear = false;
            leClear->setStyleSheet("");
            qDebug() << "Испустили сигнал очистить" << modifyClear << "___" << clear << " от кнопки " << key << " с модификатором " << modify;
            emit signal_changeClear(modifyClear,clear);
            leClear->setText((modifyClear!= 0)?(leClear->text() + getNameKey(key)):(getNameKey(key)));
        }
    }


    else if(changeExit){
        if(modify!= 0 && modifyExit == 0) {
            modifyExit = modify;
            leExit->setText(getNameKey(static_cast<int>(modifyExit))+ "+");
        }
        else {
            exit = key;
            changeExit = false;
            leExit->setStyleSheet("");
            qDebug() << "Испустили сигнал выход" << modifyExit << "___" << exit << " от кнопки " << key << " с модификатором " << modify;
            emit signal_changeExit(modifyExit,exit);
            leExit->setText((modifyExit!= 0)?(leExit->text() + getNameKey(key)):(getNameKey(key)));
        }
    }
}

void Settings::on_cbDark_clicked()
{
    if (cbDark->isChecked()){
        // Устанавливаем данную палитру на приложение
        qApp->setPalette(darkPalette);
        emit redraw(qssDark, darkPalette);
    }
    else{
        // Для возврата к светлой палитре достаточно
        // будет установить стандартную палитру из темы оформления
         qApp->setPalette(style()->standardPalette());
         emit redraw(qss, palette);
    }
}

void Settings::on_cbLanguage_clicked() {
    if (cbLanguage->isChecked()){//русский
        translator = new QTranslator(qApp);
        static_cast<void>(translator->load(":/translator/RU_tu.qm"));
        translatorStandart = new QTranslator(qApp);
        static_cast<void>(translatorStandart->load(":/translator/qtbase_ru.qm"));
        qApp->installTranslator(translator);
        qApp->installTranslator(translatorStandart);
        emit retranslate();
    }
    else{//английский
        delete translator;
        delete translatorStandart;
        emit retranslate();
    }
}

void Settings::retranslateUI() {
    lbDark->setText(tr("Dark theme"));
    lbLanguage->setText(tr("Set Russian Language"));
    this->setWindowTitle(tr("Settings"));

    lbOpen->setText(tr("Hot Key \"Open\""));
    lbSave->setText(tr("Hot Key \"Save\""));
    lbClear->setText(tr("Hot Key \"Clear Text Editor\""));
    lbExit->setText(tr("Hot Key \"Exit\""));
}

void Settings::redrawUI(QString _qss, QPalette _palette) {
    this->setStyleSheet(_qss);
    this->setPalette(_palette);
}

void Settings::leOpen_onTextEdit()
{
    qDebug() << "Сработало открыть";
    modifyOpen = open = 0;
    changeOpen = true;
    changeClear = changeSave = changeExit = false;
    leOpen->setStyleSheet("background-color: rgb(0, 179, 255)");
    leSave->setStyleSheet("");
    leClear->setStyleSheet("");
    leExit->setStyleSheet("");
}

void Settings::leSave_onTextEdit()
{
    qDebug() << "Сработало сохранить";
    modifySave = save = 0;
    changeSave = true;
    changeExit =  false;
    changeOpen =  false;
    modifyClear =  false;
    leSave->setStyleSheet("background-color: rgb(0, 179, 255)");
    leOpen->setStyleSheet("");
    leClear->setStyleSheet("");
    leExit->setStyleSheet("");
}

void Settings::leClear_onTextEdit()
{
    qDebug() << "Сработало очистить";
    modifyClear = clear = 0;
    changeClear = true;
    changeOpen = changeSave = changeExit = false;
    leClear->setStyleSheet("background-color: rgb(0, 179, 255)");
    leOpen->setStyleSheet("");
    leSave->setStyleSheet("");
    leExit->setStyleSheet("");
}

void Settings::leExit_onTextEdit()
{
    qDebug() << "Сработало выход";
    modifyExit = exit = 0;
    changeExit = true;
    changeOpen = changeSave = changeClear = false;
    leExit->setStyleSheet("background-color: rgb(0, 179, 255)");
    leOpen->setStyleSheet("");
    leSave->setStyleSheet("");
    leClear->setStyleSheet("");
}


