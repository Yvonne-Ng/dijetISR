#include "TH1F.h"
#include "THStack.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include <iostream>
#include "TLatex.h"
#include "TProfile.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TPave.h"
#include "TBox.h"
#include <utility>
#include <string>
#include <vector>
#include <math.h>
#include <array>
#include "TFile.h"
#include "TROOT.h"

using namespace std;
using std::string;
using std::vector;

void usage(){
    cout<<"Usage:" <<"plotPdfErr" <<"<variable name> <filename> <weight.txt>" <<endl;

}

void variableUsage(){
    
        cout<<"Variable can only be \"tau21JDDT\" or \"massJ\""<<endl;
}


int main(int argc, char* argv[]){
    //gROOT->Reset();
    if (argc!=3){
        cout<<"Usage:" <<argv[0] <<"<variable name> <filename>" <<endl;
        return 1;
    }

    //Finding the branch name corresponding to input
    string varInput= argv[1];
    if (varInput!="tau21JDDT" && varInput!="massJ"){
        cout<<"Usage:"<<argv[0] <<"<variable name> <filename>"<<endl;
        cout<<"Variable can only be \"tau21JDDT\" or \"massJ\""<<endl;
        return 1;
    }
    
    //Input File Name and Massname 
    string inputFileName=argv[2];

    //Declare input files
    TFile *inputFile;
        
    //Open input Files
    inputFile = TFile::Open(inputFileName.c_str());
    //Declare input tree
     TTree *Tree;
    //Declare histograms
     TH1F *hist[104];

    //Initializing histograms
     string histName;
     for (int j=0; j<104 ; j++){
        cout<<"j number: "<<j<<endl;
        if (j==0){
            histName="Nominal";
        }   
        else if (j>0 && j<101){
            histName="pdfIntErr";
            histName+=to_string(j);
        }
        else if (j==101){
            histName="pdfExtVS_CT4";
        }
        else if (j==102){
            histName="pdfExtVS_MMHT";
        }
        /*else {
            cout<<"histogram number out of range"<<endl;
            return 2;
        }*/
        else if (j==103){
            break;
        }
            cout<<"Histogram name: "<<histName<<endl;
            hist[j]= new TH1F(histName.c_str(), histName.c_str(), 45,0,0.9); //Range for tau21
            hist[j]->Sumw2();
     }

    //Opening the input TTree
    Tree= (TTree*)inputFile->Get("output");

    //Getting entries
    int nentries = Tree->GetEntries();
    cout<<"Tree entries #: "<<nentries<<endl;

    //Filling the histogram
    string histFilling; //var>>hist (first param of Draw)
    string weighting;   //the weight used (second param of Draw)
    string histogramName;
    for (int i=0; i< 103; i++){
        //weighting="";
//creating the hist filling option
        histFilling=varInput;
        histFilling+=">>";
        if (i==0){
            histogramName="Nominal";
        }   
        else if (i>0 && i<101){
            histogramName="pdfIntErr";
            histogramName+=to_string(i);
        }
        else if (i==101){
            histogramName="pdfExtVS_CT4";
        }
        else if (i==102){
            histogramName="pdfExtVS_MMHT";
        }
        cout<<"histogram Name: "<<histogramName<<endl;
        histFilling+=histogramName;
//creating the weighting option
        weighting="(tau21JDDT<0.47)*weight*";
        if (i<101){
            weighting+="pdfErr_Weight_";
            weighting+=to_string(i);
        }
        else if (i==101){
            weighting+="pdfweightVS_Ct4";
        }
        else if (i==102){
            weighting+="pdfweightVS_MMHT";
        }

        //Print out the Hist filling and weighting option 
        cout<<"Hist Filling: "<<histFilling<<endl;
        cout<<"weighting: "<<weighting<<endl;
        
        //Actually doing the drawing
        Tree->Draw(histFilling.c_str(), weighting.c_str(), "goff");
        hist[i]=(TH1F*)(gROOT->FindObject(histogramName.c_str()));
    }

//Drawing the histogram 
    // Setting the canvas name 
    string histTitle=varInput;
    histTitle+=" with pdfweighting";

    string setHistTitle=histTitle; //param for setHistTitle
    setHistTitle+=";";
    setHistTitle+=varInput;
    setHistTitle+=";";
    setHistTitle+="Event #";
    hist[0]->SetTitle(setHistTitle.c_str());

    //Setting line color
    for (int i=0; i<103; i++){
        
        if (i==0){
            hist[0]->SetLineColor(1);
        }
        else if (i<101){
            hist[i]->SetLineColor(kPink-4);
        }
        else if (i==101){
            hist[i]->SetLineColor(kGreen);
        }
        else if (i==102){
            hist[i]->SetLineColor(kBlue);
        }
        /*else {
            cout<<"histSetColor out of range. aborted"<<endl;
            return 3;
        }   */
    }
    //Declaring and initializing the canvas
    TCanvas *c1= new TCanvas();

    //Drawing the histogram
    for (int i=0; i<103; i++){
        cout<<(hist[i]->Integral())/hist[0]->Integral()<<endl;
        if (i==0){
            hist[0]->Draw();
        }
        else{
            hist[i]->Draw("same");
        }
        hist[0]->Draw("same");
    }

    //Adding the legend 
    TLegend *leg = new TLegend(0.62,0.7,0.98,0.95);
    leg->AddEntry(hist[0],"nominal","l");
    leg->AddEntry(hist[1],"Reweighted by Internal pdf","l");
    leg->AddEntry(hist[101],"Reweighted by External Ct4 pdf set","l");
    leg->AddEntry(hist[102],"Reweighted by External MMHT4 pdf set", "l");

    //Drawing the legend
    leg->Draw("same");

    //saving the file as a pdf 
    string pdfName=inputFileName+".pdf";
    string macroName=inputFileName+".C";
    c1->SaveAs(pdfName.c_str());
    c1->SaveAs(macroName.c_str());
    return 0;

    }   
