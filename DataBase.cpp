//
// Created by jose on 05/06/19.
//

#include "DataBase.h"


DataBase::DataBase() {}
/*
/**
 * Metodo que permite agregar galerias
 * @param _galeryName

void DataBase::addGalery(string _galeryName){
    bool galeryexist=0;
    for (Galery *g:galeries){
        if (g->getName()==_galeryName){
            cout<<"Error: The gallery "<<_galeryName<<" already exists"<<endl;
            galeryexist=1;
            break;
        }
    }
    if (not galeryexist){
        Galery *galery = new Galery(_galeryName);
        galeries.push_back(galery);
        cout<<"The gallery has created successfully!"<<endl;
    }
}
*/

/*

void DataBase::addImage(string _galeryName, string _imgId) {
    for (Galery *g:galeries){
        if (g->getName()==_galeryName){
            for(int i=0; i<g->getSize();i++) {
                if (g->recorrer(i)->getId()==_imgId) {
                    cout<<"Error: The image "<<_imgId<<" already exists"<<endl;
                    return;
                }
            }
            imagen *img = new imagen(_imgId);
            g->insertFirst(img);
            cout<<"The image "<<_imgId<<" been added successfully!"<<endl;
            return;
        }
    }
    cout<<"Error: The gallery "<<_galeryName<<" doesn't exist"<<endl;
}
*/

/*
/**
 * Metodo para agragar metadata a una imagen especifica en una galeria especifica
 * @param _galeryName
 * @param _imgId
 * @param _imgName
 * @param _autor
 * @param _year
 * @param _size
 * @param _description

void DataBase::addMetadata(string _galeryName, string _imgId, string _imgName,string _author, string _year,
                           string _size, string _description) {
    for (Galery *g:galeries){
        if (g->getName()==_galeryName){
            cout<<g->getSize()<<endl;
            for(int i=0; i<g->getSize();i++) {
                cout<<g->recorrer(i)->getId()<<endl;
                if (g->recorrer(i)->getId()==_imgId) {
                    g->recorrer(i)->setMetadata(_imgName, _author, _year, _size, _description);
                    cout<<"Metadata has been added successfully!"<<endl;
                    return;
                }
            }
            cout<<"Error: The image "<<_imgId<<" doesn't exist"<<endl;
            return;
        }
    }
    cout<<"Error: The gallery "<<_galeryName<<" doesn't exist"<<endl;
}
*/
/**
 * Metodo que permite consultar la metadata de una imagen especifica en una galeria especifica
 * @param _galeryName
 * @param _imgId
 * @return
 */
DBList DataBase::consultMetadata(string _galeryName, string _imgId) {
    for (Galery *g:galeries){
        if (g->getName()==_galeryName){
            for(int i=0; i<g->getSize();i++) {
                //cout << g->recorrer(i)->getId() << endl;
                if (g->recorrer(i)->getId() == _imgId) {
                    cout<<g->recorrer(i)->getMetadata().recorrer(0)<<endl;
                    cout<<g->recorrer(i)->getMetadata().recorrer(1)<<endl;
                    cout<<g->recorrer(i)->getMetadata().recorrer(2)<<endl;
                    cout<<g->recorrer(i)->getMetadata().recorrer(3)<<endl;
                    cout<<g->recorrer(i)->getMetadata().recorrer(4)<<endl;
                    return g->recorrer(i)->getMetadata();
                }
            }
            cout<<"Error: The meta data for the image "<<_imgId<<" doesn't exist"<<endl;
            DBList x;

            return x;
        }
    }
    cout<<"Error: The gallery "<<_galeryName<<" doesn't exist"<<endl;
}

//void DataBase::deleteMetadata(string _galeryName, string _imgId) {
//    for (Galery *g:galeries){
//        if (g->getName()==_galeryName){
//            for(int i=0; i<g->getSize();i++) {
//                //cout << g->recorrer(i)->getId() << endl;
//                if (g->recorrer(i)->getId() == _imgId) {
//                    g->sacar(i);
//                    cout<<"The metadata has been eliminated successfully!"<<endl;
//                    return;
//                }
//            }
//            cout<<"Error: The meta data for the image "<<_imgId<<" doesn't exist"<<endl;
//            return;
//        }
//    }
//    cout<<"Error: The gallery "<<_galeryName<<" doesn't exist"<<endl;
//}

