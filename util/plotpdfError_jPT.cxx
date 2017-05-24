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
#include "TLine.h"

using namespace std;
using std::string;
using std::vector;

#define totalHistNum 105

//For variable displayed = tau21JDDT
#if 1
#define totalBins 140
#define binMax 1400
#define binMin 0.
#endif


void usage(){
    cout<<"Usage:" <<"plotPdfErr" <<"<variable name> <filename> <weight.txt>" <<endl;

}

void variableUsage(){
    
        cout<<"Variable can only be \"tau21JDDT\" or \"massJ\ or \"ptj\" "<<endl;
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


int main(int argc, char* argv[]){
    //gROOT->Reset();
    if (argc!=4){
        cout<<"Usage:" <<argv[0] <<"<variable name> <filename>" <<endl;
        return 1;
    }

    //Finding the branch name corresponding to input
    string varInput= argv[1];
    if (varInput!="tau21JDDT" && varInput!="massJ" && varInput != "ptj" && varInput != "ptJUG"){
        cout<<"Usage:"<<argv[0] <<"<variable name> <filename>"<<endl;
        cout<<"Variable can only be \"tau21JDDT\" or \"massJ\"or \"ptj\" or \"ptJUG\""<<endl;
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

//Input Weight File
    string weightFileName=argv[3]; 
    
//Reading the weight 
    vector<double> pdfWeights=reading_weights(weightFileName);

    TH1F * hist[totalHistNum];
    TH1F * preCutHist[totalHistNum];
    TH1F * histTau21[totalHistNum];
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
	else if (j==105){
        cerr<<"histogram naming out of bound"<<endl;
	}

           cout<<"hist Tau21 Name: "<<histTau21Name<<endl;
            hist[j]= new TH1F(histName.c_str(), histName.c_str(), totalBins,binMin,binMax); //Range for tau21
    	    preCutHist[j]=new TH1F(preCutHistName.c_str(), preCutHistName.c_str(),  totalBins,binMin,binMax);
            histTau21[j]= new TH1F(histTau21Name.c_str(), histTau21Name.c_str(),totalBins,binMin,binMax);
            hist[j]->Sumw2();
            preCutHist[j]->Sumw2();
            histTau21[j]->Sumw2();
     }

     hist[103]= new TH1F("Error Max", "Error Max", totalBins, binMin, binMax);
     hist[104]= new TH1F("Error Min", "Error Min", totalBins, binMin, binMax);
     hist[103]->Sumw2();
     hist[104]->Sumw2();

    //Opening the input TTree
    Tree= (TTree*)inputFile->Get("output");

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
//preCutHistogram: pre tau21 cut
//tau21Histogram after tau21 cut (reweighted by the histogram cut)
//regular histogram: reweighting
	preCutWeight="weight*";
        weighting="(tau21JDDT<0.47)*weight*";
        histTau21Weight="(tau21JDDT<0.47)*weight*";
        if (i<101){
            weighting+="pdfErr_Weight_";
            weighting+=to_string(i);
            //weighting+="*";
            //weighting+=to_string(pdfWeights.at(0));
            //weighting+="/";
            //weighting+=to_string(pdfWeights.at(i));
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
            /*
            weighting+="*";
            weighting+=to_string(pdfWeights.at(0));
            weighting+="/";
            weighting+=to_string(pdfWeights.at(i));

            histTau21Weight+="pdfweightVS_Ct4";*/
       }
        else if (i==102){
            weighting+="pdfweightVS_MMHT";
        /*
            weighting+="*";
            weighting+=to_string(pdfWeights.at(0));
            weighting+="/";
            weighting+=to_string(pdfWeights.at(i));
*/
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
    }
//Adding the histogram bin loop to find the min and  
    //for (int i=0; i
cout<<"test pt1"<<endl;
//Drawing the histogram 
    // Setting e canvas name 
    string histTitle=varInput;
    histTitle+=" with pdfweighting";

    string setHistTitle=histTitle; //param for setHistTitle
    setHistTitle+=";";
    setHistTitle+=varInput;
    setHistTitle+=";";
    setHistTitle+="Event #";
    hist[0]->SetTitle(setHistTitle.c_str());

        /*else {
            cout<<"histSetColor out of range. aborted"<<endl;
            return 3;
        }   */
cout<<"test point 2"<<endl;    
    //Declaring and initializing the canvas
    
    //Drawing the histograms
    //the Histograms are already reweighted above,
    float scl=(binMax-binMin)/totalBins;
    for (int i =0; i< totalBins; i++) //Looping through bin numbers 
    {
        float maxEntriesinBin=0;
        float minEntriesinBin=99999999;
        for (int j=0; j<103; j++) //Looping through the number of histograms
        {
            float testpt=hist[j]->GetBinContent(i);
            if (testpt>maxEntriesinBin)
                maxEntriesinBin=testpt;
            if (testpt<minEntriesinBin)
                minEntriesinBin=testpt;
        }   
        hist[103]->Fill(scl*(i-0.5), maxEntriesinBin); 
        hist[104]->Fill(scl*(i-0.5), minEntriesinBin);
        
    }
cout<<"test point 3 "<<endl;
    //Setting line color
    for (int i=0; i<105; i++){
        
        if (i==0){
            hist[0]->SetLineColor(1);
        }
        else if (i<101){
            hist[i]->SetLineColor(kPink-4);
        }
        else if (i==101){
            hist[i]->SetLineColor(kGreen);
        }
        else if (i==102)
            hist[i]->SetLineColor(kBlue);
        else if (i==103){
            hist[i]->SetLineColor(kRed);
            hist[i]->SetMarkerStyle(1);
                    }
        else if (i==104){
            hist[i]->SetLineColor(kBlue);
            hist[i]->SetMarkerStyle(1);
            }
        }
cout<<"test point 4"<<endl;
    //histTau21 needs to be furthre reweighted from the result of preCuthistogram
    TCanvas *c1= new TCanvas();

    double histScale;
    //Drawing the histogram
    for (int i=0; i<103; i++){
        //cout<<"Integral for Tau21 hist [0]"<<histTau21[0]->Integral()<<endl;
        if (i==0){
            hist[0]->Draw();

	    cout<<histTau21[i]->Integral()/histTau21[0]->Integral()<<endl;
        }
        else{
            histScale=(preCutHist[0]->Integral())/(preCutHist[i]->Integral());
    
	    preCutHist[i]->Scale(histScale);
        histTau21[i]->Scale(histScale);
        cout<<hist[i]->Integral()/hist[0]->Integral()<<endl;
        //hist[i]->Draw("hist same");
        }
        //hist[0]->Draw("same");
    }
cout<<"test point 5"<<endl;
    //Adding the legend 
    TLegend *leg = new TLegend(0.62,0.7,0.98,0.95);
    leg->AddEntry(hist[0],"nominal","l");
    leg->AddEntry(hist[1],"Reweighted by Internal pdf","l");
    leg->AddEntry(hist[101],"Reweighted by External Ct4 pdf set","l");
    leg->AddEntry(hist[102],"Reweighted by External MMHT4 pdf set", "l");

    //Drawing the legend
    leg->Draw("same");

     
cout<<"test point5 "<<endl;
//Canvas that draws the nominal and the rest and print out the entries number
    
    TCanvas * c2 = new TCanvas();
//    Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
       TPad *pad2 = new TPad("pad2", "pad2", 0, 0.00, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);
    pad2->SetGridx(); // vertical grid
    pad2->Draw();
    pad1->cd();               // pad1 becomes the current pad

    hist[0]->Draw();
    hist[103]->Draw("hist same");
    hist[104]->Draw("hist same");

    TLegend *leg2 = new TLegend(0.62,0.7,0.98,0.95);
    leg2->AddEntry(hist[0],"nominal","l");
    leg2->AddEntry(hist[103],"Max of bin in int/ext pdf sets","l");
    leg2->AddEntry(hist[104],"Min of each bin in int/ext pdf sets", "l");

    leg2->Draw();

  
    //Define the Ratio plot
    TH1F * h103Ratio = (TH1F*) hist[103]->Clone("h103Ratio");
    TH1F * h104Ratio = (TH1F*) hist[104]->Clone("h104Ratio");   
    TLine *line =  new TLine(0, 1, 1400, 1);
    h103Ratio->Sumw2();
    h104Ratio->Sumw2();
    h103Ratio->Divide(hist[0]);
    h104Ratio->Divide(hist[0]);
    h103Ratio->Sumw2();
    h104Ratio->Sumw2();
    h103Ratio->SetMinimum(0.2);
    h103Ratio->SetMaximum(1.80);
    h103Ratio->SetStats(0);
  
    h103Ratio->SetLineColor(kRed);
    h104Ratio->SetLineColor(kBlue);
    
    h103Ratio->SetTitle("");
    h103Ratio->GetYaxis()->SetTitle("ratio max or min divided by nominal");
    h103Ratio->GetXaxis()->SetTitle("pt of leading AkT4 Jet");
    h103Ratio->GetYaxis()->SetLabelFont(43);
    h104Ratio->GetXaxis()->SetLabelFont(43);
    pad2->cd();
    h103Ratio->Draw("hist");
    h104Ratio->Draw("hist same");
    line->Draw("same");
    //h103Ratio->GetY
    //cout<<"
    //saving the file as a pdf 
    string pdfName=inputFileName+varInput+".pdf";
    string macroName=inputFileName+varInput+".C";

    string pdfName2=inputFileName+varInput+"maxNmin.pdf";
    string macroName2= inputFileName +varInput+"maxNmin.C";

    string outFileName="MaxMinPdfPacket";
    outFileName+=".txt";
    ofstream outfile;
    outfile.open(outFileName, std::ios_base::app);
    double maxPercent= hist[103]->Integral()/hist[0]->Integral();
    double minPercent = hist[104]->Integral()/hist[0]->Integral();

    outfile<<inputFileName<<"    "<<maxPercent<<"    "<<minPercent<<"    "<<(maxPercent-minPercent);

    outfile.close();
    c1->SaveAs(pdfName.c_str());
    c1->SaveAs(macroName.c_str());

    c2->SaveAs(pdfName2.c_str());
    c2->SaveAs(macroName2.c_str());
    return 0;

    }   
