#ifndef FINEDIALOG_H
#define FINEDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class FineDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QTextEdit *textEdit WRITE setTextEdit)

    public:
       explicit FineDialog(QWidget *parent = nullptr);
       virtual ~FineDialog() override;
       void setTextEdit(QTextEdit *textEdit);
    signals:

    public slots:
       void findPrev();                  // Сигнал от кнопки Поиск (до курсора или после)
       void findNext();
       void retranslateUI();
    private:
       QTextEdit *textEdit {nullptr};
       QGridLayout *layout {nullptr};
       QLabel *label {nullptr};
       QLineEdit *lineEdit {nullptr};              // Строка для ввода строки поиска
       QPushButton *findButtons[2] {nullptr};      // 2 кнопки поиска
    signals:
       void setCursorPos(int, int, int); // Сигнал с указанием выделения текста и новой позиции курсора. Начало, длина, новая позиция
};

#endif // FINEDIALOG_H