//void DataBase::modifyMetadata(string _galeryName, string _imgId, string _metadataId, string _data) {
//    for (Galery *g:galeries){
//        if (g->getName()==_galeryName){
//            for(int i=0; i<g->getSize();i++) {
//                //cout << g->recorrer(i)->getId() << endl;
//                if (g->recorrer(i)->getId() == _imgId) {
//                    g->recorrer(i)->setMetadata(_metadataId,_data);
//                    cout<<"The metadata "<<_metadataId<<" has been modified successfully!"<<endl;
//                    return;
//                }
//            }
//            cout<<"Error: The metadata for the image "<<_imgId<<" doesn't exist"<<endl;
//            return;
//        }
//    }
//    cout<<"Error: The gallery "<<_galeryName<<" doesn't exist"<<endl;
//}

void DataBase::readJson(){
    FILE *fp;

    char readbuff[20000];

    struct json_object *parsed_json;
    struct json_object *galleries;
    struct json_object *gallery;
    struct json_object *gallery_name;
    struct json_object *images;
    struct json_object *image;
    struct json_object *image_filename;
    struct json_object *image_name;
    struct json_object *image_author;
    struct json_object *image_year;
    struct json_object *image_size;
    struct json_object *image_desc;

    struct json_object *test;

    int n_galleries;
    int n_images;

    fp = fopen("/home/jose/ProyectosGit/MyInvincibleLibrary-MetaDataDB/metadata/metadata.json","r");

    fread(readbuff, 20000, 1, fp);

    fclose(fp);

    parsed_json = json_tokener_parse(readbuff);


    ////////////////////////////////////////////////////////////
    ///PRUEBA de correcta ejecucion
    json_object_object_get_ex(parsed_json, "PRUEBA", &test);

    cout<<json_object_get_string(test)<<endl;
    ////////////////////////////////////////////////////////////


    ///Ingresa a las GALERIAS
    json_object_object_get_ex(parsed_json, "GALLERIES", &galleries);

    ///Elige una GALERIA para evaluar
    gallery = json_object_array_get_idx(galleries,0);

    ///Obtiene NOMBRE de la GALERIA
    json_object_object_get_ex(gallery, "NAME", &gallery_name);

    ///Obtiene lista de IMAGENES
    json_object_object_get_ex(gallery, "IMAGES", &images);

    ///Elige una IMAGEN para evaluar
    image = json_object_array_get_idx(images, 1);

    ///Obtiene los elementos de la METADATA
    json_object_object_get_ex(image, "FILENAME", &image_filename);
    json_object_object_get_ex(image, "NAME", &image_name);
    json_object_object_get_ex(image, "AUTHOR", &image_author);
    json_object_object_get_ex(image, "YEAR", &image_year);
    json_object_object_get_ex(image, "SIZE", &image_size);
    json_object_object_get_ex(image, "DESCRIPTION", &image_desc);

    ///Obtiene la cantidad de GALERIAS
    n_galleries = json_object_array_length(galleries);
    ///Obtiene la cantidad de IMAGENES
    n_images = json_object_array_length(images);

    cout<<"Cantidad de galerias: "<<n_galleries<<endl;
    cout<<"Cantidad de imagenes: "<<n_images<<endl;

    cout<<"Nombre de la galeria: "<<json_object_get_string(gallery_name)<<endl;

    cout<<"Nombre del archivo: "<<json_object_get_string(image_filename)<<endl;
    cout<<"Nombre de la imagen: "<<json_object_get_string(image_name)<<endl;
    cout<<"Nombre del autor: "<<json_object_get_string(image_author)<<endl;
    cout<<"Ano de la imagen: "<<json_object_get_string(image_year)<<endl;
    cout<<"Tamano de la imagen: "<<json_object_get_string(image_size)<<endl;
    cout<<"Descripcion de la imagen: "<<json_object_get_string(image_desc)<<endl;

}

void DataBase::restartDataBase(){
    char writebuff[1024];

    FILE *file;

    file = fopen("/home/jose/ProyectosGit/MyInvincibleLibrary-MetaDataDB/metadata/metadata.json", "w");

    json_object *jobj = json_object_new_object();
    json_object *jGalleries = json_object_new_array();

    json_object_object_add(jobj,"GALLERIES", jGalleries);

    strcpy(writebuff, json_object_to_json_string(jobj));

    fputs(writebuff, file);

    fclose(file);
}

