#ifndef ENDLESSPROGRESSBAR_H
#define ENDLESSPROGRESSBAR_H

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>

class EndlessProgressBar : public QDialog
{
    Q_OBJECT
public:
    explicit EndlessProgressBar(QString text = "", QWidget *parent = 0);
    virtual ~EndlessProgressBar();

signals:

public slots:

private:
    QProgressBar *progressBar;
    QVBoxLayout *layout;
    QLabel *label;
};

#endif // ENDLESSPROGRESSBAR_H
