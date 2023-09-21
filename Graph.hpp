/*
Assignment 4 (Dijkstra's Algorithm)
Driver: Max Aubuchon
Navigator: Dawson Murphy
*/

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <limits.h>

//Class used for the edges
class edge {
    public:
        std::string dst;
        unsigned long length;
        edge(){};
        edge(std::string label, unsigned long w){
            dst = label;
            length = w;
        }

        //returns the destination of the edge
        std::string getDst(){
            return dst;
        }

        //returns the weight of the edge
        unsigned long getLength(){
            return length;
        }
};

//class used for the vertices
class vertex{
    public:
    std::string name;
    int id;

    //each vertex contains a list of edges
    std::list<edge> edgeList;
    vertex(){
    }
    vertex(std::string label, int number){
        name = label;
        id = number;
    }

    //returns the ID of the vertex
    int getID(){
        return id;
    }

    //sets the ID of the vertex
    void setID(int i){
        id = i;
    }

    //returns the name of the vertex
    std::string getName(){
        return name; 
    }

    //sets the name of the vertex
    void setName(std::string label){
        name = label; 
    }

    //returns the list of edges connected to the vertex
    std::list<edge> getedgeList(){
        return edgeList;
    }
};

//class for the graph
class Graph {
    public:
        //constructor
        Graph();

        int numVert = 0; //number of vertices in the graph
        std::vector<vertex> vertices; //creates a vector of vertices

        //adds a vertex to the graph
        void addVertex(std::string label){
            bool check = checkVertex(label);
            if(check){
                return;
            }
            else{
                vertex temp;
                temp.setName(label);
                temp.setID(numVert);
                numVert++;
                vertices.push_back(temp);
            }
        }

        //removes a vertex from the graph
        void removeVertex(std::string label){
            int vin = 0;
            for(unsigned int i = 0; i < vertices.size(); i++){
                if(vertices.at(i).getName() == label){
                    vin = i;
                }
            }
            for(unsigned int i = 0; i<vertices.size(); i++) {
                removeEdge(label, vertices.at(i).getName());
            }
            vertices.erase(vertices.begin() + vin);
            for(unsigned int i = vin; i<vertices.size(); i++){
                vertices.at(i).setID(i);
            }
        }

        //adds an edge to the graph
        void addEdge(std::string label1, std::string label2, unsigned long weight){
            bool check1 = checkVertex(label1);
            bool check2 = checkVertex(label2);
            if(check1 && check2 == true){
                bool check3 = checkEdge(label1, label2);
                if(check3 == true){
                    return;
                }
            else {
                for(unsigned int i =0; i < vertices.size(); i++){
                    if(vertices.at(i).getName() == label1){
                        edge e(label2, weight);
                        vertices.at(i).edgeList.push_back(e);
                    }
                    else if (vertices.at(i).getName() == label2){
                        edge e(label1, weight);
                        vertices.at(i).edgeList.push_back(e);
                    }   
                }//end for loop
            }// end else       
        }// end outter if
    }//end func

        //removes an edge from the graph
        void removeEdge(std::string label1, std::string label2){
            bool check = checkEdge(label1, label2);
            
            if(check == true){
                
                for(unsigned int i = 0; i < vertices.size(); i++){
                    if(vertices.at(i).getName() == label1){
                        for(auto it = vertices.at(i).edgeList.begin(); it != vertices.at(i).edgeList.end(); it++){
                            if(it-> getDst() == label2){
                                
                                vertices.at(i).edgeList.erase(it);
                                break;
                            }
                        }//end inner for loop
                    }//end inner if
                    if (vertices.at(i).getName() == label2){
                        for(auto it = vertices.at(i).edgeList.begin(); it != vertices.at(i).edgeList.end(); it++){
                            if(it-> getDst() == label1){
                                vertices.at(i).edgeList.erase(it);
                                break;
                            }
                        }//end inner for
                    }//end inner if
                }// end outter for loop
            }// end first if
        }// end func
    
        //finds the index of the vertex with the minimum distance
        unsigned long minDistance(unsigned long dist[], bool sptSet[]){
            unsigned int min = INT_MAX;
            int min_index;

            for(unsigned int v = 0; v < vertices.size(); v++){
                if(sptSet[v] == false && dist[v] <= min){
                    min = dist[v];
                    min_index = v;
                }
            }

            return min_index;
        }

