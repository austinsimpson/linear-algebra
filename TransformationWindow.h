#ifndef TRANSFORMATIONWINDOW_H
#define TRANSFORMATIONWINDOW_H

#include <QMainWindow>

#include "ui_TransformationWindow.h"
#include "ConvolutionalImageFilter.h"

#include <QMap>

class TransformationWindow : public QMainWindow, public Ui::TransformationWindow
{
    Q_OBJECT

public:
    explicit TransformationWindow(QWidget* parent = nullptr);
    ~TransformationWindow();

private slots:
	void on__browseImageButton_clicked();

	void on__selectedFilterComboBox_currentIndexChanged(const QString& currentIndex);

	void on__actionSave_Matrix_triggered();

private:
    Ui::TransformationWindow *ui;

	QImage _currentImage;
	QMap<QString, ConvolutionalImageFilter> _filters;

};

#endif // TRANSFORMATIONWINDOW_H
