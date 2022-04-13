#include "texteditor.h"

TextEditor::TextEditor(QWidget *parent) : QMainWindow(parent)
{
    tabArea = new QTabWidget(this);
    tabArea->setMovable(true);
    tabArea->setTabsClosable(true);
    tabArea->setUsesScrollButtons(true);

    //Диалог выхода из программы
    messExit = new QMessageBox(this);
    yes = new QPushButton(this);
    messExit->addButton(yes, QMessageBox::YesRole);
    no = new QPushButton(this);
    messExit->addButton(no, QMessageBox::NoRole);

    messTabClose = new QMessageBox(this);
    yesClose = new QPushButton(this);
    noClose = new QPushButton(this);
    messTabClose->addButton(yesClose, QMessageBox::YesRole);
    messTabClose->addButton(noClose, QMessageBox::NoRole);

    mainGrid = new QGridLayout(this);
    mainGrid->setContentsMargins(0,0,0,0);
    centralWgt = new QWidget(this);
    mainGrid->addWidget(tabArea, 0, 0, 1, 6);


    createButton();  //create ALL BUTTON


    setting = new Settings(this);
    setting->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    find = new FineDialog(this);
    find->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    modifyOpen = setting->getHotkey()[0];
    open = static_cast<int>(setting->getHotkey()[1]);
    modifySave = setting->getHotkey()[2];
    save = static_cast<int>(setting->getHotkey()[3]);
    modifyClear = setting->getHotkey()[4];
    clear = static_cast<int>(setting->getHotkey()[5]);
    modifyExit = setting->getHotkey()[6];
    exit = static_cast<int>(setting->getHotkey()[7]);

    centralWgt->setLayout(mainGrid);
    this->setCentralWidget(centralWgt);


    createMenu(); //Create MENU


    connect(setting, SIGNAL(retranslate()), this, SLOT(retranslateUI()));
    connect(setting, SIGNAL(redraw(QString, QPalette)), this, SLOT(redrawUI(QString, QPalette)));
    connect(setting, SIGNAL(signal_changeOpen(unsigned int, int)), this, SLOT(swtHotKeyOpen(unsigned int, int)));
    connect(setting, SIGNAL(signal_changeSave(unsigned int, int)), this, SLOT(swtHotKeySave(unsigned int, int)));
    connect(setting, SIGNAL(signal_changeClear(unsigned int, int)), this, SLOT(swtHotKeyClear(unsigned int, int)));
    connect(setting, SIGNAL(signal_changeExit(unsigned int, int)), this, SLOT(swtHotKeyExit(unsigned int, int)));

    connect(tabArea, SIGNAL(currentChanged(int)), this, SLOT(changeTabArea())); //активна новая вкладка
    connect(tabArea, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTabArea(int)));//нажата кнопка закрытия вкладки

    retranslateUI();
    redrawUI(setting->getQss(), setting->getPalette());
}

void TextEditor::closeEvent(QCloseEvent *event)
{
        event->ignore();

        messExit->setIcon(QMessageBox::Information);
        messExit->exec();

        if (messExit->clickedButton() == yes) {
            while(tabArea->count() != 0){
                closeTabArea(tabArea->currentIndex());
            }
            event->accept();
        }
}

