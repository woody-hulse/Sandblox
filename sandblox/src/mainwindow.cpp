#include "mainwindow.h"
#include "settings.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QSettings>
#include <QLabel>
#include <QGroupBox>
#include <QCursor>
#include <iostream>

void MainWindow::initialize() {
    sandblox = new Sandblox;
    aspectRatioWidget = new AspectRatioWidget(this);

    float aspectRatio = 9.f / 16.f;
    aspectRatioWidget->setAspectWidget(sandblox, aspectRatio);
    QHBoxLayout *hLayout = new QHBoxLayout; // horizontal alignment
    QVBoxLayout *vLayout = new QVBoxLayout(); // vertical alignment
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addLayout(vLayout);
    hLayout->addWidget(aspectRatioWidget, 1);
    this->setLayout(hLayout);
}

void MainWindow::finish() {
    sandblox->finish();
    delete(sandblox);
}
