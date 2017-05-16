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
#include <fstream>

using namespace std;



void usage(){
    cout<<"Usage:" <<"plotPdfErr" <<"<variable name> <filename> <weight.txt>" <<endl;

}

void variableUsage(){
    
        cout<<"Variable can only be \"tau21JDDT\" or \"massJ\""<<endl;
}

//Take in weight file name, output vector of weights
vector<double> reading_weights(string & weightInputName){
    cout<<"---Reading in weight file----"<<endl;
    //Declaring ifstream
    ifstream weightInput;
    //Opening weightFile
    weightInput.open(weightInputName.c_str()); 
    
    vector<double> weights;
    double input;
    while (!weightInput.eof()){
        weightInput >> input;
        weights.push_back(input);
    }
   return weights;
}

struct HistGroup{
	//public members
	TH1F *preCutHist;
	TH1F *hist;
	TH1F *histTau21;

	//constructor
	HistGroup(string histName){ //constructor
		string preCutHistName = histName +"_preCut";
		string histTau21Name=histName+ "_tau21";
		this->hist = new TH1F(histName.c_str(), histName.c_str(), 60,0,0.9);
		this->preCutHist= new TH1F(preCutHistName.c_str(), preCutHistName.c_str(),60,0,0.9);
		this->histTau21 = new TH1F(histTau21Name.c_str(), histTau21Name.c_str(),60,0,0,0.9);

		this->hist->Sumw2();
		this->preCutHist->Sumw2();
		this->histTau21->Sumw2();
		
	}
	void Filling(Histogroup& histGroup ); //Filling 
	~HistoGroup(){
		delete this->hist;
		delete this->preCutHist;
		delete this->histTau21;
	}
}

