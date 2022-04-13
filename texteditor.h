#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QtWidgets>
#include <QtGui>
#include <QPushButton>
#include <QMessageBox>
#include <QMainWindow>
#include <QPrinter>
#include <QPrintDialog>
#include <QMdiArea>

#include "settings.h"
#include "finedialog.h"

class TextEditor : public QMainWindow
{
    Q_OBJECT
public:
    explicit TextEditor(QWidget *parent = nullptr);

private:
    void closeEvent(QCloseEvent * event) override;
    void inline createButton();
    void inline createMenu();

    QWidget *centralWgt {nullptr};
    QGridLayout *mainGrid {nullptr}; //создаем грид, который кладем на вкладку

    QToolButton *buttonOpen {nullptr};
    QAction *openReadAct {nullptr};

    QPushButton *buttonSave {nullptr};
    QPushButton *buttonPrint {nullptr};
    QPushButton *buttonHelp {nullptr};
    QPushButton *buttonSettings {nullptr};
    QPushButton *actionTextSelect {nullptr};
    QPushButton *actionTextFormat {nullptr};
    QPushButton *actionTextLeft {nullptr};
    QPushButton *actionTextCentre {nullptr};
    QPushButton *actionTextRight {nullptr};

    Settings *setting {nullptr};
    FineDialog *find {nullptr};

    QMenuBar *menubar {nullptr};
    QMenu *fileMenu {nullptr};
    QMenu *editMenu {nullptr};
    QMenu *helpMenu {nullptr};
    QToolBar *fileToolBar {nullptr};
    QToolBar *formatTextToolBar {nullptr};
    QToolBar *aboutToolBar {nullptr};
    QAction *actionFileCreate {nullptr};
    QAction *actionFileOpen {nullptr};
    QAction *actionFileSave {nullptr};
    QAction *actionFilePrint {nullptr};
    QAction *actionFileExit {nullptr};
    QAction *actionEditFind {nullptr};
    QAction *actionEditSetings {nullptr};
    QAction *actionEditDateTime {nullptr};
    QAction *actionHelpAbout {nullptr};

    unsigned int modifyOpen , modifySave, modifyClear, modifyExit;
    int open, save, clear, exit;

    QTabWidget *tabArea {nullptr};

    bool setOnlyReadText = false;

    QMessageBox *messExit {nullptr};
    QPushButton *yes {nullptr};
    QPushButton *no {nullptr};
    QMessageBox *messTabClose {nullptr};
    QPushButton *yesClose {nullptr};
    QPushButton *noClose {nullptr};

    QFont m_font;
    QTextCharFormat m_fmt;
    bool isCopyTextFormat = false;
    bool processSelected = false;

protected:
//    virtual void mousePressEvent(QMouseEvent *event) override;   // событие при нажатии кнопки мыши
 //   virtual void mouseReleaseEvent(QMouseEvent *event) override; // событие при отжатии кнопки мыши
//    virtual void mouseMoveEvent(QMouseEvent *event) override;    // событие при перемещении курсора
    virtual void keyPressEvent(QKeyEvent *event) override;       // событие при нажатии клавиши клавиатуры
//    virtual void keyReleaseEvent(QKeyEvent *event) override;     // событие при отжатии клавиши клавиатуры

signals:

public slots:
    void on_buttonOpen_clicked();
    void on_buttonSave_clicked();
    void on_buttonHelp_clicked();
    void on_buttonSettings_clicked();
    void on_buttonOpenOnlyRead_clicked();
    void on_buttonCreateNew_clicked();
    void on_buttonInsertDateTime_clicked();

    void retranslateUI();
    void redrawUI(QString _qss, QPalette _palette);

    void swtHotKeyOpen(unsigned int _modify, int _key);
    void swtHotKeySave(unsigned int _modify, int _key);
    void swtHotKeyClear(unsigned int _modify, int _key);
    void swtHotKeyExit(unsigned int _modify, int _key);
    void findText();                    // Слот вызова поиска
    void setNewPosition(int, int, int); // Слот на обработку выделения текста и установки указателя
    void on_actionFilePrint_clicked();
    void changeTabArea();
    void closeTabArea(int _tab);

    void planeTxtChange();
    void planeSelectionChange(bool select);
    void planeMoveCursor();

    void on_actionTextSelect_clicked();
    void on_actionTextFormat_clicked();
    void on_actionTextLeft_clicked();
    void on_actionTextCentre_clicked();
    void on_actionTextRight_clicked();
};

#endif // TEXTEDITOR_H
