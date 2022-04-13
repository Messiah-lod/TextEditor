#include "finedialog.h"

FineDialog::FineDialog(QWidget *parent) : QDialog(parent),
   textEdit(nullptr), layout(nullptr)
{

   setFixedSize(300, 100);
   layout = new QGridLayout();
   setLayout(layout);
   label = new QLabel(this);
   layout->addWidget(label, 1, 1, 1, 4);
   lineEdit = new QLineEdit(this);
   layout->addWidget(lineEdit, 2, 1, 1, 7);

   findButtons[0] = new QPushButton(this);
   findButtons[1] = new QPushButton(this);

   connect(findButtons[0], SIGNAL(clicked()), this, SLOT(findPrev()));
   connect(findButtons[1], SIGNAL(clicked()), this, SLOT(findNext()));

   layout->addWidget(findButtons[0], 4, 1, 1, 3);
   layout->addWidget(findButtons[1], 4, 5, 1, 3);
}

FineDialog::~FineDialog()
{

}

void FineDialog::setTextEdit(QTextEdit *_textEdit)
{
    textEdit = _textEdit;
}

void FineDialog::findPrev()
{
   QString str = lineEdit->text();
   int pos = textEdit->textCursor().position(); // Получаем позицию курсора
   QString txt = textEdit->toPlainText();       // Получаем текст
   if (txt.size() == 0 || str.size() == 0) return;
   QString p = txt.mid(0, pos);                 // Копируем текст от начала до курсора
   int last = -1;                               // Последний индекс с текстом
   int ps = 0;
   for (bool b = true;b;)
   {
       int index = static_cast<int>(p.indexOf(str, ps));             // Получаем индекс начала искомого текста
       if (index == -1) b = false;                 // Если текст не найден, завершаем поиск
       else {
           last = index;                           // Сохраняем последний найденный индекс на искомую строку
           ps = static_cast<int>(index + str.length());              // Смещаем поиск на длину искомой строки. Продолжаем искать повторение строки
       }
   }
   if (last != -1)
       emit setCursorPos(last, static_cast<int>(str.length()), last);// Посылаем позицию выделения и нового положения курсора
}

void FineDialog::findNext()
{
   QString str = lineEdit->text();//получили что ищем
   QString txt = textEdit->toPlainText(); //где ищем
   if (txt.size() == 0 || str.size() == 0) return;
   int pos = textEdit->textCursor().position();     // Получаем позицию курсора
   int index = static_cast<int>(txt.indexOf(str, pos));
   if (index != -1) {
       emit setCursorPos(index, static_cast<int>(str.length()), static_cast<int>(str.length() + index));
   }
}

void FineDialog::retranslateUI() {
    setWindowTitle(tr("Find"));
    label->setText(tr("Find string"));
    findButtons[0]->setText(tr("Find previous"));
    findButtons[1]->setText(tr("Find next"));
}
