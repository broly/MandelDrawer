#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsPixmapItem>
#include <QMainWindow>

#include <Source/MandelDrawer.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void InitializeMandelDrawer();

    void Startup();

    QGraphicsPixmapItem* GraphicsPixmapItem;
    QGraphicsScene* GraphicsScene;

    void Redraw();


private slots:
    void on_ChJuliaMode_stateChanged(int arg1);

    void on_horizontalSlider_valueChanged(int value);

    void on_verticalSlider_valueChanged(int value);

    void on_verticalSlider_2_valueChanged(int value);

    void on_ChCustomFormula_stateChanged(int arg1);

    void on_EditCustomFormula_textChanged(const QString &arg1);

    void on_EditCustomFormula_textEdited(const QString &arg1);

private:
    Ui::MainWindow *ui;

    Mandel::MandelDrawer MandelDrawer;

    Mandel::FloatVector2D CurrentJuliaValue;
    float CurrentZoomValue;
};
#endif // MAINWINDOW_H
