#include "endlessprogressbar.h"


EndlessProgressBar::EndlessProgressBar(QString text, QWidget *parent) : QDialog(parent)
{
    layout = new QVBoxLayout(this);
    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(0);
    label = new QLabel(text, this);

    layout->addWidget(label);
    layout->addWidget(progressBar);
}

EndlessProgressBar::~EndlessProgressBar()
{
    delete progressBar;
}
