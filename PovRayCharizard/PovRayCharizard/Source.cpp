#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

int main()
{
    ////////////////////////////Variables////////////////////////////
    ifstream file("Charizard.obj");//File being read

    vector<string> data; //Stores File Data
    vector<string> vertexData; //Stores Vertex Data
    vector<string> faceData; //Stores Face Data
    vector<string> Output; //Stores what will be written to the File
    vector<tuple<double, double, double>> VertexCoords; //Stores the Locations of where Vertex are located
    vector<tuple<string, string, string>> FaceCoords; //Stores the Locations of where Faces are located

    pair<int, int>v_range = make_pair(0, 0); //Range of data where vertex Data Exists
    pair<int, int>vt_range = make_pair(0, 0); //Range of data where vertex-tangent Data Exits
    pair<int, int>vn_range = make_pair(0, 0); //Range of data where vertex-normal Data Exists
    pair<int, int>f_range = make_pair(0, 0); //Range of data where face Data Exists

    string v_coord[3]; //Temporay Storage for each coordinate for a vertex
    string f_coord[3]; //Temporary Storage for each coordinate for a face

    string input; //Used as an object to aid with storage of data
    string tmp; //Used for temporary storage of a string value as needed
    int index; //The index variables that is used as either a delimiter or index value for coordinates
    int c_index = 0;
    int CoordEntered = 0;
    double CoordVal;

    ////////////////////////////Read File////////////////////////////
    while (!file.eof())
    {
        getline(file, input);
        data.push_back(input);
    }
    file.close();

    ////////////////Erase the first few elements of v////////////////
    for (int i = 0; i < 4; i++)
    {
        data.erase(data.begin());
    }

    ///////////////////////////////Calculations//////////////////////
    //Calculate the range for vertices
    for (int i = 0; i < data.size(); i++)
    {
        string seg = data[i].substr(0, 2);
        if (seg == "v ")
        {
            v_range.second++;
        }
    }
    v_range.second--;

    //Calculate the range for vertex_tangents
    vt_range.first = v_range.second + 1;
    for (int i = vt_range.first; i < data.size(); i++)
    {
        string seg = data[i].substr(0, 2);
        if (seg == "vt")
        {
            vt_range.second++;
        }
    }
    vt_range.second = vt_range.second + vt_range.first;

    //Calcualte the range for vertex normals
    vn_range.first = vt_range.second + 1;
    for (int i = vn_range.first; i < data.size(); i++)
    {
        string seg = data[i].substr(0, 2);
        if (seg == "vn")
        {
            vn_range.second++;
        }
    }
    vn_range.second = vn_range.second + vn_range.first;

    //Calculate the range for faces
    f_range.first = vn_range.second + 2;
    for (int i = f_range.first; i < data.size(); i++)
    {
        f_range.second++;
    }
    f_range.second = f_range.second + f_range.first;
    f_range.second -= 2;

    //Store Vertex Data
    for (int i = v_range.first; i < v_range.second + 1; i++)
    {
        vertexData.push_back(data[i]);
    }

    //Store Face Data
    for (int i = f_range.first; i < f_range.second + 1; i++)
    {
        input = data[i].substr(0, 6);
        if (!(input == "usemtl"))
        {
            faceData.push_back(data[i]);
        }
    }

    //Store Vertex Coordinate Data
    for (int i = 0; i < vertexData.size(); i++)
    {
        index = vertexData[i].find(" ");
        input = vertexData[i].substr(index + 1, vertexData[i].length() - 1);

        for (int j = 0; j < 3; j++)
        {
            if (j < 2)
            {
                index = input.find(" ");
                v_coord[j] = input.substr(0, index - 1);
                input = input.substr(index + 1, input.length() - 1);
            }
            else
            {
                v_coord[j] = input;
            }
        }
        VertexCoords.push_back(make_tuple(stod(v_coord[0]), stod(v_coord[1]), stod(v_coord[2])));
    }

    //Store Face Coordinate Data
    for (int i = 0; i < faceData.size(); i++)
    {
        index = faceData[i].find(" ");
        input = faceData[i].substr(index + 1, faceData[i].length() - 1);

        for (int j = 0; j < 3; j++)
        {
            if (j < 2)
            {
                index = input.find(" ");
                f_coord[j] = input.substr(0, index);
                input = input.substr(index + 1, input.length() - 1);
            }
            else
            {
                f_coord[j] = input;
            }
        }
        FaceCoords.push_back(make_tuple(f_coord[0], f_coord[1], f_coord[2]));
    }

    ////////////////////////////Generation of File Content////////////////////////////
    Output.push_back("union{");
    for (int i = 0; i < FaceCoords.size(); i++)
    {
        tmp = "  triangle{<";

        input = get<0>(FaceCoords[i]);

        index = input.find("/");
        c_index = stoi(input.substr(0, index));
        input = input.substr(index + 1, input.length() - 1);
        tmp.append(to_string(get<0>(VertexCoords[c_index - 1])));
        tmp.append(", ");

        index = input.find("/");
        c_index = stoi(input.substr(0, index));
        input = input.substr(index + 1, input.length() - 1);
        tmp.append(to_string(get<1>(VertexCoords[c_index - 1])));
        tmp.append(", ");

        index = input.find("/");
        c_index = stoi(input.substr(0, index));
        input = input.substr(index + 1, input.length() - 1);
        tmp.append(to_string(get<2>(VertexCoords[c_index - 1])));
        tmp.append(">,<");

        input = get<1>(FaceCoords[i]);

        index = input.find("/");
        c_index = stoi(input.substr(0, index));
        input = input.substr(index + 1, input.length() - 1);
        tmp.append(to_string(get<0>(VertexCoords[c_index - 1])));
        tmp.append(", ");

        index = input.find("/");
        c_index = stoi(input.substr(0, index));
        input = input.substr(index + 1, input.length() - 1);
        tmp.append(to_string(get<1>(VertexCoords[c_index - 1])));
        tmp.append(", ");

        index = input.find("/");
        c_index = stoi(input.substr(0, index));
        input = input.substr(index + 1, input.length() - 1);
        tmp.append(to_string(get<2>(VertexCoords[c_index - 1])));
        tmp.append(">,<");

        input = get<2>(FaceCoords[i]);

        index = input.find("/");
        c_index = stoi(input.substr(0, index));
        input = input.substr(index + 1, input.length() - 1);
        tmp.append(to_string(get<0>(VertexCoords[c_index - 1])));
        tmp.append(", ");

        index = input.find("/");
        c_index = stoi(input.substr(0, index));
        input = input.substr(index + 1, input.length() - 1);
        tmp.append(to_string(get<1>(VertexCoords[c_index - 1])));
        tmp.append(", ");

        index = input.find("/");
        c_index = stoi(input.substr(0, index));
        input = input.substr(index + 1, input.length() - 1);
        tmp.append(to_string(get<2>(VertexCoords[c_index - 1])));
        tmp.append(">}");

        Output.push_back(tmp);
    }
    Output.push_back("texture{White}");
    Output.push_back("}");

    
    ofstream output;
    output.open("PovCharizard.txt");
    for (int i = 0; i < Output.size(); i++)
    {
        output << Output[i] << endl;
    }
    output.close();
    return 0;
}