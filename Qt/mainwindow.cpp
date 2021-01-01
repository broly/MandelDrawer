#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTimer>

#define RESOLUTION 512

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
        {RESOLUTION, RESOLUTION},
        16,
        150,
        8.f,
        1.f,
        0.5f,
    };

    Settings.CustomFormula = "z^2";

    MandelDrawer.SetSettings(Settings);

    GraphicsPixmapItem = new QGraphicsPixmapItem;
    GraphicsScene = new QGraphicsScene;

    GraphicsScene->addItem(&*GraphicsPixmapItem);

    QGraphicsView_MandelDrawer* GraphicsView = findChild<QGraphicsView_MandelDrawer*>("GraphicsView");
    GraphicsView->OnZoom = std::bind(&MainWindow::OnZoom, this, std::placeholders::_1);
    GraphicsView->OnPan = std::bind(&MainWindow::OnPan, this, std::placeholders::_1);

    GraphicsView->setScene(GraphicsScene);

    QTimer* timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(Tick()));
    timer->start();
}


void MainWindow::Startup()
{
    Redraw();
}

void MainWindow::Tick()
{
    if (bIsDrawing)
    {
        if (!MandelDrawer.IsBusy())
        {
            bIsDrawing = false;
            MandelDrawer.CookImage();

            if (GraphicsPixmapItem)
            {

                QImage image = MandelDrawer.GetTargetPicture().ToQImage();

                GraphicsPixmapItem->setPixmap(QPixmap::fromImage(image));


                QGraphicsView* GraphicsView = findChild<QGraphicsView*>("GraphicsView");
                QRectF Rect = {0, 0, RESOLUTION, RESOLUTION};
                GraphicsView->fitInView(Rect);
            }
        }
    }

    if (bHasPendingRedraw)
    {
        if (!MandelDrawer.IsBusy())
        {
            bHasPendingRedraw = false;
            Redraw();
        }
    }
}


void MainWindow::Redraw()
{
    if (!MandelDrawer.IsBusy())
    {
        bIsDrawing = true;
        MandelDrawer.SetJuliaValue(CurrentJuliaValue);
        MandelDrawer.SetDrawScale(CurrentZoomValue);
        MandelDrawer.Settings.DrawOffset = CurrentPan;
        MandelDrawer.Start();
    } else
    {
        bHasPendingRedraw = true;
    }
}

void MainWindow::OnZoom(float Delta)
{
    CurrentZoomValue += (Delta / 1200.f) * (CurrentZoomValue);
    bHasPendingRedraw = true;
}


void MainWindow::OnPan(QPoint Delta)
{
    Mandel::FloatVector2D delta(Delta.x(), Delta.y());

    CurrentPan += (delta / 100.f) / CurrentZoomValue;
    bHasPendingRedraw = true;
}

void MainWindow::on_ChJuliaMode_stateChanged(int arg1)
{
    MandelDrawer.SetJuliaMode((bool)arg1);
    bHasPendingRedraw = true;
    // Redraw();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    CurrentJuliaValue.Y = ((float)value) / 500.f;
    bHasPendingRedraw = true;
    // Redraw();
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    CurrentJuliaValue.X = ((float)value) / 500.f;
    bHasPendingRedraw = true;
    // Redraw();
}



void MainWindow::on_ChCustomFormula_stateChanged(int arg1)
{
    MandelDrawer.Settings.bUsesCustomFormula = (bool)arg1;
    bHasPendingRedraw = true;

    // Redraw();
}

void MainWindow::on_EditCustomFormula_textChanged(const QString &arg1)
{
    std::string text = arg1.toStdString();
    MandelDrawer.Settings.CustomFormula = text;
    bHasPendingRedraw = true;
    // Redraw();
}

void MainWindow::on_EditCustomFormula_textEdited(const QString &arg1)
{

}
