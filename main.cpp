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
 * File:   main.cpp
 * Author: Totoros
 *
 * Created on May 27, 2017, 10:54 PM
 */

//#include "stdafx.h"
#include "ProcessFiles.h"
#include "TextRetrievalEngine.h"

using namespace std;


int main(int argc, char** argv) {

    ProcessFiles p;
    ifstream& i1 = p.getDocumentsText();
    //eixa provlima me to parakato giati?????????
    //res = p.readDocumentsFile(p.getDocumentsText(), fileName);
    //mallon ginetai kai me ena ifstream
    p.readDocumentsFile(i1);
    
    ifstream& i2 = p.getQueriesText();
    p.readQueriesFile(i2);
    
    ////////////////it's ok
    /*for (int i = 0; i <= p.getNDocuments(); i++) {
        cout << "document " << i << ":" << endl;
        list<string>::iterator iter;
        for (iter = p.getDocumentsTokens()[i].begin(); iter != p.getDocumentsTokens()[i].end(); iter++)
            cout << *iter << endl;
    }*/
    
	/*It's ok
	cout << endl << endl << endl << endl << endl;
	for (int i = 0; i <= p.getNQueries(); i++) {
        cout << "query " << i << ":" << endl;
        list<string>::iterator iter;
        for (iter = p.getQueriesTokens()[i].begin(); iter != p.getQueriesTokens()[i].end(); iter++)
            cout << *iter << endl;
    }*/

    TextRetrievalEngine t(&p);
    //t.computeFrequencies();
	////////////////it' ok
	/*cout << endl << endl << endl << endl;
	for (int i = 0; i <= p.getNDocuments(); i++) {
        cout << "document " << i << ":" << endl;
        list<string>::iterator iter;
        for (iter = p.getDocumentsTokens()[i].begin(); iter != p.getDocumentsTokens()[i].end(); iter++)
            cout << *iter << endl;
    }*/
    //t.computeDocsWeight(true);
    t.computeDocsWeight(false);

	
	/*
	vector<double>::iterator iter;
	for (int i = 1; i <= 12; i++) {
		cout << endl << "Document " << i << ":" << endl;
		//for (iter = t.getDocumentsWeight()[i].begin(); iter != t.getDocumentsWeight()[i].end(); iter++) {
		for (iter = t.getWeights(false)[i].begin(); iter != t.getWeights(false)[i].end(); iter++) {
			cout << *iter << ", ";
		}
	}

	for (int i = 1; i <= 7 ; i++) {
		cout << endl << "Query " << i << ":" << endl;
		//for (iter = t.getQueriesWeight()[i].begin(); iter != t.getQueriesWeight()[i].end(); iter++) {
		for (iter = t.getWeights(true)[i].begin(); iter != t.getWeights(true)[i].end(); iter++) {
			cout << *iter << ", ";
		}
	}
	*/
}

