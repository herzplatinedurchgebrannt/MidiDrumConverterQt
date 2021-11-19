#include "mainwindow.h"
#include "about.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <QFileDialog>
#include <QStringListModel>
#include "MidiFile.h"
#include "Options.h"
#include <QMessageBox>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;
using namespace smf;

QStringListModel *model = new QStringListModel();


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listView->setModel(model);

    // on_actionLoad_preset_file_triggered();

    connect(ui->comboBox, &QComboBox::currentIndexChanged,this, &MainWindow::updateCurrentPreset);
}


MainWindow::~MainWindow()
{
    delete ui;
}


// OPEN FILE DIALOG
void MainWindow::on_actionOpen_triggered()
{
    currentFiles.clear();
    model->removeRows(0, model->rowCount());

    QList <QString> filesDialog = QFileDialog::getOpenFileNames(this, "Open midi file");

    if (filesDialog.size() > 0)
    {
        ui->listView-> clearSelection();
        currentFiles = filesDialog;
        model->setStringList(filesDialog);
    }
}


// LOOP CONVERT FILES
void MainWindow::on_actionConvert_triggered()
{
    QString qExtension = ui->lineEdit->text();
    string extension = qExtension.toStdString();

    // check if extension box is filled
    if (qExtension.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Please fill in extension text field.");
        msgBox.exec();
        return;
    }
    else
    {
        if (extension.find(".mid") > 0)
        {
            extension.replace(extension.size() -4 ,4,"");
        }
    }

    // check if files are selected
    if (currentFiles.count() == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Please open midi files first.");
        msgBox.exec();
        return;
    }


    // check if preset file was loaded
    if (midiConvVectorPreset.size() == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Please load presets.xml file.");
        msgBox.exec();
        return;
    }


    // EXCEPTION HANDLING ------------------>
    foreach(QString str, currentFiles)
    {
        std::cout << str.toStdString() << std::endl;

        bool logConvert = true;
        std::list<int> missedNotes;
        std::vector<int> v;

        /*
        struct midiConv
        {
            int noteBefore;
            int noteAfter;
            string instrument;
            string pluginBefore;
            string pluginAfter;
        };

        midiConv midiConvArray[20] =
        {
            {36,24,"KICK","SSD5","GGD4"},
            {38,26,"SNARE","SSD5","GGD4"},
            {46,26,"TOM_LO","SSD5","GGD4"},
            {57,56,"TOM_HI","SSD5","GGD4"},
            {55,54,"HIHAT","SSD5","GGD4"},
            {42,23,"CRASH_LE","SSD5","GGD4"},
            {48,41,"CRASH_RI","SSD5","GGD4"},
            {50,28,"RIDE","SSD5","GGD4"},
            {43,41,"RIDE","SSD5","GGD4"},
            {24,24,"KICK","SSD5","GGD4"},
            {26,26,"SNARE","SSD5","GGD4"},
            {60,43,"HH_CL_2","SSD5","GGD4"},
            {66,42,"HH_CL_1","SSD5","GGD4"},
            {51,61,"RIDE","SSD5","GGD4"},
            {54,42,"HH_CL_1","SSD5","GGD4"},
            {56,61,"RIDE","SSD5","GGD4"},
            {40,47,"RIDE","SSD5","GGD4"},
            {65,56,"RIDE","SSD5","GGD4"},
            {59,54,"RIDE","SSD5","GGD4"},
            {47,47,"RIDE","SSD5","GGD4"}
        };*/


        //int midiConvArraySize = sizeof(midiConvArray)/sizeof(midiConvArray[0]);
        int midiConvArraySize = midiConvVectorPreset[currentPreset].blub.size();

        Options options;
        MidiFile midifile;

        midifile.read(str.toStdString());

        bool foundMatch = false;

        for (int i=0; i<midifile.getTrackCount(); i++) {
            for (int j=0; j<midifile[i].getEventCount(); j++)
            {
                if (!midifile[i][j].isNote()) continue;
                if (midifile[i][j].getChannel() == 9) continue;

                // loop matrix to find notes
                for (int c = 0; c < midiConvArraySize; c++)
                {
                    foundMatch = false;

                    if (logConvert == false)
                    {
                        cout << midifile[i][j].getP1() << endl;
                        //cout << midiConvArray[c].noteBefore << endl;
                        cout << midiConvVectorPreset[currentPreset].blub[c].noteBefore << endl;
                    }

                    if (midifile[i][j].getP1() == midiConvVectorPreset[currentPreset].blub[c].noteBefore /*| midiConvArray[c].noteBefore == midiConvArray[c].noteAfter*/)
                    {
                        foundMatch = true;
                        midifile[i][j].setP1(midiConvVectorPreset[currentPreset].blub[c].noteAfter);
                        if (logConvert == false)
                            cout << "match" << endl;
                        break;
                    }
                }
                // note not found in matrix -> add notes to matrix
                if (foundMatch == false){

                    // write first value to vector
                    if (v.empty())
                    {
                        v.push_back(midifile[i][j].getP1());
                    }
                    // check if vector contains value
                    else
                    {
                        bool addMissedNote = true;
                        for (int k = 0; k < (int)v.size(); k++)
                        {
                            if (v.at(k) == midifile[i][j].getP1())
                            {
                                addMissedNote = false;
                                break;
                            }
                        }
                        if (addMissedNote == true)
                        {
                            v.push_back(midifile[i][j].getP1());
                        }
                    }
                }
            }
            if (logConvert == true)
            {
                if (v.size() > 0)
                {
                    //cout << "Missing notes! :-(" << endl;
                    //cout << "add those notes to matrix: " << endl;
                    //std::sort(std::begin(v), std::end(v));
                    //for (int n : v){
                        //cout << "{" << n << "," << n << ",Instrument," << midiConvArray[0].pluginBefore << "," << midiConvArray[0].pluginAfter << "},"<< endl;
                    //}
                }
                else
                {
                    cout << "Success!" << endl;
                }
            }
        }

        //cout << midifile;

        string newFileName = str.toStdString();
        newFileName.replace(newFileName.size() - 4,4, extension + ".mid");

        cout << "New file created: " + newFileName << endl;

        midifile.write(newFileName);
    }
    model->removeRows(0, model->rowCount());

    QMessageBox msgBox;
    msgBox.setText("Done :-)!");
    msgBox.exec();
    return;
}


