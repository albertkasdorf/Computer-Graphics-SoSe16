#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	/*
	QObject::connect(
		ui->sldRotationZ, SIGNAL(valueChanged(int)),
		ui->myGLWidget, SLOT(receiveRotationZ(int)));
	*/
}

MainWindow::~MainWindow()
{
	delete ui;
}