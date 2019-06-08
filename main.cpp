//
// Created by jose on 05/06/19.
//

#include "DataBase.h"

int main(){
    DataBase *db1 = new DataBase();
    db1->addGalery("Viajes");

    db1->addMetadata("Viajes", "DMCI1290193", "fotoplaya", "josesol", "2018", "1200x780", "Una foto del atardecer en la playa");
    //db1->addMetadata("Viajes", "DMCI1290193", "fotoplaya", "josesol", "2018", "1200x780", "Una foto del atardecer en la playa");

    db1->consultMetadata("Viajes", "DMCI1290193");

    db1->deleteMetadata("Viajes", "DMCI1290193");

    db1->consultMetadata("Viajes", "DMCI1290193");

    db1->addMetadata("Viajes", "DMCI1290193", "fotoplaya", "josesol", "2018", "1200x780", "Una foto del atardecer en la playa");

    db1->consultMetadata("Viajes", "DMCI1290193");
    return 0;
}
