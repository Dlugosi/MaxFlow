#include <iostream>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

struct edge{    //es un vol
    int final;  //al que esta conectat
    int min;
    int max;
    int usat; //per l'algorisme
};

int main ()
{
	vector < vector < int > > Entrada; 
	int num;
	while (cin >> num)
    {
		vector < int > Viatge (4);
		Viatge[0] = num;   //Entrada[x][0] es el origen
		cin >> num;
		Viatge[1] = num;   //Entrada[x][1] es el desti
		cin >> num;
		Viatge[2] = num;   //Entrada[x][2] es el temps de sortida
		cin >> num;
		Viatge[3] = num;   //Entrada[x][3] es le temps de arrivada
		Entrada.push_back (Viatge);
    }                      //<O(n);
    int size = Entrada.size();
    vector < vector < edge> > Graf (2*size+2);
    int sink = (2*size)+1;
	
    for(int i=0; i<size; i++){  
		//conectem el font als origens
        edge nova;
        nova.final = i+1;
        nova.min = 0;
        nova.max = 1;
        nova.usat = 0;
        Graf[0].push_back(nova);
		//conectem els origens als destins
        nova.final = i+size+1;
        nova.min = 1 ;
        nova.max = 1;
        nova.usat = 0;
        Graf[i+1].push_back(nova);
		//conectem els destins al sink
        nova.final = sink;
        nova.min=0;
        nova.max=1;
        nova.usat =0;
        Graf[i+size+1].push_back(nova);
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
            if (((Entrada[i][3]+15)<Entrada[j][2])&&(Entrada[i][1]==Entrada[j][0])){
                edge nova;
                nova.final=j+1;
                nova.min=0;
                nova.max=1;
                nova.usat =0;
                Graf[i+size+1].push_back(nova);
                transbord[i][j]=true;
            }
        }
    }               //O(n^2)
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
                            Graf[i+size+1].push_back(nova);
                            transbord[i][k]=true;
                        }
                    }
                }
            }
        }        
	}//pitjor cas de n^4 :(

	
	for (int i=0; i<Graf.size(); i++) {
		for (int j=0; j<Graf[i].size(); j++) {
			cout << Graf[i][j].final <<" ";
		}
		cout << endl;
	}
			
	
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
