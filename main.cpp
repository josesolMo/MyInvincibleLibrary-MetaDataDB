//
// Created by jose on 05/06/19.
//

#include <iostream>
#include "DataBase.h"
#include <fstream>
#include "json-c/json.h"
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 3550
#define BACKLOG 4
#define MAXDATASIZE 1000

using namespace std;

static DataBase *dataBase;



string sendNewGallery(string _newGallery) {

    json_object *jobj = json_object_new_object();
    json_object *jresponse;

    if(dataBase->addGallery(_newGallery)){
        jresponse = json_object_new_string("1");
    }else{
        jresponse = json_object_new_string("0");
    }
    json_object_object_add(jobj, "NEWGALLERY", jresponse);

    return json_object_to_json_string(jobj);
}

string sendNewImage(string _galleryName, string _newImage){
    json_object *jobj = json_object_new_object();
    json_object *jresponse;

    if(dataBase->addImage(_galleryName, _newImage)){
        jresponse = json_object_new_string("1");
    }else{
        jresponse = json_object_new_string("0");
    }

    json_object_object_add(jobj, "NEWIMAGE", jresponse);

    return json_object_to_json_string(jobj);
}

string sendGallery(string gallery, string arrowIndex) {}

string sendFile(string gallery, string arrowIndex) {}

string sendBinary(string gallery, string arrowIndex) {}

/**
 * Corre el servidor
 * @return int
 */
int runServer() {

    int fd, fd2;

    struct sockaddr_in server;

    struct sockaddr_in client;


    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("error en socket()\n");
        exit(-1);
    }

    server.sin_family = AF_INET;

    server.sin_port = htons(PORT);

    server.sin_addr.s_addr = INADDR_ANY;

    bzero(&(server.sin_zero), 8);

    if (bind(fd, (struct sockaddr *) &server,
             sizeof(struct sockaddr)) == -1) {
        printf("error en bind() \n");
        exit(-1);
    }

    if (listen(fd, BACKLOG) == -1) {
        printf("error en listen()\n");
        exit(-1);
    }

    printf("\nINVINCIBLE DATABASE opened!.\n");

    while (true) {

        unsigned int address_size = sizeof(client);

        if ((fd2 = accept(fd, (struct sockaddr *) &client,
                          &address_size)) == -1) {
            printf("error en accept()\n");
            exit(-1);
        }

        printf("\nSe obtuvo una conexión de un cliente.\n");

        ssize_t r;

        char buff[MAXDATASIZE];

        for (;;) {
            r = read(fd2, buff, MAXDATASIZE);

            if (r == -1) {
                perror("read");
                return EXIT_FAILURE;
            }
            if (r == 0)
                break;
            printf("\nREAD: %s\n", buff);



            ///JSON Reads



            ///KEY: NEWGALLERY
            ///Obtiene el nombre de la nueva galeria para verificar si puede ser guardada.
            struct json_object *tempNewGallery;
            //cout<<"NEWGALLERY"<<endl;
            json_object *parsed_jsonNewGallery = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonNewGallery, "NEWGALLERY", &tempNewGallery);

            ///KEY: NEWIMAGE
            ///Obtiene el nombre de la nueva imagen para verificar si puede ser guardada.
            struct json_object *tempNewImage;
            //cout<<"NEWIMAGE"<<endl;
            json_object *parsed_jsonNewImage = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonNewImage, "NEWIMAGE", &tempNewImage);

            ///KEY: GALLERY
            ///Obtiene el nombre de la galeria asociada a la direccion que se quiere acceder
            struct json_object *tempGallery;
            //cout<<"GALLERY"<<endl;
            json_object *parsed_jsonGallery = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonGallery, "GALLERY", &tempGallery);



            /*
            ///KEY: TEMPLATE
            ///Obtiene un request para
            struct json_object *tempZZ;
            cout<<"ZZ"<<endl;
            json_object *parsed_jsonZZ = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonZZ, "ZZ", &tempZZ);
             */





            ///JSON Writes



            ///Obtendra un request para obtener la galeria
            ///Verifica que reciba los KEYS: NEWGALLERY
            if (json_object_get_string(tempNewGallery) != nullptr ) {
                ///JSON saliente del servidor
                string newgallery = sendNewGallery(json_object_get_string(tempNewGallery));

                cout << newgallery << endl;

                ///Envio al cliente
                send(fd2, newgallery.c_str(), MAXDATASIZE, 0);
            }

            ///Obtendra un request para obtener la galeria
            ///Verifica que reciba los KEYS: NEWIMAGE
            if (json_object_get_string(tempNewImage) != nullptr && json_object_get_string(tempGallery)!=nullptr ) {
                ///JSON saliente del servidor
                string newimage = sendNewImage(json_object_get_string(tempGallery),
                                               json_object_get_string(tempNewImage));

                cout << newimage << endl;

                ///Envio al cliente
                send(fd2, newimage.c_str(), MAXDATASIZE, 0);
            }






            /*
            ///Obtendra un request para obtener
            ///Verifica que reciba los KEYS: TEMPLATE
            if (json_object_get_string(tempZZ) != nullptr ) {
                ///JSON saliente del servidor
                string aTypeZZ = sendAType("ZZ",json_object_get_string(tempZZ));
                ///Envio al cliente
                send(fd2, aTypeZZ.c_str(), MAXDATASIZE, 0);
            }

            */


        }

        ///Reestablece el buffer
        memset(buff, 0, MAXDATASIZE);

        cout <<
             "\n\n--------------------------------------------------------------------------------"
             "END--------------------------------------------------------------------------------\n"
             << endl;

    }

    close(fd2);

}

int main(){

    dataBase = new DataBase();
    //runServer();
    //db1->restartDataBase();
    //db1->updateBackup();
    //dataBase->addGallery("Carros");
    //dataBase->addImage("Carros", "DCMI123");
    //dataBase->addMetadata("Carros", "DCMI123","Ferrari", "Edgitar", 2010,"1080x720", "Un auto deportivo rojo");
    //db1->deleteMetadata("Carros", "DCMI123");
    //db1->modifyMetadata("Carros", "DCMI123","AUTHOR","RubyRuby");
    dataBase->consultMetadata("Carros", "DCMI123", "NAME");

    return 0;
}


