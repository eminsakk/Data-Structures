#include "Graph.h"
Graph::Graph() {
    
    adjList = HashTable<string, list<Edge>>();
    
}

Graph::Graph(const Graph& rhs) {
    
    adjList = rhs.adjList;
}

Graph& Graph::operator=(const Graph& rhs) {
    

    adjList = rhs.adjList;
    return (*this);
}

Graph::~Graph() {
    
    adjList.~HashTable();
}


void Graph::addNode(const Node& node) {
   

    std::list<Edge> edgeS;
    adjList.Insert(node.getCountry(), edgeS);

}

void Graph::addConnection(const Node& headNode, const Node& tailNode, int import) {
    
    Edge newEdge(tailNode, import);

    std::list<Edge>& myEdges = adjList.Get(headNode.getCountry());

    myEdges.push_back(newEdge);

}

list<Node> Graph::getAdjacentNodes(const Node& node) {
    

    std::list<Node> returningList;

    std::list<Edge>& myEdges = adjList.Get(node.getCountry());

    auto iter = myEdges.begin();

    for (iter; iter != myEdges.end(); iter++) {
        returningList.push_back((*iter).getTailNode());
    }

    return returningList;
}

long Graph::getTotalImports(const Node& node) {
    

    std::list<Edge>& myEdges = adjList.Get(node.getCountry());

    long int importValue = 0;

    auto iter = myEdges.begin();

    for (iter; iter != myEdges.end(); iter++) {
        importValue += ((*iter).getImport());
    }

    return importValue;
}

void Graph::deleteNode(const Node& node) {
    
    string* deletingKeys = new string[adjList.Size()];
    adjList.getKeys(deletingKeys);



    for (int i = 0; i < adjList.Size(); i++) {
        std::list<Edge>& deletingList = adjList.Get(deletingKeys[i]);

        auto iter = deletingList.begin();

        for (iter; iter != deletingList.end(); iter++) {

            if ((*iter).getTailNode().getCountry() == node.getCountry()) {

                deletingList.erase(iter);
                break;

            }

        }


    }

    delete[] deletingKeys;

    adjList.Delete(node.getCountry());

}

list<string> Graph::findLeastCostPath(const Node& srcNode, const Node& destNode) {
    
    HashTable<string, long int> distancesFromSRC = HashTable<string, long int>(); 
    std::list<string> path = std::list<string>();
    

    std::string* myKeys = new std::string[adjList.Size()];
    adjList.getKeys(myKeys);

    for (int i = 0; i < adjList.Size(); i++) { 

        distancesFromSRC.Insert(myKeys[i], LONG_MAX);

    }
    distancesFromSRC.Insert(srcNode.getCountry(), 0);
    std::vector<std::pair<string, Node>> pathSSSSs;
    for (int i = 0; i < adjList.Size(); i++) {
        pathSSSSs.push_back(make_pair(myKeys[i], Node()));
    }

    delete[] myKeys; 


    std::priority_queue<std::pair<long int, Node>,std::vector<std::pair<long int, Node>>,comparator> priorityQ;  //priority queue for dijkstra

    priorityQ.push(make_pair(0, srcNode));

    
    
    
    
    while (!priorityQ.empty()) {


        long int tmpDistance = priorityQ.top().first;
        Node tmpNode = priorityQ.top().second;
        priorityQ.pop();
        auto iter123 = adjList.Get(tmpNode.getCountry()).begin();
        
        for (iter123; iter123 != adjList.Get(tmpNode.getCountry()).end(); iter123++) {

            if (distancesFromSRC.Get((*iter123).getTailNode().getCountry()) > tmpDistance + (*iter123).getImport()) {
                
                distancesFromSRC.Get((*iter123).getTailNode().getCountry()) = tmpDistance + (*iter123).getImport();
                priorityQ.push(make_pair(distancesFromSRC.Get((*iter123).getTailNode().getCountry()), (*iter123).getTailNode()));
                int i = 0;
                for (i; i < pathSSSSs.size(); i++) {
                    if ((*iter123).getTailNode().getCountry() == pathSSSSs[i].first) {
                        break;
                    }
                }
                pathSSSSs[i].second = tmpNode;
                
            }
                
        }
        
    }
    int j = 0;
    for (; j < pathSSSSs.size(); j++) {

        if (pathSSSSs[j].first == destNode.getCountry()) {
            break;
        }

    }
    
    
    while (j < pathSSSSs.size() && pathSSSSs[j].first != srcNode.getCountry()) {
        std::string prvNode = pathSSSSs[j].second.getCountry();
        int k = 0;
        path.push_front(pathSSSSs[j].first);
        
        for (; k < pathSSSSs.size(); k++) {
            if (prvNode == pathSSSSs[k].first) {
                break;
            }
        }
        
        j = k;
    
    }
    
    path.push_front(srcNode.getCountry());
    return path;
    

}
bool Graph::isCyclicHelper(const string& key, HashTable<string, bool> visitedTable, HashTable<string, bool> isInRecursion) {

    if (!visitedTable.Get(key)) {
        visitedTable.Insert(key, true);
        isInRecursion.Insert(key, true);

        std::list<Edge>& edgeS = adjList.Get(key);

        std::list<string> tmpAdjacents;

        for (auto iter = edgeS.begin(); iter != edgeS.end(); iter++) {

            tmpAdjacents.push_back((*iter).getTailNode().getCountry());
        }

        for (auto iter = tmpAdjacents.begin(); iter != tmpAdjacents.end(); iter++) {
            if (!visitedTable.Get((*iter)) && isCyclicHelper((*iter), visitedTable, isInRecursion)) {
                
                return true;
            }
            else if (isInRecursion.Get((*iter)) && (*iter) != key) {
                return true;
            }
        }
    }

    isInRecursion.Insert(key, false);
    return false;
}