void TextEditor::createButton()
{
    buttonOpen = new QToolButton(this);
    buttonOpen->setIcon(QIcon(":/Icons/Icons/open.png"));
    buttonOpen->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonOpen->setPopupMode(QToolButton::MenuButtonPopup);
    openReadAct = new QAction(this);
    actionFileOpen = new QAction(this);
    buttonOpen->addAction(actionFileOpen);
    buttonOpen->addAction(openReadAct);
    buttonOpen->setFixedSize(46, 30);
    connect(buttonOpen, SIGNAL(clicked()), this, SLOT(on_buttonOpen_clicked()));
    connect(openReadAct, SIGNAL(triggered()), this, SLOT(on_buttonOpenOnlyRead_clicked()));

    buttonSave = new QPushButton(this);
    buttonSave->setIcon(QIcon(":/Icons/Icons/save.png"));
    buttonSave->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(buttonSave, SIGNAL(clicked()), this, SLOT(on_buttonSave_clicked()));

    buttonPrint = new QPushButton(this);
    buttonPrint->setIcon(QIcon(":/Icons/Icons/print.png"));
    buttonPrint->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(buttonPrint, SIGNAL(clicked()), this, SLOT(on_actionFilePrint_clicked()));

    buttonHelp = new QPushButton(this);
    buttonHelp->setIcon(QIcon(":/Icons/Icons/help.png"));
    buttonHelp->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(buttonHelp, SIGNAL(clicked()), this, SLOT(on_buttonHelp_clicked()));

    buttonSettings = new QPushButton(this);
    buttonSettings->setIcon(QIcon(":/Icons/Icons/setings.png"));
    buttonSettings->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(buttonSettings, SIGNAL(clicked()), this, SLOT(on_buttonSettings_clicked()));

    actionTextSelect = new QPushButton(this);
    actionTextSelect->setIcon(QIcon(":/Icons/Icons/031-type.png"));
    actionTextSelect->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(actionTextSelect, SIGNAL(clicked()), this, SLOT(on_actionTextSelect_clicked()));

    actionTextFormat = new QPushButton(this);
    actionTextFormat->setIcon(QIcon(":/Icons/Icons/061-highlighter.png"));
    actionTextFormat->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(actionTextFormat, SIGNAL(clicked()), this, SLOT(on_actionTextFormat_clicked()));

    actionTextLeft = new QPushButton(this);
    actionTextLeft->setIcon(QIcon(":/Icons/Icons/045-align-left-1.png"));
    actionTextLeft->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(actionTextLeft, SIGNAL(clicked()), this, SLOT(on_actionTextLeft_clicked()));

    actionTextCentre = new QPushButton(this);
    actionTextCentre->setIcon(QIcon(":/Icons/Icons/046-align-center-1.png"));
    actionTextCentre->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(actionTextCentre, SIGNAL(clicked()), this, SLOT(on_actionTextCentre_clicked()));

    actionTextRight = new QPushButton(this);
    actionTextRight->setIcon(QIcon(":/Icons/Icons/047-right-alignment-1.png"));
    actionTextRight->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(actionTextRight, SIGNAL(clicked()), this, SLOT(on_actionTextRight_clicked()));
}

