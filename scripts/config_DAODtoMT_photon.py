from xAH_config import xAH_config

c = xAH_config()

triggersList = [
    "HLT_g120_loose",
    "HLT_g140_loose",
    "HLT_g200_etcut",
    "HLT_g300_etcut",
]
triggers = ",".join(triggersList)

deriv = 'EXOT18Kernel'

c.setalg("BasicEventSelection", { "m_name"                  : "BasicEventSelection",
                                  "m_debug"                 : False,
                                  "m_derivationName"        : deriv,
                                  "m_applyGRLCut"           : False,
                                  "m_doPUreweighting"       : False,
                                  "m_vertexContainerName"   : "PrimaryVertices",
                                  "m_PVNTrack"              : 2,
                                  "m_applyPrimaryVertexCut" : True,
                                  "m_applyEventCleaningCut" : True,
                                  "m_applyCoreFlagsCut"     : True,
                                  "m_triggerSelection"      : triggers,
                                  "m_storeTrigDecisions"    : True,
                                  "m_applyTriggerCut"       : False,
                                  "m_useMetaData"           : True
                                } )

c.setalg("JetCalibrator", { "m_name"                  : "FatJetCalibrator",
                            "m_inContainerName"       : "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets",
                            "m_jetAlgo"               : "AntiKt10LCTopoTrimmedPtFrac5SmallR20",
                            "m_outputAlgo"            : "AntiKt10LCTopoTrimmedPtFrac5SmallR20_Calib_Algo",
                            "m_outContainerName"      : "CalibFatJets",
                            "m_debug"                 : False,
                            "m_verbose"               : False,
                            "m_sort"                  : True,
                            "m_saveAllCleanDecisions" : True,
                            "m_calibConfigFullSim"    : "JES_MC15recommendation_FatJet_June2015.config",
                            "m_calibConfigData"       : "JES_MC15recommendation_FatJet_June2015.config",
                            "m_doCleaning"            : False,
                            #"m_JESUncertConfig"       : "$ROOTCOREBIN/data/JetUncertainties/UJ_2015/ICHEP2016/HbbTagging_strong.config",
                            #"m_JESUncertMCType"       : "MC15",
                            "m_calibSequence"         : "EtaJES_JMS",
                            "m_setAFII"               : False,
                            "m_jetCleanCutLevel"      : "LooseBad",
                            "m_jetCleanUgly"          : True,
                            "m_cleanParent"           : True,
                            "m_applyFatJetPreSel"     : True,
                            "m_systName"              : "Nominal",
                            "m_systVal"               : 0
                          } )

c.setalg("JetSelector", { "m_name"                    : "FatJetSelector",
                          "m_inContainerName"         : "CalibFatJets",
                          "m_inputAlgo"               : "AntiKt10LCTopoTrimmedPtFrac5SmallR20_Calib_Algo",
                          "m_outContainerName"        : "SelFatJets",
                          "m_outputAlgo"              : "SelFatJets_Algo",
                          "m_decorateSelectedObjects" : False,
                          "m_createSelectedContainer" : True,  
                          "m_cleanJets"               : False,
                          "m_pT_min"                  : 200e3,
                          "m_eta_max"                 : 2.0,
                          "m_mass_min"                : 0.1, 
                          "m_useCutFlow"              : True,
                        } )

"""c.setalg("PhotonCalibrator", { "m_name"                    : "PhotonCalibrator",
                               "m_inContainerName"         : "Photons",
                               "m_outContainerName"        : "CalibPhotons",
                               "m_outputAlgoSystNames"     : "Photons_Calib_Algo",
                               "m_esModel"                 : "es2015cPRE",
                               "m_decorrelationModel"      : "1NP_v1",
                               "m_useAFII"                 : False,
                               "m_systName"                : "Nominal",
                               "m_systVal"                 : 0,
                               "m_sort"                    : True,
                               "m_conEffCalibPath"         : "PhotonEfficiencyCorrection/2015_2016/rel20.7/Moriond2017_v1/PIDSF/efficiencySF.offline.Tight.2016.13TeV.rel20.7.25ns.con.v00.root", 
                               "m_uncEffCalibPath"         : "PhotonEfficiencyCorrection/2015_2016/rel20.7/Moriond2017_v1/PIDSF/efficiencySF.offline.Tight.2016.13TeV.rel20.7.25ns.unc.v00.root"


                              } )
"""
c.setalg("PhotonCalibrator", { "m_name" : "PhotonCalibrator",
                                #"m_debug" : debug, 
                                "m_inContainerName" : "Photons",
                                "m_outContainerName" : "Photons_Calib",
                                "m_outputAlgoSystNames" : "Photons_Calib_Algo",
                                "m_conEffCalibPath" : "PhotonEfficiencyCorrection/2015_2016/rel20.7/Moriond2017_v1/PIDSF/efficiencySF.offline.Tight.2016.13TeV.rel20.7.25ns.con.v00.root",
                                "m_uncEffCalibPath" : "PhotonEfficiencyCorrection/2015_2016/rel20.7/Moriond2017_v1/PIDSF/efficiencySF.offline.Tight.2016.13TeV.rel20.7.25ns.unc.v00.root",
                                "m_conEffAFIICalibPath" : "PhotonEfficiencyCorrection/efficiencySF.offline.Tight.2015.13TeV.rel20.AFII.con.v01.root",
                                "m_uncEffAFIICalibPath" : "PhotonEfficiencyCorrection/efficiencySF.offline.Tight.2015.13TeV.rel20.AFII.unc.v01.root",
                                "m_tightIDConfigPath" : "ElectronPhotonSelectorTools/offline/mc15_20150712/PhotonIsEMTightSelectorCutDefs.conf",
                                "m_mediumIDConfigPath" : "ElectronPhotonSelectorTools/offline/mc15_20150712/PhotonIsEMMediumSelectorCutDefs.conf",
                                "m_looseIDConfigPath" : "ElectronPhotonSelectorTools/offline/mc15_20150712/PhotonIsEMLooseSelectorCutDefs.conf",
                                "m_esModel" : "es2016data_mc15c",
                                "m_decorrelationModel" : "1NP_v1",
                                "m_useAFII" : False,
                               "m_systName" : "Nominal",
                                "m_systVal" : 0,
                                "m_sort" : True
} )


c.setalg("PhotonSelector", { "m_name"                    : "PhotonsSelector",
                             "m_inContainerName"         : "CalibPhotons",
                             "m_inputAlgoSystNames"      : "Photons_Calib_Algo",
                             "m_outContainerName"        : "SelPhotons",
                             "m_outputAlgoSystNames"     : "SelPhotons_Algo",
                             "m_decorateSelectedObjects" : False,
                             "m_createSelectedContainer" : True,
                             "m_pass_min"                : 0,
                             "m_pT_min"                  : 10e3,
                             "m_eta_max"                 : 2.37,
                             "m_vetoCrack"               : True,
                             "m_doAuthorCut"             : True,
                             "m_doOQCut"                 : True,
                             "m_photonIdCut"             : "Tight",
                             "m_MinIsoWPCut"             : "FixedCutTightCaloOnly"
                           } )

c.setalg("dijetISR_DAODtoMT", { "m_doJets"               : False,
                                "m_doPhotons"            : True,
                                "m_fatJetContainerName"  : "SelFatJets",
                                "m_photonContainerName"  : "SelPhotons",
                                "m_eventInfoDetailStr"   : "pileup truth",
                                "m_trigDetailStr"        : "passTriggers",
                                "m_fatJetDetailStr"      : "kinematic substructure",
                                "m_photonDetailStr"      : "kinematic"
                              } )
