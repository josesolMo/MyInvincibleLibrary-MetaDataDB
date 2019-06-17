
#include <iostream>
#include "DataBase.h"
#include <fstream>
#include "json-c/json.h"
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include "huffman_helper.h"
#include "SQLController.h"

#define PORT 3550
#define BACKLOG 4
#define MAXDATASIZE 1000

using namespace std;


/**
 * Main de MetadataDB
 *
 * @since 05/06/19
 */


static SQLController *sqlController;

//static DataBase *dataBase;



string sendNewGallery(string _newGallery) {

    json_object *jobj = json_object_new_object();
    json_object *jresponse;

    if(sqlController->getDataBase()->addGallery(_newGallery)){
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

    if(sqlController->getDataBase()->addImage(_galleryName, _newImage)){
        jresponse = json_object_new_string("1");
    }else{
        jresponse = json_object_new_string("0");
    }

    json_object_object_add(jobj, "NEWIMAGE", jresponse);

    return json_object_to_json_string(jobj);
}



string sendConsole(string _console, string _key) {

    //vector<vector<string>> matrix = sqlController->makeFunction(_console);

    vector<vector<string>> matrix;

    vector<string> test1;

    for(int i=0;i<3;i++){
        test1.push_back("");
    }
    for(int i=0;i<6;i++){
        matrix.push_back(test1);
    }

    matrix[0][0]="FILE_ID";
    matrix[1][0]="NAME";
    matrix[2][0]="AUTHOR";
    matrix[3][0]="YEAR";
    matrix[4][0]="SIZE";
    matrix[5][0]="DESCRIPTION";

    matrix[0][1]="jiabsffbjas";
    matrix[0][2]="fjbjjssjkjk";

    matrix[1][1]="fotogay1";
    matrix[1][2]="foto_orgia";

    matrix[2][1]="Ruben";
    matrix[2][2]="Jose";

    matrix[3][1]="2018";
    matrix[3][2]="2019";

    matrix[4][1]="1080x720";
    matrix[4][2]="720x480";

    matrix[5][1]="Una foto muy gay";
    matrix[5][2]="Una foto de un fiesta loca";


    json_object *jobj = json_object_new_object();
    json_object *jarray1 = json_object_new_array();
    json_object *jarray2;
    json_object *jstrings;

    int len1 = matrix.size();
    int len2 = matrix[0].size();

    for(int i = 0;i<len1;i++){
        jarray2 = json_object_new_array();
        for(int j = 0;j<len2;j++){
            jstrings = json_object_new_string(matrix[i][j].c_str());
            json_object_array_add(jarray2, jstrings);
        }
        json_object_array_add(jarray1, jarray2);
    }


    json_object_object_add(jobj, "CONSOLE", jarray1);

    //cout<<json_object_to_json_string(jobj)<<endl;

    return json_object_to_json_string(jobj);



}

string sendInitial(){
    vector<vector<string>> matrix = sqlController->getDataBase()->getAllGalleries();

    int len1 = matrix.size();
    int len2;

    json_object *jobj = json_object_new_object();
    json_object *jarray1 = json_object_new_array();
    json_object *jarray2;
    json_object *jstrings;

    for(int i = 0;i<len1;i++){
        jarray2 = json_object_new_array();
        len2 = matrix[i].size();
        for(int j = 0;j<len2;j++){
            jstrings = json_object_new_string(matrix[i][j].c_str());
            json_object_array_add(jarray2, jstrings);
        }
        json_object_array_add(jarray1, jarray2);
    }

    json_object_object_add(jobj, "INITIAL", jarray1);

    return json_object_to_json_string(jobj);
}

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

            ///KEY: CONSOLE
            ///Obtiene el nombre de la galeria asociada a la direccion que se quiere acceder
            struct json_object *tempConsole;
            //cout<<"GALLERY"<<endl;
            json_object *parsed_jsonConsole = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonConsole, "CONSOLE", &tempConsole);

            ///KEY: INITIAL
            ///Obtiene el nombre de la galeria asociada a la direccion que se quiere acceder
            struct json_object *tempInitial;
            //cout<<"GALLERY"<<endl;
            json_object *parsed_jsonInitial = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonInitial, "INITIAL", &tempInitial);

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

            ///Obtendra un request para obtener la galeria
            ///Verifica que reciba los KEYS: CONSOLE
            if (json_object_get_string(tempConsole) != nullptr) {
                ///JSON saliente del servidor
                string tablas = sendConsole(json_object_get_string(tempConsole),"CONSOLE");

                cout << tablas << endl;

                ///Envio al cliente
                send(fd2, tablas.c_str(), MAXDATASIZE, 0);
            }

            ///Obtendra un request para obtener la galeria
            ///Verifica que reciba los KEYS: INITIAL
            if (json_object_get_string(tempInitial) != nullptr) {
                ///JSON saliente del servidor
                string initial = sendInitial();

                cout << initial << endl;

                ///Envio al cliente
                send(fd2, initial.c_str(), MAXDATASIZE, 0);
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

    ///Instanciacion del objeto de SQLController
    sqlController = new SQLController();

    ///Corre el servidor
    runServer();


    //huffman_helper *hh = new huffman_helper();

    //hh->compress("mama amasa");
//
//    char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
//    int freq[] = { 65, 9, 12, 13, 16, 45 };
//
//    int size = sizeof(arr) / sizeof(arr[0]);
//
//    cout<< sizeof(arr)<<endl;
//
//    hh->HuffmanCodes(arr, freq, size);



//    db1->restartDataBase();
//    db1->updateBackup();
//    dataBase->addGallery("Carros");
//    dataBase->addImage("Carros", "DCMI124");
//    dataBase->addMetadata("Carros", "DCMI124","Toyota", "Ruben", 2011,"20x20", "Un auto feo");
//
//    dataBase->addImage("Carros", "DCMI125");
//    dataBase->addMetadata("Carros", "DCMI125","Lamborgini", "Andrey", 2003,"70x70", "Una mierda");
//    db1->deleteMetadata("Carros", "DCMI123");
//    db1->modifyMetadata("Carros", "DCMI123","AUTHOR","RubyRuby");
//    dataBase->consultMetadata("Carros", "DCMI123", "NAME");

    //dataBase->getColumn("Carros", "DESCRIPTION");



//    vector<vector<string>> matrix;
//
//    vector<string> test1;
//
//    for(int i=0;i<3;i++){
//        test1.push_back("");
//    }
//    for(int i=0;i<5;i++){
//        matrix.push_back(test1);
//    }
//
//    matrix[0][0]="NAME";
//    matrix[1][0]="AUTHOR";
//    matrix[2][0]="YEAR";
//    matrix[3][0]="SIZE";
//    matrix[4][0]="DESCRIPTION";
//
//    matrix[0][1]="fotogay1";
//    matrix[0][2]="foto_orgia";
//
//    matrix[1][1]="Ruben";
//    matrix[1][2]="Jose";
//
//    matrix[2][1]="2018";
//    matrix[2][2]="2019";
//
//    matrix[3][1]="1080x720";
//    matrix[3][2]="720x480";
//
//    matrix[4][1]="Una foto muy gay";
//    matrix[4][2]="Una foto de un fiesta loca";
//
//
//    json_object *jobj = json_object_new_object();
//    json_object *jarray1 = json_object_new_array();
//    json_object *jarray2;
//    json_object *jstrings;
//
//    int len1 = matrix.size();
//    int len2 = matrix[0].size();
//
//    for(int i = 0;i<len1;i++){
//        jarray2 = json_object_new_array();
//        for(int j = 0;j<len2;j++){
//            jstrings = json_object_new_string(matrix[i][j].c_str());
//            json_object_array_add(jarray2, jstrings);
//        }
//        json_object_array_add(jarray1, jarray2);
//    }
//
//
//    json_object_object_add(jobj, "CONSOLE", jarray1);
//
//    cout<<json_object_to_json_string(jobj)<<endl;

//    DataBase *dataBase = new DataBase();
//    dataBase->getAllGalleries();
//
//    dataBase->getRow("Carros", 1);
//    dataBase->getColumnSize("Carros");

    return 0;
}


