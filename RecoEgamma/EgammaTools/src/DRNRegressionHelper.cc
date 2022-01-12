#include "RecoEgamma/EgammaTools/interface/DRNRegressionHelper.h"

/*
 * TODO
 * Eventually this will be decoupled from the fact that these are photons
 * Maybe I can extract a SuperClusterCollection from the PhotonCollection? TBD
 * Or maybe I overlead this function and internally call something else?
 */

/*
void DRNRegressionHelper::makeInput(const reco::PhotonCollection& photons, Input& input){
    auto& xECAL = input.at("xECAL__0"); //ECAL (x,y,z,E)
    auto& fECAL = input.at("fECAL__1"); //ECAL flags
    auto& gainECAL = input.at("gainECAL__2"); //ECAL gain
    auto& xES = input.at("xES__3"); //ES (x.y.z.E)
    auto& fES = input.at("fES__4"); //ES flags
    auto& gx = input.at("gx__5"); //extra high-level features (rho, H/E)
    auto& batch_ECAL = input.at("batch_ECAL__6"); //need to manully pass batching indices
    auto& batch_ES = input.at("batch_ES__7");

    //determine input shapes
    unsigned nSCs = 0, nHitsECAL = 0, nHitsES = 0;
    for (const auto& photon : photons){
        ++nSCs;
        nHitsECAL += <something>; //TODO
        nHitsES += <something>; //TODO
    }

    //set shapes
    xECAL.setShape(0, nHitsECAL);
    fECAL.setShape(0, nHitsECAL);
    gainECAL.setShape(0, nHitsECAL);
    xES.setShape(0, nHitsES);
    fES.setShape(0, nHitsES);
    gx.setShape(0, 2*nSCS); 
    batch_ECAL.setShape(0, nHitsECAL);
    batch_ES.setShape(0, nHitsES);

    //allocate tensors
    auto xECAL_data = xECAL.allocate<float>();
    auto xECAL_vdata = (*xECAL_data)[0];
    auto fECAL_data = fECAL.allocate<int64_t>();
    auto fECAL_vdata = (*fECAL_data)[0];
    auto gainECAL_data = gainECAL.allocate<int64_t>();
    auto gainECAL_vdata = (*gainECAL_data)[0];
    auto xES_data = xES.allocate<float>();
    auto xES_vdata = (*xES_data)[0];
    auto fES_data = fES.allocate<int64_t>();
    auto fES_vdata = (*fES_data)[0];
    auto gx_data = gx.allocate<float>();
    auto gx_vdata = (*gx_data)[0];
    auto batch_ECAL_data = batch_ECAL.allocate<float>();
    auto batch_ECAL_vdata = (*batch_ECAL_data)[0];
    auto batch_ES_data = batch_ES.allocate<float>();
    auto batch_ES_vdata = (*batch_ES_data)[0];

    //fill tensors
    float x, y, z, En, frac;
    unsigned kGood, kOutOfTime, kPoorCalib;
    unsigned gain;
    float HoE;
    unsigned batchNum = 0;
    for (const auto photon : photons){
        const auto& hitsECAL = <something>; //TODO
        const auto& hitsES = <something>; //TODO

        HoE = <something>; //TODO
    
        //TODO: need normalization
        for (const auto& hitECAL : hitsECAL){ //TODO
            En = <something>; //TODO
            x = <something>; //TODO
            y = <something>; //TODO
            z = <something>; //TODO
            frac = <something>; //TODO
            
            xECAL_vdata.push_back(x);
            xECAL_vdata.push_back(y);
            xECAL_vdata.push_back(z);
            xECAL_vdata.push_back(En*frac);

            kGood = <something>; //TODO
            kOutOfTime = <something>; //TODO
            kPoorCalib = <something>; //TODO

            fECAL_vdata.push_back(kGood + 2*kOutOfTime + 4*kPoorCalib); //should be done with bit shifts

            gain = <something>; //TODO
            gainECAL_vdata.push_back(gain);

            batch_ECAL_vdata.push_back(batchNum);
        }

        for (const auto hitES : hitsES){
            x = <something>; //TODO
            y = <something>; //TODO
            z = <something>; //TODO
            En = <something>; //TODO

            xES_vdata.push_back(x);
            xES_vdata.push_back(y);
            xES_vdata.push_back(z);
            xES_vdata.push_back(En);

            kGood = <something>; //TODO
            fES_vdata.push_back(kGood);

            batch_ES_push_back(batchNum);
        }

        gx_vdata.push_back(*rhoHandle_);
        gx_vdata.push_back(HoE);
        ++batchNum;
    }

    //convert to server format
    xECAL.toServer(xECAL_data);
    fECAL.toServer(fECAL_data);
    gainECAL.toServer(gainECAL_data);
    xES.toServer(xES_data);
    fES.toServer(fES_data);
    gx.toServer(gx_data);
    batch_ECAL.toServer(batch_ECAL_data);
    batch_ES.toServer(batch_ES_data);
}*/