void TextEditor::createMenu()
{
    //Создадим меню основного окна *********
        menubar = new QMenuBar(this);

        fileMenu = new QMenu(menubar);
        editMenu = new QMenu(menubar);
        helpMenu = new QMenu(menubar);

        actionFileCreate = new QAction(this);
        actionFileCreate->setIcon(QIcon(":/Icons/Icons/main.png"));
        connect(actionFileCreate, SIGNAL(triggered(bool)), this, SLOT(on_buttonCreateNew_clicked()));

        actionFileOpen->setIcon(QIcon(":/Icons/Icons/open.png"));
        connect(actionFileOpen, SIGNAL(triggered(bool)), this, SLOT(on_buttonOpen_clicked()));

        actionFileSave = new QAction(this);
        actionFileSave->setIcon(QIcon(":/Icons/Icons/save.png"));
        connect(actionFileSave, SIGNAL(triggered(bool)), this, SLOT(on_buttonSave_clicked()));

        actionFilePrint = new QAction(this);
        actionFilePrint->setIcon(QIcon(":/Icons/Icons/print.png"));
        connect(actionFilePrint, SIGNAL(triggered(bool)), this, SLOT(on_actionFilePrint_clicked()));

        actionFileExit = new QAction(this);
        actionFileExit->setIcon(QIcon(":/Icons/Icons/cancel.png"));
        connect(actionFileExit, SIGNAL(triggered(bool)), this, SLOT(close()));

        actionEditSetings = new QAction(this);
        actionEditSetings->setIcon(QIcon(":/Icons/Icons/setings.png"));
        connect(actionEditSetings, SIGNAL(triggered(bool)), this, SLOT(on_buttonSettings_clicked()));

        actionEditFind = new QAction(this);
        actionEditFind->setIcon(QIcon(":/Icons/Icons/find.png"));
        connect(actionEditFind, SIGNAL(triggered(bool)), this, SLOT(findText()));

        actionEditDateTime = new QAction(this);
//        actionEditDateTime->setIcon(QIcon(":/Icons/Icons/find.png"));
        connect(actionEditDateTime, SIGNAL(triggered(bool)), this, SLOT(on_buttonInsertDateTime_clicked()));

        actionHelpAbout = new QAction(this);
        actionHelpAbout->setIcon(QIcon(":/Icons/Icons/help.png"));
        connect(actionHelpAbout, SIGNAL(triggered(bool)), this, SLOT(on_buttonHelp_clicked()));

        menubar->addAction(fileMenu->menuAction());
        menubar->addAction(editMenu->menuAction());
        menubar->addAction(helpMenu->menuAction());

        fileMenu->addAction(actionFileCreate);
        fileMenu->addAction(actionFileOpen);
        fileMenu->addAction(actionFileSave);
        fileMenu->addAction(actionFilePrint);
        fileMenu->addSeparator();
        fileMenu->addAction(actionFileExit);
        editMenu->addAction(actionEditSetings);
        editMenu->addAction(actionEditFind);
        editMenu->addAction(actionEditDateTime);
        helpMenu->addAction(actionHelpAbout);

        this->setMenuBar(menubar);

        fileToolBar = new QToolBar;
        this->addToolBar(fileToolBar);
        fileToolBar->setOrientation(Qt::Horizontal);
        fileToolBar->addWidget(buttonOpen);
        fileToolBar->addWidget(buttonSave);
        fileToolBar->addWidget(buttonPrint);

        formatTextToolBar = new QToolBar;
        this->addToolBar(formatTextToolBar);
        formatTextToolBar->addWidget(actionTextSelect);
        formatTextToolBar->addWidget(actionTextFormat);
        formatTextToolBar->addWidget(actionTextLeft);
        formatTextToolBar->addWidget(actionTextCentre);
        formatTextToolBar->addWidget(actionTextRight);

        aboutToolBar = new QToolBar;
        this->addToolBar(aboutToolBar);
        aboutToolBar->addWidget(buttonSettings);
        aboutToolBar->addWidget(buttonHelp);
}

void TextEditor::keyPressEvent(QKeyEvent *event)
{
    if( (event->modifiers() == modifyOpen && event->key() == open) ||
            (modifyOpen == 0 && event->key() == open)){
        on_buttonOpen_clicked();
    }
    else if ( ((event->modifiers() ==  modifySave) && event->key() == save) ||
               (modifySave == 0 && event->key() == save)){
        on_buttonSave_clicked();
    }
    else if ( ((event->modifiers() ==  modifyClear) && event->key() == clear) ||
                (modifyClear == 0 && event->key() == clear)){
        on_buttonCreateNew_clicked();
    }
    else if ( ((event->modifiers() == modifyExit) && event->key() == exit) ||
                (modifyExit == 0 && event->key() == exit)){
        qApp->exit();
    }
}

void TextEditor::on_buttonOpen_clicked(){
    QTextEdit *textEdit = new QTextEdit(this);
    textEdit->setReadOnly(setOnlyReadText);
    textEdit->setPlainText("");
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(planeTxtChange()));
    connect(textEdit, SIGNAL(copyAvailable(bool)), this, SLOT(planeSelectionChange(bool)));
    connect(textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(planeMoveCursor()));

    //проверка, были ли изменения на старой вкладке, чтобы не ставилась лишняя '*'
