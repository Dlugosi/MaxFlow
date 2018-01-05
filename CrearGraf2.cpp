#include <iostream>
#include <vector>

using namespace std;

struct edge{    //es un vol
    int final;  //al que esta conectat
    int min;
    int max;
    int usat; //per l'algorisme
};

int main ()
{
  vector < vector < int >> Entrada; 
  vector < int > Viatge;
 int num;
  while (cin >> num)
    {
      Viatge.push_back (num);   //Entrada[x][0] es el origen
      cin >> num;
      Viatge.push_back (num);   //Entrada[x][1] es el desti
      cin >> num;
      Viatge.push_back (num);   //Entrada[x][2] es el temps de sortida
      cin >> num;
      Viatge.push_back (num);   //Entrada[x][3] es le temps de arrivada
      Entrada.push_back (Viatge);
    }                           //<O(n);
    int size = Entrada.size();
    vector < vector < edge>> Graf (2*size+2);
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
                            Graf[i+size+1].push_back(nova);
                            transbord[i][k]=true;
                        }
                    }
                }
            }
        }        
   }//pitjor cas de n^4 :(
}