void DataBase::updateBackup() {
    char updatebuff[20000];

    FILE *file;

    file = fopen("/home/jose/ProyectosGit/MyInvincibleLibrary-MetaDataDB/metadata/metadata.json", "r");

    fread(updatebuff, 20000, 1, file);

    fclose(file);

    file = fopen("/home/jose/ProyectosGit/MyInvincibleLibrary-MetaDataDB/metadata/metadata_backup.json", "w");

    fputs(updatebuff, file);
}

bool DataBase::addGallery(string _galleryName) {
    FILE *fp;

    char readbuff[20000];
    char writebuff[20000];

    struct json_object *parsed_json;
    struct json_object *galleries;
    struct  json_object *gallery;
    struct  json_object *galleryName;

    int n_galleries;

    string evaluar;

    fp = fopen("/home/jose/ProyectosGit/MyInvincibleLibrary-MetaDataDB/metadata/metadata.json","r");

    fread(readbuff, 20000, 1, fp);

    fclose(fp);

    parsed_json = json_tokener_parse(readbuff);

    ///Ingresa a las GALERIAS
    json_object_object_get_ex(parsed_json, "GALLERIES", &galleries);
    ///Obtiene la cantidad de GALERIAS
    n_galleries = json_object_array_length(galleries);

    for (int i=0;i<n_galleries;i++){
        ///Elige una GALERIA para evaluar
        gallery = json_object_array_get_idx(galleries,i);
        ///Obtiene NOMBRE de la GALERIA
        json_object_object_get_ex(gallery, "NAME", &galleryName);

        evaluar = json_object_get_string(galleryName);

        if(evaluar==_galleryName){
            cerr<<"Error: The gallery "<<_galleryName<<" already exists"<<endl;
            return 0;
        }
    }

    ///Crea los objetos JSON para agregar una nueva GALERIA
    json_object *jGallery = json_object_new_object();
    json_object *jGalleryName = json_object_new_string(_galleryName.c_str());
    json_object *jImages = json_object_new_array();

    ///Se asignan las KEYS correspondientes
    json_object_object_add(jGallery, "NAME", jGalleryName);
    json_object_object_add(jGallery, "IMAGES", jImages);

    ///Se agrega la nueva GALERIA al arreglo de GALERIAS
    json_object_array_add(galleries,jGallery);

    ///Se copia el nuevo contenido en el Buffer de escritura
    strcpy(writebuff, json_object_to_json_string(parsed_json));

    //cout<<json_object_to_json_string(parsed_json)<<endl;

    fp = fopen("/home/jose/ProyectosGit/MyInvincibleLibrary-MetaDataDB/metadata/metadata.json","w");
    fputs(writebuff, fp);
    fclose(fp);

    cout<<"The gallery has created successfully!"<<endl;
    return 1;
}