//    bool isCahngeCurrentTab = false;
//    int currentTab = tabArea->currentIndex();
//    if(tabArea->tabText(currentTab)[0] == '*') isCahngeCurrentTab = true;

    QString fileName;

    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open file"), QDir::currentPath(), "Files (*.* ALL)");

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    QByteArray data; // Создаем объект класса QByteArray, куда мы будем считывать данные
    if (!file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
        return;
    data = file.readAll(); //считываем все данные с файла в объект data

    QFileInfo fileInfo(fileName);
    int newTab = tabArea->addTab(textEdit, fileInfo.fileName());
    tabArea->setTabToolTip(newTab, fileName);
    tabArea->setCurrentIndex(newTab);
    textEdit->setPlainText(data);
    tabArea->setTabText(newTab, tabArea->tabText(newTab).remove(0,1));

    setOnlyReadText = false;

//        if(!isCahngeCurrentTab) //выполним затирание лишнего символа, если не было изменений
//            tabArea->setTabText(currentTab, tabArea->tabText(currentTab).remove(0,1));
}

void TextEditor::on_buttonSave_clicked(){
    QString fileName;
        try	{
            fileName = QFileDialog::getSaveFileName(this,
                tr("Save file"), QDir::currentPath(), "Files (*.txt)");
        }
        catch (const std::exception&){
                fileName = "";
        }

        if (!fileName.isEmpty()){
            QFile file(fileName);
            if (!file.open(QFile::WriteOnly)) return; // Проверяем, возможно ли открыть наш файл для записи
            QTextStream stream(&file);

            QWidget *widg = tabArea->currentWidget();
            if(widg) {
                QPlainTextEdit *textEditor = static_cast<QPlainTextEdit*>(widg);
                stream << textEditor->toPlainText();
                QFileInfo fileInfo(fileName);
                tabArea->setTabToolTip(tabArea->currentIndex(), fileName);
                tabArea->setTabText(tabArea->currentIndex(), fileInfo.fileName());
            }
            file.close();
        }
}

void TextEditor::on_buttonHelp_clicked(){
    QFile file(":/Txt/readme.txt");
    file.open(QIODevice::ReadOnly);
    QString info = file.readAll();

    QMessageBox::information(nullptr, tr("Help"),info);
}

void TextEditor::on_buttonSettings_clicked()
{
    setting->show();
}

void TextEditor::on_buttonOpenOnlyRead_clicked()
{
    setOnlyReadText = true;
    on_buttonOpen_clicked();
}

void TextEditor::on_buttonCreateNew_clicked()
{
    QTextEdit *textEdit = new QTextEdit(this);
    textEdit->setReadOnly(setOnlyReadText);
    textEdit->setPlainText("");
    textEdit->setToolTip("");
    int newTab = tabArea->addTab(textEdit, "*unknown");
    tabArea->setCurrentIndex(newTab);
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(planeTxtChange()));
    connect(textEdit, SIGNAL(copyAvailable(bool)), this, SLOT(planeSelectionChange(bool)));
}

void TextEditor::on_buttonInsertDateTime_clicked()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString time = dateTime.time().toString("hh:mm");
    QString date = dateTime.date().toString("dd.MM.yyyy");

    QWidget *widg = tabArea->currentWidget();
    QTextEdit *textEditor = static_cast<QTextEdit*>(widg);

    textEditor->insertPlainText(time + " " + date);
}

