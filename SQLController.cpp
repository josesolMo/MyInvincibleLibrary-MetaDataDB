
#include "SQLController.h"
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
#include <vector>


/**
 * Controlador del manejo de la sintaxis SQL.
 *
 * @since 13/06/19
 */


using namespace std;
/**
 * Constructor
 */
SQLController::SQLController() {
    dataBase = new DataBase();
}


///Metodos

vector<vector<string>> SQLController::funcionInsert(string comando)
{
    vector<vector<string>> matrix;
    vector <string> verify;
    string subs = comando.substr(0,5);
    if (subs.compare("INTO ") == 0 ||subs.compare("into ") == 0){
        subs = comando.substr(5);
        size_t space = subs.find(" ");
        if (subs.length()< space){
            cout << "Syntax error" << endl;
            ///database.sendJSON("CONSOLE","0");
            verify.push_back("0");
            matrix.push_back(verify);
            return matrix;
        }
        string imagen = subs.substr(0, space);
        cout << imagen << endl;
        subs = subs.substr(space+1);
        vector <string> order;
        if((subs.substr(0,1)).compare("(") == 0){
            size_t columnsEnd = subs.find(")");
            if (subs.length()< columnsEnd){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string columns = subs.substr(1, columnsEnd);
            cout << columns << endl;
            subs = subs.substr(columnsEnd+1);
            while (columns.compare(")") != 0){
                size_t coma = columns.find(",");
                string current;
                if (columns.length()< coma){
                    current = columns.substr(0, columns.length()-1);
                    //cout << current << endl;
                    columns = columns.substr(current.length(),columns.length()-1);
                }
                else {
                    current = columns.substr(0, coma);
                    //cout << current << endl;
                    columns = columns.substr(coma+1);
                    //cout << columns << endl;
                }
                if(current[0] == ' '){
                    current = current.substr(1);
                }
                if (current[current.length()-1] == ' '){
                    current = current.substr(0, current.length()-1);
                }
                cout << current << endl;
                cout << columns << endl;
                current = columnaGET(current);
                if(current.compare("F") == 0){
                    cout << "No existe ese dato en la tabla" << endl;
                    verify.push_back("0");
                    matrix.push_back(verify);
                    return matrix;
                }
                else {
                    order.push_back(current);
                }
            }
            int C = 0;
            while (C < order.size()){
                cout << order[C] << endl;
                C++;
            }
        }
        else {
            order.push_back("A");
            order.push_back("B");
            order.push_back("C");
            order.push_back("D");
            order.push_back("E");

        }
        size_t values = subs.find("VALUES ");
        if(values > 2){
            cout << "Syntax error" << endl;
            verify.push_back("0");
            matrix.push_back(verify);
            return matrix;
        }

        subs = subs.substr(values+7);
        cout << subs << endl;

        if((subs.substr(0,1)).compare("(") == 0){
            size_t valuesEnd = subs.find(")");
            if (subs.length()< valuesEnd){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string values = subs.substr(1, valuesEnd);
            cout << values << endl;
            int c = 0;
            while (values.compare(")") != 0){
                size_t coma = values.find(",");
                string current;
                if (values.length()< coma){
                    current = values.substr(0, values.length()-1);
                    cout << current << endl;
                    values = values.substr(current.length(),values.length()-1);
                    int min = order.size()-1;
                    if(c != min){
                        cout << "Faltan valores o sobran" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                }
                else {
                    current = values.substr(0, coma);
                    cout << current << endl;
                    values = values.substr(coma+1);
                    cout << values << endl;
                }
                if(current[0] == '"'){
                    current = current.substr(1);
                }
                if (current[current.length()-1] == '"'){
                    current = current.substr(0, current.length()-1);
                }
                //cout << current << endl;
                //cout << columns << endl;
                addToTable(order[c],current, imagen);
                c++;
            }
            //ui->LineaCMD->clear();
            verify.push_back("1");
            matrix.push_back(verify);
            return matrix;
        }
        else {
            cout << "Syntax error" << endl;
            verify.push_back("0");
            matrix.push_back(verify);
            return matrix;
        }
    }
    else {
        cout << "Syntax error" << endl;
        verify.push_back("0");
        matrix.push_back(verify);
        return matrix;
    }
}

vector<vector<string>> SQLController::funcionSelect(string comando)
{
    vector<vector<string>> matrix;
    vector <string> verify;
    string subs = comando;
    cout << subs << endl;
    if (subs[0] == '*'){
        size_t From = subs.find("FROM ");
        if (3 < From){
            cout << "Syntax error" << endl;
            verify.push_back("0");
            matrix.push_back(verify);
            return matrix;
        }
        subs = subs.substr(From+5);
        size_t pycoma = subs.find(";");
        if (subs.length()< pycoma){
            cout << "Syntax error" << endl;
            verify.push_back("0");
            matrix.push_back(verify);
            return matrix;
        }
        string tabla = subs.substr(0, pycoma);
        cout << tabla << endl;
        if(tabla[0] == ' '){
            tabla = tabla.substr(1);
        }
        if (tabla[tabla.length()-1] == ' '){
            tabla = tabla.substr(0, tabla.length()-1);
        }
        subs = subs.substr(pycoma+1);
        cout << subs << endl;
        if (dataBase->getColumn(tabla,"NAME")[0].compare("ERROR") == 0){
            verify.push_back("0");
            matrix.push_back(verify);
            return matrix;
        }
        if(subs.compare("") == 0 || subs.compare(" ") == 0){
            cout << "Imprimir valores de tabla" << endl;
            matrix.push_back(dataBase->getColumn(tabla,"NAME"));
            matrix.push_back(dataBase->getColumn(tabla,"AUTHOR"));
            matrix.push_back(dataBase->getColumn(tabla,"YEAR"));
            matrix.push_back(dataBase->getColumn(tabla,"SIZE"));
            matrix.push_back(dataBase->getColumn(tabla,"DESCRIPTION"));
            return matrix;
        }
        else{
            size_t where = subs.find("WHERE ");
            if (2 < where){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            subs = subs.substr(where+6);
            if (subs.substr(0,4).compare("NOT ") == 0){
                subs = subs.substr(4);
                size_t comand = subs.find("=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR
                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR
                    vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i].compare(value) != 0){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                    return matrix;
                }


                comand = subs.find("<");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] >= sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] >= v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                    return matrix;
                }

                comand = subs.find(">");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] <= sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] <= v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                    return matrix;
                }

                comand = subs.find(">=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] < sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] < v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                    return matrix;
                }

                comand = subs.find("<=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] > sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] > v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                    return matrix;
                }

                else{
                    cout << "Syntax error" << endl;
                    verify.push_back("0");
                    matrix.push_back(verify);
                    return matrix;
                }

            }
            else {

                size_t comand = subs.find("BETWEEN ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);


                    subs = subs.substr(comand+8);
                    size_t notCmd = column.find(" NOT ");
                    bool notcmd;
                    if (column.length()< notcmd){
                        notcmd = false;
                    }
                    else{
                        column = column.substr(0, notCmd);
                        notcmd = true;
                    }
                    cout << column << endl;

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }

                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    subs = subs.substr(0,pycoma-1);

                    size_t AND = subs.find(" AND ");
                    if (AND< subs.length()){
                        string value1 = subs.substr(0, AND);

                        string value2 = subs.substr(AND+5);

                        if(value1[0] == ' '){
                            value1 = value1.substr(1);
                        }
                        if (value1[value1.length()-1] == ' '){
                            value1 = value1.substr(0, value1.length()-1);
                        }

                        if(value1[0] == '"'){
                            value1 = value1.substr(1);
                        }
                        if (value1[value1.length()-1] == '"'){
                            value1 = value1.substr(0, value1.length()-1);
                        }

                        if(value2[0] == ' '){
                            value2 = value2.substr(1);
                        }
                        if (value2[value2.length()-1] == ' '){
                            value2 = value2.substr(0, value2.length()-1);
                        }

                        if(value2[0] == '"'){
                            value2 = value2.substr(1);
                        }
                        if (value2[value2.length()-1] == '"'){
                            value2 = value2.substr(0, value2.length()-1);
                        }


                        if(column.compare("YEAR") == 0) {

                            stringstream toint1(value1);
                            int v1;
                            toint1 >> v1;

                            cout<< v1 << endl;

                            stringstream toint2(value2);
                            int v2;
                            toint2 >> v2;
                            cout<< v2 << endl;

                            if(v1 == v2){
                                vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                                vector<int> indices;
                                for (int i = 1; i < dataToVerify.size(); i++) {
                                    if(notcmd == false) {
                                        if (v2 == dataToVerify[i] ) {
                                            indices.push_back(i);
                                        }
                                    }
                                    else{
                                        if (v2 != dataToVerify[i] ) {
                                            indices.push_back(i);
                                        }
                                    }
                                }
                                for (int i = 0; i < indices.size(); i++) {
                                    matrix.push_back(dataBase->getRow(tabla, indices[i]));
                                }
                                return matrix;
                            }
                            else if (v1 > v2) {
                                vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                                vector<int> indices;
                                for (int i = 1; i < dataToVerify.size(); i++) {
                                    if(notcmd == false) {
                                        if (v2 <= dataToVerify[i] <= v1) {
                                            indices.push_back(i);
                                        }
                                    }
                                    else{
                                        if (v2 > dataToVerify[i] > v1) {
                                            indices.push_back(i);
                                        }
                                    }
                                }
                                for (int i = 0; i < indices.size(); i++) {
                                    matrix.push_back(dataBase->getRow(tabla, indices[i]));
                                }
                                return matrix;
                            } else {
                                vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                                vector<int> indices;
                                for (int i = 1; i < dataToVerify.size(); i++) {
                                    if(notcmd == false) {
                                        if (v1 <= dataToVerify[i] <= v2) {
                                            indices.push_back(i);
                                        }
                                    }
                                    else{
                                        if (v1 > dataToVerify[i] > v2) {
                                            indices.push_back(i);
                                        }
                                    }
                                }
                                for (int i = 0; i < indices.size(); i++) {
                                    matrix.push_back(dataBase->getRow(tabla, indices[i]));
                                }
                                return matrix;
                            }
                        }
                        else{

                            size_t x = value1.find("x");

                            if (value1.length()< x){
                                cout << "Formato invalido de tamano" << endl;
                                verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                                matrix.push_back(verify);
                                return matrix;
                            }
                            string size1 = value1.substr(0,x);
                            string size2 = value1.substr(x+1);

                            stringstream toint1(size1);
                            int p11;
                            toint1 >> p11;

                            stringstream toint2(size2);
                            int p12;
                            toint2 >> p12;

                            int v1 = p11*p12; /// VALOR A COMPARAR 1

                            x = value2.find("x");

                            if (value2.length()< x){
                                cout << "Formato invalido de tamano" << endl;
                                verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                                matrix.push_back(verify);
                                return matrix;
                            }
                            size1 = value2.substr(0,x);
                            size2 = value2.substr(x+1);

                            stringstream toint3(size1);
                            int p21;
                            toint1 >> p21;

                            stringstream toint4(size2);
                            int p22;
                            toint2 >> p22;

                            int v2 = p21*p22; /// VALOR A COMPARAR 1


                            if(v1 == v2){
                                vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                                vector<int> indices;
                                for (int i = 1; i < dataToVerify.size(); i++) {
                                    if(notcmd == false) {
                                        if (v2 == dataToVerify[i] ) {
                                            indices.push_back(i);
                                        }
                                    }
                                    else{
                                        if (v2 != dataToVerify[i] ) {
                                            indices.push_back(i);
                                        }
                                    }
                                }
                                for (int i = 0; i < indices.size(); i++) {
                                    matrix.push_back(dataBase->getRow(tabla, indices[i]));
                                }
                                return matrix;
                            }
                            else if (v1 > v2) {
                                vector<int> dataToVerify = dataBase->getColumnSize(tabla);
                                vector<int> indices;
                                for (int i = 1; i < dataToVerify.size(); i++) {
                                    if(notcmd == false) {
                                        if (v2 <= dataToVerify[i] <= v1) {
                                            indices.push_back(i);
                                        }
                                    }
                                    else{
                                        if (v2 > dataToVerify[i] > v1) {
                                            indices.push_back(i);
                                        }
                                    }
                                }
                                for (int i = 0; i < indices.size(); i++) {
                                    matrix.push_back(dataBase->getRow(tabla, indices[i]));
                                }
                                return matrix;
                            } else {
                                vector<int> dataToVerify = dataBase->getColumnSize(tabla);
                                vector<int> indices;
                                for (int i = 1; i < dataToVerify.size(); i++) {
                                    if(notcmd == false) {
                                        if (v1 <= dataToVerify[i] <= v2) {
                                            indices.push_back(i);
                                        }
                                    }
                                    else{
                                        if (v1 > dataToVerify[i] > v2) {
                                            indices.push_back(i);
                                        }
                                    }
                                }
                                for (int i = 0; i < indices.size(); i++) {
                                    matrix.push_back(dataBase->getRow(tabla, indices[i]));
                                }
                                return matrix;
                            }
                        }

                    }
                    else{
                        cout << "No hay ningun operador para comparar"<< endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }


                }

                comand = subs.find("IN ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+3);
                    size_t notCmd = column.find(" NOT ");
                    bool notcmd;
                    if (column.length()< notcmd){
                        notcmd = false;
                    }
                    else{
                        column = column.substr(0, notCmd);
                        notcmd = true;
                    }
                    if((subs.substr(0,1)).compare("(") == 0) {
                        size_t valuesEnd = subs.find(")");
                        if (subs.length() < valuesEnd) {
                            cout << "Syntax error" << endl;
                            verify.push_back("0");
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string values = subs.substr(1, valuesEnd);
                        cout << values << endl;
                        vector <string> search;
                        while (values.compare(")") != 0){
                            size_t coma = values.find(",");
                            string current;
                            if (values.length()< coma){
                                current = values.substr(0, values.length()-1);
                                //cout << current << endl;
                                values = values.substr(current.length(),values.length()-1);
                            }
                            else {
                                current = values.substr(0, coma);
                                //cout << current << endl;
                                values = values.substr(coma+1);
                                //cout << columns << endl;
                            }
                            if(current[0] == ' '){
                                current = current.substr(1);
                            }
                            if (current[current.length()-1] == ' '){
                                current = current.substr(0, current.length()-1);
                            }
                            if(current[0] == '"'){
                                current = current.substr(1);
                            }
                            if (current[current.length()-1] == '"'){
                                current = current.substr(0, current.length()-1);
                            }
                            cout << current << endl;
                            cout << values << endl;
                            for(int i=0; i< search.size(); i++){
                                if (search[i].compare(current) != 0){
                                    search.push_back(current);
                                }
                            }

                        }
                        int c1 = 0;
                        int c2 = 1;
                        vector <string> dataToVerify = dataBase->getColumn(tabla, column);
                        vector<int> indices;
                        while (c2 < dataToVerify.size()){
                            while (c1 < search.size()){
                                if (search[c1].compare(dataToVerify[c2]) == 0){
                                    indices.push_back(c2);
                                }
                                c1++;
                            }
                            c1=0;
                            c2++;
                        }
                        for (int i = 0; i < indices.size(); i++) {
                            matrix.push_back(dataBase->getRow(tabla, indices[i]));
                        }
                        return matrix;

                    }
                    else {
                        cout << "Syntax error"<< endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                }

                comand = subs.find("=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR
                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR
                    vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i].compare(value) == 0){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                    return matrix;
                }

                comand = subs.find("<");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] < sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] < v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                    return matrix;
                }

                comand = subs.find(">");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] > sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] > v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                    return matrix;
                }

                comand = subs.find(">=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] >= sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] >= v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                    return matrix;
                }

                comand = subs.find("<=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] <= sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] <= v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                    return matrix;
                }

                comand = subs.find("IS ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+3);
                    vector <string> dataToVerify = dataBase->getColumn(tabla, column);
                    vector <int> indices;
                    if(subs.compare("NOT NULL;") == 0){
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i].compare("NULL") != 0){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        return matrix;
                    }
                    else if(subs.compare("NULL;") == 0){
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i].compare("NULL") == 0){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        return matrix;
                    }
                    else{
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                }

                comand = subs.find("LIKE ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+5 );
                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }

                    string pattern = subs.substr(0, pycoma);
                    if(pattern[0] == ' '){
                        pattern = pattern.substr(1);
                    }
                    if (pattern[pattern.length()-1] == ' '){
                        pattern = pattern.substr(0, pattern.length()-1);
                    }

                    if(pattern[0] == '"'){
                        pattern = pattern.substr(1);
                    }
                    if (pattern[pattern.length()-1] == '"'){
                        pattern = pattern.substr(0, pattern.length()-1);
                    }
                    size_t operador = pattern.find("%");
                    if (pattern.length() < operador){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    else{
                        if (pattern[0] == '%') {
                            pattern = pattern.substr(1);
                            size_t spattern = pattern.find("%");
                            if (pattern.length() > spattern){
                                pattern = pattern.substr(0,spattern-1);
                                cout << "Patron en cualquier pos: " + pattern << endl;

                                vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                                vector <int> indices;
                                for(int i=1; i< dataToVerify.size(); i++){
                                    string evaluate = dataToVerify[i];
                                    if(evaluate.find(pattern) < evaluate.length()){
                                        indices.push_back(i);
                                    }
                                }
                                for(int i=0; i< indices.size(); i++){
                                    matrix.push_back(dataBase->getRow(tabla,indices[i]));
                                }
                                return matrix;
                            }
                            else{
                                cout << "Terminan en: " + pattern << endl;

                                vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                                vector <int> indices;
                                for(int i=1; i< dataToVerify.size(); i++){
                                    string evaluate = dataToVerify[i];
                                    if((evaluate.length() - evaluate.find(pattern)) == pattern.length()){
                                        indices.push_back(i);
                                    }
                                }
                                for(int i=0; i< indices.size(); i++){
                                    matrix.push_back(dataBase->getRow(tabla,indices[i]));
                                }
                                return matrix;
                            }

                        }
                        else if(pattern[0] == '_'){
                            pattern = pattern.substr(1);
                            int spaces = 1;
                            while(pattern[0] == '_'){
                                spaces += 1;
                                pattern = pattern.substr(1);
                            }
                            size_t spattern = pattern.find("%");
                            if (pattern.length() > spattern){
                                pattern = pattern.substr(0,spattern-1);
                                cout << "Patron : " + pattern + " luego de " ;
                                cout << spaces;
                                cout << " espacios" << endl;

                                vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                                vector <int> indices;
                                for(int i=1; i< dataToVerify.size(); i++){
                                    string evaluate = dataToVerify[i];
                                    if(evaluate.find(pattern) == spaces){
                                        indices.push_back(i);
                                    }
                                }
                                for(int i=0; i< indices.size(); i++){
                                    matrix.push_back(dataBase->getRow(tabla,indices[i]));
                                }
                                return matrix;
                            }
                            else{
                                cout << "Patron mal definido" << endl;
                                verify.push_back("0");
                                matrix.push_back(verify);
                                return matrix;
                            }

                        }
                        else{
                            if(pattern[pattern.length()-1] == '%'){
                                pattern = pattern.substr(0,pattern.length()-1);
                                size_t spattern = pattern.find("_");
                                if(pattern.length() > spattern){
                                    pattern = pattern.substr(0,spattern-1);
                                    string spaces = pattern.substr(spattern);
                                    int space = 0;
                                    while(spaces[0] == '_'){
                                        space += 1;
                                        spaces = spaces.substr(1);
                                    }
                                    cout << "Palabra que inicia con : " + pattern + " de largo de " ;
                                    cout << space << endl;

                                    vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                                    vector <int> indices;
                                    for(int i=1; i< dataToVerify.size(); i++){
                                        string evaluate = dataToVerify[i];
                                        if((evaluate.length() == space+ pattern.length() ) && ( evaluate.substr(0, pattern.length()-1).compare(pattern) == 0)){
                                            indices.push_back(i);
                                        }
                                    }
                                    for(int i=0; i< indices.size(); i++){
                                        matrix.push_back(dataBase->getRow(tabla,indices[i]));
                                    }
                                    return matrix;
                                }
                                else{
                                    cout << "Palabra que inicia con : " + pattern << endl;

                                    vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                                    vector <int> indices;
                                    for(int i=1; i< dataToVerify.size(); i++){
                                        string evaluate = dataToVerify[i];
                                        if(evaluate.substr(0, pattern.length()-1).compare(pattern) == 0){
                                            indices.push_back(i);
                                        }
                                    }
                                    for(int i=0; i< indices.size(); i++){
                                        matrix.push_back(dataBase->getRow(tabla,indices[i]));
                                    }
                                    return matrix;
                                }
                            }
                            size_t spattern = pattern.find("%");
                            if(pattern.length() > spattern){
                                string inicio = pattern.substr(0,spattern-1);
                                string fin = pattern.substr(spattern);
                                cout << "Palabra que inicia con : " + inicio + " y termina con " + fin << endl;

                                vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                                vector <int> indices;
                                for(int i=1; i< dataToVerify.size(); i++){
                                    string evaluate = dataToVerify[i];
                                    if((evaluate.substr(fin.length()-1).compare(fin) == 0) && ( evaluate.substr(0, inicio.length()-1).compare(inicio) == 0)){
                                        indices.push_back(i);
                                    }
                                }
                                for(int i=0; i< indices.size(); i++){
                                    matrix.push_back(dataBase->getRow(tabla,indices[i]));
                                }
                                return matrix;
                            }
                            else{
                                cout << "Patron mal definido" << endl;
                                verify.push_back("0");
                                matrix.push_back(verify);
                                return matrix;
                            }
                        }
                    }
                }
                else{
                    cout << "Syntax error" << endl;
                    verify.push_back("0");
                    matrix.push_back(verify);
                    return matrix;
                }
            }


            /*
             * COLOCAR TODAS LAS POSIBLES CONDICIONALES[BETWEEN(AND, OR, NOT),IS(NULL, NOT NULL),LIKE]
             */
        }
    }
    else if(subs.find("FROM ") < subs.length()){ ///CAMBIAR EL GETROW PARA OBTENER SOLO COLUMNAS DESEADAS



        size_t From = subs.find("FROM ");
        if (3 < From){
            cout << "Syntax error" << endl;
            verify.push_back("0");
            matrix.push_back(verify);
            return matrix;
        }
        string columnas = subs.substr(0,From);
        vector <string> columns;

        while (columnas.compare("") != 0 || columnas.compare(" ") != 0 ){
            size_t coma = columnas.find(",");
            string current;
            if (columnas.length()< coma){
                current = columnas.substr(0, columnas.length()-1);
                //cout << current << endl;
                columnas = columnas.substr(current.length(),columnas.length()-1);
            }
            else {
                current = columnas.substr(0, coma);
                //cout << current << endl;
                columnas = columnas.substr(coma+1);
                //cout << columns << endl;
            }
            if(current[0] == ' '){
                current = current.substr(1);
            }
            if (current[current.length()-1] == ' '){
                current = current.substr(0, current.length()-1);
            }

            cout << current << endl;
            cout << columnas << endl;
            for(int i=0; i< columns.size(); i++){
                if (columns[i].compare(current) != 0){
                    columns.push_back(current);
                }
            }

        }


        subs = subs.substr(From+5);
        size_t pycoma = subs.find(";");
        if (subs.length()< pycoma){
            cout << "Syntax error" << endl;
            verify.push_back("0");
            matrix.push_back(verify);
            return matrix;
        }
        string tabla = subs.substr(0, pycoma);
        cout << tabla << endl;
        if(tabla[0] == ' '){
            tabla = tabla.substr(1);
        }
        if (tabla[tabla.length()-1] == ' '){
            tabla = tabla.substr(0, tabla.length()-1);
        }
        subs = subs.substr(pycoma+1);
        cout << subs << endl;
        if (dataBase->getColumn(tabla,"NAME")[0].compare("ERROR") == 0){
            verify.push_back("0");
            matrix.push_back(verify);
            return matrix;
        }
        if(subs.compare("") == 0 || subs.compare(" ") == 0){
            cout << "Imprimir valores de tabla" << endl;
            int c = 0;
            while(c < columns.size()) {
                if (columns[c].compare("NAME") == 0) {
                    matrix.push_back(dataBase->getColumn(tabla, "NAME"));
                }
                else if (columns[c].compare("AUTHOR") == 0) {
                    matrix.push_back(dataBase->getColumn(tabla, "AUTHOR"));
                }
                else if (columns[c].compare("YEAR") == 0) {
                    matrix.push_back(dataBase->getColumn(tabla, "YEAR"));
                }
                else if (columns[c].compare("SIZE") == 0) {
                    matrix.push_back(dataBase->getColumn(tabla, "SIZE"));
                }
                else if (columns[c].compare("DESCRIPTION") == 0) {
                    matrix.push_back(dataBase->getColumn(tabla, "DESCRIPTION"));
                }
                else{
                    cout << "No existe esa columna en la tabla" << endl;
                    verify.push_back("0");
                    matrix.push_back(verify);
                    return matrix;
                }
                c++;
            }
            return matrix;
        }
        else{
            size_t where = subs.find("WHERE ");
            if (2 < where){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            subs = subs.substr(where+6);
            if (subs.substr(0,4).compare("NOT ") == 0){
                subs = subs.substr(4);
                size_t comand = subs.find("=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR
                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR
                    vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i].compare(value) != 0){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        vector <string> toShow;
                        toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                        int c = 0;
                        while (c < columns.size()) {
                            if (columns[c].compare("NAME") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                            } else if (columns[c].compare("AUTHOR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                            } else if (columns[c].compare("YEAR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                            } else if (columns[c].compare("SIZE") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                            } else if (columns[c].compare("DESCRIPTION") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                            } else {
                                cout << "No existe esa columna en la tabla" << endl;
                                verify.push_back("0");
                                matrix.push_back(verify);
                                return matrix;
                            }
                            c++;
                        }
                        matrix.push_back(toShow);
                    }
                    return matrix;
                }


                comand = subs.find("<");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] >= sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            vector <string> toShow;
                            toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                            int c = 0;
                            while (c < columns.size()) {
                                if (columns[c].compare("NAME") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                } else if (columns[c].compare("AUTHOR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                } else if (columns[c].compare("YEAR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                } else if (columns[c].compare("SIZE") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                } else if (columns[c].compare("DESCRIPTION") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                } else {
                                    cout << "No existe esa columna en la tabla" << endl;
                                    verify.push_back("0");
                                    matrix.push_back(verify);
                                    return matrix;
                                }
                                c++;
                            }
                            matrix.push_back(toShow);
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] >= v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        vector <string> toShow;
                        toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                        int c = 0;
                        while (c < columns.size()) {
                            if (columns[c].compare("NAME") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                            } else if (columns[c].compare("AUTHOR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                            } else if (columns[c].compare("YEAR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                            } else if (columns[c].compare("SIZE") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                            } else if (columns[c].compare("DESCRIPTION") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                            } else {
                                cout << "No existe esa columna en la tabla" << endl;
                                verify.push_back("0");
                                matrix.push_back(verify);
                                return matrix;
                            }
                            c++;
                        }
                        matrix.push_back(toShow);
                    }
                    return matrix;
                }

                comand = subs.find(">");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] <= sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            vector <string> toShow;
                            toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                            int c = 0;
                            while (c < columns.size()) {
                                if (columns[c].compare("NAME") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                } else if (columns[c].compare("AUTHOR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                } else if (columns[c].compare("YEAR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                } else if (columns[c].compare("SIZE") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                } else if (columns[c].compare("DESCRIPTION") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                } else {
                                    cout << "No existe esa columna en la tabla" << endl;
                                    verify.push_back("0");
                                    matrix.push_back(verify);
                                    return matrix;
                                }
                                c++;
                            }
                            matrix.push_back(toShow);
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] <= v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                    return matrix;
                }

                comand = subs.find(">=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] < sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            vector <string> toShow;
                            toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                            int c = 0;
                            while (c < columns.size()) {
                                if (columns[c].compare("NAME") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                } else if (columns[c].compare("AUTHOR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                } else if (columns[c].compare("YEAR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                } else if (columns[c].compare("SIZE") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                } else if (columns[c].compare("DESCRIPTION") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                } else {
                                    cout << "No existe esa columna en la tabla" << endl;
                                    verify.push_back("0");
                                    matrix.push_back(verify);
                                    return matrix;
                                }
                                c++;
                            }
                            matrix.push_back(toShow);
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] < v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        vector <string> toShow;
                        toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                        int c = 0;
                        while (c < columns.size()) {
                            if (columns[c].compare("NAME") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                            } else if (columns[c].compare("AUTHOR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                            } else if (columns[c].compare("YEAR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                            } else if (columns[c].compare("SIZE") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                            } else if (columns[c].compare("DESCRIPTION") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                            } else {
                                cout << "No existe esa columna en la tabla" << endl;
                                verify.push_back("0");
                                matrix.push_back(verify);
                                return matrix;
                            }
                            c++;
                        }
                        matrix.push_back(toShow);
                    }
                    return matrix;
                }

                comand = subs.find("<=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] > sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            vector <string> toShow;
                            toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                            int c = 0;
                            while (c < columns.size()) {
                                if (columns[c].compare("NAME") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                } else if (columns[c].compare("AUTHOR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                } else if (columns[c].compare("YEAR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                } else if (columns[c].compare("SIZE") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                } else if (columns[c].compare("DESCRIPTION") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                } else {
                                    cout << "No existe esa columna en la tabla" << endl;
                                    verify.push_back("0");
                                    matrix.push_back(verify);
                                    return matrix;
                                }
                                c++;
                            }
                            matrix.push_back(toShow);
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] > v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        vector <string> toShow;
                        toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                        int c = 0;
                        while (c < columns.size()) {
                            if (columns[c].compare("NAME") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                            } else if (columns[c].compare("AUTHOR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                            } else if (columns[c].compare("YEAR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                            } else if (columns[c].compare("SIZE") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                            } else if (columns[c].compare("DESCRIPTION") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                            } else {
                                cout << "No existe esa columna en la tabla" << endl;
                                verify.push_back("0");
                                matrix.push_back(verify);
                                return matrix;
                            }
                            c++;
                        }
                        matrix.push_back(toShow);
                    }
                    return matrix;
                }

                else{
                    cout << "Syntax error" << endl;
                    verify.push_back("0");
                    matrix.push_back(verify);
                    return matrix;
                }

            }
            else {

                size_t comand = subs.find("BETWEEN ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);


                    subs = subs.substr(comand+8);
                    size_t notCmd = column.find(" NOT ");
                    bool notcmd;
                    if (column.length()< notcmd){
                        notcmd = false;
                    }
                    else{
                        column = column.substr(0, notCmd);
                        notcmd = true;
                    }
                    cout << column << endl;

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }

                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    subs = subs.substr(0,pycoma-1);

                    size_t AND = subs.find(" AND ");
                    if (AND< subs.length()){
                        string value1 = subs.substr(0, AND);

                        string value2 = subs.substr(AND+5);

                        if(value1[0] == ' '){
                            value1 = value1.substr(1);
                        }
                        if (value1[value1.length()-1] == ' '){
                            value1 = value1.substr(0, value1.length()-1);
                        }

                        if(value1[0] == '"'){
                            value1 = value1.substr(1);
                        }
                        if (value1[value1.length()-1] == '"'){
                            value1 = value1.substr(0, value1.length()-1);
                        }

                        if(value2[0] == ' '){
                            value2 = value2.substr(1);
                        }
                        if (value2[value2.length()-1] == ' '){
                            value2 = value2.substr(0, value2.length()-1);
                        }

                        if(value2[0] == '"'){
                            value2 = value2.substr(1);
                        }
                        if (value2[value2.length()-1] == '"'){
                            value2 = value2.substr(0, value2.length()-1);
                        }


                        if(column.compare("YEAR") == 0) {

                            stringstream toint1(value1);
                            int v1;
                            toint1 >> v1;

                            cout<< v1 << endl;

                            stringstream toint2(value2);
                            int v2;
                            toint2 >> v2;
                            cout<< v2 << endl;

                            if(v1 == v2){
                                vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                                vector<int> indices;
                                for (int i = 1; i < dataToVerify.size(); i++) {
                                    if(notcmd == false) {
                                        if (v2 == dataToVerify[i] ) {
                                            indices.push_back(i);
                                        }
                                    }
                                    else{
                                        if (v2 != dataToVerify[i] ) {
                                            indices.push_back(i);
                                        }
                                    }
                                }
                                for(int i=0; i< indices.size(); i++){
                                    vector <string> toShow;
                                    toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                                    int c = 0;
                                    while (c < columns.size()) {
                                        if (columns[c].compare("NAME") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                        } else if (columns[c].compare("AUTHOR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                        } else if (columns[c].compare("YEAR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                        } else if (columns[c].compare("SIZE") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                        } else if (columns[c].compare("DESCRIPTION") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                        } else {
                                            cout << "No existe esa columna en la tabla" << endl;
                                            verify.push_back("0");
                                            matrix.push_back(verify);
                                            return matrix;
                                        }
                                        c++;
                                    }
                                    matrix.push_back(toShow);
                                }
                                return matrix;
                            }
                            else if (v1 > v2) {
                                vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                                vector<int> indices;
                                for (int i = 1; i < dataToVerify.size(); i++) {
                                    if(notcmd == false) {
                                        if (v2 <= dataToVerify[i] <= v1) {
                                            indices.push_back(i);
                                        }
                                    }
                                    else{
                                        if (v2 > dataToVerify[i] > v1) {
                                            indices.push_back(i);
                                        }
                                    }
                                }
                                for(int i=0; i< indices.size(); i++){
                                    vector <string> toShow;
                                    toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                                    int c = 0;
                                    while (c < columns.size()) {
                                        if (columns[c].compare("NAME") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                        } else if (columns[c].compare("AUTHOR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                        } else if (columns[c].compare("YEAR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                        } else if (columns[c].compare("SIZE") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                        } else if (columns[c].compare("DESCRIPTION") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                        } else {
                                            cout << "No existe esa columna en la tabla" << endl;
                                            verify.push_back("0");
                                            matrix.push_back(verify);
                                            return matrix;
                                        }
                                        c++;
                                    }
                                    matrix.push_back(toShow);
                                }
                                return matrix;
                            } else {
                                vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                                vector<int> indices;
                                for (int i = 1; i < dataToVerify.size(); i++) {
                                    if(notcmd == false) {
                                        if (v1 <= dataToVerify[i] <= v2) {
                                            indices.push_back(i);
                                        }
                                    }
                                    else{
                                        if (v1 > dataToVerify[i] > v2) {
                                            indices.push_back(i);
                                        }
                                    }
                                }
                                for(int i=0; i< indices.size(); i++){
                                    vector <string> toShow;
                                    toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                                    int c = 0;
                                    while (c < columns.size()) {
                                        if (columns[c].compare("NAME") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                        } else if (columns[c].compare("AUTHOR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                        } else if (columns[c].compare("YEAR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                        } else if (columns[c].compare("SIZE") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                        } else if (columns[c].compare("DESCRIPTION") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                        } else {
                                            cout << "No existe esa columna en la tabla" << endl;
                                            verify.push_back("0");
                                            matrix.push_back(verify);
                                            return matrix;
                                        }
                                        c++;
                                    }
                                    matrix.push_back(toShow);
                                }
                                return matrix;
                            }
                        }
                        else{

                            size_t x = value1.find("x");

                            if (value1.length()< x){
                                cout << "Formato invalido de tamano" << endl;
                                verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                                matrix.push_back(verify);
                                return matrix;
                            }
                            string size1 = value1.substr(0,x);
                            string size2 = value1.substr(x+1);

                            stringstream toint1(size1);
                            int p11;
                            toint1 >> p11;

                            stringstream toint2(size2);
                            int p12;
                            toint2 >> p12;

                            int v1 = p11*p12; /// VALOR A COMPARAR 1

                            x = value2.find("x");

                            if (value2.length()< x){
                                cout << "Formato invalido de tamano" << endl;
                                verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                                matrix.push_back(verify);
                                return matrix;
                            }
                            size1 = value2.substr(0,x);
                            size2 = value2.substr(x+1);

                            stringstream toint3(size1);
                            int p21;
                            toint1 >> p21;

                            stringstream toint4(size2);
                            int p22;
                            toint2 >> p22;

                            int v2 = p21*p22; /// VALOR A COMPARAR 1


                            if(v1 == v2){
                                vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                                vector<int> indices;
                                for (int i = 1; i < dataToVerify.size(); i++) {
                                    if(notcmd == false) {
                                        if (v2 == dataToVerify[i] ) {
                                            indices.push_back(i);
                                        }
                                    }
                                    else{
                                        if (v2 != dataToVerify[i] ) {
                                            indices.push_back(i);
                                        }
                                    }
                                }
                                for(int i=0; i< indices.size(); i++){
                                    vector <string> toShow;
                                    toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                                    int c = 0;
                                    while (c < columns.size()) {
                                        if (columns[c].compare("NAME") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                        } else if (columns[c].compare("AUTHOR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                        } else if (columns[c].compare("YEAR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                        } else if (columns[c].compare("SIZE") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                        } else if (columns[c].compare("DESCRIPTION") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                        } else {
                                            cout << "No existe esa columna en la tabla" << endl;
                                            verify.push_back("0");
                                            matrix.push_back(verify);
                                            return matrix;
                                        }
                                        c++;
                                    }
                                    matrix.push_back(toShow);
                                }
                                return matrix;
                            }
                            else if (v1 > v2) {
                                vector<int> dataToVerify = dataBase->getColumnSize(tabla);
                                vector<int> indices;
                                for (int i = 1; i < dataToVerify.size(); i++) {
                                    if(notcmd == false) {
                                        if (v2 <= dataToVerify[i] <= v1) {
                                            indices.push_back(i);
                                        }
                                    }
                                    else{
                                        if (v2 > dataToVerify[i] > v1) {
                                            indices.push_back(i);
                                        }
                                    }
                                }
                                for(int i=0; i< indices.size(); i++){
                                    vector <string> toShow;
                                    toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                                    int c = 0;
                                    while (c < columns.size()) {
                                        if (columns[c].compare("NAME") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                        } else if (columns[c].compare("AUTHOR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                        } else if (columns[c].compare("YEAR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                        } else if (columns[c].compare("SIZE") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                        } else if (columns[c].compare("DESCRIPTION") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                        } else {
                                            cout << "No existe esa columna en la tabla" << endl;
                                            verify.push_back("0");
                                            matrix.push_back(verify);
                                            return matrix;
                                        }
                                        c++;
                                    }
                                    matrix.push_back(toShow);
                                }
                                return matrix;
                            } else {
                                vector<int> dataToVerify = dataBase->getColumnSize(tabla);
                                vector<int> indices;
                                for (int i = 1; i < dataToVerify.size(); i++) {
                                    if(notcmd == false) {
                                        if (v1 <= dataToVerify[i] <= v2) {
                                            indices.push_back(i);
                                        }
                                    }
                                    else{
                                        if (v1 > dataToVerify[i] > v2) {
                                            indices.push_back(i);
                                        }
                                    }
                                }
                                for(int i=0; i< indices.size(); i++){
                                    vector <string> toShow;
                                    toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                                    int c = 0;
                                    while (c < columns.size()) {
                                        if (columns[c].compare("NAME") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                        } else if (columns[c].compare("AUTHOR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                        } else if (columns[c].compare("YEAR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                        } else if (columns[c].compare("SIZE") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                        } else if (columns[c].compare("DESCRIPTION") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                        } else {
                                            cout << "No existe esa columna en la tabla" << endl;
                                            verify.push_back("0");
                                            matrix.push_back(verify);
                                            return matrix;
                                        }
                                        c++;
                                    }
                                    matrix.push_back(toShow);
                                }
                                return matrix;
                            }
                        }

                    }
                    else{
                        cout << "No hay ningun operador para comparar"<< endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }


                }

                comand = subs.find("IN ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+3);
                    size_t notCmd = column.find(" NOT ");
                    bool notcmd;
                    if (column.length()< notcmd){
                        notcmd = false;
                    }
                    else{
                        column = column.substr(0, notCmd);
                        notcmd = true;
                    }
                    if((subs.substr(0,1)).compare("(") == 0) {
                        size_t valuesEnd = subs.find(")");
                        if (subs.length() < valuesEnd) {
                            cout << "Syntax error" << endl;
                            verify.push_back("0");
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string values = subs.substr(1, valuesEnd);
                        cout << values << endl;
                        vector <string> search;
                        while (values.compare(")") != 0){
                            size_t coma = values.find(",");
                            string current;
                            if (values.length()< coma){
                                current = values.substr(0, values.length()-1);
                                //cout << current << endl;
                                values = values.substr(current.length(),values.length()-1);
                            }
                            else {
                                current = values.substr(0, coma);
                                //cout << current << endl;
                                values = values.substr(coma+1);
                                //cout << columns << endl;
                            }
                            if(current[0] == ' '){
                                current = current.substr(1);
                            }
                            if (current[current.length()-1] == ' '){
                                current = current.substr(0, current.length()-1);
                            }
                            if(current[0] == '"'){
                                current = current.substr(1);
                            }
                            if (current[current.length()-1] == '"'){
                                current = current.substr(0, current.length()-1);
                            }
                            cout << current << endl;
                            cout << values << endl;
                            for(int i=0; i< search.size(); i++){
                                if (search[i].compare(current) != 0){
                                    search.push_back(current);
                                }
                            }

                        }
                        int c1 = 0;
                        int c2 = 1;
                        vector <string> dataToVerify = dataBase->getColumn(tabla, column);
                        vector<int> indices;
                        while (c2 < dataToVerify.size()){
                            while (c1 < search.size()){
                                if (search[c1].compare(dataToVerify[c2]) == 0){
                                    indices.push_back(c2);
                                }
                                c1++;
                            }
                            c1=0;
                            c2++;
                        }
                        for(int i=0; i< indices.size(); i++){
                            vector <string> toShow;
                            toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                            int c = 0;
                            while (c < columns.size()) {
                                if (columns[c].compare("NAME") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                } else if (columns[c].compare("AUTHOR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                } else if (columns[c].compare("YEAR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                } else if (columns[c].compare("SIZE") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                } else if (columns[c].compare("DESCRIPTION") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                } else {
                                    cout << "No existe esa columna en la tabla" << endl;
                                    verify.push_back("0");
                                    matrix.push_back(verify);
                                    return matrix;
                                }
                                c++;
                            }
                            matrix.push_back(toShow);
                        }
                        return matrix;

                    }
                    else {
                        cout << "Syntax error"<< endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                }

                comand = subs.find("=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR
                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR
                    vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i].compare(value) == 0){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        vector <string> toShow;
                        toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                        int c = 0;
                        while (c < columns.size()) {
                            if (columns[c].compare("NAME") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                            } else if (columns[c].compare("AUTHOR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                            } else if (columns[c].compare("YEAR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                            } else if (columns[c].compare("SIZE") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                            } else if (columns[c].compare("DESCRIPTION") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                            } else {
                                cout << "No existe esa columna en la tabla" << endl;
                                verify.push_back("0");
                                matrix.push_back(verify);
                                return matrix;
                            }
                            c++;
                        }
                        matrix.push_back(toShow);
                    }
                    return matrix;
                }

                comand = subs.find("<");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] < sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            vector <string> toShow;
                            toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                            int c = 0;
                            while (c < columns.size()) {
                                if (columns[c].compare("NAME") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                } else if (columns[c].compare("AUTHOR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                } else if (columns[c].compare("YEAR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                } else if (columns[c].compare("SIZE") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                } else if (columns[c].compare("DESCRIPTION") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                } else {
                                    cout << "No existe esa columna en la tabla" << endl;
                                    verify.push_back("0");
                                    matrix.push_back(verify);
                                    return matrix;
                                }
                                c++;
                            }
                            matrix.push_back(toShow);
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] < v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        vector <string> toShow;
                        toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                        int c = 0;
                        while (c < columns.size()) {
                            if (columns[c].compare("NAME") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                            } else if (columns[c].compare("AUTHOR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                            } else if (columns[c].compare("YEAR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                            } else if (columns[c].compare("SIZE") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                            } else if (columns[c].compare("DESCRIPTION") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                            } else {
                                cout << "No existe esa columna en la tabla" << endl;
                                verify.push_back("0");
                                matrix.push_back(verify);
                                return matrix;
                            }
                            c++;
                        }
                        matrix.push_back(toShow);
                    }
                    return matrix;
                }

                comand = subs.find(">");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] > sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            vector <string> toShow;
                            toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                            int c = 0;
                            while (c < columns.size()) {
                                if (columns[c].compare("NAME") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                } else if (columns[c].compare("AUTHOR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                } else if (columns[c].compare("YEAR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                } else if (columns[c].compare("SIZE") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                } else if (columns[c].compare("DESCRIPTION") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                } else {
                                    cout << "No existe esa columna en la tabla" << endl;
                                    verify.push_back("0");
                                    matrix.push_back(verify);
                                    return matrix;
                                }
                                c++;
                            }
                            matrix.push_back(toShow);
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] > v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        vector <string> toShow;
                        toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                        int c = 0;
                        while (c < columns.size()) {
                            if (columns[c].compare("NAME") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                            } else if (columns[c].compare("AUTHOR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                            } else if (columns[c].compare("YEAR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                            } else if (columns[c].compare("SIZE") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                            } else if (columns[c].compare("DESCRIPTION") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                            } else {
                                cout << "No existe esa columna en la tabla" << endl;
                                verify.push_back("0");
                                matrix.push_back(verify);
                                return matrix;
                            }
                            c++;
                        }
                        matrix.push_back(toShow);
                    }
                    return matrix;
                }

                comand = subs.find(">=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] >= sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            vector <string> toShow;
                            toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                            int c = 0;
                            while (c < columns.size()) {
                                if (columns[c].compare("NAME") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                } else if (columns[c].compare("AUTHOR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                } else if (columns[c].compare("YEAR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                } else if (columns[c].compare("SIZE") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                } else if (columns[c].compare("DESCRIPTION") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                } else {
                                    cout << "No existe esa columna en la tabla" << endl;
                                    verify.push_back("0");
                                    matrix.push_back(verify);
                                    return matrix;
                                }
                                c++;
                            }
                            matrix.push_back(toShow);
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] >= v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        vector <string> toShow;
                        toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                        int c = 0;
                        while (c < columns.size()) {
                            if (columns[c].compare("NAME") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                            } else if (columns[c].compare("AUTHOR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                            } else if (columns[c].compare("YEAR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                            } else if (columns[c].compare("SIZE") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                            } else if (columns[c].compare("DESCRIPTION") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                            } else {
                                cout << "No existe esa columna en la tabla" << endl;
                                verify.push_back("0");
                                matrix.push_back(verify);
                                return matrix;
                            }
                            c++;
                        }
                        matrix.push_back(toShow);
                    }
                    return matrix;
                }

                comand = subs.find("<=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+1);

                    if (column[column.length()-1] == ' '){
                        column = column.substr(0, column.length()-1);
                    }

                    cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

                    if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                        verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                        matrix.push_back(verify);
                        return matrix;
                    }


                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == ' '){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == ' '){
                        value = value.substr(0, value.length()-1);
                    }

                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

                    if (column.compare("SIZE") == 0){


                        size_t x = value.find("x");

                        if (value.length()< x){
                            cout << "Formato invalido de tamano" << endl;
                            verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                            matrix.push_back(verify);
                            return matrix;
                        }
                        string size1 = value.substr(0,x);
                        string size2 = value.substr(x+1);

                        stringstream toint1(size1);
                        int v1;
                        toint1 >> v1;

                        stringstream toint2(size2);
                        int v2;
                        toint2 >> v2;

                        int sizeM = v1*v2; /// VALOR A COMPARAR
                        vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i] <= sizeM){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        return matrix;
                    }

                    stringstream toint1(value);
                    int v1;
                    toint1 >> v1;
                    vector <int> dataToVerify = dataBase->getColumnYear(tabla);
                    vector <int> indices;
                    for(int i=1; i< dataToVerify.size(); i++){
                        if(dataToVerify[i] <= v1){
                            indices.push_back(i);
                        }
                    }
                    for(int i=0; i< indices.size(); i++){
                        vector <string> toShow;
                        toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                        int c = 0;
                        while (c < columns.size()) {
                            if (columns[c].compare("NAME") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                            } else if (columns[c].compare("AUTHOR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                            } else if (columns[c].compare("YEAR") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                            } else if (columns[c].compare("SIZE") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                            } else if (columns[c].compare("DESCRIPTION") == 0) {
                                toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                            } else {
                                cout << "No existe esa columna en la tabla" << endl;
                                verify.push_back("0");
                                matrix.push_back(verify);
                                return matrix;
                            }
                            c++;
                        }
                        matrix.push_back(toShow);
                    }
                    return matrix;
                }

                comand = subs.find("IS ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+3);
                    vector <string> dataToVerify = dataBase->getColumn(tabla, column);
                    vector <int> indices;
                    if(subs.compare("NOT NULL;") == 0){
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i].compare("NULL") != 0){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        return matrix;
                    }
                    else if(subs.compare("NULL;") == 0){
                        for(int i=1; i< dataToVerify.size(); i++){
                            if(dataToVerify[i].compare("NULL") == 0){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            vector <string> toShow;
                            toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                            int c = 0;
                            while (c < columns.size()) {
                                if (columns[c].compare("NAME") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                } else if (columns[c].compare("AUTHOR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                } else if (columns[c].compare("YEAR") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                } else if (columns[c].compare("SIZE") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                } else if (columns[c].compare("DESCRIPTION") == 0) {
                                    toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                } else {
                                    cout << "No existe esa columna en la tabla" << endl;
                                    verify.push_back("0");
                                    matrix.push_back(verify);
                                    return matrix;
                                }
                                c++;
                            }
                            matrix.push_back(toShow);
                        }
                        return matrix;
                    }
                    else{
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                }

                comand = subs.find("LIKE ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+5 );
                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }

                    string pattern = subs.substr(0, pycoma);
                    if(pattern[0] == ' '){
                        pattern = pattern.substr(1);
                    }
                    if (pattern[pattern.length()-1] == ' '){
                        pattern = pattern.substr(0, pattern.length()-1);
                    }

                    if(pattern[0] == '"'){
                        pattern = pattern.substr(1);
                    }
                    if (pattern[pattern.length()-1] == '"'){
                        pattern = pattern.substr(0, pattern.length()-1);
                    }
                    size_t operador = pattern.find("%");
                    if (pattern.length() < operador){
                        cout << "Syntax error" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    else{
                        if (pattern[0] == '%') {
                            pattern = pattern.substr(1);
                            size_t spattern = pattern.find("%");
                            if (pattern.length() > spattern){
                                pattern = pattern.substr(0,spattern-1);
                                cout << "Patron en cualquier pos: " + pattern << endl;

                                vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                                vector <int> indices;
                                for(int i=1; i< dataToVerify.size(); i++){
                                    string evaluate = dataToVerify[i];
                                    if(evaluate.find(pattern) < evaluate.length()){
                                        indices.push_back(i);
                                    }
                                }
                                for(int i=0; i< indices.size(); i++){
                                    vector <string> toShow;
                                    toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                                    int c = 0;
                                    while (c < columns.size()) {
                                        if (columns[c].compare("NAME") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                        } else if (columns[c].compare("AUTHOR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                        } else if (columns[c].compare("YEAR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                        } else if (columns[c].compare("SIZE") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                        } else if (columns[c].compare("DESCRIPTION") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                        } else {
                                            cout << "No existe esa columna en la tabla" << endl;
                                            verify.push_back("0");
                                            matrix.push_back(verify);
                                            return matrix;
                                        }
                                        c++;
                                    }
                                    matrix.push_back(toShow);
                                }
                                return matrix;
                            }
                            else{
                                cout << "Terminan en: " + pattern << endl;

                                vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                                vector <int> indices;
                                for(int i=1; i< dataToVerify.size(); i++){
                                    string evaluate = dataToVerify[i];
                                    if((evaluate.length() - evaluate.find(pattern)) == pattern.length()){
                                        indices.push_back(i);
                                    }
                                }
                                for(int i=0; i< indices.size(); i++){
                                    vector <string> toShow;
                                    toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                                    int c = 0;
                                    while (c < columns.size()) {
                                        if (columns[c].compare("NAME") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                        } else if (columns[c].compare("AUTHOR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                        } else if (columns[c].compare("YEAR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                        } else if (columns[c].compare("SIZE") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                        } else if (columns[c].compare("DESCRIPTION") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                        } else {
                                            cout << "No existe esa columna en la tabla" << endl;
                                            verify.push_back("0");
                                            matrix.push_back(verify);
                                            return matrix;
                                        }
                                        c++;
                                    }
                                    matrix.push_back(toShow);
                                }
                                return matrix;
                            }

                        }
                        else if(pattern[0] == '_'){
                            pattern = pattern.substr(1);
                            int spaces = 1;
                            while(pattern[0] == '_'){
                                spaces += 1;
                                pattern = pattern.substr(1);
                            }
                            size_t spattern = pattern.find("%");
                            if (pattern.length() > spattern){
                                pattern = pattern.substr(0,spattern-1);
                                cout << "Patron : " + pattern + " luego de " ;
                                cout << spaces;
                                cout << " espacios" << endl;

                                vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                                vector <int> indices;
                                for(int i=1; i< dataToVerify.size(); i++){
                                    string evaluate = dataToVerify[i];
                                    if(evaluate.find(pattern) == spaces){
                                        indices.push_back(i);
                                    }
                                }
                                for(int i=0; i< indices.size(); i++){
                                    vector <string> toShow;
                                    toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                                    int c = 0;
                                    while (c < columns.size()) {
                                        if (columns[c].compare("NAME") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                        } else if (columns[c].compare("AUTHOR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                        } else if (columns[c].compare("YEAR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                        } else if (columns[c].compare("SIZE") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                        } else if (columns[c].compare("DESCRIPTION") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                        } else {
                                            cout << "No existe esa columna en la tabla" << endl;
                                            verify.push_back("0");
                                            matrix.push_back(verify);
                                            return matrix;
                                        }
                                        c++;
                                    }
                                    matrix.push_back(toShow);
                                }
                                return matrix;
                            }
                            else{
                                cout << "Patron mal definido" << endl;
                                verify.push_back("0");
                                matrix.push_back(verify);
                                return matrix;
                            }

                        }
                        else{
                            if(pattern[pattern.length()-1] == '%'){
                                pattern = pattern.substr(0,pattern.length()-1);
                                size_t spattern = pattern.find("_");
                                if(pattern.length() > spattern){
                                    pattern = pattern.substr(0,spattern-1);
                                    string spaces = pattern.substr(spattern);
                                    int space = 0;
                                    while(spaces[0] == '_'){
                                        space += 1;
                                        spaces = spaces.substr(1);
                                    }
                                    cout << "Palabra que inicia con : " + pattern + " de largo de " ;
                                    cout << space << endl;

                                    vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                                    vector <int> indices;
                                    for(int i=1; i< dataToVerify.size(); i++){
                                        string evaluate = dataToVerify[i];
                                        if((evaluate.length() == space+ pattern.length() ) && ( evaluate.substr(0, pattern.length()-1).compare(pattern) == 0)){
                                            indices.push_back(i);
                                        }
                                    }
                                    for(int i=0; i< indices.size(); i++){
                                        vector <string> toShow;
                                        toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                                        int c = 0;
                                        while (c < columns.size()) {
                                            if (columns[c].compare("NAME") == 0) {
                                                toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                            } else if (columns[c].compare("AUTHOR") == 0) {
                                                toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                            } else if (columns[c].compare("YEAR") == 0) {
                                                toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                            } else if (columns[c].compare("SIZE") == 0) {
                                                toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                            } else if (columns[c].compare("DESCRIPTION") == 0) {
                                                toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                            } else {
                                                cout << "No existe esa columna en la tabla" << endl;
                                                verify.push_back("0");
                                                matrix.push_back(verify);
                                                return matrix;
                                            }
                                            c++;
                                        }
                                        matrix.push_back(toShow);
                                    }
                                    return matrix;
                                }
                                else{
                                    cout << "Palabra que inicia con : " + pattern << endl;

                                    vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                                    vector <int> indices;
                                    for(int i=1; i< dataToVerify.size(); i++){
                                        string evaluate = dataToVerify[i];
                                        if(evaluate.substr(0, pattern.length()-1).compare(pattern) == 0){
                                            indices.push_back(i);
                                        }
                                    }
                                    for(int i=0; i< indices.size(); i++){
                                        vector <string> toShow;
                                        toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                                        int c = 0;
                                        while (c < columns.size()) {
                                            if (columns[c].compare("NAME") == 0) {
                                                toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                            } else if (columns[c].compare("AUTHOR") == 0) {
                                                toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                            } else if (columns[c].compare("YEAR") == 0) {
                                                toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                            } else if (columns[c].compare("SIZE") == 0) {
                                                toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                            } else if (columns[c].compare("DESCRIPTION") == 0) {
                                                toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                            } else {
                                                cout << "No existe esa columna en la tabla" << endl;
                                                verify.push_back("0");
                                                matrix.push_back(verify);
                                                return matrix;
                                            }
                                            c++;
                                        }
                                        matrix.push_back(toShow);
                                    }
                                    return matrix;
                                }
                            }
                            size_t spattern = pattern.find("%");
                            if(pattern.length() > spattern){
                                string inicio = pattern.substr(0,spattern-1);
                                string fin = pattern.substr(spattern);
                                cout << "Palabra que inicia con : " + inicio + " y termina con " + fin << endl;

                                vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                                vector <int> indices;
                                for(int i=1; i< dataToVerify.size(); i++){
                                    string evaluate = dataToVerify[i];
                                    if((evaluate.substr(fin.length()-1).compare(fin) == 0) && ( evaluate.substr(0, inicio.length()-1).compare(inicio) == 0)){
                                        indices.push_back(i);
                                    }
                                }
                                for(int i=0; i< indices.size(); i++){
                                    vector <string> toShow;
                                    toShow.push_back(dataBase->getRow(tabla,indices[i])[0]);
                                    int c = 0;
                                    while (c < columns.size()) {
                                        if (columns[c].compare("NAME") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[1]);
                                        } else if (columns[c].compare("AUTHOR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[2]);
                                        } else if (columns[c].compare("YEAR") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[3]);
                                        } else if (columns[c].compare("SIZE") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[4]);
                                        } else if (columns[c].compare("DESCRIPTION") == 0) {
                                            toShow.push_back(dataBase->getRow(tabla, indices[i])[5]);
                                        } else {
                                            cout << "No existe esa columna en la tabla" << endl;
                                            verify.push_back("0");
                                            matrix.push_back(verify);
                                            return matrix;
                                        }
                                        c++;
                                    }
                                    matrix.push_back(toShow);
                                }
                                return matrix;
                            }
                            else{
                                cout << "Patron mal definido" << endl;
                                verify.push_back("0");
                                matrix.push_back(verify);
                                return matrix;
                            }
                        }
                    }
                }
                else{
                    cout << "Syntax error" << endl;
                    verify.push_back("0");
                    matrix.push_back(verify);
                    return matrix;
                }
            }


            /*
             * COLOCAR TODAS LAS POSIBLES CONDICIONALES[BETWEEN(AND, OR, NOT),IS(NULL, NOT NULL),LIKE]
             */
        }
    }

    else {
        cout << "Syntax error" << endl;
        verify.push_back("0");
        matrix.push_back(verify);
        return matrix;
    }

}



vector<vector<string>> SQLController::funcionDelete(string comando)
{
    string subs = comando;
    vector<vector<string>> matrix;
    vector <string> verify;

    size_t pycoma = subs.find(";");
    if (subs.length()< pycoma){
        cout << "Syntax error" << endl;
        verify.push_back("0");
        matrix.push_back(verify);
        return matrix;
    }
    string tabla = subs.substr(0, pycoma);
    cout << tabla << endl;
    if(tabla[0] == ' '){
        tabla = tabla.substr(1);
    }
    if (tabla[tabla.length()-1] == ' '){
        tabla = tabla.substr(0, tabla.length()-1);
    }
    subs = subs.substr(pycoma+1);
    cout << subs << endl;
    if (dataBase->getColumn(tabla,"NAME")[0].compare("ERROR") == 0){
        verify.push_back("0");
        matrix.push_back(verify);
        return matrix;
    }


    size_t where = subs.find("WHERE ");
    if (2 < where){
        cout << "Syntax error" << endl;
        verify.push_back("0");
        matrix.push_back(verify);
        return matrix;
    }
    subs = subs.substr(where+6);
    if (subs.substr(0,4).compare("NOT ") == 0){
        subs = subs.substr(4);
        size_t comand = subs.find("=");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR
            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR
            vector <string> dataToVerify = dataBase->getColumn(tabla,column);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i].compare(value) != 0){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++){
                dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }


        comand = subs.find("<");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] >= sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++){
                    dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] >= v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++){
                dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find(">");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] <= sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++){
                    dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] <= v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++){
                dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find(">=");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] < sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++){
                    dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] < v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++){
                dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find("<=");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] > sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++){
                    dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] > v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++){
                dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        else{
            cout << "Syntax error" << endl;
            verify.push_back("0");
            matrix.push_back(verify);
            return matrix;
        }

    }
    else {

        size_t comand = subs.find("BETWEEN ");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);


            subs = subs.substr(comand+8);
            size_t notCmd = column.find(" NOT ");
            bool notcmd;
            if (column.length()< notcmd){
                notcmd = false;
            }
            else{
                column = column.substr(0, notCmd);
                notcmd = true;
            }
            cout << column << endl;

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }

            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            subs = subs.substr(0,pycoma-1);

            size_t AND = subs.find(" AND ");
            if (AND< subs.length()){
                string value1 = subs.substr(0, AND);

                string value2 = subs.substr(AND+5);

                if(value1[0] == ' '){
                    value1 = value1.substr(1);
                }
                if (value1[value1.length()-1] == ' '){
                    value1 = value1.substr(0, value1.length()-1);
                }

                if(value1[0] == '"'){
                    value1 = value1.substr(1);
                }
                if (value1[value1.length()-1] == '"'){
                    value1 = value1.substr(0, value1.length()-1);
                }

                if(value2[0] == ' '){
                    value2 = value2.substr(1);
                }
                if (value2[value2.length()-1] == ' '){
                    value2 = value2.substr(0, value2.length()-1);
                }

                if(value2[0] == '"'){
                    value2 = value2.substr(1);
                }
                if (value2[value2.length()-1] == '"'){
                    value2 = value2.substr(0, value2.length()-1);
                }


                if(column.compare("YEAR") == 0) {

                    stringstream toint1(value1);
                    int v1;
                    toint1 >> v1;

                    cout<< v1 << endl;

                    stringstream toint2(value2);
                    int v2;
                    toint2 >> v2;
                    cout<< v2 << endl;

                    if(v1 == v2){
                        vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                        vector<int> indices;
                        for (int i = 1; i < dataToVerify.size(); i++) {
                            if(notcmd == false) {
                                if (v2 == dataToVerify[i] ) {
                                    indices.push_back(i);
                                }
                            }
                            else{
                                if (v2 != dataToVerify[i] ) {
                                    indices.push_back(i);
                                }
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                            //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    else if (v1 > v2) {
                        vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                        vector<int> indices;
                        for (int i = 1; i < dataToVerify.size(); i++) {
                            if(notcmd == false) {
                                if (v2 <= dataToVerify[i] <= v1) {
                                    indices.push_back(i);
                                }
                            }
                            else{
                                if (v2 > dataToVerify[i] > v1) {
                                    indices.push_back(i);
                                }
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                            //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    } else {
                        vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                        vector<int> indices;
                        for (int i = 1; i < dataToVerify.size(); i++) {
                            if(notcmd == false) {
                                if (v1 <= dataToVerify[i] <= v2) {
                                    indices.push_back(i);
                                }
                            }
                            else{
                                if (v1 > dataToVerify[i] > v2) {
                                    indices.push_back(i);
                                }
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                            //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }
                }
                else{

                    size_t x = value1.find("x");

                    if (value1.length()< x){
                        cout << "Formato invalido de tamano" << endl;
                        verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string size1 = value1.substr(0,x);
                    string size2 = value1.substr(x+1);

                    stringstream toint1(size1);
                    int p11;
                    toint1 >> p11;

                    stringstream toint2(size2);
                    int p12;
                    toint2 >> p12;

                    int v1 = p11*p12; /// VALOR A COMPARAR 1

                    x = value2.find("x");

                    if (value2.length()< x){
                        cout << "Formato invalido de tamano" << endl;
                        verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                        matrix.push_back(verify);
                        return matrix;
                    }
                    size1 = value2.substr(0,x);
                    size2 = value2.substr(x+1);

                    stringstream toint3(size1);
                    int p21;
                    toint1 >> p21;

                    stringstream toint4(size2);
                    int p22;
                    toint2 >> p22;

                    int v2 = p21*p22; /// VALOR A COMPARAR 1


                    if(v1 == v2){
                        vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                        vector<int> indices;
                        for (int i = 1; i < dataToVerify.size(); i++) {
                            if(notcmd == false) {
                                if (v2 == dataToVerify[i] ) {
                                    indices.push_back(i);
                                }
                            }
                            else{
                                if (v2 != dataToVerify[i] ) {
                                    indices.push_back(i);
                                }
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                            //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    else if (v1 > v2) {
                        vector<int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector<int> indices;
                        for (int i = 1; i < dataToVerify.size(); i++) {
                            if(notcmd == false) {
                                if (v2 <= dataToVerify[i] <= v1) {
                                    indices.push_back(i);
                                }
                            }
                            else{
                                if (v2 > dataToVerify[i] > v1) {
                                    indices.push_back(i);
                                }
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                            //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    } else {
                        vector<int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector<int> indices;
                        for (int i = 1; i < dataToVerify.size(); i++) {
                            if(notcmd == false) {
                                if (v1 <= dataToVerify[i] <= v2) {
                                    indices.push_back(i);
                                }
                            }
                            else{
                                if (v1 > dataToVerify[i] > v2) {
                                    indices.push_back(i);
                                }
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                            //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }
                }

            }
            else{
                cout << "No hay ningun operador para comparar"<< endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }


        }

        comand = subs.find("IN ");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+3);
            size_t notCmd = column.find(" NOT ");
            bool notcmd;
            if (column.length()< notcmd){
                notcmd = false;
            }
            else{
                column = column.substr(0, notCmd);
                notcmd = true;
            }
            if((subs.substr(0,1)).compare("(") == 0) {
                size_t valuesEnd = subs.find(")");
                if (subs.length() < valuesEnd) {
                    cout << "Syntax error" << endl;
                    verify.push_back("0");
                    matrix.push_back(verify);
                    return matrix;
                }
                string values = subs.substr(1, valuesEnd);
                cout << values << endl;
                vector <string> search;
                while (values.compare(")") != 0){
                    size_t coma = values.find(",");
                    string current;
                    if (values.length()< coma){
                        current = values.substr(0, values.length()-1);
                        //cout << current << endl;
                        values = values.substr(current.length(),values.length()-1);
                    }
                    else {
                        current = values.substr(0, coma);
                        //cout << current << endl;
                        values = values.substr(coma+1);
                        //cout << columns << endl;
                    }
                    if(current[0] == ' '){
                        current = current.substr(1);
                    }
                    if (current[current.length()-1] == ' '){
                        current = current.substr(0, current.length()-1);
                    }
                    if(current[0] == '"'){
                        current = current.substr(1);
                    }
                    if (current[current.length()-1] == '"'){
                        current = current.substr(0, current.length()-1);
                    }
                    cout << current << endl;
                    cout << values << endl;
                    for(int i=0; i< search.size(); i++){
                        if (search[i].compare(current) != 0){
                            search.push_back(current);
                        }
                    }

                }
                int c1 = 0;
                int c2 = 1;
                vector <string> dataToVerify = dataBase->getColumn(tabla, column);
                vector<int> indices;
                while (c2 < dataToVerify.size()){
                    while (c1 < search.size()){
                        if (search[c1].compare(dataToVerify[c2]) == 0){
                            indices.push_back(c2);
                        }
                        c1++;
                    }
                    c1=0;
                    c2++;
                }
                for(int i=0; i< indices.size(); i++){
                    dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;

            }
            else {
                cout << "Syntax error"<< endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
        }

        comand = subs.find("=");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR
            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR
            vector <string> dataToVerify = dataBase->getColumn(tabla,column);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i].compare(value) == 0){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++){
                dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find("<");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] < sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++){
                    dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] < v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++){
                dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find(">");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] > sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++){
                    dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] > v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++){
                dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find(">=");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] >= sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++){
                    dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] >= v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++){
                dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find("<=");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] <= sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++){
                    dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] <= v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++){
                dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find("IS ");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+3);
            vector <string> dataToVerify = dataBase->getColumn(tabla, column);
            vector <int> indices;
            if(subs.compare("NOT NULL;") == 0){
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i].compare("NULL") != 0){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++){
                    dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }
            else if(subs.compare("NULL;") == 0){
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i].compare("NULL") == 0){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++){
                    dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }
            else{
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
        }

        comand = subs.find("LIKE ");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+5 );
            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }

            string pattern = subs.substr(0, pycoma);
            if(pattern[0] == ' '){
                pattern = pattern.substr(1);
            }
            if (pattern[pattern.length()-1] == ' '){
                pattern = pattern.substr(0, pattern.length()-1);
            }

            if(pattern[0] == '"'){
                pattern = pattern.substr(1);
            }
            if (pattern[pattern.length()-1] == '"'){
                pattern = pattern.substr(0, pattern.length()-1);
            }
            size_t operador = pattern.find("%");
            if (pattern.length() < operador){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            else{
                if (pattern[0] == '%') {
                    pattern = pattern.substr(1);
                    size_t spattern = pattern.find("%");
                    if (pattern.length() > spattern){
                        pattern = pattern.substr(0,spattern-1);
                        cout << "Patron en cualquier pos: " + pattern << endl;

                        vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            string evaluate = dataToVerify[i];
                            if(evaluate.find(pattern) < evaluate.length()){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                            //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    else{
                        cout << "Terminan en: " + pattern << endl;

                        vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            string evaluate = dataToVerify[i];
                            if((evaluate.length() - evaluate.find(pattern)) == pattern.length()){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                            //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }

                }
                else if(pattern[0] == '_'){
                    pattern = pattern.substr(1);
                    int spaces = 1;
                    while(pattern[0] == '_'){
                        spaces += 1;
                        pattern = pattern.substr(1);
                    }
                    size_t spattern = pattern.find("%");
                    if (pattern.length() > spattern){
                        pattern = pattern.substr(0,spattern-1);
                        cout << "Patron : " + pattern + " luego de " ;
                        cout << spaces;
                        cout << " espacios" << endl;

                        vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            string evaluate = dataToVerify[i];
                            if(evaluate.find(pattern) == spaces){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                            //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    else{
                        cout << "Patron mal definido" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }

                }
                else{
                    if(pattern[pattern.length()-1] == '%'){
                        pattern = pattern.substr(0,pattern.length()-1);
                        size_t spattern = pattern.find("_");
                        if(pattern.length() > spattern){
                            pattern = pattern.substr(0,spattern-1);
                            string spaces = pattern.substr(spattern);
                            int space = 0;
                            while(spaces[0] == '_'){
                                space += 1;
                                spaces = spaces.substr(1);
                            }
                            cout << "Palabra que inicia con : " + pattern + " de largo de " ;
                            cout << space << endl;

                            vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                            vector <int> indices;
                            for(int i=1; i< dataToVerify.size(); i++){
                                string evaluate = dataToVerify[i];
                                if((evaluate.length() == space+ pattern.length() ) && ( evaluate.substr(0, pattern.length()-1).compare(pattern) == 0)){
                                    indices.push_back(i);
                                }
                            }
                            for(int i=0; i< indices.size(); i++){
                                dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                            }
                            verify.push_back("2");
                            matrix.push_back(verify);
                            return matrix;
                        }
                        else{
                            cout << "Palabra que inicia con : " + pattern << endl;

                            vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                            vector <int> indices;
                            for(int i=1; i< dataToVerify.size(); i++){
                                string evaluate = dataToVerify[i];
                                if(evaluate.substr(0, pattern.length()-1).compare(pattern) == 0){
                                    indices.push_back(i);
                                }
                            }
                            for(int i=0; i< indices.size(); i++){
                                dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                            }
                            verify.push_back("2");
                            matrix.push_back(verify);
                            return matrix;
                        }
                    }
                    size_t spattern = pattern.find("%");
                    if(pattern.length() > spattern){
                        string inicio = pattern.substr(0,spattern-1);
                        string fin = pattern.substr(spattern);
                        cout << "Palabra que inicia con : " + inicio + " y termina con " + fin << endl;

                        vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            string evaluate = dataToVerify[i];
                            if((evaluate.substr(fin.length()-1).compare(fin) == 0) && ( evaluate.substr(0, inicio.length()-1).compare(inicio) == 0)){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++){
                            dataBase->deleteMetadata(tabla,dataBase->getRow(tabla,indices[i])[0]);
                            //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    else{
                        cout << "Patron mal definido" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                }
            }
        }
        else{
            cout << "Syntax error" << endl;
            verify.push_back("0");
            matrix.push_back(verify);
            return matrix;
        }
    }



}

vector<vector<string>> SQLController::funcionUpdate(string comando)
{
    vector<vector<string>> matrix;
    vector <string> verify;
    string subs = comando;
    size_t Set = subs.find("SET ");

    if (3 < Set){
        cout << "Syntax error" << endl;
        verify.push_back("0");
        matrix.push_back(verify);
        return matrix;
    }

    string tabla = subs.substr(0,Set);

    if(tabla[0] == ' '){
        tabla = tabla.substr(1);
    }
    if (tabla[tabla.length()-1] == ' '){
        tabla = tabla.substr(0, tabla.length()-1);
    }

    subs = subs.substr(Set+4);
    size_t pycoma = subs.find(";");
    if (subs.length()< pycoma){
        cout << "Syntax error" << endl;
        verify.push_back("0");
        matrix.push_back(verify);
        return matrix;
    }
    string evaluate = subs.substr(0, pycoma);

    size_t changeEnd = subs.find(";");
    if (subs.length()< changeEnd){
        cout << "Syntax error" << endl;
        verify.push_back("0");
        matrix.push_back(verify);
        return matrix;
    }
    string values = subs.substr(1, changeEnd);
    subs = subs.substr(changeEnd+1);
    cout << values << endl;
    vector<vector<string>> toChange;
    while (values.compare(";") != 0) {
        size_t coma = values.find(",");
        string current;
        if (values.length() < coma) {
            current = values.substr(0, values.length() - 1);
            cout << current << endl;
            values = values.substr(current.length(), values.length() - 1);

        } else {
            current = values.substr(0, coma);
            cout << current << endl;
            values = values.substr(coma + 1);
            cout << values << endl;
        }
        if (current[0] == ' ') {
            current = current.substr(1);
        }
        if (current[current.length() - 1] == ' ') {
            current = current.substr(0, current.length() - 1);
        }

        size_t comand = current.find("=");
        vector <string> changing;
        if (current.length() > comand) {
            string column = current.substr(0, comand);
            current = current.substr(comand + 1);

            if (column[column.length() - 1] == ' ') {
                column = column.substr(0, column.length() - 1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR
            size_t pycoma = current.find(",");
            if (current.length() < pycoma) {
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = current.substr(0, pycoma);
            cout << value << endl;
            if (value[0] == ' ') {
                value = value.substr(1);
            }
            if (value[value.length() - 1] == ' ') {
                value = value.substr(0, value.length() - 1);
            }

            if (value[0] == '"') {
                value = value.substr(1);
            }
            if (value[value.length() - 1] == '"') {
                value = value.substr(0, value.length() - 1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            changing.push_back(column);
            changing.push_back(value);

            toChange.push_back(changing);
            //cout << current << endl;
            //cout << columns << endl;
        } else {
            cout << "Syntax error" << endl;
            verify.push_back("0");
            matrix.push_back(verify);
            return matrix;
        }




    }

    size_t where = subs.find("WHERE ");
    if (2 < where){
        cout << "Syntax error" << endl;
        verify.push_back("0");
        matrix.push_back(verify);
        return matrix;
    }
    subs = subs.substr(where+6);
    if (subs.substr(0,4).compare("NOT ") == 0){
        subs = subs.substr(4);
        size_t comand = subs.find("=");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR
            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR
            vector <string> dataToVerify = dataBase->getColumn(tabla,column);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i].compare(value) != 0){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++) {
                for (int c = 0; i < toChange.size(); c++) {
                    dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                             toChange[c][1]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }


        comand = subs.find("<");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] >= sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++) {
                    for (int c = 0; i < toChange.size(); c++) {
                        dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                 toChange[c][1]);
                        //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] >= v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++) {
                for (int c = 0; i < toChange.size(); c++) {
                    dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                             toChange[c][1]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find(">");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] <= sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++) {
                    for (int c = 0; i < toChange.size(); c++) {
                        dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                 toChange[c][1]);
                        //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] <= v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++) {
                for (int c = 0; i < toChange.size(); c++) {
                    dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                             toChange[c][1]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find(">=");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] < sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++) {
                    for (int c = 0; i < toChange.size(); c++) {
                        dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                 toChange[c][1]);
                        //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] < v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++) {
                for (int c = 0; i < toChange.size(); c++) {
                    dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                             toChange[c][1]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find("<=");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] > sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++) {
                    for (int c = 0; i < toChange.size(); c++) {
                        dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                 toChange[c][1]);
                        //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] > v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++) {
                for (int c = 0; i < toChange.size(); c++) {
                    dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                             toChange[c][1]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        else{
            cout << "Syntax error" << endl;
            verify.push_back("0");
            matrix.push_back(verify);
            return matrix;
        }

    }
    else {

        size_t comand = subs.find("BETWEEN ");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);


            subs = subs.substr(comand+8);
            size_t notCmd = column.find(" NOT ");
            bool notcmd;
            if (column.length()< notcmd){
                notcmd = false;
            }
            else{
                column = column.substr(0, notCmd);
                notcmd = true;
            }
            cout << column << endl;

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }

            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            subs = subs.substr(0,pycoma-1);

            size_t AND = subs.find(" AND ");
            if (AND< subs.length()){
                string value1 = subs.substr(0, AND);

                string value2 = subs.substr(AND+5);

                if(value1[0] == ' '){
                    value1 = value1.substr(1);
                }
                if (value1[value1.length()-1] == ' '){
                    value1 = value1.substr(0, value1.length()-1);
                }

                if(value1[0] == '"'){
                    value1 = value1.substr(1);
                }
                if (value1[value1.length()-1] == '"'){
                    value1 = value1.substr(0, value1.length()-1);
                }

                if(value2[0] == ' '){
                    value2 = value2.substr(1);
                }
                if (value2[value2.length()-1] == ' '){
                    value2 = value2.substr(0, value2.length()-1);
                }

                if(value2[0] == '"'){
                    value2 = value2.substr(1);
                }
                if (value2[value2.length()-1] == '"'){
                    value2 = value2.substr(0, value2.length()-1);
                }


                if(column.compare("YEAR") == 0) {

                    stringstream toint1(value1);
                    int v1;
                    toint1 >> v1;

                    cout<< v1 << endl;

                    stringstream toint2(value2);
                    int v2;
                    toint2 >> v2;
                    cout<< v2 << endl;

                    if(v1 == v2){
                        vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                        vector<int> indices;
                        for (int i = 1; i < dataToVerify.size(); i++) {
                            if(notcmd == false) {
                                if (v2 == dataToVerify[i] ) {
                                    indices.push_back(i);
                                }
                            }
                            else{
                                if (v2 != dataToVerify[i] ) {
                                    indices.push_back(i);
                                }
                            }
                        }
                        for(int i=0; i< indices.size(); i++) {
                            for (int c = 0; i < toChange.size(); c++) {
                                dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                         toChange[c][1]);
                                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                            }
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    else if (v1 > v2) {
                        vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                        vector<int> indices;
                        for (int i = 1; i < dataToVerify.size(); i++) {
                            if(notcmd == false) {
                                if (v2 <= dataToVerify[i] <= v1) {
                                    indices.push_back(i);
                                }
                            }
                            else{
                                if (v2 > dataToVerify[i] > v1) {
                                    indices.push_back(i);
                                }
                            }
                        }
                        for(int i=0; i< indices.size(); i++) {
                            for (int c = 0; i < toChange.size(); c++) {
                                dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                         toChange[c][1]);
                                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                            }
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    } else {
                        vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                        vector<int> indices;
                        for (int i = 1; i < dataToVerify.size(); i++) {
                            if(notcmd == false) {
                                if (v1 <= dataToVerify[i] <= v2) {
                                    indices.push_back(i);
                                }
                            }
                            else{
                                if (v1 > dataToVerify[i] > v2) {
                                    indices.push_back(i);
                                }
                            }
                        }
                        for(int i=0; i< indices.size(); i++) {
                            for (int c = 0; i < toChange.size(); c++) {
                                dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                         toChange[c][1]);
                                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                            }
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }
                }
                else{

                    size_t x = value1.find("x");

                    if (value1.length()< x){
                        cout << "Formato invalido de tamano" << endl;
                        verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                        matrix.push_back(verify);
                        return matrix;
                    }
                    string size1 = value1.substr(0,x);
                    string size2 = value1.substr(x+1);

                    stringstream toint1(size1);
                    int p11;
                    toint1 >> p11;

                    stringstream toint2(size2);
                    int p12;
                    toint2 >> p12;

                    int v1 = p11*p12; /// VALOR A COMPARAR 1

                    x = value2.find("x");

                    if (value2.length()< x){
                        cout << "Formato invalido de tamano" << endl;
                        verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                        matrix.push_back(verify);
                        return matrix;
                    }
                    size1 = value2.substr(0,x);
                    size2 = value2.substr(x+1);

                    stringstream toint3(size1);
                    int p21;
                    toint1 >> p21;

                    stringstream toint4(size2);
                    int p22;
                    toint2 >> p22;

                    int v2 = p21*p22; /// VALOR A COMPARAR 1


                    if(v1 == v2){
                        vector<int> dataToVerify = dataBase->getColumnYear(tabla);
                        vector<int> indices;
                        for (int i = 1; i < dataToVerify.size(); i++) {
                            if(notcmd == false) {
                                if (v2 == dataToVerify[i] ) {
                                    indices.push_back(i);
                                }
                            }
                            else{
                                if (v2 != dataToVerify[i] ) {
                                    indices.push_back(i);
                                }
                            }
                        }
                        for(int i=0; i< indices.size(); i++) {
                            for (int c = 0; i < toChange.size(); c++) {
                                dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                         toChange[c][1]);
                                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                            }
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    else if (v1 > v2) {
                        vector<int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector<int> indices;
                        for (int i = 1; i < dataToVerify.size(); i++) {
                            if(notcmd == false) {
                                if (v2 <= dataToVerify[i] <= v1) {
                                    indices.push_back(i);
                                }
                            }
                            else{
                                if (v2 > dataToVerify[i] > v1) {
                                    indices.push_back(i);
                                }
                            }
                        }
                        for(int i=0; i< indices.size(); i++) {
                            for (int c = 0; i < toChange.size(); c++) {
                                dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                         toChange[c][1]);
                                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                            }
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    } else {
                        vector<int> dataToVerify = dataBase->getColumnSize(tabla);
                        vector<int> indices;
                        for (int i = 1; i < dataToVerify.size(); i++) {
                            if(notcmd == false) {
                                if (v1 <= dataToVerify[i] <= v2) {
                                    indices.push_back(i);
                                }
                            }
                            else{
                                if (v1 > dataToVerify[i] > v2) {
                                    indices.push_back(i);
                                }
                            }
                        }
                        for(int i=0; i< indices.size(); i++) {
                            for (int c = 0; i < toChange.size(); c++) {
                                dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                         toChange[c][1]);
                                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                            }
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }
                }

            }
            else{
                cout << "No hay ningun operador para comparar"<< endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }


        }

        comand = subs.find("IN ");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+3);
            size_t notCmd = column.find(" NOT ");
            bool notcmd;
            if (column.length()< notcmd){
                notcmd = false;
            }
            else{
                column = column.substr(0, notCmd);
                notcmd = true;
            }
            if((subs.substr(0,1)).compare("(") == 0) {
                size_t valuesEnd = subs.find(")");
                if (subs.length() < valuesEnd) {
                    cout << "Syntax error" << endl;
                    verify.push_back("0");
                    matrix.push_back(verify);
                    return matrix;
                }
                string values = subs.substr(1, valuesEnd);
                cout << values << endl;
                vector <string> search;
                while (values.compare(")") != 0){
                    size_t coma = values.find(",");
                    string current;
                    if (values.length()< coma){
                        current = values.substr(0, values.length()-1);
                        //cout << current << endl;
                        values = values.substr(current.length(),values.length()-1);
                    }
                    else {
                        current = values.substr(0, coma);
                        //cout << current << endl;
                        values = values.substr(coma+1);
                        //cout << columns << endl;
                    }
                    if(current[0] == ' '){
                        current = current.substr(1);
                    }
                    if (current[current.length()-1] == ' '){
                        current = current.substr(0, current.length()-1);
                    }
                    if(current[0] == '"'){
                        current = current.substr(1);
                    }
                    if (current[current.length()-1] == '"'){
                        current = current.substr(0, current.length()-1);
                    }
                    cout << current << endl;
                    cout << values << endl;
                    for(int i=0; i< search.size(); i++){
                        if (search[i].compare(current) != 0){
                            search.push_back(current);
                        }
                    }

                }
                int c1 = 0;
                int c2 = 1;
                vector <string> dataToVerify = dataBase->getColumn(tabla, column);
                vector<int> indices;
                while (c2 < dataToVerify.size()){
                    while (c1 < search.size()){
                        if (search[c1].compare(dataToVerify[c2]) == 0){
                            indices.push_back(c2);
                        }
                        c1++;
                    }
                    c1=0;
                    c2++;
                }
                for(int i=0; i< indices.size(); i++) {
                    for (int c = 0; i < toChange.size(); c++) {
                        dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                 toChange[c][1]);
                        //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;

            }
            else {
                cout << "Syntax error"<< endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
        }

        comand = subs.find("=");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR
            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR
            vector <string> dataToVerify = dataBase->getColumn(tabla,column);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i].compare(value) == 0){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++) {
                for (int c = 0; i < toChange.size(); c++) {
                    dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                             toChange[c][1]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find("<");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] < sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++) {
                    for (int c = 0; i < toChange.size(); c++) {
                        dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                 toChange[c][1]);
                        //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] < v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++) {
                for (int c = 0; i < toChange.size(); c++) {
                    dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                             toChange[c][1]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find(">");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] > sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++) {
                    for (int c = 0; i < toChange.size(); c++) {
                        dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                 toChange[c][1]);
                        //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] > v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++) {
                for (int c = 0; i < toChange.size(); c++) {
                    dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                             toChange[c][1]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find(">=");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] >= sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++) {
                    for (int c = 0; i < toChange.size(); c++) {
                        dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                 toChange[c][1]);
                        //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] >= v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++) {
                for (int c = 0; i < toChange.size(); c++) {
                    dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                             toChange[c][1]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find("<=");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+1);

            if (column[column.length()-1] == ' '){
                column = column.substr(0, column.length()-1);
            }

            cout << column << endl; ///SE OBTIENE LA COLUMNA A EVALUAR

            if (column.compare("YEAR") != 0 && column.compare("SIZE") != 0){ ///SE VERIFICA QUE SEA YEAR O SIZE LA COLUMNA
                verify.push_back("-10"); /// -10 para indicar que no se puede aplicar el operador o no existe la columna
                matrix.push_back(verify);
                return matrix;
            }


            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            string value = subs.substr(0, pycoma);
            cout << value << endl;
            if(value[0] == ' '){
                value = value.substr(1);
            }
            if (value[value.length()-1] == ' '){
                value = value.substr(0, value.length()-1);
            }

            if(value[0] == '"'){
                value = value.substr(1);
            }
            if (value[value.length()-1] == '"'){
                value = value.substr(0, value.length()-1);
            }
            cout << value << endl; ///SE OBTIENE EL VALOR A COMPARAR

            if (column.compare("SIZE") == 0){


                size_t x = value.find("x");

                if (value.length()< x){
                    cout << "Formato invalido de tamano" << endl;
                    verify.push_back("0"); /// FORMATO INVALIDO DE SIZE
                    matrix.push_back(verify);
                    return matrix;
                }
                string size1 = value.substr(0,x);
                string size2 = value.substr(x+1);

                stringstream toint1(size1);
                int v1;
                toint1 >> v1;

                stringstream toint2(size2);
                int v2;
                toint2 >> v2;

                int sizeM = v1*v2; /// VALOR A COMPARAR
                vector <int> dataToVerify = dataBase->getColumnSize(tabla);
                vector <int> indices;
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i] <= sizeM){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++) {
                    for (int c = 0; i < toChange.size(); c++) {
                        dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                 toChange[c][1]);
                        //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }

            stringstream toint1(value);
            int v1;
            toint1 >> v1;
            vector <int> dataToVerify = dataBase->getColumnYear(tabla);
            vector <int> indices;
            for(int i=1; i< dataToVerify.size(); i++){
                if(dataToVerify[i] <= v1){
                    indices.push_back(i);
                }
            }
            for(int i=0; i< indices.size(); i++) {
                for (int c = 0; i < toChange.size(); c++) {
                    dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                             toChange[c][1]);
                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                }
            }
            verify.push_back("2");
            matrix.push_back(verify);
            return matrix;
        }

        comand = subs.find("IS ");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+3);
            vector <string> dataToVerify = dataBase->getColumn(tabla, column);
            vector <int> indices;
            if(subs.compare("NOT NULL;") == 0){
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i].compare("NULL") != 0){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++) {
                    for (int c = 0; i < toChange.size(); c++) {
                        dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                 toChange[c][1]);
                        //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }
            else if(subs.compare("NULL;") == 0){
                for(int i=1; i< dataToVerify.size(); i++){
                    if(dataToVerify[i].compare("NULL") == 0){
                        indices.push_back(i);
                    }
                }
                for(int i=0; i< indices.size(); i++) {
                    for (int c = 0; i < toChange.size(); c++) {
                        dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                 toChange[c][1]);
                        //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                    }
                }
                verify.push_back("2");
                matrix.push_back(verify);
                return matrix;
            }
            else{
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
        }

        comand = subs.find("LIKE ");
        if (subs.length() > comand){
            string column = subs.substr(0, comand);
            subs = subs.substr(comand+5 );
            size_t pycoma = subs.find(";");
            if (subs.length()< pycoma){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }

            string pattern = subs.substr(0, pycoma);
            if(pattern[0] == ' '){
                pattern = pattern.substr(1);
            }
            if (pattern[pattern.length()-1] == ' '){
                pattern = pattern.substr(0, pattern.length()-1);
            }

            if(pattern[0] == '"'){
                pattern = pattern.substr(1);
            }
            if (pattern[pattern.length()-1] == '"'){
                pattern = pattern.substr(0, pattern.length()-1);
            }
            size_t operador = pattern.find("%");
            if (pattern.length() < operador){
                cout << "Syntax error" << endl;
                verify.push_back("0");
                matrix.push_back(verify);
                return matrix;
            }
            else{
                if (pattern[0] == '%') {
                    pattern = pattern.substr(1);
                    size_t spattern = pattern.find("%");
                    if (pattern.length() > spattern){
                        pattern = pattern.substr(0,spattern-1);
                        cout << "Patron en cualquier pos: " + pattern << endl;

                        vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            string evaluate = dataToVerify[i];
                            if(evaluate.find(pattern) < evaluate.length()){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++) {
                            for (int c = 0; i < toChange.size(); c++) {
                                dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                         toChange[c][1]);
                                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                            }
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    else{
                        cout << "Terminan en: " + pattern << endl;

                        vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            string evaluate = dataToVerify[i];
                            if((evaluate.length() - evaluate.find(pattern)) == pattern.length()){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++) {
                            for (int c = 0; i < toChange.size(); c++) {
                                dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                         toChange[c][1]);
                                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                            }
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }

                }
                else if(pattern[0] == '_'){
                    pattern = pattern.substr(1);
                    int spaces = 1;
                    while(pattern[0] == '_'){
                        spaces += 1;
                        pattern = pattern.substr(1);
                    }
                    size_t spattern = pattern.find("%");
                    if (pattern.length() > spattern){
                        pattern = pattern.substr(0,spattern-1);
                        cout << "Patron : " + pattern + " luego de " ;
                        cout << spaces;
                        cout << " espacios" << endl;

                        vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            string evaluate = dataToVerify[i];
                            if(evaluate.find(pattern) == spaces){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++) {
                            for (int c = 0; i < toChange.size(); c++) {
                                dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                         toChange[c][1]);
                                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                            }
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    else{
                        cout << "Patron mal definido" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }

                }
                else{
                    if(pattern[pattern.length()-1] == '%'){
                        pattern = pattern.substr(0,pattern.length()-1);
                        size_t spattern = pattern.find("_");
                        if(pattern.length() > spattern){
                            pattern = pattern.substr(0,spattern-1);
                            string spaces = pattern.substr(spattern);
                            int space = 0;
                            while(spaces[0] == '_'){
                                space += 1;
                                spaces = spaces.substr(1);
                            }
                            cout << "Palabra que inicia con : " + pattern + " de largo de " ;
                            cout << space << endl;

                            vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                            vector <int> indices;
                            for(int i=1; i< dataToVerify.size(); i++){
                                string evaluate = dataToVerify[i];
                                if((evaluate.length() == space+ pattern.length() ) && ( evaluate.substr(0, pattern.length()-1).compare(pattern) == 0)){
                                    indices.push_back(i);
                                }
                            }
                            for(int i=0; i< indices.size(); i++) {
                                for (int c = 0; i < toChange.size(); c++) {
                                    dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                             toChange[c][1]);
                                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                                }
                            }
                            verify.push_back("2");
                            matrix.push_back(verify);
                            return matrix;
                        }
                        else{
                            cout << "Palabra que inicia con : " + pattern << endl;

                            vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                            vector <int> indices;
                            for(int i=1; i< dataToVerify.size(); i++){
                                string evaluate = dataToVerify[i];
                                if(evaluate.substr(0, pattern.length()-1).compare(pattern) == 0){
                                    indices.push_back(i);
                                }
                            }
                            for(int i=0; i< indices.size(); i++) {
                                for (int c = 0; i < toChange.size(); c++) {
                                    dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                             toChange[c][1]);
                                    //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                                }
                            }
                            verify.push_back("2");
                            matrix.push_back(verify);
                            return matrix;
                        }
                    }
                    size_t spattern = pattern.find("%");
                    if(pattern.length() > spattern){
                        string inicio = pattern.substr(0,spattern-1);
                        string fin = pattern.substr(spattern);
                        cout << "Palabra que inicia con : " + inicio + " y termina con " + fin << endl;

                        vector <string> dataToVerify = dataBase->getColumn(tabla,column);
                        vector <int> indices;
                        for(int i=1; i< dataToVerify.size(); i++){
                            string evaluate = dataToVerify[i];
                            if((evaluate.substr(fin.length()-1).compare(fin) == 0) && ( evaluate.substr(0, inicio.length()-1).compare(inicio) == 0)){
                                indices.push_back(i);
                            }
                        }
                        for(int i=0; i< indices.size(); i++) {
                            for (int c = 0; i < toChange.size(); c++) {
                                dataBase->modifyMetadata(tabla, dataBase->getRow(tabla, indices[i])[0], toChange[c][0],
                                                         toChange[c][1]);
                                //matrix.push_back(dataBase->getRow(tabla,indices[i]));
                            }
                        }
                        verify.push_back("2");
                        matrix.push_back(verify);
                        return matrix;
                    }
                    else{
                        cout << "Patron mal definido" << endl;
                        verify.push_back("0");
                        matrix.push_back(verify);
                        return matrix;
                    }
                }
            }
        }
        else{
            cout << "Syntax error" << endl;
            verify.push_back("0");
            matrix.push_back(verify);
            return matrix;
        }
    }



}


string SQLController::columnaGET(string evaluar)
{
    if(evaluar.compare("YEAR") == 0){
        return "A";
    }
    else if(evaluar.compare("NAME") == 0){
        return "B";

    }
    else if(evaluar.compare("AUTHOR") == 0){
        return "C";

    }
    else if(evaluar.compare("SIZE") == 0){
        return "D";

    }
    else if(evaluar.compare("DESCRIPTION") == 0){
        return "E";

    }
    else{
        return "F";

    }
}

void SQLController::addToTable(string columna, string value, string imagen)
{
    if(columna.compare("A") == 0){
        //this->fecha = value;
        dataBase->modifyMetadata("", imagen, "YEAR", value);
    }
    else if(columna.compare("B") == 0){
        //this->nombre = value;
        dataBase->modifyMetadata("", imagen, "NAME", value);
    }
    else if(columna.compare("C") == 0){
        //this->autor = value;
        dataBase->modifyMetadata("", imagen, "AUTHOR", value);
    }
    else if(columna.compare("D") == 0){
        //this->size = value;
        dataBase->modifyMetadata("", imagen, "SIZE", value);
    }
    else if(columna.compare("E") == 0){
        //this->descripcion = value;
        dataBase->modifyMetadata("", imagen, "DESCRIPTION", value);
    }
}


void SQLController::makeFunction(string comando) {

    if(comando.length() < 7){
        cout << "Syntax error" << endl;

        ///dataBase.sendJSON("CONSOLE","-1");
    }
    else {
        string subs = comando.substr(0,7);
        string cmd = comando.substr(7);
        if (subs.compare("INSERT ") == 0 || subs.compare("insert ") == 0){
            cout << "Funcion INSERT" << endl;
            funcionInsert(cmd);
        }else if (subs.compare("SELECT ") == 0 || subs.compare("select ") == 0){
            cout << "Funcion SELECT" << endl;
            funcionSelect(cmd);
            //ui->LineaCMD->clear();
        }else if (subs.compare("UPDATE ") == 0 || subs.compare("update ") == 0){
            cout << "Funcion UPDATE" << endl;
            funcionUpdate(cmd);
            //ui->LineaCMD->clear();
        }
        subs = comando.substr(0,12);
        cmd = comando.substr(12);
        if (subs.compare("DELETE FROM ") == 0 || subs.compare("delete from ") == 0){
            cout << "Funcion DELETE FROM" << endl;
            funcionDelete(cmd);
            //ui->LineaCMD->clear();
        }else {
            cout << "Syntax error" << endl;
        }
    }

}


///Getters y Setters


/**
 * Getter de dataBase
 * @return DataBase
 */
DataBase *SQLController::getDataBase() {
    return dataBase;
}

/**
 * Setter de dataBase
 * @param _dataBase
 */
void SQLController::setDataBase(DataBase *_dataBase) {
    dataBase = _dataBase;
}