bool Graph::isCyclic() {
    
    HashTable<string, bool> visitedTable;
    string* myKeys = new string[adjList.Size()];
    adjList.getKeys(myKeys);
    HashTable<string, bool> isIntheRecursion;
    int returningFlag = 0;

    for (int i = 0; i < adjList.Size(); i++) {
        visitedTable.Insert(myKeys[i], false);
        isIntheRecursion.Insert(myKeys[i], false);
    }
    for (int i = 0; i < adjList.Size(); i++) {

        if (isCyclicHelper(myKeys[i], visitedTable, isIntheRecursion)) {
            returningFlag = 1;
        }

    }

    delete[] myKeys;

    if (returningFlag) {
        return true;
    }
    else {
        return false;
    }
}


list<string> Graph::getBFSPath(const Node& srcNode, const Node& destNode) {
    

    std::list<string> path;
    queue<string> tmpQueue;
    HashTable<string, int> visitedTable;
    string* myKeys = new string[adjList.Size()];
    adjList.getKeys(myKeys);

    for (int i = 0; i < adjList.Size(); i++) {
        visitedTable.Insert(myKeys[i], 0);
    }

    delete[] myKeys;
    std::list<Edge> edgeS = adjList.Get(srcNode.getCountry());

    tmpQueue.push(srcNode.getCountry());
    visitedTable.Insert(srcNode.getCountry(), 1);

    int flag = 1;
    int controlVariable = 0;
    std::list<Node> helperList;
    while (!tmpQueue.empty()) {

        string tmp = tmpQueue.front();
        tmpQueue.pop();
        path.push_back(tmp);

        if (flag) {

            for (auto iter = edgeS.begin(); iter != edgeS.end(); iter++) {

                helperList.push_back((*iter).getTailNode());

                if (!(visitedTable.Get((*iter).getTailNode().getCountry()))) {
                    tmpQueue.push((*iter).getTailNode().getCountry());
                    visitedTable.Insert((*iter).getTailNode().getCountry(), 1);
                }

                if ((*iter).getTailNode().getCountry() == destNode.getCountry()) {

                    flag = 0;
                    break;
                }

            }

            auto iter1 = helperList.begin();
            for (int j = 0; iter1 != helperList.end() && j < controlVariable; iter1++, j++);
            edgeS = adjList.Get((*iter1).getCountry());
            controlVariable++;

        }

    }
    return path;
}