void TextEditor::retranslateUI()
{
    this->setWindowTitle(tr("Text editor"));
    openReadAct->setText(tr("Open Read only"));
    buttonOpen->setToolTip(tr("Open"));
    buttonSave->setToolTip(tr("Save"));
    buttonHelp->setToolTip(tr("Help"));
    buttonSettings->setToolTip(tr("Settings"));
    buttonPrint->setToolTip(tr("Print"));

    actionTextSelect->setToolTip(tr("Change format of selected text"));
    actionTextFormat->setToolTip(tr("Copy format"));
    actionTextLeft->setToolTip(tr("Align text left"));
    actionTextCentre->setToolTip(tr("Align text centre"));
    actionTextRight->setToolTip(tr("Align text right"));

    fileMenu->setTitle(tr("File"));
    editMenu->setTitle(tr("Edit"));
    helpMenu->setTitle(tr("Help"));

    actionFileCreate->setText(tr("Create"));
    actionFileOpen->setText(tr("Open"));
    actionFileSave->setText(tr("Save"));
    actionFilePrint->setText(tr("Print"));
    actionFileExit->setText(tr("Exit"));
    actionEditSetings->setText(tr("Setings"));
    actionEditFind->setText(tr("Find..."));
    actionEditDateTime->setText(tr("Date && Time"));
    actionHelpAbout->setText(tr("About"));

    messExit->setWindowTitle(tr("Exit"));
    messExit->setText(tr("To exit the programm?"));
    messTabClose->setWindowTitle(tr("Close tab"));
    messTabClose->setText(tr("Save changes?"));
    yes->setText(tr("Yes"));
    no->setText(tr("No"));
    yesClose->setText(tr("Yes"));
    noClose->setText(tr("No"));

    setting->retranslateUI();
    find->retranslateUI();
}

void TextEditor::redrawUI(QString _qss, QPalette _palette)
{
    this->setPalette(_palette);
    buttonOpen->setStyleSheet(_qss);
    buttonSave->setStyleSheet(_qss);
    buttonPrint->setStyleSheet(_qss);
    buttonHelp->setStyleSheet(_qss);
    buttonSettings->setStyleSheet(_qss);
    actionTextSelect->setStyleSheet(_qss);
    actionTextFormat->setStyleSheet(_qss);
    actionTextLeft->setStyleSheet(_qss);
    actionTextCentre->setStyleSheet(_qss);
    actionTextRight->setStyleSheet(_qss);

    centralWgt->setStyleSheet(_qss);

    setting->redrawUI(_qss, _palette);
}

void TextEditor::swtHotKeyOpen(unsigned int _modify, int _key)
{
    open = _key; modifyOpen = _modify;
}

void TextEditor::swtHotKeySave(unsigned int _modify, int _key)
{
    save = _key; modifySave = _modify;
}

void TextEditor::swtHotKeyClear(unsigned int _modify, int _key)
{
    clear = _key; modifyClear = _modify;
}

void TextEditor::swtHotKeyExit(unsigned int _modify, int _key)
{
    exit = _key; modifyExit = _modify;
}

void TextEditor::findText()
{
    QWidget *widg = tabArea->currentWidget();
    QTextEdit *textEditor = static_cast<QTextEdit*>(widg);
    find->setTextEdit(textEditor); //передадим весь текст в поиск
    QObject::connect(find, SIGNAL(setCursorPos(int, int, int)), this, SLOT(setNewPosition(int, int, int)));   // Соединяем сигнал  от диалогового окна к слоту
    find->show();              // Запускаем диалог без блокировки главного окна
}

void TextEditor::setNewPosition(int start, int lenght, int npos)
{                                                          // Если поиск прошел удачно, получаем координаты выделения текста
  QWidget *widg = tabArea->currentWidget();
  QTextEdit *textEditor = static_cast<QTextEdit*>(widg);
  QTextCursor tcursor = textEditor->textCursor();            // Копируем объект QTextCursor
  if (npos > start) {                                      // Если нужно сместить курсор вперед
      tcursor.setPosition(start, QTextCursor::MoveAnchor); // Устанавливаем курсор в начало искомой строки по индексу
      tcursor.setPosition(npos, QTextCursor::KeepAnchor);  // Выделяем текст до новой позиции, которая равна сумме  позиции начала искомой строки  и ее длины
  }
  else {                                                   // Если нужно сместить вперед
      tcursor.setPosition(start + lenght, QTextCursor::MoveAnchor);  // Берем позицию окончания искомой строки
      tcursor.setPosition(start, QTextCursor::KeepAnchor); // Переносим выделение на начало искомой строки
  }
  textEditor->setTextCursor(tcursor);                        // Устанавливаем новые настройки курсора QTextEdit
}

