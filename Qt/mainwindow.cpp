#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CurrentJuliaValue = 0.f;
    CurrentZoomValue = 1.f;

    InitializeMandelDrawer();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete GraphicsPixmapItem;
    delete GraphicsScene;
}

void MainWindow::InitializeMandelDrawer()
{
    Mandel::MandelDrawerSettings Settings
    {
        {128, 128},
        16,
        150,
        8.f,
        1.f,
        0.5f,
    };

    Settings.CustomFormula = "z^-2";

    MandelDrawer.SetSettings(Settings);

    GraphicsPixmapItem = new QGraphicsPixmapItem;
    GraphicsScene = new QGraphicsScene;

    GraphicsScene->addItem(&*GraphicsPixmapItem);

    QGraphicsView* GraphicsView = findChild<QGraphicsView*>("GraphicsView");

    GraphicsView->setScene(GraphicsScene);

    QTimer::singleShot(100, this, &MainWindow::Startup);

}


void MainWindow::Startup()
{
    Redraw();
}


void MainWindow::Redraw()
{
    MandelDrawer.SetJuliaValue(CurrentJuliaValue);
    MandelDrawer.SetDrawScale(CurrentZoomValue);
    MandelDrawer.Start();

    if (GraphicsPixmapItem)
    {

        QImage image = MandelDrawer.GetTargetPicture().ToQImage();

        GraphicsPixmapItem->setPixmap(QPixmap::fromImage(image));


        QGraphicsView* GraphicsView = findChild<QGraphicsView*>("GraphicsView");
        QRectF Rect = {0, 0, 128, 128};
        GraphicsView->fitInView(Rect);
    }
}

void MainWindow::on_ChJuliaMode_stateChanged(int arg1)
{
    MandelDrawer.SetJuliaMode((bool)arg1);
    Redraw();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    CurrentJuliaValue.Y = ((float)value) / 500.f;
    Redraw();
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    CurrentJuliaValue.X = ((float)value) / 500.f;
    Redraw();
}

void MainWindow::on_verticalSlider_2_valueChanged(int value)
{
    CurrentZoomValue = float(1000 + value) / 1000;
    Redraw();
}

void MainWindow::on_ChCustomFormula_stateChanged(int arg1)
{
    MandelDrawer.Settings.bUsesCustomFormula = (bool)arg1;

    Redraw();
}

void MainWindow::on_EditCustomFormula_textChanged(const QString &arg1)
{
    std::string text = arg1.toStdString();
    MandelDrawer.Settings.CustomFormula = text;
    Redraw();
}

void MainWindow::on_EditCustomFormula_textEdited(const QString &arg1)
{

}
