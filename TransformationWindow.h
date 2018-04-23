#ifndef TRANSFORMATIONWINDOW_H
#define TRANSFORMATIONWINDOW_H

#include <QMainWindow>
#include "ui_TransformationWindow.h"

class TransformationWindow : public QMainWindow, public Ui::TransformationWindow
{
    Q_OBJECT

public:
    explicit TransformationWindow(QWidget* parent = nullptr);
    ~TransformationWindow();

private:
    Ui::TransformationWindow *ui;
};

#endif // TRANSFORMATIONWINDOW_H