void TextEditor::on_actionFilePrint_clicked()
{
   QPrinter printer;
   QPrintDialog dlg(&printer, this);
   dlg.setWindowTitle("Print");
   if (dlg.exec() != QDialog::Accepted)
       return;
   QWidget *widg = tabArea->currentWidget();
   QTextEdit *textEditor = static_cast<QTextEdit*>(widg);
   textEditor->print(&printer);
}

void TextEditor::changeTabArea()
{
    QWidget *widg = tabArea->currentWidget();
    if(widg) {
        QTextEdit *textEditor = static_cast<QTextEdit*>(widg);
        find->setTextEdit(textEditor);
    }
//    QMessageBox::information(this, "rfrf", "rfrf");
}

void TextEditor::closeTabArea(int _tab)
{
    if(tabArea->tabText(_tab)[0] == '*') {
        messTabClose->setIcon(QMessageBox::Information);
        messTabClose->exec();
        if (messTabClose->clickedButton() == yesClose){
//            QMessageBox::information(this, "rfrf", "rfrf");
            on_buttonSave_clicked(); //если был текст изменне - предложим сохранить
        }
    }
    tabArea->removeTab(_tab);
}

void TextEditor::planeTxtChange()
{
    if(tabArea->tabText(tabArea->currentIndex())[0] != '*')
        tabArea->setTabText(tabArea->currentIndex(), tabArea->tabText(tabArea->currentIndex()).insert(0, '*'));
}

void TextEditor::planeSelectionChange(bool select)
{
    if(select && !processSelected && isCopyTextFormat) {
        processSelected = true;
    }
    else if (isCopyTextFormat && processSelected && !select){
        isCopyTextFormat = false;
        processSelected = false;
    }
}

void TextEditor::planeMoveCursor()
{
    if(isCopyTextFormat && processSelected) { //если мы копируем формат
        QWidget *widg = tabArea->currentWidget();
        QTextEdit *textEditor = static_cast<QTextEdit*>(widg);
        QTextCursor cursor = textEditor->textCursor();
        cursor.mergeCharFormat(m_fmt);
        textEditor->mergeCurrentCharFormat(m_fmt);
    }
}

void TextEditor::on_actionTextSelect_clicked()
{
    QWidget *widg = tabArea->currentWidget();
    if(widg) {
       QTextEdit *textEditor = static_cast<QTextEdit*>(widg);
       QFontDialog fntDlg(m_font,this);
       bool ok;
       m_font = fntDlg.getFont(&ok); // Запускаем диалог настройки шрифта
       if (ok){                 // Если нажата кнопка OK, применяем новые настройки шрифта
           QTextCharFormat fmt;
           fmt.setFont(m_font);
           textEditor->textCursor().setCharFormat(fmt);
       }
    }
}

void TextEditor::on_actionTextFormat_clicked()
{
    QWidget *widg = tabArea->currentWidget();
    if(widg) {
        QTextEdit *textEditor = static_cast<QTextEdit*>(widg);
        m_font = textEditor->textCursor().charFormat().font(); // получаем текущий шрифт
        m_fmt.setFont(m_font); //устанавливаем шрифт в формат

        isCopyTextFormat = true;
        actionTextFormat->setDown(isCopyTextFormat);
    }
}

void TextEditor::on_actionTextLeft_clicked()
{
    QWidget *widg = tabArea->currentWidget();
    if(widg) {
        QTextEdit *textEditor = static_cast<QTextEdit*>(widg);
        textEditor->setAlignment(Qt::AlignLeft);
    }
}

void TextEditor::on_actionTextCentre_clicked()
{
    QWidget *widg = tabArea->currentWidget();
    if(widg) {
        QTextEdit *textEditor = static_cast<QTextEdit*>(widg);
        textEditor->setAlignment(Qt::AlignCenter);
    }
}

void TextEditor::on_actionTextRight_clicked()
{
    QWidget *widg = tabArea->currentWidget();
    if(widg) {
        QTextEdit *textEditor = static_cast<QTextEdit*>(widg);
        textEditor->setAlignment(Qt::AlignRight);
    }
}
