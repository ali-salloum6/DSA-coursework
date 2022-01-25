#include <iostream>
#include <vector>
#include <set>
#include <iterator>

using namespace std;

template<typename T>
class Vertex {
private:

    static int idCounter;
    int id;
    T value;

public:

    Vertex() {}

    Vertex(T val) {
        this->value=val;
        this->id=++idCounter;
    }

    T getValue() const {
        return value;
    }

    int getId() {
        return id;
    }

    bool operator<(const Vertex<T>& v) const {
        return this->id<v.id;
    }

    bool operator==(const Vertex<T>& v) const {
        return this->id==v.id;
    }
};
template<typename T> int Vertex<T>::idCounter=0;
Vertex<string> nullVertex("-");

template<typename T,typename U>
class Edge {
private:

    Vertex<T> from,to;
    U weight;

public:

    Edge() {}

    Edge (Vertex<T> from,Vertex<T> to,U weight) {
        this->from=from;
        this->to=to;
        this->weight=weight;
    }

    Vertex<T> getFrom() {
        return this->from;
    }

    Vertex<T> getTo() {
        return this->to;
    }

    U getWeight() {
        return this->weight;
    }

    bool operator==(const Edge<T,U>& e) const {
        return !(!(this->from==e.from && this->to==e.to && this->weight==e.weight));
    }
};
Edge<string,int> nullEdge(nullVertex,nullVertex,0);

template<typename T,typename U>
class GraphADT {
public:
    virtual Vertex<T> addVertex(T value) = 0;
    virtual void removeVertex(Vertex<T> v) = 0;
    virtual Edge<T,U> addEdge(Vertex<T> from,Vertex<T> to,U weight) = 0;
    virtual void removeEdge(Edge<T,U> e) = 0;
    virtual vector<Edge<T,U>> edgesFrom(Vertex<T> v) = 0;
    virtual vector<Edge<T,U>> edgesTo(Vertex<T> v) = 0;
    virtual Vertex<T> findVertex(T value) = 0;
    virtual Edge<T,U> findEdge(T from_value, T to_value) = 0;
    virtual bool hasEdge(Vertex<T> v, Vertex<T> u) = 0;
};


template<typename T, typename U>
class AdjacencyMatrixGraph : public GraphADT<T,U> {
private:

    vector<Vertex<T>> vertices;
    vector<vector<Edge<T,U>>> adjMatrix;

public:

    AdjacencyMatrixGraph() {
        vertices.resize(500);
        adjMatrix.resize(500);
        for (int i=0;i<500;i++) {
            adjMatrix[i].resize(500);
        }
        for (int i=0;i<500;i++) {
            for (int j=0;j<500;j++) {
                adjMatrix[i][j]=nullEdge;
            }
        }
    }

    Vertex<T> addVertex(T value) {
        Vertex<T> v(value);
        vertices.push_back(v);
        return v;
    }

    void removeVertex(Vertex<T> v) {
        vertices[v.getId()]=nullVertex;
        for (int i=0;i<500;i++) {
            adjMatrix[v.getId()][i]=nullEdge;
            adjMatrix[i][v.getId()]=nullEdge;
        }
    }

    Edge<T,U> addEdge(Vertex<T> from,Vertex<T> to,U weight) {
        Edge<T,U> e(from,to,weight);
        adjMatrix[from.getId()][to.getId()]=e;
        return e;
    }

    void removeEdge(Edge<T,U> e) {
        adjMatrix[e.getFrom().getId()][e.getTo().getId()]=nullEdge;
    }

    vector<Edge<T,U>> edgesFrom(Vertex<T> v) {
        vector<Edge<T,U>> toReturn;
        for (auto it:adjMatrix[v.getId()]) {
            if (!(it==nullEdge)) toReturn.push_back(it);
        }
        return toReturn;
    }

    vector<Edge<T,U>> edgesTo(Vertex<T> v) {
        vector<Edge<T,U>> toReturn;
        for (auto it:vertices) {
            if (!(adjMatrix[it.getId()][v.getId()]==nullEdge)) {
                Edge<T,U> e=adjMatrix[it.getId()][v.getId()];
                toReturn.push_back(e);
            }
        }
        return toReturn;
    }

    Vertex<T> findVertex(T value) {
        for (auto it:vertices) {
            if (it.getValue()==value) {
                return it;
            }
        }
        return nullVertex;
    }

    Edge<T,U> findEdge(T from_value, T to_value) {
        Vertex<T> f = findVertex(from_value);
        Vertex<T> t = findVertex(to_value);
        return adjMatrix[f.getId()][t.getId()];
    }

    bool hasEdge(Vertex<T> v, Vertex<T> u) {
        if (adjMatrix[v.getId()][u.getId()]==nullEdge) {
            return false;
        }
        return true;
    }
};

void query(string instruction) {
    static AdjacencyMatrixGraph<string,int> g;
    if (instruction=="ADD_VERTEX") {
        string name;
        cin>>name;
        g.addVertex(name);
    }
    if (instruction=="REMOVE_VERTEX") {
        string name;
        cin>>name;
        Vertex<string> v=g.findVertex(name);
        g.removeVertex(v);
    }
    if (instruction=="ADD_EDGE") {
        string fromName,toName;
        int weight;
        cin>>fromName>>toName>>weight;
        Vertex<string> f=g.findVertex(fromName);
        Vertex<string> t=g.findVertex(toName);
        Edge<string,int> e = g.addEdge(f,t,weight);
    }
    if (instruction=="REMOVE_EDGE") {
        string fromName,toName;
        cin>>fromName>>toName;
        Edge<string,int> e=g.findEdge(fromName,toName);
        g.removeEdge(e);
    }
    if (instruction=="HAS_EDGE") {
        string fromName,toName;
        cin>>fromName>>toName;
        Vertex<string> f=g.findVertex(fromName);
        Vertex<string> t=g.findVertex(toName);
        if (g.hasEdge(f,t)==true) {
            cout<<"TRUE\n";
        }
        else {
            cout<<"FALSE\n";
        }
    }
}

int main()
{
    string instruction;
    while (cin>>instruction) {
        query(instruction);
    }
}