        //finds the minimum distance between two vertices
        unsigned long shortestPath(std::string startLabel, std::string endLabel, std::vector<std::string> &path){
            unsigned long pathLength = 0;

            //arrays of the same size as the vector are used to sort through the vector
            unsigned long dist[vertices.size()];
            bool sptSet[vertices.size()];
            unsigned int parent[vertices.size()];

            //sets all the values in dist and sptSet to their default values
            for (unsigned long i = 0; i < vertices.size(); i++){
                dist[i] = INT_MAX;
                sptSet[i] = false;
            }

            //index is used to keep track of which vertex is being used
            int index = 0;

            //sets index to the starting vertex
            for(unsigned int i = 0; i < vertices.size(); i++){
                if(vertices.at(i).getName() == startLabel){
                    index = i;
                }
            }

            //sets the value at the starting vertex's index to infinity
            parent[index] = INT_MAX;

            //sets the distance from the starting vertex to itself as 0
            dist[index] = 0;

            //sets the distances for the starting vertex to each vertex 
            for(unsigned int i = 0; i < vertices.size(); i++){
                unsigned long u = minDistance(dist, sptSet);
                sptSet[u] = true;
                for(unsigned int v = 0; v < vertices.size(); v++){
                    if(!sptSet[v] && checkEdge(vertices.at(u).getName(), vertices.at(v).getName()) && dist[u] + edgeLength(vertices.at(u).getName(), vertices.at(v).getName()) < dist[v]){
                        parent[v] = vertices.at(u).getID();
                        dist[v] = dist[u] + edgeLength(vertices.at(u).getName(), vertices.at(v).getName());
                    }
                }
            }

            //Creates the path from the starting vertex to the end vertex
            printSolution(dist, vertices.size(), parent, path, startLabel, endLabel);
            
            //Sets the final length of the path
            for(unsigned int i = 0; i < vertices.size(); i++){
                if(vertices.at(i).getName() == endLabel){
                    pathLength = dist[i];
                }
            }

            //returns the path length
            return pathLength;
        }

        //recursive function that creates the path
        void printPath(unsigned int parent[], unsigned int j, std::vector<std::string> &path){
            if (parent[j] == INT_MAX){
                return;
            }
            
            printPath(parent, parent[j], path);
            path.push_back(vertices.at(j).getName());
        }
        
        //sets the path from one vertex to another
        int printSolution(unsigned long dist[], int n, unsigned int parent[], std::vector<std::string> &path, std::string startLabel, std::string endLabel){
            std::string src = startLabel;
            for(unsigned int i=0; i < vertices.size(); i++){
                if(vertices.at(i).getName() == endLabel){
                    path.push_back(src);
                    printPath(parent, i, path);
                }
            }
            return 0;
        }

        //checks to see if a vertex already exists
        bool checkVertex(std::string name){
            for(unsigned int i = 0; i<vertices.size(); i++){
                if(vertices.at(i).getName()== name)
                    return true;
                }
            return false;
        }

        //returns a vertex for a given name
        vertex getVertex(std::string label){
            vertex temp;
            for(unsigned int i = 0; i < vertices.size(); i++){
                if(temp.getName() == label)
                return temp;
            }
        }

        //returns the weight of an edge
        unsigned long edgeLength(std::string scr, std::string dst){
            for(unsigned int i = 0; i < vertices.size(); i++){
                if(vertices.at(i).getName() == scr){
                    for(auto it = vertices.at(i).edgeList.begin(); it != vertices.at(i).edgeList.end(); it++){
                        if(it -> getDst() == dst){
                            return it -> getLength();
                            break;
                        }//end if
                    }//end for
                }
            }
            
            return 0;
        }

        //checks if an edge exists
        bool checkEdge(std::string scr, std::string dst){
            for(unsigned int i = 0; i < vertices.size(); i++){
                if(vertices.at(i).getName() == scr){
                    for(auto it = vertices.at(i).edgeList.begin(); it != vertices.at(i).edgeList.end(); it++){
                        if(it -> getDst() == dst){
                            return true;
                            break;
                        }//end if
                    }//end for
                }
            }
            
                return false; 
        }//end func

};//end graph class

#endif