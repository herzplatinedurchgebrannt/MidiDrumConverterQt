#include "about.h"
#include "ui_about.h"
#include <QDesktopServices>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
}

About::~About()
{
    delete ui;
}

void About::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/herzplatinedurchgebrannt/MidiDrumConverterQt/blob/master/README.md"));
}

