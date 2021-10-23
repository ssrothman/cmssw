
#include "../interface/TruthMergerTools.h"
#include <algorithm>
#include <iostream>


class floatMergeWrapper: public merger::MergeWrapper<float,floatMergeWrapper>{

public:


    floatMergeWrapper(const float* x):x_(*x){}

    float mergeScore(const floatMergeWrapper* rhs)const{
        //this cannot be anything but floatMergeWrapper
        return 1./(std::abs(x_-rhs->x_)+1e-4);
    }

    float mergeAllInList(const std::vector<floatMergeWrapper* >& tomerge)const{
        float out = 0;
        for(const auto mr: tomerge){
            out += mr->x_;
        }
        return out;
    }


private:
    float x_;
};




int main(){

    // using logic of floatMergeWrapper to merge floats
    // in principle other wrappers can define other logic for floats,
    // therefore both, the logic in the wrapper, as well as the target
    // need to be defined
    merger::ObjectMerger<floatMergeWrapper,float> merger;

    std::vector<float> ps={
            0.9,1.,1.1,0.95,  // 3.95, needs one iteration
            1.9, 2.19, 2., // 6.09 , needs to iterations
            4., //4
            8. //8
    };

    float threshold = 1/0.2;//will merge the groups as in the initialiser list
    for(const auto& p:ps){
        merger.add(&p);
    }
    std::vector<float> merged = merger.mergeSymmetric(threshold);

    std::cout << "size " << merged.size() << std::endl;
    for(const auto& m:merged)
        std::cout << m << std::endl;

    return 1;
}
