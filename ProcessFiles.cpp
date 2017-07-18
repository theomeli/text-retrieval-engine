/*
 * The MIT License
 *
 * Copyright 2017 Theomeli.
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
 * File:   processFiles.cpp
 * Author: Theomeli
 * 
 * Created on June 5, 2017, 1:43 PM
 */

#include "ProcessFiles.h"
#include <iostream>


using namespace std;

ProcessFiles::ProcessFiles(): documentsText("documentsText.txt"), queriesText("queriesText.txt") {
    if (documentsText.fail()) {
        std::cout << "documents file opening failed.";
            exit(1);
    }
    if (queriesText.fail()) {
        std::cout << "queries file opening failed.";
            exit(1);
    }
}


ProcessFiles::ProcessFiles(const ProcessFiles& orig)
    :nDocuments(orig.getNDocuments()), nQueries(orig.getNQueries()), nResponses(orig.getNResponses()) {
    documentsTokens = new list<string>[nDocuments + 1];
    list<string>::iterator iter;
    for (size_t i = 1; i <= nDocuments; i++) {
        for (iter = orig.getDocumentsTokens()[i].begin(); iter != orig.getDocumentsTokens()[i].end(); iter++)
            documentsTokens[i].push_back(*iter);
    }
    
    queriesTokens = new list<string>[nQueries + 1];
    for (size_t i = 1; i <= nQueries; i++) {
        for (iter = orig.getQueriesTokens()[i].begin(); iter != orig.getQueriesTokens()[i].end(); iter++)
            queriesTokens[i].push_back(*iter);
    }
}


ProcessFiles& ProcessFiles::operator =(const ProcessFiles& rightSide) {
   	documentsTokens = new list<string>[rightSide.getNDocuments() + 1];

    nDocuments = rightSide.getNDocuments();
    list<string>::iterator iter;
    for (size_t i = 0; i <= nDocuments; i++) {
        for (iter = rightSide.getDocumentsTokens()[i].begin(); iter != rightSide.getDocumentsTokens()[i].end(); iter++) {
			documentsTokens[i].push_back(*iter);
        }
    }      
    queriesTokens = new list<string>[rightSide.getNQueries() + 1];

    nQueries = rightSide.getNQueries();
    for (size_t i = 0; i <= nQueries; i++) {
        for (iter = rightSide.getQueriesTokens()[i].begin(); iter != rightSide.getQueriesTokens()[i].end(); iter++)
            queriesTokens[i].push_back(*iter);
    }
    
    nResponses = rightSide.getNResponses();
    
    return *this;
}


ProcessFiles::~ProcessFiles() {
}


string ProcessFiles::makeLower(const string& s) {
    size_t sLength = s.length();
    string temp(s);
    for (size_t i = 0; i < sLength; i++) {
        temp[i] = tolower(s[i]);
    }
    
    return temp;
}


string ProcessFiles::removePunct(const string& s, const string& punct) {
    string noPunct; //initialized to empty string
    size_t sLength = s.length();
    size_t punctLength = punct.length( );
    for (size_t i = 0; i < sLength; i++) {
        string aChar = s.substr(i,1); //A one-character string
	size_t location = punct.find(aChar, 0);
        //Find location of successive characters of src in punct.
        if (location < 0 || location >= punctLength)
            noPunct += aChar;//aChar is not in punct, so keep it
    }

    return noPunct;
 }


string ProcessFiles::lowerRemovedPunct(const string& s) {
    string punct(",;:.?!'\" \n"); //includes a blank
    string str(s);
    str = makeLower(str);
    
    return removePunct(str, punct);
}


void ProcessFiles::readDocumentsFile(ifstream& stream) {
    size_t documentId;
    string token;

    stream >> nDocuments;
    //we leave documentsTokens[0] blank
    documentsTokens = new list<string>[nDocuments + 1];
    while (!stream.eof()) {
        stream >> token;
        if (isdigit(token[0])) {
            documentId = atoi(token.c_str());
        }
        else {
            token = lowerRemovedPunct(token);
            documentsTokens[documentId].push_back(token);
        }
    }
}


void ProcessFiles::readQueriesFile(ifstream& stream) {
    size_t queryId;
    size_t nResultsOfQuery;
    string token;
    //if integersRead is equal to zero we are waiting another int to be read
    //if it is one, which it means that we have already read one int, we do the mapping
    size_t integersRead = 0;

    stream >> nQueries;
    //we leave queriesTokens[0] blank
    queriesTokens = new list<string>[nQueries + 1];
    while (!stream.eof()) {
        stream >> token;
        if (isdigit(token[0])) {
            if (integersRead == 0) {
                queryId = atoi(token.c_str());
		integersRead++;
            }
            else if (integersRead == 1) {
                nResultsOfQuery = atoi(token.c_str());
		nResponses.insert(pair<size_t, size_t>(queryId, nResultsOfQuery));
                integersRead = 0;
            }
        }
        else {
            token = lowerRemovedPunct(token);
            queriesTokens[queryId].push_back(token);
        }
    }
}