bool DataBase::addImage(string _galleryName, string _imgId) {
    FILE *fp;

    char readbuff[20000];
    char writebuff[20000];

    struct json_object *parsed_json;
    struct json_object *galleries;
    struct  json_object *gallery;
    struct  json_object *galleryName;
    struct  json_object *images;
    struct  json_object *image;
    struct  json_object *image_filename;

    int n_galleries;
    int n_images;

    string evaluarGal;
    string evaluarImg;

    fp = fopen("/home/jose/ProyectosGit/MyInvincibleLibrary-MetaDataDB/metadata/metadata.json","r");

    fread(readbuff, 20000, 1, fp);

    fclose(fp);

    parsed_json = json_tokener_parse(readbuff);

    ///Ingresa a las GALERIAS
    json_object_object_get_ex(parsed_json, "GALLERIES", &galleries);

    ///Obtiene la cantidad de GALERIAS
    n_galleries = json_object_array_length(galleries);


    for (int i=0;i<n_galleries;i++){

        ///Elige una GALERIA para evaluar
        gallery = json_object_array_get_idx(galleries,i);
        ///Obtiene NOMBRE de la GALERIA
        json_object_object_get_ex(gallery, "NAME", &galleryName);

        evaluarGal = json_object_get_string(galleryName);

        if (evaluarGal==_galleryName){


            json_object_object_get_ex(gallery, "IMAGES", &images);

            ///Obtiene la cantidad de IMAGENES
            n_images = json_object_array_length(images);


            for(int j=0; j<n_images;j++) {

                image = json_object_array_get_idx(images, j);

                json_object_object_get_ex(image, "FILENAME", &image_filename);

                evaluarImg = json_object_get_string(image_filename);


                if (evaluarImg==_imgId) {
                    cerr<<"Error: The image "<<_imgId<<" already exists"<<endl;
                    return 0;
                }
            }

            ///Crea los objetos JSON para agregar una nueva IMAGEN
            json_object *jImage = json_object_new_object();
            json_object *jImageFilename = json_object_new_string(_imgId.c_str());
            json_object *jImageName = json_object_new_string("NULL");
            json_object *jImageAuthor= json_object_new_string("NULL");
            json_object *jImageYear = json_object_new_int(0);
            json_object *jImageSize = json_object_new_string("NULL");
            json_object *jImageDescription = json_object_new_string("NULL");


            ///Se asignan las KEYS correspondientes
            json_object_object_add(jImage, "FILENAME", jImageFilename);
            json_object_object_add(jImage, "NAME", jImageName);
            json_object_object_add(jImage, "AUTHOR", jImageAuthor);
            json_object_object_add(jImage, "YEAR", jImageYear);
            json_object_object_add(jImage, "SIZE", jImageSize);
            json_object_object_add(jImage, "DESCRIPTION", jImageDescription);


            json_object_array_add(images, jImage);


            //imagen *img = new imagen(_imgId);
            //g->insertFirst(img);


            ///Se copia el nuevo contenido en el Buffer de escritura
            strcpy(writebuff, json_object_to_json_string(parsed_json));

            //cout<<json_object_to_json_string(parsed_json)<<endl;

            fp = fopen("/home/jose/ProyectosGit/MyInvincibleLibrary-MetaDataDB/metadata/metadata.json","w");
            fputs(writebuff, fp);
            fclose(fp);

            cout<<"The image "<<_imgId<<" been added successfully!"<<endl;

            return 1;

        }
    }
    cerr<<"Error: The gallery "<<_galleryName<<" doesn't exist"<<endl;

    return 0;
}

/**
 * Metodo para agragar metadata a una imagen especifica en una galeria especifica
 * @param _galeryName
 * @param _imgId
 * @param _imgName
 * @param _autor
 * @param _year
 * @param _size
 * @param _description
 * */
void DataBase::addMetadata(string _galleryName, string _imgId, string _imgName,string _author, int _year,
                           string _size, string _description) {
    FILE *fp;

    char readbuff[20000];
    char writebuff[20000];

    struct json_object *parsed_json;
    struct json_object *galleries;
    struct  json_object *gallery;
    struct  json_object *galleryName;
    struct  json_object *images;
    struct  json_object *image;
    struct  json_object *image_filename;

    int n_galleries;
    int n_images;

    string evaluarGal;
    string evaluarImg;

    fp = fopen("/home/jose/ProyectosGit/MyInvincibleLibrary-MetaDataDB/metadata/metadata.json","r");

    fread(readbuff, 20000, 1, fp);

    fclose(fp);

    parsed_json = json_tokener_parse(readbuff);

    ///Ingresa a las GALERIAS
    json_object_object_get_ex(parsed_json, "GALLERIES", &galleries);

    ///Obtiene la cantidad de GALERIAS
    n_galleries = json_object_array_length(galleries);


    for (int i=0;i<n_galleries;i++) {

        ///Elige una GALERIA para evaluar
        gallery = json_object_array_get_idx(galleries, i);
        ///Obtiene NOMBRE de la GALERIA
        json_object_object_get_ex(gallery, "NAME", &galleryName);

        evaluarGal = json_object_get_string(galleryName);

        if (evaluarGal == _galleryName) {


            json_object_object_get_ex(gallery, "IMAGES", &images);

            ///Obtiene la cantidad de IMAGENES
            n_images = json_object_array_length(images);

            for(int j=0; j<n_images;j++) {

                image = json_object_array_get_idx(images, j);

                json_object_object_get_ex(image, "FILENAME", &image_filename);

                evaluarImg = json_object_get_string(image_filename);


                if (evaluarImg == _imgId) {

                    ///Crea los objetos JSON para agregar METADATA a una IMAGEN
                    json_object *jImageName = json_object_new_string(_imgName.c_str());
                    json_object *jImageAuthor = json_object_new_string(_author.c_str());
                    json_object *jImageYear = json_object_new_int(_year);
                    json_object *jImageSize = json_object_new_string(_size.c_str());
                    json_object *jImageDescription = json_object_new_string(_description.c_str());


                    ///Se asignan las KEYS correspondientes
                    json_object_object_add(image, "NAME", jImageName);
                    json_object_object_add(image, "AUTHOR", jImageAuthor);
                    json_object_object_add(image, "YEAR", jImageYear);
                    json_object_object_add(image, "SIZE", jImageSize);
                    json_object_object_add(image, "DESCRIPTION", jImageDescription);


                    cout << json_object_to_json_string(parsed_json) << endl;


                    ///Se copia el nuevo contenido en el Buffer de escritura
                    strcpy(writebuff, json_object_to_json_string(parsed_json));

                    //cout<<json_object_to_json_string(parsed_json)<<endl;

                    fp = fopen("/home/jose/ProyectosGit/MyInvincibleLibrary-MetaDataDB/metadata/metadata.json","w");
                    fputs(writebuff, fp);
                    fclose(fp);


                    return;
                }
            }
            cerr<<"Error: The image "<<_imgId<<" doesn't exist"<<endl;
            return;
        }
    }
    cerr<<"Error: The gallery "<<_galleryName<<" doesn't exist"<<endl;
}

