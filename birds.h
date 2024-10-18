#ifndef BIRDS_H
#define BIRDS_H
#include <QString>
#include <QChar>
#include <QDate>
class Birds
{
    QString scientificName;
    QString hungarianName;
    QString HURING;
    QString catchType;
    QString ringNumber;
    QString age;
    QChar gender;
    QDate catchDate;
public:
    Birds(QString _scientificName, QString _hungarianName, QString _HURING, QString _catchType, QString _ringNumber, QString _age, QChar _gender, QDate _catchDate)
        : scientificName(_scientificName), hungarianName(_hungarianName), HURING(_HURING), catchType(_catchType), ringNumber(_ringNumber), age(_age), gender(_gender), catchDate(_catchDate){}
    Birds(QString _scientificName, QString _hungarianName, QString _HURING)
        : scientificName(_scientificName), hungarianName(_hungarianName), HURING(_HURING){}
    Birds();

    QString getHURING();
    QString getHungarianName();
    QString getScientificName();
    QString getRingNumber();
    QString getCatchType();
    QString getAge();
    QChar getGender();
    QDate getCatchDate();
};

#endif // BIRDS_H
