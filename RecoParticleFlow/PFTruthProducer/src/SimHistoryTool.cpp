

#include "../interface/SimHistoryTool.h"

namespace dummy{
std::vector<SimTrack> dummyst;
std::vector<SimVertex> dummysv;
}


const SimTrack* SimHistoryTool::traverseStep(const SimTrack * st)const{

    const SimTrack* out=st;
    int vidx = out->vertIndex();
    if(vidx<0)
        return out; //no vertex
    const SimVertex& vertex = simvertices_.at(vidx);
    int stid = vertex.parentIndex();//this is Geant track ID, not vector index
    if(stid < 0)
        return out;//no parent
    int stidx = trackIdToTrackIdxAsso_.at(stid); //get vector index
    if(stidx < 0)
        return out;//id does not exist
    const SimTrack & simtrack = simtracks_.at(stidx);
    vidx = simtrack.vertIndex();
    out=&simtrack;
    return out;
}

std::vector<const SimTrack*> SimHistoryTool::createHistory(const SimTrack * st)const{
    std::vector<const SimTrack*> outv;
    const SimTrack* out=st;
    outv.push_back(st);//also self-reference
    while(true){
        const SimTrack * test = traverseStep(out);
        if(test == out || test==0)//root or invalid
            break;
        out=test;
        outv.push_back(test);
    }
    return outv;
}

const SimTrack* SimHistoryTool::getRoot(const SimTrack * st) const{
    const SimTrack* out=st;
    while(true){
        const SimTrack * test = traverseStep(out);
        if(test == out || test==0)//root or invalid
            break;
        out=test;
    }
    return out;
}

void SimHistoryTool::createIdxAsso(){
    trackIdToTrackIdxAsso_.clear();
    for(size_t i=0;i<simtracks_.size();i++){
        trackIdToTrackIdxAsso_[simtracks_.at(i).trackId()] = i;
    }
}