void DataBase::deleteMetadata(string _galleryName, string _imgId) {
    FILE *fp;

    char readbuff[20000];
    char writebuff[20000];

    struct json_object *parsed_json;
    struct json_object *galleries;
    struct  json_object *gallery;
    struct  json_object *galleryName;
    struct  json_object *images;
    struct  json_object *image;
    struct  json_object *image_filename;

    int n_galleries;
    int n_images;

    string evaluarGal;
    string evaluarImg;

    fp = fopen("/home/jose/ProyectosGit/MyInvincibleLibrary-MetaDataDB/metadata/metadata.json","r");

    fread(readbuff, 20000, 1, fp);

    fclose(fp);

    parsed_json = json_tokener_parse(readbuff);

    ///Ingresa a las GALERIAS
    json_object_object_get_ex(parsed_json, "GALLERIES", &galleries);

    ///Obtiene la cantidad de GALERIAS
    n_galleries = json_object_array_length(galleries);


    for (int i=0;i<n_galleries;i++) {

        ///Elige una GALERIA para evaluar
        gallery = json_object_array_get_idx(galleries, i);
        ///Obtiene NOMBRE de la GALERIA
        json_object_object_get_ex(gallery, "NAME", &galleryName);

        evaluarGal = json_object_get_string(galleryName);

        if (evaluarGal == _galleryName) {

            json_object_object_get_ex(gallery, "IMAGES", &images);

            ///Obtiene la cantidad de IMAGENES
            n_images = json_object_array_length(images);

            for(int j=0; j<n_images;j++) {
                cout<<"aca"<<endl;

                image = json_object_array_get_idx(images, j);

                json_object_object_get_ex(image, "FILENAME", &image_filename);

                evaluarImg = json_object_get_string(image_filename);

                cout<<evaluarImg<<endl;

                if (evaluarImg == _imgId) {
                    ///Crea los objetos JSON para agregar METADATA a una IMAGEN
                    json_object *jImageName = json_object_new_string("NULL");
                    json_object *jImageAuthor = json_object_new_string("NULL");
                    json_object *jImageYear = json_object_new_int(0);
                    json_object *jImageSize = json_object_new_string("NULL");
                    json_object *jImageDescription = json_object_new_string("NULL");


                    ///Se asignan las KEYS correspondientes
                    json_object_object_add(image, "NAME", jImageName);
                    json_object_object_add(image, "AUTHOR", jImageAuthor);
                    json_object_object_add(image, "YEAR", jImageYear);
                    json_object_object_add(image, "SIZE", jImageSize);
                    json_object_object_add(image, "DESCRIPTION", jImageDescription);


                    cout << json_object_to_json_string(parsed_json) << endl;


                    ///Se copia el nuevo contenido en el Buffer de escritura
                    strcpy(writebuff, json_object_to_json_string(parsed_json));

                    cout<<"Metadata deleted successfully!"<<endl;

                    fp = fopen("/home/jose/ProyectosGit/MyInvincibleLibrary-MetaDataDB/metadata/metadata.json","w");
                    fputs(writebuff, fp);
                    fclose(fp);

                    return;
                }
            }
            cerr<<"Error: The image "<<_imgId<<" doesn't exist"<<endl;
            return;
        }
    }
    cerr<<"Error: The gallery "<<_galleryName<<" doesn't exist"<<endl;
}

