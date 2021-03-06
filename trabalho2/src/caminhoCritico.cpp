#include "../headers/grafo.h"
#include "../headers/vertice.h"
#include "../headers/ordenacaoTopologica.h"
#include <deque>
#include <algorithm> // for using reverse in a vector

using namespace std;

vector<int> caminhoCriticoAux(int n, vector<int>& distancia, Grafo& grafoPre, deque<Vertice>& ordenacao, vector<int>& camCriticoAux){
	camCriticoAux.push_back(n);
	vector<Vertice*> requisitos = ordenacao[n-1].adjacentes;
	if(requisitos.size() < 1){
		return camCriticoAux;	
	}
	reverse(requisitos.begin(), requisitos.end());
	for(Vertice* v: requisitos){
		if(distancia[n] == distancia[v->getId()] + ordenacao[n-1].peso()){
			caminhoCriticoAux(v->getId(), distancia, grafoPre, ordenacao, camCriticoAux);
		}
	}
	return camCriticoAux;
}

void caminhoCritico(Grafo& grafoFluxo) {

	Grafo grafoPre = grafoFluxo.inverso();

	deque<Vertice> ordenacao = ordenacaoTopologica(grafoFluxo);
	deque<Vertice> ordenacaoInversa = ordenacaoTopologica(grafoPre);
	vector<int> distancia((int)ordenacao.size(), 0); //vector de distancias com nVertices posições todas iniciadas com 0
	reverse(ordenacaoInversa.begin(), ordenacaoInversa.end());	
	for(Vertice& v: ordenacaoInversa){ // achar a distancia maxima de cada vertice
		if(v.grau() <= 0){
			distancia[v.getId()] = v.peso();	
		}
		else{
			vector<Vertice*> requisitos = v.adjacentes;
			for(int i = 0; i < (int)requisitos.size(); i++){
				distancia[v.getId()] = max(distancia[v.getId()], distancia[requisitos[i]->getId()] + v.peso());
			}
		}
	}
	int maximo=0, aux;
	for(int i=1; i< (int)ordenacaoInversa.size(); i++){	
		aux = max(distancia[i], distancia[i-1]);
		maximo = max(aux, maximo);
	}
	for(int i=0; i< (int)ordenacaoInversa.size(); i++){	
		if(distancia[i]==maximo)	
			aux = i;
	}

	vector<int> camCriticoAux;
	vector<int> camCritico = caminhoCriticoAux(aux, distancia, grafoPre, ordenacaoInversa, camCriticoAux);
	
	reverse(camCritico.begin(), camCritico.end());
	printf("\nCAMINHO CRÍTICO:\n");
	for(int i=0; i < (int)camCritico.size(); i++){
		cout << i+1 << "º: " << ordenacaoInversa[camCritico[i]-1].getNome() << endl;
	}
	cout << "Com peso total de " << distancia[aux] << endl;
}

