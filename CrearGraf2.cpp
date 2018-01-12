/*
 *  CrearGraf1.cpp
 *  
 *
 *  Copyright 2018 Grup21deAlgorismia. Some rights reserved.
 *
 */

#include <iostream>
#include <stdlib.h> 
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <time.h>

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
	path = vector <int> (r.size(),-1);
	vector <int> cap(r.size(),0);
	queue<int> vertexs;
	vertexs.push(supersource);
	cap[supersource] = 5000000;
	path[supersource] = -2;
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
    return 0;
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

void generarGraf( vector < vector < edge> > &Graf, int &size, vector < vector < int > > &Entrada, int &valor){
    for(int i=0; i<size; i++){  
		//conectem el source als origens
        edge nova;
        nova.final = i+4;
        nova.capacitat = 1;
        nova.flow = 0;
        Graf[source].push_back(nova);
		//no conectem els origens als destins, sino que els transformem en sinks i sources per complir
		//el lower bound (no queda aresta). Per fer aixo cal un super sink i un super source que rep i dona tot el fluxe
	    //primer els nous sink(origens) al supersink (falta el sink original) 
		
        nova.final = supersink;
        nova.capacitat = 1;
        nova.flow = 0;
        Graf[i+4].push_back(nova);
		
	    //ara el supersource als destins falta conectar al source
		
		nova.final = i+size+4;
        nova.capacitat = 1;
        nova.flow = 0;
        Graf[supersource].push_back(nova);
		
		//conectem els destins al sink
        nova.final = sink;
        nova.capacitat=1;
        nova.flow = 0;
        Graf[i+size+4].push_back(nova);
    }                           //O(n)
    
    //ja nomes falta conectar els destins amb els origens adients
    bool transbord[size][size];
      for (int i=0; i<size; i++){
 	for(int j = 0; j<size; j++){
 		transbord[i][j]=false;
	}
      } 
	
    for(int i = 0; i<size; i++){
        for (int j=0; j<size; j++){
			
            if ((Entrada[i][3]+15<Entrada[j][2])&&(Entrada[i][1]==Entrada[j][0])){
                edge nova;
                nova.final=j+4;
                nova.capacitat=1;
		        nova.flow=0;
                Graf[i+size+4].push_back(nova);
            }
        }//O(n^2)
	    //es la unica diferencia amb la versio 1
    //ara per opcio amb transport de crews
 

	bool transbords = true;
	while (transbords){
	transbords=false;//per poder sortir abans si eso del bucle
	for(int i = 0; i<size; i++){
            for (int j=0; j<size; j++){
               	 if (transbord[i][j]==true){
                    for(int k=0; k<size; k++){
                        if ((transbord[j][k])&&(!transbord[i][k])){
                            transbords=true;
                            edge nova;
                            nova.final=k+1;
                            nova.min=0;
                            nova.max=1;
                            nova.usat=0;
                            Graf[i+size+4].push_back(nova);
                            transbord[i][k]=true;
                        }
                    }
                }
            }
        }        
	}//pitjor cas de n^4 :(
    }              

    edge nova;
    nova.final = supersink;
    nova.capacitat = valor;
    nova.flow = 0;
    Graf[sink].push_back(nova);//conectem sink a supersink

    nova.final = source;
    nova.capacitat = valor;
    nova.flow = 0;
    Graf[supersource].push_back(nova);//conectem sink a supersink

}

void volsPilot(vector < vector < edge> >& g, vector<bool>& vis, int u, int &size) {
    vector <int> aux;
    int x = u;
	while(x != sink) {
		if (not vis[(x-4)%size]) {
            vis[(x-4)%size] = true;
			aux.push_back((x-4)%size);
		}
		for (int j = 0; j < g[x+size].size(); j++) {
			if (g[x+size][j].flow) {
				g[x+size][j].flow--;
				x = g[x+size][j].final;
				break;
            }
		}
	}
	for (int i = 0; i < aux.size(); i++) {
		if (i) cout << " ";
		cout << aux[i];
	}
	cout << endl;
}

void printVols(vector < vector < edge> >& g, int& size) {
	vector<bool> visit(size,false);
	for (auto e : g[source]) {

		if (e.flow) volsPilot(g,visit,e.final,size);
	}
}

int main ()
{
	clock_t Timestart = clock();
	ifstream in("inputs/instance_100_2_1.air",ios::in);
	
	vector < vector < int > > Entrada; 
	int num;
	while (in >> num)
    {
        vector < int > Viatge (4);
        Viatge[0] = num;   //Entrada[x][0] es el origen
		in >> num;
        Viatge[1] = num;   //Entrada[x][1] es el desti
		in >> num;
		Viatge[2] = num;   //Entrada[x][2] es el temps de sortida
		in >> num;
        Viatge[3] = num;   //Entrada[x][3] es le temps de arrivada
        Entrada.push_back (Viatge);
		// cout <<"viatge es " << Viatge[0] << " "<< Viatge[1] << " "<< Viatge[2] << " "<< Viatge[3] << endl;
    }   //<O(n);
    in.close();
    int size = Entrada.size();
    vector < vector < edge> > Graf (2*size+4);
    sink = 3;
    source = 2;
    supersource = 0;
    supersink = 1;
	
    
	
	/*for (int i=0; i<Graf.size(); i++) {
	 for (int j=0; j<Graf[i].size(); j++) {
	 cout << Graf[i][j].final <<" ";
	 }
	 cout << endl;
	 }*/
	int Pmin=0;
	int Pmax=size;
    int millorflow = 0;
    vector < vector < edge> > solucio (2*size+4);

	while(Pmin <= Pmax){
        int valor = (Pmin+Pmax)/2;
        vector < vector < edge> > Graf (2*size+4);
        generarGraf(Graf, size, Entrada, valor);
        int flow = maxFlowAlgorithm(Graf);
		if ((valor+size) == flow) { //aixo cal canviarho
			Pmax = valor -1;//no volem tornar a provar aquest
            millorflow = valor;
            solucio = Graf;
			//guardar resposta
		}
		else {
			Pmin = valor+1;
		}        
	}
    cout << "Flow: " << millorflow << endl; //imprimir flow i pilots
	printVols(solucio, size);
	/*for (int i=0; i<Graf.size(); i++) {
		for (int j=0; j<Graf[i].size(); j++) {
			cout << Graf[i][j].final <<" ";
		}
		cout << endl;
	}*/    
    
    // for(int i = 0; i < flow; ++i){
    //     cout << pilots[i][pilots[i].size() - 1];
    //     for(int j = pilots[i].size() - 2; j >= 0; ++j) cout << " " << pilots[i][j];
    //     cout << endl;
    // }
	float f = clock()-Timestart;
	cout <<"La execucio ha tardat" <<clock()-Timestart <<"ticks,"<< f/CLOCKS_PER_SEC << "segons"<<endl;
	
}


}