// CLEAR LISTVIEW
void MainWindow::on_actionClear_files_triggered()
{
    // clear listview
    model->removeRows(0, model->rowCount());
    currentFiles.clear();
}


// update combobox selection
void MainWindow::updateCurrentPreset()
{
    currentPreset = ui->comboBox->currentIndex();

    QString currentPresetName = QString::fromStdString(midiConvVectorPreset[currentPreset].presetName);
    currentPresetName = "_" + currentPresetName + ".mid";
    ui->lineEdit->setText(currentPresetName);
}


// HELP PAGE
void MainWindow::on_actionHelp_triggered()
{
    // open new window
    // user instructions
    About about;
    about.setModal(true);
    about.exec();
}


// CLOSE PROGRAM
void MainWindow::on_actionClose_triggered()
{
    QCoreApplication::quit();
}


// LOAD PRESETS.XML FILE
void MainWindow::on_actionLoad_presets_xml_triggered()
{
    QString openPresetFile = QFileDialog::getOpenFileName(this, "Open midi file","","xml file (*.xml)");

    if (openPresetFile.size() > 0)
    {
        ui->comboBox->clear();

        try
        {
            // Read the sample.xml file
            XMLDocument doc;
            //doc.LoadFile( "/Users/alexandermathieu/Coding/TestArea/xml/presets.xml" );

            string presetFile = openPresetFile.toStdString();
            const char *cstr = presetFile.c_str();

            doc.LoadFile(cstr);


            // parse xml file
            XMLElement* root = doc.FirstChildElement("Presets");

            for(XMLElement* e = root->FirstChildElement("Preset"); e != NULL; e = e->NextSiblingElement("Preset"))
            {
                string presetNameXml = e->Attribute("name");
                QString qPresetNameXml = QString::fromStdString(presetNameXml);
                std::cout << "PRESET:   " + presetNameXml + "\n" << std::endl;

                for(XMLElement* f = e->FirstChildElement("Convert"); f != NULL; f = f->NextSiblingElement("Convert"))
                {
                    string noteBeforeXml = f->FirstChildElement("NoteBefore")->GetText();
                    string noteAfterXml = f->FirstChildElement("NoteAfter")->GetText();
                    string instrumentXml = f->FirstChildElement("Instrument")->GetText();

                    std::cout << "NoteBefore:   " + noteBeforeXml << std::endl;
                    std::cout << "NoteAfter:    " + noteAfterXml << std::endl;
                    std::cout << "Instrument    " + instrumentXml << std::endl;

                    // add current values to vector
                    struct midiConv currentMidiConv {std::stoi(noteBeforeXml),std::stoi(noteAfterXml),instrumentXml,presetNameXml,""};
                    midiConvVector.push_back(currentMidiConv);
                }
                std::cout << "-------------" << std::endl;

                // write values to vector
                struct midiConvPreset currentMidiConvPreset {midiConvVector,presetNameXml};
                midiConvVectorPreset.push_back(currentMidiConvPreset);

                ui->comboBox->addItem(qPresetNameXml);

                midiConvVector.clear();
            }
            throw 1;
        }
        catch(...)
        {
            std::cout << "Crash while parsing xml file" << std::endl;
        }


        currentPreset = 0;
    }
}

