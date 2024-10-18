#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "birds.h"
#include "QFile"
#include "QMessageBox"
#include "QFileDialog"
#include "QStringListModel"
#include <iostream>
void MainWindow::loadBirds(){
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Megnyitas"), "./", tr("*.txt"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error", "Unable to open the file");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList words = line.split(' ');
        QString _hungarianName;
        QString _scientificName;
        QString _HURING;
        if (words.size() == 4){
            _hungarianName = words[0];
            _scientificName = words[1] + ' ' + words[2];
            _HURING = words[3];
        } else {
            _hungarianName = words[0] + ' ' + words[1];
            _scientificName = words[2] + ' ' + words[3];
            _HURING = words[4];
        }
        Birds bird(_scientificName, _hungarianName, _HURING);
        loadedBirds[_HURING] = bird;
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringListModel *model = new QStringListModel(this);

    // Step 2: Load bird data into the map
    loadBirds();

    // Step 3: Extract bird species names from the map and store in a QStringList
    QStringList birdHURINGs;
    for (Birds entry : loadedBirds) {
        birdHURINGs << entry.getHungarianName();
    }

    // Step 4: Populate the model with the bird species names
    model->setStringList(birdHURINGs);

    // Step 5: Set the model to the QListView
    ui->listView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_searchButton_clicked()
{

}

void MainWindow::filterList(const QString &text) {
    QStringList filteredList;

    // Iterate through the bird map and check if the search text matches any of the attributes
    for (Birds &entry : loadedBirds) {
        QString hungarianName = entry.getHungarianName();
        QString huringCode = entry.getHURING();
        QString scientificName = entry.getScientificName();

        // Check if the search text matches Hungarian name, HURING code, or scientific name
        if (hungarianName.contains(text, Qt::CaseInsensitive) ||
            huringCode.contains(text, Qt::CaseInsensitive) ||
            scientificName.contains(text, Qt::CaseInsensitive)) {
            filteredList << hungarianName;  // Add the Hungarian name to the filtered list
        }
    }

    // Update the model with the filtered list
    QStringListModel *model = new QStringListModel(this);
    model->setStringList(filteredList);
    ui->listView->setModel(model);
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    filterList(ui->lineEdit->text());
}


void MainWindow::on_pushButton_clicked()
{
    QModelIndexList indexes = ui->listView->selectionModel()->selectedIndexes();

    if (!indexes.isEmpty()) {
        // Get the selected Hungarian name from the QModelIndex
        QString selectedHungarianName = indexes.first().data().toString();
        QString hungarianName;
        QString huringCode;
        QString scientificName;
        for (Birds entry : loadedBirds) {

            if (entry.getHungarianName() == selectedHungarianName) {
                // Access all three names: Hungarian name, HURING code, and scientific name
                hungarianName = entry.getHungarianName();
                huringCode = entry.getHURING();
                scientificName = entry.getScientificName();

                // For example, display them using qDebug or set them in some QLabel
                qDebug() << "Hungarian Name:" << hungarianName;
                qDebug() << "HURING Code:" << huringCode;
                qDebug() << "Scientific Name:" << scientificName;

                break;
            }
        }

        Birds bird(
            scientificName,
            hungarianName,
            huringCode,
            ui->catchTypeLine->text(),
            ui->ringNumberLine->text(),
            ui->ageLine->text(),
            ui->genderLine->text().at(0),
            QDate::fromString(ui->catchDateLine->text(), "yyyy.MM.dd")
            );


        ringedBirds[bird.getRingNumber()] = bird;

        QStringList birdList;
        for (Birds entry : ringedBirds){
            birdList << entry.getRingNumber();
        }
        QStringListModel *model = new QStringListModel(this);
        model->setStringList(birdList);
        ui->listView_2->setModel(model);
    }
}


void MainWindow::filterRingedList(const QString &text) {
    QStringList filteredList;

    // Iterate through the bird map and check if the search text matches any of the attributes
    for (Birds &entry : ringedBirds) {
        QString hungarianName = entry.getHungarianName();
        QString huringCode = entry.getHURING();
        QString scientificName = entry.getScientificName();
        QString ringNumber = entry.getRingNumber();

        // Check if the search text matches Hungarian name, HURING code, or scientific name
        if (hungarianName.contains(text, Qt::CaseInsensitive) ||
            huringCode.contains(text, Qt::CaseInsensitive) ||
            scientificName.contains(text, Qt::CaseInsensitive) ||
            ringNumber.contains(text, Qt::CaseInsensitive)) {
            filteredList << ringNumber;  // Add the Hungarian name to the filtered list
        }
    }

    // Update the model with the filtered list
    QStringListModel *model = new QStringListModel(this);
    model->setStringList(filteredList);
    ui->listView_2->setModel(model);
}

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    filterRingedList(ui->lineEdit_2->text());
}


void MainWindow::on_saveButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Mentes"), "./", tr("*.txt"));
    QFile f(fileName);
    if(f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f);
        for (Birds entry : ringedBirds){
            QDate date = entry.getCatchDate();

            out << entry.getScientificName() << ';' << entry.getHungarianName() << ';' << entry.getHURING() << ';' << entry.getCatchType() << ';' << entry.getRingNumber()
                << ';' << entry.getAge() << ';' << entry.getGender() << ';' << entry.getCatchDate().toString("yyyy.MM.dd") << '\n';
        }
    }
}


void MainWindow::on_loadButton_clicked()
{
    ringedBirds.clear();
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Megnyitas"), "./", tr("*.txt"));
    QFile f(fileName);
    if(f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&f);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(';');

            if (fields.size() == 8) {
                QString scientificName = fields[0];
                QString hungarianName = fields[1];
                QString huring = fields[2];
                QString catchType = fields[3];
                QString ringNumber = fields[4];
                QString age = fields[5];
                QChar gender = fields[6].at(0);
                QDate catchDate = QDate::fromString(fields[7], "yyyy.MM.dd");

                ringedBirds[ringNumber] = Birds(scientificName, hungarianName, huring, catchType, ringNumber, age, gender, catchDate);
            } else {
                qDebug() << "Invalid line format:" << line;
            }
        }
        filterRingedList("");
    }

}


void MainWindow::on_listView_2_clicked(const QModelIndex &index)
{
    if (!index.isValid()) return; // Check if the index is valid

    // Get the selected Hungarian name from the QListView
    QString ringNumber = index.data().toString();
    Birds bird = ringedBirds[ringNumber];
    ui->hunLabel->setText(bird.getHungarianName());
    ui->sciLabel->setText(bird.getScientificName());
    ui->huringLabel->setText(bird.getHURING());
    ui->catchtypeLabel->setText(bird.getCatchType());
    ui->ageLabel->setText(bird.getAge());
    ui->genderLabel->setText(bird.getGender());
    ui->dateLabel->setText(bird.getCatchDate().toString("yyyy.MM.dd")); // Format as needed
}

