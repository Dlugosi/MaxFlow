/*
 *  CrearGraf1.cpp
 *  
 *
 *  Copyright 2018 GrupXdeAlgorismia. Some rights reserved.
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

struct edge{    //es un vol
    int final;  //al que esta conectat
    int cap;
    int flow;
    // bool usat; //per l'algorisme
};

void grafAuxiliar(const vector < vector < edge> > & g, vector < vector < edge> > & r) {
	for (int i = 0; i < (int)g.size(); i++) {
		for (int j = 0; j < (int)g[i].size(); j++) {
            // Edges start with all the flow available.
            edge nova;
            nova.final = g[i][j].final;
            nova.flow = g[i][j].cap;
            nova.cap = g[i][j].cap;
			r[i].push_back(nova);
		}
	}
}

void maxFlowGraf(const vector < vector < edge> > & r, vector < vector < edge> > & g) {
	for (int i = 0; i < (int) r.size(); i++) {
		for (int j = 0; j < (int) r[i].size(); j++) {
			// If capacity is equal to -1 the edge is not an edge of the genuine vector < vector < edge> >.
            if (r[i][j].cap != -1){
                edge nova;
                nova.final = r[i][j].final;
                nova.flow = r[i][j].cap - r[i][j].flow;
                nova.cap = r[i][j].cap;
                g[i].push_back(nova);
            }
		}
	}
}

int findPos(vector < vector < edge> >& r, int ini, int end) {
	for (int i = 0; i < r[ini].size(); ++i) {
		if (r[ini][i].final == end) return i;
    }
    
    edge nova;
    nova.final = end;
    nova.flow = 0;
    nova.cap = -1;
	r[ini].push_back(nova);
	return r[ini].size()-1;
}

void augmentaFlow(vector < vector < edge> >& r, const vector <int>& path, int bottleneck) {
	int act = r.size()-1;//SuperSink
	while (act != 0) {//SuperSource
		int ini = path[act];
		int p1 = findPos(r,act,ini);
		r[act][p1].flow += bottleneck;
		int p2 = findPos(r,ini,act);
		r[ini][p2].flow -= bottleneck;

		act = path[act];
	}
}


int findPath(const vector < vector < edge> >& r, vector <int>& path) {
    int bottleneck = 0;
	path = vector <int> (r.size(),-1);
	vector <int> cap(r.size(),0);
	queue<int> vertexs;
	vertexs.push(0);//SuperSource
	cap[0] = 5000000;
	path[0] = -2;
	while (not vertexs.empty()) {
		int ini = vertexs.front();
		vertexs.pop();
		for (auto e : r[ini]) {
			int act = e.final;
			int capRes = e.flow;
			if (capRes > 0 and path[act] == -1) {
				path[act] = ini;
				cap[act] = min(cap[ini],capRes);
				if (act == r.size() - 1) return cap[act]; //SuperSink
				vertexs.push(act);
			}
        }
	}
    return bottleneck;
}

int maxFlowAlgorithm(vector < vector < edge> > &g){
    vector < vector < edge> > r(g.size());
	grafAuxiliar(g,r);
	vector <int> path;
    int bottleneck;
    int maxflow=0;
	while((bottleneck = findPath(r,path)) > 0) {
		augmentaFlow(r,path,bottleneck);
		maxflow += bottleneck;
	}
	vector < vector < edge> > g1(g.size());
	
	maxFlowGraf(r, g1);
	g = g1;
	return maxflow;
}

int main ()
{
    ifstream input;
    input.open ("inputs/instance_100_2_1.air");
    if(not input.is_open()) cout << "no s'ha trobat el fitxer" << endl;

	vector < vector < int > > Entrada; 
	string line;
	while (not input.eof())
    {
        getline(input, line);
        if(line != ""){
            // cout << line << endl;
            vector < int > Viatge (4);
            Viatge[0] = line[0] - '0';   //Entrada[x][0] es el origen
            // cin >> num;
            Viatge[1] = line[2] - '0';   //Entrada[x][1] es el desti
            // cin >> num;
            Viatge[2] = line[4] - '0';   //Entrada[x][2] es el temps de sortida
            // cin >> num;
            Viatge[3] = line[6] - '0';   //Entrada[x][3] es le temps de arrivada
		//for	(int i=0; i<Viatge.size();i++) cout << Viatge[i] << "  ";
		//cout << endl;
            Entrada.push_back (Viatge);
        }
    }   //<O(n);
    input.close();
                            
    int size = Entrada.size();
    vector < vector < edge> > Graf (2*size+2);
    int sink = (2*size)+1;
	
    for(int i=0; i<size; i++){  
		//conectem el font als origens
        edge nova;
        nova.final = i+1;
        nova.flow = 0;
        nova.cap = 1;
        // nova.ant = 0;
        // nova.usat = false;
        Graf[0].push_back(nova);
		//conectem els origens als destins
        nova.final = i+size+1;
        nova.flow = 0;
        nova.cap = 1;
        // nova.ant = i+1;
        // nova.usat = false;
        Graf[i+1].push_back(nova);
		//conectem els destins al sink
        nova.final = sink;
        nova.flow=0;
        nova.cap=1;
        // nova.ant = i+1+size;
        // nova.usat = false;
        Graf[i+size+1].push_back(nova);
    }                           //O(n)
    
    //ja nomes falta conectar els destins amb els origens adients
    
    for(int i = 0; i<size; i++){
        for (int j=0; j<size; j++){

            if ((Entrada[i][3]+15<Entrada[j][2])&&(Entrada[i][1]==Entrada[j][0])){
                edge nova;
                nova.final=j+1;
                nova.flow=0;
                nova.cap=1;
                // nova.ant = i+size+1;
                // nova.usat = false;
                Graf[i+size+1].push_back(nova);
            }
        }
    }               //O(n^2)
	
	/*for (int i=0; i<Graf.size(); i++) {
		for (int j=0; j<Graf[i].size(); j++) {
			cout << Graf[i][j].final <<" ";
		}
		cout << endl;
	}*/
    
    int flow = maxFlowAlgorithm(Graf);
    
    
    cout << "Flow: " << flow << endl; //imprimir flow i pilots
    // for(int i = 0; i < flow; ++i){
    //     cout << pilots[i][pilots[i].size() - 1];
    //     for(int j = pilots[i].size() - 2; j >= 0; ++j) cout << " " << pilots[i][j];
    //     cout << endl;
    // }
	
}
