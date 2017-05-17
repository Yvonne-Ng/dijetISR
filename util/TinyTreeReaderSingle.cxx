#include "TH1F.h"
#include "TTree.h"
#include "TCanvas.h"
#include <iostream>
#include "TProfile.h"
#include "TH2F.h"
#include "TLegend.h"
#include <utility>
#include <string>
#include <vector>
#include <math.h>
#include <array>
#include "TFile.h"
#include "../OutputTree/OutputTree/OutputTree.h"
#include <LHAPDF/LHAPDF.h>
#include "LHAPDF/Reweighting.h"

using namespace std;
using std::string;
using std::vector;

//Going to put this in /scripts

int main (int argc, char *argv[]){
    if (argc!=2){
        cout<<"usage: "<<argv[0] << "<location/filename>\n";
        return 1;
    }

    //Open input TFile
    TFile *inputFile;
    string inputFileName="../outputTTree/";
    inputFileName+=argv[1];

    inputFile = TFile::Open(inputFileName.c_str());
 
//Declaring Input Tree Array
    TTree* Tree;
   
//Making the output file name string
    string delimiter="/";
    string arg1(argv[1]);
    string outputFileName= arg1.substr(0,arg1.find(delimiter));
    outputFileName+="_Nanotree.root";

//Open Output TFile
  TFile *output_file = new TFile(outputFileName.c_str(),"RECREATE");

//output TTree
  OutputTree *output_tree= new OutputTree("output");

  
//Opening the input TTree
    Tree= (TTree*)inputFile->Get("TinyTree");

//Declaring input tree varaibles
    float mass, tau21P, x1,x2, q, weight;
    int pdfId1,pdfId2;

//Associating input variables with branch address in inTree
    Tree->SetBranchAddress("mj", &mass);
    Tree->SetBranchAddress("tau21JDDT", &tau21P);
    Tree->SetBranchAddress("pdfId1", &pdfId1);
    Tree->SetBranchAddress("pdfId2", &pdfId2);
    Tree->SetBranchAddress("x1", &x1);
    Tree->SetBranchAddress("x2", &x2);
    Tree->SetBranchAddress("q", &q);
    Tree->SetBranchAddress("weight", &weight);

//Adding the LHA PDF stuff
//the Nominal pdf Set
    LHAPDF::PDF* p0 = LHAPDF::mkPDF("NNPDF30_lo_as_0130/0");

//initializing the pdfErrWeight vector  
    vector<float> pdfErrWeight;

//initializing the internal pdf error sets.
    vector<LHAPDF::PDF*> pErr;

    for (int i=0; i<101; i++){
        string internalErrPdf = "NNPDF30_lo_as_0130/";
        string errSetNum = to_string(i);
        internalErrPdf.append(errSetNum);
        pErr.push_back(LHAPDF::mkPDF(internalErrPdf));
                                    }
//Initializing the external pdf error set.
        LHAPDF::PDF* p1 = LHAPDF::mkPDF("CT14lo/0");
        LHAPDF::PDF* p2 = LHAPDF::mkPDF("MMHT2014lo68cl/0");

//Finding nevt 
    int nevt= Tree->GetEntries();
//Event Loop 
    for (int ievt=0; ievt<nevt; ievt++){
        //pdfErrWeight.clear();//clearing the vector
        output_tree->clear();//clearing the tree
//pdf weight calculation
        Tree->GetEntry(ievt);
        double pdfwVsCt4 = LHAPDF::weightxxQ2(pdfId1,pdfId2, x1, x2, q, p0, p1);
        double pdfwVsMMHT = LHAPDF::weightxxQ2(pdfId1, pdfId2, x1, x2, q, p0, p2);

        //Calculating the Internal pdf & writing a branch for each weight to the output tree
        for (int i =0; i<101; i++ ){
	   //Clearing the pdfErrWeight Error
	   pdfErrWeight.clear();
           pdfErrWeight.push_back(LHAPDF::weightxxQ2(pdfId1, pdfId2, x1, x2, q, p0, pErr.at(i)));
        string strpdfErrWeight="pdfErr_Weight_";
        strpdfErrWeight+=to_string(i);
        output_tree->add_vector(strpdfErrWeight, pdfErrWeight);
        }

        //Calculating the 2 external pdfs
        string strpdfErrCt4Weight="pdfweightVS_Ct4";
        string strpdfErrMMHTWeight="pdfweightVS_MMHT";

        //Adding the two external pdfs to the output tree
        output_tree->add_scalar(strpdfErrCt4Weight, pdfwVsCt4);
        output_tree->add_scalar(strpdfErrMMHTWeight, pdfwVsMMHT);
        //Adding physical variables to the nanoTree
        output_tree->add_scalar("tau21JDDT", tau21P);
        output_tree->add_scalar("massJ", mass);
        output_tree->add_scalar("weight", weight);
        output_tree->Fill();  
    
    }

  output_file->cd();
  output_tree->Write();
  output_file->Close();

    return 0;
  
  } 
