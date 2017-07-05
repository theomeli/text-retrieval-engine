/*
 * The MIT License
 *
 * Copyright 2017 Totoros.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* 
 * File:   TextRetrievalEngine.cpp
 * Author: Totoros
 * 
 * Created on June 12, 2017, 10:02 PM
 */

#include "TextRetrievalEngine.h"

TextRetrievalEngine::TextRetrievalEngine() {
    p = new ProcessFiles;
}

TextRetrievalEngine::TextRetrievalEngine(ProcessFiles* p) 
	:maxFrequency(p->getNDocuments() + 1, 1) {
	this -> p = new ProcessFiles;
    *(this->p) = *p;
    
    vector<map<string, int>> temp1(p->getNQueries() + 1, map<string, int>());	
	frequencies[true] = temp1;
	vector<map<string, int>> temp2(p->getNDocuments() + 1, map<string, int>());
	frequencies[false] = temp2;

	vector<vector<double>> temp3(p->getNQueries() + 1, vector<double>());
	weights[true] = temp3;
	vector<vector<double>> temp4(p->getNDocuments() + 1, vector<double>());
	weights[false] = temp4;
}

TextRetrievalEngine::TextRetrievalEngine(const TextRetrievalEngine& orig)
	:frequencies(orig.getFrequencies()), maxFrequency(orig.getMaxFrequency()) {
    *(this->p) = *(orig.getP());
}

TextRetrievalEngine::~TextRetrievalEngine() {
    delete p;
}

int TextRetrievalEngine::computeFreqOfTerm(const string& term, int documentId, bool isTermOfQuery) {
    list<string>::iterator iter;
    int frequency = 0;

    if (!isTermOfQuery) {
        for (iter = p->getDocumentsTokens()[documentId].begin(); iter != p->getDocumentsTokens()[documentId].end(); iter++)
            if (*iter == term)
                frequency++;
	}
    else {
        for (iter = p->getQueriesTokens()[documentId].begin(); iter != p->getQueriesTokens()[documentId].end(); iter++)
            if (*iter == term)
                frequency++;
    }
    
    return frequency;
}

void TextRetrievalEngine::computeFrequencies() {
    int nDocuments = p->getNDocuments();
    list<string>::iterator iter;
    for (int i = 0; i <= nDocuments; i++) 
        for (iter = p->getDocumentsTokens()[i].begin(); iter != p->getDocumentsTokens()[i].end(); iter++) {
            frequencies[false][i][*iter] = 0;
			terms.insert(*iter);
        }
    
    int nQueries = p->getNQueries();
    for (int i = 0; i <= nQueries; i++) {
        for (iter = p->getQueriesTokens()[i].begin(); iter != p->getQueriesTokens()[i].end(); iter++) {
            terms.insert(*iter);
			frequencies[true][i][*iter] = 0;
		}
	}
    
    for (int i = 0; i <= nDocuments; i++)
        for (iter = p->getDocumentsTokens()[i].begin(); iter != p->getDocumentsTokens()[i].end(); iter++)
            frequencies[false][i][*iter]++;
    
	for (int i = 0; i <= nQueries; i++)
        for (iter = p->getQueriesTokens()[i].begin(); iter != p->getQueriesTokens()[i].end(); iter++)
			frequencies[true][i][*iter]++;
}

void TextRetrievalEngine::computeMaxFreq(int docId, bool isQuery) {
	map<string, int> myMap =  frequencies[isQuery][docId];
    
    for (auto const &ent1 : myMap) {
        maxFrequency[docId] = max(ent1.second, maxFrequency[docId]);
    }
}

size_t TextRetrievalEngine::getNDocsWithTerm(const string term) {
    int nDocsWithTerm = 0;
    int nDocuments = p->getNDocuments();
    for (int i = 0; i <= nDocuments; i++) {
		if (frequencies[false][i].find(term) != frequencies[false][i].end()) {
            nDocsWithTerm++;
        }
    }
    
    return nDocsWithTerm;
}

void TextRetrievalEngine::initializeIdfs() {
	vector<double> temp(terms.size() + 1, 0);
	idfs[true] = temp;
	idfs[false] = temp;
}

void TextRetrievalEngine::computeIdfs(bool isQuery) {
	size_t nDocuments = p->getNDocuments();
	int i = 1;
	for (auto const &term : terms) {
		double nt = getNDocsWithTerm(term);
		if (nt == 0)
			nt = 1;
		double temp = log(double(nDocuments / nt));
		if (isQuery == true)
			idfs[isQuery][i] = temp;
		else {
			idfs[isQuery][i] = temp / log(nDocuments); 
			temp = temp / log(nDocuments);
		}
		i++;
	}
}

double TextRetrievalEngine::computeNormalizedFreq(const string term, const int documentId, bool isTermOfQuery) {
	double result = computeFreqOfTerm(term, documentId, isTermOfQuery) / maxFrequency[documentId];
    if (!isTermOfQuery)
        return result;
    else
        return result *= 0.5;
}

void TextRetrievalEngine::computeDocWeight(const int documentId, bool isQuery) {
	int i = 1;
	for (auto const &term : terms) {
		weights[isQuery][documentId].push_back(computeNormalizedFreq(term, documentId, isQuery) * idfs[isQuery][i]);
		i++;
	}
}

void TextRetrievalEngine::computeDocsWeight(bool isQuery) {
    computeFrequencies();
	initializeIdfs();
	computeIdfs(isQuery);

	for (int i = 1; i < weights[isQuery].size(); i++) {
		computeMaxFreq(i, isQuery);
		computeDocWeight(i, isQuery);
	}
}