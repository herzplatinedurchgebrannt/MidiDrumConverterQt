#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <vector>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionConvert_triggered();
    void on_actionHelp_triggered();
    void on_actionClose_triggered();
    void on_actionClear_files_triggered();
    void updateCurrentPreset();
    void on_actionLoad_presets_xml_triggered();

private:
    Ui::MainWindow *ui;


    QList <QString> currentFiles = {};
    QList <QString> availablePresets = {};

    struct midiConv
    {
        int noteBefore;
        int noteAfter;
        string instrument;
        string pluginBefore;
        string pluginAfter;
    };

    struct midiConvPreset
    {
        std::vector<midiConv> blub;
        string presetName;
    };

    std::vector<midiConv> midiConvVector;
    std::vector<midiConvPreset> midiConvVectorPreset;

    int currentPreset;

};
#endif // MAINWINDOW_H
