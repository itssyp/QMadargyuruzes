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


    loadBirds();
    sessionBirds.clear();


    QStringList birdHURINGs;
    for (Birds entry : loadedBirds) {
        birdHURINGs << entry.getHungarianName();
    }

    model->setStringList(birdHURINGs);

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

    for (Birds &entry : loadedBirds) {
        QString hungarianName = entry.getHungarianName();
        QString huringCode = entry.getHURING();
        QString scientificName = entry.getScientificName();

        if (hungarianName.contains(text, Qt::CaseInsensitive) ||
            huringCode.contains(text, Qt::CaseInsensitive) ||
            scientificName.contains(text, Qt::CaseInsensitive)) {
            filteredList << hungarianName;
        }
    }

    QStringListModel *model = new QStringListModel(this);
    model->setStringList(filteredList);
    ui->listView->setModel(model);
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    filterList(ui->lineEdit->text());
}

void MainWindow::updateTop3List() {
    std::vector<std::pair<QString, int>> birdCounts;

    for (auto it = sessionBirds.begin(); it != sessionBirds.end(); ++it) {
        birdCounts.push_back(std::make_pair(it.key(), it.value()));
    }

    std::sort(birdCounts.begin(), birdCounts.end(),
              [](const std::pair<QString, int>& a, const std::pair<QString, int>& b) {
                  return a.second > b.second;
              });

    QStringList top3List;
    int count = 0;
    for (const auto& pair : birdCounts) {
        if (count < 3) {
            top3List << pair.first + " (" + QString::number(pair.second) + ")";
            count++;
        } else {
            break;
        }
    }

    QStringListModel *model = new QStringListModel(this);
    model->setStringList(top3List);
    ui->listView_recommendation->setModel(model);
}



void MainWindow::on_pushButton_clicked()
{
    bool rec = false;
    QModelIndexList indexes;
    if ( ui->listView->selectionModel()->hasSelection()){
        indexes = ui->listView->selectionModel()->selectedIndexes();
    } else {
        indexes = ui->listView_recommendation->selectionModel()->selectedIndexes();
        rec = true;
    }

    if (!indexes.isEmpty()) {
        QString selectedHungarianName = indexes.first().data().toString();
        if (rec) selectedHungarianName = selectedHungarianName.left(selectedHungarianName.length()-4);
        QString hungarianName;
        QString huringCode;
        QString scientificName;
        for (Birds entry : loadedBirds) {

            if (entry.getHungarianName() == selectedHungarianName) {
                hungarianName = entry.getHungarianName();
                huringCode = entry.getHURING();
                scientificName = entry.getScientificName();

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
        sessionBirds[bird.getHungarianName()]++;
        updateTop3List();

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

    for (Birds &entry : ringedBirds) {
        QString hungarianName = entry.getHungarianName();
        QString huringCode = entry.getHURING();
        QString scientificName = entry.getScientificName();
        QString ringNumber = entry.getRingNumber();

        if (hungarianName.contains(text, Qt::CaseInsensitive) ||
            huringCode.contains(text, Qt::CaseInsensitive) ||
            scientificName.contains(text, Qt::CaseInsensitive) ||
            ringNumber.contains(text, Qt::CaseInsensitive)) {
            filteredList << ringNumber;
        }
    }

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
    if (!index.isValid()) return;

    QString ringNumber = index.data().toString();
    Birds bird = ringedBirds[ringNumber];
    ui->hunLabel->setText(bird.getHungarianName());
    ui->sciLabel->setText(bird.getScientificName());
    ui->huringLabel->setText(bird.getHURING());
    ui->catchtypeLabel->setText(bird.getCatchType());
    ui->ageLabel->setText(bird.getAge());
    ui->genderLabel->setText(bird.getGender());
    ui->dateLabel->setText(bird.getCatchDate().toString("yyyy.MM.dd"));
}


void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    ui->listView_recommendation->clearSelection();
}


void MainWindow::on_listView_recommendation_clicked(const QModelIndex &index)
{
    ui->listView->clearSelection();
}

