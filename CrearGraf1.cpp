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
    bool usat;
};

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
        nova.usat = false;
        Graf[source].push_back(nova);
		//no conectem els origens als destins, sino que els transformem en sinks i sources per complir
		//el lower bound (no queda aresta). Per fer aixo cal un super sink i un super source que rep i dona tot el fluxe
	    //primer els nous sink(origens) al supersink (falta el sink original) 
		
        nova.final = supersink;
        nova.capacitat = 1;
        nova.flow = 0;
        nova.usat = false;
        Graf[i+1].push_back(nova);
		
	    //ara el supersource als destins falta conectar al source
		
		nova.final = i+size+1;
        nova.capacitat = 1;
        nova.flow = 0;
        nova.usat = false;
        Graf[supersource].push_back(nova);
		
		//conectem els destins al sink
        nova.final = sink;
        nova.capacitat=1;
        nova.flow = 0;
        nova.usat = false;
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
                nova.usat = false;
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
	int millorflow;
	while(Pmax > Pmin){
		primer = false;

		int valor = (Pmin+Pmax)/2;
		
		edge nova;
        nova.final = supersink;
        nova.capacitat = valor;
        nova.flow = 0;
        nova.usat = false;
		if (primer){
			Graf[sink].push_back(nova);//conectem sink a supersink
		}
		else {
			Graf[sink][Graf[sink].size()-1] = nova;
		}
        nova.final = source;
        nova.capacitat = valor;
        nova.flow = 0;
        nova.usat = false;
		if (primer){
			Graf[supersource].push_back(nova);//conectem sink a supersink
		}
		else {
			Graf[supersource][Graf[supersource].size()-1] = nova;
		}
				
		if (flow(Graf, valor)) { //aixo cal canviarho
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
	
    vector < vector < int > > pilots (size, vector <int> (0));
    
    int flow = 0;
    bool stop = false;
    while(not stop){
        vector < pair<int,int> >  pred (Graf.size(), make_pair(-1,-1));   //Representem els edges obligats
        for(int i = 0; i < size; ++i) pred[1+size+i] = make_pair(1+i, 0);
        
        queue <int> q;
        q.push(0);
        while(not q.empty()){
            int curr = q.front();
			
			
            for(int i = 0; i < Graf[curr].size(); ++i){
                edge act = Graf[curr][i];
                if(not act.usat and pred[act.final].first == -1 and act.final != 0 and act.max > act.min){
                    pred[act.final].first = curr;
                    pred[act.final].second = i;
                    if(act.final != sink) q.push(Graf[act.final][0].final); //l'enllacem directament amb el seu desti
                    else q.push(act.final);
                }
            }
            q.pop();
        }
		
		
        if(not (pred[sink].first == -1)){
            pair <int,int> aux = pred[sink];
            while(aux.first != -1){
                pilots[flow].push_back(aux.first);
                Graf[aux.first][aux.second].min = 1;
                Graf[aux.first][aux.second].usat = true;
                aux = pred[aux.first];
            } 
        }
		
        if(pred[sink].first == -1 or flow == size - 1) stop = true;
        ++flow;
    }
    
    cout << "Flow: " << flow << endl; //imprimir flow i pilots
    // for(int i = 0; i < flow; ++i){
    //     cout << pilots[i][pilots[i].size() - 1];
    //     for(int j = pilots[i].size() - 2; j >= 0; ++j) cout << " " << pilots[i][j];
    //     cout << endl;
    // }
	
}
