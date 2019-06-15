
#include "SQLController.h"
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <string>
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
                    return;
                }

                comand = subs.find(">");
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
                    return;
                }
                comand = subs.find(">=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+2);

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
                    return;
                }

                comand = subs.find("<=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+2);

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
                    return;
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

                        cout<< value1 << endl;
                        cout<< value2 << endl;

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
                    return;
                }

                comand = subs.find("<");
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
                    return;
                }

                comand = subs.find(">");
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
                    return;
                }
                comand = subs.find(">=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+2);

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
                    return;
                }

                comand = subs.find("<=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+2);

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
                    return;
                }

                comand = subs.find("IS ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+3);
                    if(subs.compare("NOT NULL;") == 0){
                        return;
                    }
                    else if(subs.compare("NULL;") == 0){
                        return;
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
                                return;
                            }
                            else{
                                cout << "Terminan en: " + pattern << endl;
                                return;
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
                                return;
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
                                    return;
                                }
                                else{
                                    cout << "Palabra que inicia con : " + pattern << endl;
                                    return;
                                }
                            }
                            size_t spattern = pattern.find("%");
                            if(pattern.length() > spattern){
                                string inicio = pattern.substr(0,spattern-1);
                                string fin = pattern.substr(spattern);
                                cout << "Palabra que inicia con : " + inicio + " y termina con " + fin << endl;
                                return;
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
    else if(subs.find("FROM ") < subs.length()){



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
        if(subs.compare("") == 0 || subs.compare(" ") == 0){
            cout << "Imprimir valores de tabla" << endl;
            return;
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
                    return;
                }

                comand = subs.find("<");
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
                    return;
                }

                comand = subs.find(">");
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
                    return;
                }
                comand = subs.find(">=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+2);

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
                    return;
                }

                comand = subs.find("<=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+2);

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
                    return;
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

                        cout<< value1 << endl;
                        cout<< value2 << endl;

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
                    return;
                }

                comand = subs.find("<");
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
                    return;
                }

                comand = subs.find(">");
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
                    return;
                }
                comand = subs.find(">=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+2);

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
                    return;
                }

                comand = subs.find("<=");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+2);

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
                    return;
                }

                comand = subs.find("IS ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+3);
                    if(subs.compare("NOT NULL;") == 0){
                        return;
                    }
                    else if(subs.compare("NULL;") == 0){
                        return;
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
                                return;
                            }
                            else{
                                cout << "Terminan en: " + pattern << endl;
                                return;
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
                                return;
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
                                    return;
                                }
                                else{
                                    cout << "Palabra que inicia con : " + pattern << endl;
                                    return;
                                }
                            }
                            size_t spattern = pattern.find("%");
                            if(pattern.length() > spattern){
                                string inicio = pattern.substr(0,spattern-1);
                                string fin = pattern.substr(spattern);
                                cout << "Palabra que inicia con : " + inicio + " y termina con " + fin << endl;
                                return;
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


void SQLController::funcionUpdate(string comando)
{
    cout << comando << endl;
}

void SQLController::funcionDelete(string comando)
{
    cout << comando << endl;
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
        }else if (subs.compare("DELETE ") == 0 || subs.compare("delete ") == 0){
            cout << "Funcion DELETE" << endl;
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
