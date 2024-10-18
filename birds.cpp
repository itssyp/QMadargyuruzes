#include "birds.h"

Birds::Birds() {

}

QString Birds::getHURING(){
    return HURING;
}

QString Birds::getHungarianName(){
    return hungarianName;
}

QString Birds::getScientificName(){
    return scientificName;
}

QString Birds::getRingNumber(){
    return ringNumber;
}

QString Birds::getCatchType(){
    return catchType;
}

QString Birds::getAge(){
    return age;
}

QChar Birds::getGender(){
    return gender;
}

QDate Birds::getCatchDate(){
    return catchDate;
}
