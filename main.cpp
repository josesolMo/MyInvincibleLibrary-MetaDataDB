//
// Created by jose on 05/06/19.
//

#include <iostream>
#include "DataBase.h"
#include <fstream>
#include "json-c/json.h"
#include <string.h>


void writeJson(){
    char buffer[1024];
    FILE *file;
    file = fopen("/home/jose/ProyectosGit/MyInvincibleLibrary-MetaDataDB/metadata/metadata.json", "w");
    if(file!=NULL){
        cout<<"Se va a escribir"<<endl;

        json_object *jobj = json_object_new_object();

        json_object *jstring = json_object_new_string("fotocasa");

        json_object_object_add(jobj,"imagen", jstring);


        strcpy(buffer, json_object_to_json_string(jobj));

        fputs(buffer,file);
    }
}


int main(){

    DataBase *db1 = new DataBase();
    db1->readJson();
    /*
    db1->addGalery("Viajes");

    db1->addImage("Viajes","DMCI1290193");

    db1->addMetadata("Viajes", "DMCI1290193", "fotoplaya", "josesol", "2018", "1200x780", "Una foto del atardecer en la playa");
    //db1->addMetadata("Viajes", "DMCI1290193", "fotoplaya", "josesol", "2018", "1200x780", "Una foto del atardecer en la playa");

    db1->consultMetadata("Viajes", "DMCI1290193");

    //db1->modifyMetadata("Viajes", "DMCI1290193", "NAME", "fotomar");
    //db1->modifyMetadata("Viajes", "DMCI1290193", "AUTHOR", "ruben");
    //db1->modifyMetadata("Viajes", "DMCI1290193", "YEAR", "1996");
    //db1->modifyMetadata("Viajes", "DMCI1290193", "SIZE", "5x5");
    //db1->modifyMetadata("Viajes", "DMCI1290193", "DESCRIPTION", "Una foto del mar azul");



    //db1->deleteMetadata("Viajes", "DMCI1290193");
    //db1->modifyMetadata("Viajes", "DMCI1290193", "NAME", "fotomar");

    //db1->consultMetadata("Viajes", "DMCI1290193");

    //db1->addMetadata("Viajes", "DMCI1290193", "fotoplaya", "josesol", "2018", "1200x780", "Una foto del atardecer en la playa");

    //db1->consultMetadata("Viajes", "DMCI1290193");
    return 0;
     */

    //writeJson();
}


