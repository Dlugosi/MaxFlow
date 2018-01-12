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

int sink;
int source;
int supersource;
int supersink;


struct edge{    //es un vol
    int final;  //al que esta conectat
    int capacitat;
	int flow;
};

void grafAuxiliar(const vector < vector < edge> > & g, vector < vector < edge> > & r) {
	for (int i = 0; i < (int)g.size(); i++) {
		for (int j = 0; j < (int)g[i].size(); j++) {
            // Edges start with all the flow available.
            edge nova;
            nova.final = g[i][j].final;
            nova.flow = g[i][j].capacitat;
            nova.capacitat = g[i][j].capacitat;
			r[i].push_back(nova);
		}
	}
}

void maxFlowGraf(const vector < vector < edge> > & r, vector < vector < edge> > & g) {
	for (int i = 0; i < (int) r.size(); i++) {
		for (int j = 0; j < (int) r[i].size(); j++) {
			// If capacity is equal to -1 the edge is not an edge of the genuine vector < vector < edge> >.
            if (r[i][j].capacitat != -1){
                edge nova;
                nova.final = r[i][j].final;
                nova.flow = r[i][j].capacitat - r[i][j].flow;
                nova.capacitat = r[i][j].capacitat;
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
    nova.capacitat = -1;
	r[ini].push_back(nova);
	return r[ini].size()-1;
}

void augmentaFlow(vector < vector < edge> >& r, const vector <int>& path, int bottleneck) {
	int act = supersink;
	while (act != supersource) {
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
	vertexs.push(supersource);
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
				if (act == supersink) return cap[supersink];
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
    vector < vector < edge> > Graf (2*size+4);
    sink = (2*size)+1;
    source = 0;
    supersource = sink+1;
    supersink = supersource+1;
	
    for(int i=0; i<size; i++){  
		//conectem el source als origens
        edge nova;
        nova.final = i+1;
        nova.capacitat = 1;
        nova.flow = 0;
        Graf[source].push_back(nova);
		//no conectem els origens als destins, sino que els transformem en sinks i sources per complir
		//el lower bound (no queda aresta). Per fer aixo cal un super sink i un super source que rep i dona tot el fluxe
	    //primer els nous sink(origens) al supersink (falta el sink original) 
		
        nova.final = supersink;
        nova.capacitat = 1;
        nova.flow = 0;
        Graf[i+1].push_back(nova);
		
	    //ara el supersource als destins falta conectar al source
		
		nova.final = i+size+1;
        nova.capacitat = 1;
        nova.flow = 0;
        Graf[supersource].push_back(nova);
		
		//conectem els destins al sink
        nova.final = sink;
        nova.capacitat=1;
        nova.flow = 0;
        Graf[i+size+1].push_back(nova);
    }                           //O(n)
    
    //ja nomes falta conectar els destins amb els origens adients
    
    for(int i = 0; i<size; i++){
        for (int j=0; j<size; j++){
			
            if ((Entrada[i][3]+15<Entrada[j][2])&&(Entrada[i][1]==Entrada[j][0])){
                edge nova;
                nova.final=j+1;
                nova.capacitat=1;
		        nova.flow=0;
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
	bool primer = true;
	int Pmin=0;
	int Pmax=size;
	int millorflow = 0;
	while(Pmax >= Pmin){
        
        int valor = (Pmin+Pmax)/2;
		
		edge nova;
        nova.final = supersink;
        nova.capacitat = valor;
        nova.flow = 0;
		if (primer){
            Graf[sink].push_back(nova);//conectem sink a supersink
            primer = false;
		}
		else {
			Graf[sink][Graf[sink].size()-1] = nova;
		}
        nova.final = source;
        nova.capacitat = valor;
        nova.flow = 0;
		if (primer){
			Graf[supersource].push_back(nova);//conectem sink a supersink
		}
		else {
			Graf[supersource][Graf[supersource].size()-1] = nova;
		}
				
		if ((valor+size) == maxFlowAlgorithm(Graf)) { //aixo cal canviarho
			Pmax = valor -1;//no volem tornar a provar aquest
			millorflow = valor;
			//guardar resposta
		}
		else {
			Pmin = valor+1;
		}        
	}
	
	/*for (int i=0; i<Graf.size(); i++) {
		for (int j=0; j<Graf[i].size(); j++) {
			cout << Graf[i][j].final <<" ";
		}
		cout << endl;
	}*/    
    
    cout << "Flow: " << millorflow << endl; //imprimir flow i pilots
    // for(int i = 0; i < flow; ++i){
    //     cout << pilots[i][pilots[i].size() - 1];
    //     for(int j = pilots[i].size() - 2; j >= 0; ++j) cout << " " << pilots[i][j];
    //     cout << endl;
    // }
	
}