void DataBase::modifyMetadata(string _galleryName, string _imgId, string _metadataId, string _data) {

    FILE *fp;

    char readbuff[20000];
    char writebuff[20000];

    struct json_object *parsed_json;
    struct json_object *galleries;
    struct json_object *gallery;
    struct json_object *galleryName;
    struct json_object *images;
    struct json_object *image;
    struct json_object *image_filename;

    int n_galleries;
    int n_images;

    string evaluarGal;
    string evaluarImg;

    fp = fopen("/home/jose/ProyectosGit/MyInvincibleLibrary-MetaDataDB/metadata/metadata.json", "r");

    fread(readbuff, 20000, 1, fp);

    fclose(fp);

    parsed_json = json_tokener_parse(readbuff);

    ///Ingresa a las GALERIAS
    json_object_object_get_ex(parsed_json, "GALLERIES", &galleries);

    ///Obtiene la cantidad de GALERIAS
    n_galleries = json_object_array_length(galleries);


    for (int i = 0; i < n_galleries; i++) {

        ///Elige una GALERIA para evaluar
        gallery = json_object_array_get_idx(galleries, i);
        ///Obtiene NOMBRE de la GALERIA
        json_object_object_get_ex(gallery, "NAME", &galleryName);

        evaluarGal = json_object_get_string(galleryName);

        if (evaluarGal == _galleryName) {

            json_object_object_get_ex(gallery, "IMAGES", &images);

            ///Obtiene la cantidad de IMAGENES
            n_images = json_object_array_length(images);

            for (int j = 0; j < n_images; j++) {
                cout << "aca" << endl;

                image = json_object_array_get_idx(images, j);

                json_object_object_get_ex(image, "FILENAME", &image_filename);

                evaluarImg = json_object_get_string(image_filename);

                cout << evaluarImg << endl;

                if (evaluarImg == _imgId) {

                    if (_metadataId=="NAME"){
                        json_object *jImageName = json_object_new_string(_data.c_str());
                        json_object_object_add(image, "NAME", jImageName);
                    }else if(_metadataId=="AUTHOR"){
                        json_object *jImageAuthor = json_object_new_string(_data.c_str());
                        json_object_object_add(image, "AUTHOR", jImageAuthor);
                    }else if(_metadataId=="YEAR"){
                        json_object *jImageYear = json_object_new_int(stoi(_data));
                        json_object_object_add(image, "YEAR", jImageYear);
                    }else if(_metadataId=="SIZE"){
                        json_object *jImageSize = json_object_new_string(_data.c_str());
                        json_object_object_add(image, "SIZE", jImageSize);
                    }else if(_metadataId=="DESCRIPTION"){
                        json_object *jImageDescription = json_object_new_string(_data.c_str());
                        json_object_object_add(image, "DESCRIPTION", jImageDescription);
                    }


                    cout << json_object_to_json_string(parsed_json) << endl;


                    ///Se copia el nuevo contenido en el Buffer de escritura
                    strcpy(writebuff, json_object_to_json_string(parsed_json));

                    cout << "Metadata modified successfully!" << endl;

                    fp = fopen("/home/jose/ProyectosGit/MyInvincibleLibrary-MetaDataDB/metadata/metadata.json", "w");
                    fputs(writebuff, fp);
                    fclose(fp);

                    return;
                }
            }
            cerr << "Error: The image " << _imgId << " doesn't exist" << endl;
            return;
        }
    }
    cerr << "Error: The gallery " << _galleryName << " doesn't exist" << endl;
}

