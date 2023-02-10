#ifndef PASSAGE_H
#define PASSAGE_H

#include <QWidget>

namespace Ui {
class Passage;
}

class Passage : public QWidget
{
    Q_OBJECT

public:
    explicit Passage(QWidget *parent = nullptr);
    ~Passage();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Passage *ui;
};

#endif // PASSAGE_H