int main(int argc, char* argv[]){
    //gROOT->Reset();
    if (argc!=4){
        usage();
        return 1;
    }

    //Finding the branch name corresponding to input
    string varInput= argv[1];
    if (varInput!="tau21JDDT" && varInput!="massJ"){
        usage();
        variableUsage();
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

    //Input weight file
    string weightFileName=argv[3]; 
    
   //Declaring the ifstream weight file
    //Reading the weights
    vector<double> pdfWeights=reading_weights(weightFileName);
    //Initializing histograms
     string histName;
     string preCutHistName;
     string histTau21Name;
 
     for (int j=0; j<103 ; j++){
        preCutHistName="preCut";
        histTau21Name="Tau21";
        cout<<"j number: "<<j<<endl;
        if (j==0){
            histName="Nominal";
	    preCutHistName+="Nominal";
            histTau21Name+="Nominal";
        }   
        else if (j>0 && j<101){
            histName="pdfIntErr";
            histName+=to_string(j);
	    preCutHistName+="pdfIntErr";
	    preCutHistName+=to_string(j);
            histTau21Name+="pdfIntErr";
            histTau21Name+=to_string(j);
        }
        else if (j==101){
            histName="pdfExtVS_CT4";
	    preCutHistName+="pdfExtVS_CT4";
            histTau21Name+="pdfExtVS_CT4";
        }
        else if (j==102){
            histName="pdfExtVS_MMHT";
	    preCutHistName+="pdfExtVS_MMHT";
            histTau21Name+="pdfExtVS_MMHT";
        }
	else if (j==103){
	
	}

        }
        /*else {
            cout<<"histogram number out of range"<<endl;
            return 2;
        }*/
        else if (j==103){
            break;
        }
            //cout<<"Histogram name: "<<histName<<endl;
	    //cout<<"precut histogram name:"<<preCutHistName<<endl;
            cout<<"hist Tau21 Name: "<<histTau21Name<<endl;
            hist[j]= new TH1F(histName.c_str(), histName.c_str(), 60,0,0.9); //Range for tau21
	    preCutHist[j]=new TH1F(preCutHistName.c_str(), preCutHistName.c_str(), 60,0,0.9);
            histTau21[j]= new TH1F(histTau21Name.c_str(), histTau21Name.c_str(),60,0,0.9);
            hist[j]->Sumw2();
	    preCutHist[j]->Sumw2();
            histTau21[j]->Sumw2();
     } //end if loop over pdf sets.

    //Opening the input TTree
    Tree= (TTree*)inputFile->Get("output");

    //test: 
    cout << Tree << endl;

    //Getting entries
    int nentries = Tree->GetEntries();
    cout<<"Tree entries #: "<<nentries<<endl;

    //Filling the histogram
    string preCutHistFilling;
    //string preHistogramName;
    string histFilling; //var>>hist (first param of Draw)
    string weighting;   //the weight used (second param of Draw)
    string preCutWeight;
    string histogramName;
    string histTau21Filling;
    string histTau21Weight;
    for (int i=0; i< 103; i++){
        //weighting="";
//creating the hist filling option
	preCutHistFilling=varInput;
	preCutHistFilling+=">>";
        histFilling=varInput;
        histFilling+=">>";
        histTau21Filling=varInput;
        histTau21Filling+=">>";
	string preHistogramName="preCut";
        string histTau21HistogramName="Tau21";
        if (i==0){
            histogramName="Nominal";
	    preHistogramName+="Nominal";
            histTau21HistogramName+="Nominal";
        }   
        else if (i>0 && i<101){
            histogramName="pdfIntErr";
            histogramName+=to_string(i);
	    preHistogramName+="pdfIntErr";
	    preHistogramName+=to_string(i);
            histTau21HistogramName+="pdfIntErr";
            histTau21HistogramName+=to_string(i);

        }
        else if (i==101){
            histogramName="pdfExtVS_CT4";
	    preHistogramName+="pdfExtVS_CT4";
            histTau21HistogramName+="pdfExtVS_CT4";
        }
        else if (i==102){
            histogramName="pdfExtVS_MMHT";
	    preHistogramName+="pdfExtVS_MMHT";
            histTau21HistogramName+="pdfExtVS_MMHT";

        }
        //cout<<"histogram Name: "<<histogramName<<endl;
	//cout<<"pre-histogram name: "<<preHistogramName<<endl;
        cout<<"histtau21 name:"<<histTau21HistogramName<<endl;

        histFilling+=histogramName;
	preCutHistFilling+=preHistogramName;
        histTau21Filling+=histTau21HistogramName;

//creating the weighting option
	preCutWeight="weight*";
        weighting="(tau21JDDT<0.47)*weight*";
        histTau21Weight="(tau21JDDT<0.47)*weight*";
        if (i<101){
            weighting+="pdfErr_Weight_";
            weighting+=to_string(i);
            weighting+="*";
            weighting+=to_string(pdfWeights.at(0));
            weighting+="/";
            weighting+=to_string(pdfWeights.at(i));
	    preCutWeight+="pdfErr_Weight_";
            preCutWeight+=to_string(i);
            /*
            preCutWeight+="*";
            preCutWeight+=to_string(pdfWeights.at(0));
            preCutWeight+="/";
            preCutWeight+=to_string(pdfWeights.at(i));
            */
            histTau21Weight+="pdfErr_Weight_";
            histTau21Weight+=to_string(i);
        }
        else if (i==101){
	    preCutWeight+="pdfweightVS_Ct4";
            /*
            preCutWeight+="*";
            preCutWeight+=to_string(pdfWeights.at(0));
	    
            preCutWeight+="/";
            preCutWeight+=to_string(pdfWeights.at(i));
*/
            weighting+="pdfweightVS_Ct4";
             weighting+="*";
            weighting+=to_string(pdfWeights.at(0));
            weighting+="/";
            weighting+=to_string(pdfWeights.at(i));

            histTau21Weight+="pdfweightVS_Ct4";
       }
        else if (i==102){
            weighting+="pdfweightVS_MMHT";
            weighting+="*";
            weighting+=to_string(pdfWeights.at(0));
            weighting+="/";
            weighting+=to_string(pdfWeights.at(i));

	    preCutWeight+="pdfweightVS_MMHT";
            histTau21Weight+="pdfweightVS_MMHT";
     /*       preCutWeight+="*";
            preCutWeight+=to_string(pdfWeights.at(0));
            preCutWeight+="/";
            preCutWeight+=to_string(pdfWeights.at(i));*/
        }

        //Print out the Hist filling and weighting option 
        cout<<"Hist Filling: "<<histFilling<<endl;
        cout<<"weighting: "<<weighting<<endl;

	cout<<"preCut hist Filling: "<<preCutHistFilling<<endl;
	cout<<"preCut weighting:" <<preCutWeight<<endl;
        
        cout<<"Tau21 hist Filling: "<<histTau21Filling<<endl;
        cout<<"Tau21 hist Weighting:"<<histTau21Weight<<endl;
        //Actually doing the drawing
        Tree->Draw(histFilling.c_str(), weighting.c_str(), "goff");
	Tree->Draw(preCutHistFilling.c_str(), preCutWeight.c_str(), "goff");
        Tree->Draw(histTau21Filling.c_str(), histTau21Weight.c_str(),"goff");
/*
        hist[i]=(TH1F*)(gROOT->FindObject(histogramName.c_str()));
	preCutHist[i]=(TH1F*)(gROOT->FindObject(preHistogramName.c_str()));
        histTau21[i]= (TH1F*)(gROOT->FindObject(histTau21Name.c_str()));*/
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
    //TCanvas *preCut= new TCanvas();
    TCanvas *c1= new TCanvas();
    double histScale; 

     //TH1F *preCutHist[104]; //pre-tau21 cut histogram 

    //Drawing the histogram
    
    /*int nHistos = sizeof(histTau21)/sizeof(histTau21[0]);
    for (int i = 0; i < nHistos; ++i) cout << histTau21[i] << endl;
*/
     

    for (int i=0; i<103; i++){
        //cout<<"Integral for Tau21 hist [0]"<<histTau21[0]->Integral()<<endl;
        if (i==0){
            hist[0]->Draw();

	    cout<<histTau21[i]->Integral()/histTau21[0]->Integral()<<endl;
        }
        else{
            histScale=(preCutHist[0]->Integral())/(preCutHist[i]->Integral());
	    //cout<<"histScale "<<histScale<<endl;
	    
            //cout<<"hist #"<<i<<endl;
           /* 
	    cout<<"precut integral of preCuthist 0: "<<preCutHist[0]->Integral()<<"  precut integral of preCuthist i: "<<preCutHist[i]->Integral()<<endl;
	    cout<<"precut integral of Tau21hist0:"<<histTau21[0]->Integral()<<" precut integral of hist i: "<<histTau21[i]->Integral()<<endl;
            //cout<<"hist scale: "<<i<<"  "<<histScale<<endl;*/
          
	   /*
	    cout<<endl;
	   cout<<"hist "<<i<<endl; 
           cout<<"Before: "<<endl;
	   cout<<"preScaleHist "<<preCutHist[i]->Integral()<<"  "<<preCutHist[0]->Integral()<<endl;
	   cout<<"histTau21 "<<histTau21[i]->Integral()<<"  "<<histTau21[0]->Integral()<<endl; 
            */
	    
	    preCutHist[i]->Scale(histScale);
	    //cout<<"i"<<i<<endl;
            histTau21[i]->Scale(histScale);

	    //hist[i]->Scale(histScale,"");
	    cout<<hist[i]->Integral()/hist[0]->Integral()<<endl;
/*
            //cout<<hist[i]->Integral()/hist[0]->Integral()<<endl;
	    cout<<endl;
	    cout<<"After "<<endl;
	    cout<<"preScaleHist "<<preCutHist[i]->Integral()<<"  "<<preCutHist[0]->Integral()<<endl;
	    cout<<"histTau21 "<<histTau21[i]->Integral()<<"  "<<histTau21[0]->Integral()<<endl; 
            */


            /*
	    cout<<"post scale integral of preCuthist 0:"<< preCutHist[0]->Integral()<<" post scale integral of  tau21Hist i: "<<histTau21[i]->Integral()<<endl;
	    cout<<"post scale & post cut  integral of Tau21hist 0:"<< histTau21[0]->Integral()<<" post scale integral of hist i: "<<histTau21[i]->Integral()<<endl;
	    cout<<"post scale integral of hist 0:"<<hist[0]->Integral()<<"post scale integral of hist i"<< hist[i]->Integral()<<endl;*/
            
            //cout<<endl;
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
