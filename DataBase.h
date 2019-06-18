//
// Created by jose on 05/06/19.
//

#ifndef MYINVINCIBLELIBRARY_METADATADB_DATABASE_H
#define MYINVINCIBLELIBRARY_METADATADB_DATABASE_H


#include <json-c/json.h>
#include <string.h>
#include <iostream>
#include <vector>

using namespace std;

class DataBase {

public:
    /**
     * Constructor de DataBase
     */
    DataBase();

    /**
     * Método que agrega galerías
     * @param _galleryName
     * @return
     */
    bool addGallery(string _galleryName);

    /**
     * Método que agraga imágenes
     * @param _galleryName
     * @param _imgId
     * @return
     */
    bool addImage(string _galleryName, string _imgId);

    /**
     * Método para agregar la metadata
     * @param _galleryName
     * @param _imgId
     * @param _imgName
     * @param _author
     * @param _year
     * @param _size
     * @param _description
     */
    void addMetadata(string _galleryName, string _imgId, string _imgName,
                                                         string _author,
                                                         int _year,
                                                         string _size,
                                                         string _description);


    /**
     * Método que consulta un dato específico de una imágen en específico
     * @param _galleryName
     * @param _imgId
     * @param _metadataId
     * @return
     */
    string consultMetadata(string _galleryName, string _imgId, string _metadataId);

    /**
     * Método que elimina toda la metadata de una imágen y la setea en NULL
     * @param _galleryName
     * @param _imgId
     * @return
     */
    vector<vector<string>> deleteMetadata(string _galleryName, string _imgId);

    /**
     * Método que modifica un dato de la metadata en específico de una imágen en específico
     * @param _galleryName
     * @param _imgId
     * @param _metadataId
     * @param _data
     * @return
     */
    vector<vector<string>> modifyMetadata(string _galleryName, string _imgId, string _metadataId, string _data);

    /**
     * Se utiliza para reiniciar el archivo JSON metadata
     */
    void restartDataBase();

    /**
     * Se utiliza para hacer commit, actualiza el metadata_backup.json
     */
    void updateBackup();

    /**
     * Restablece el metadata.json, copia del metadata_backup.json
     */
    void restoreFromBackup();

    /**
     * Obtiene la columna de una tabla(Galería) de un dato en específico
     * @param _galleryName
     * @param _columnId
     * @return
     */
    vector<string> getColumn(string _galleryName, string _columnId);

    /**
     * Obtiene la fila de una tabla(Galería) mediante un indice
     * @param _galleryName
     * @param index
     * @return
     */
    vector<string> getRow(string _galleryName, int index);

    /**
     * Obtiene la columna de años en forma numérica
     * @param _galleryName
     * @return
     */
    vector<int> getColumnYear(string _galleryName);

    /**
     * Obtiene la columna del tamaños de forma numérica
     * @param _galleryName
     * @return
     */
    vector<int> getColumnSize(string _galleryName);

    /**
     * Obtiene un vector de vectores con todas las galerías que existen en el archivo metadata.json
     * @return
     */
    vector<vector<string>> getAllGalleries();

    /**
     * Elimina una galería en específico junt a sus imágenes almacenadas
     * @param _galleryName
     * @return
     */
    string deleteGallery(string _galleryName);

private:



};


#endif //MYINVINCIBLELIBRARY_METADATADB_DATABASE_H
