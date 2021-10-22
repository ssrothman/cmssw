/*
 * TruthMergerTools.h
 *
 *  Created on: 22 Oct 2021
 *      Author: jkiesele
 */

#ifndef HGCSIMTRUTH_HGCSIMTRUTH_INTERFACE_TRUTHMERGERTOOLS_H_
#define HGCSIMTRUTH_HGCSIMTRUTH_INTERFACE_TRUTHMERGERTOOLS_H_

#include <vector>

namespace truth{

/*
 * abstract helper classes that implement a 1:1 merging functionality for objects
 * caching not needed because for every evaluation it's either merged or not
 *
 * templated class T must implement the 'physics of it' in methods:
 *
 *    //calculates a (symmetric) merge score between two instances
 *    float mergeScore(const T& rhs)const;
 *
 *    //defines rules to create a new instance out of a set of previously merged instances.
 *    //this is called at the end, not during merging
 *    T merge(std::vector<const T*>)const;
 *
 *    //NB: one could add a function that determines how to re-evaluate the merging parameters after a merge
 *    //    but that would require to change the absorb function a bit (and is not needed right now),
 *    //    and then clear the vector v if it's not needed anymore.
 *    //    We don't need this atm, but whatever
 *    void recalcVariables(std::vector<const T*>& v);
 *
 * workflow:
 *
 *   - create a wrapper class T around the object to merge that implements the above two functions
 *     (and whatever is needed for that)
 *   - add each wrapped object to a ObjectMerger<T>
 *   - call the mergeSymmetric function: returns vector of merged Ts
 *   - retrieve original objects from the Ts
 *
 *   - allows to to merge-by-group first and stuff like that
 *
 */
template<class T>
class MergeObject {
public:

    MergeObject(const T * obj){
        objects_.push_back(obj);
    }

    void absorb(MergeObject& rhs){
        if(&rhs == this)
            return;//can't absorb itself
        objects_.insert(objects_->end(),rhs->objects_.begin(),rhs->objects_->end());
        rhs->objects_.clear();
        if(objects_.size()){
            T * firstobj = objects_.at(0);
            firstobj->recalcVariables(objects_);
            if(objects_.size()==0)
                objects_.push_back(firstobj);//always keep this one
        }
    }

    bool isEmpty()const{
        return objects_.size()==0;
    }

    float maxMergeScore(const MergeObject& rhs)const;

    T mergedObject(){
        if(isEmpty())
            return T();
        return objects_.at(0).merge(objects_);
    }

private:
    std::vector<const T*> objects_;
};


/*
 * each of these can contain a group of objects that can be merged
 * if there are multiple it is safe to parallelise over them.
 *
 */
template<class T>
class ObjectMerger{
public:
    void add(const T& o){
        mergeobjects_.push_back(MergeObject<T>(o));
    }

    std::vector<T> mergeSymmetric(const float& threshold)const;

private:
    std::vector< MergeObject<T> > mergeobjects_;
    std::vector< MergeObject<T> > removeEmpty(const std::vector< MergeObject<T> >& mo)const;
};

template<class T>
float MergeObject<T>::maxMergeScore(const MergeObject& rhs)const{
    float maxscore=-1e12;
    for(const auto oi: objects_){
        for(const auto oj: rhs.objects_){
            float score = oi.mergeScore(oj);
            if(score>maxscore)
                maxscore=score;
        }
    }
    return maxscore;
}

template<class T>
std::vector<T> ObjectMerger<T>::mergeSymmetric(const float& threshold)const{

    std::vector<MergeObject<T> > merged=mergeobjects_;
    while(true){
        std::vector<MergeObject<T> > newmerged;
        for(size_t i=0;i<merged.size();i++){
            for(size_t j=i;j<merged.size();j++){
                if(merged.at(i).maxMergeScore(merged.at(j)) > threshold){
                    merged.at(i).absorb(merged.at(j));
                }
            }
        }

        newmerged = removeEmpty(merged);
        if(merged == newmerged){//nothing changed
            return newmerged;
        }
        merged=newmerged;
    }
    //has no empty instances
    std::vector<T> out;
    for(const auto& m: merged)
        out.push_back(m.mergedObject());
    return out;//just to avoid warning
}


template<class T>
std::vector< MergeObject<T> > ObjectMerger<T>::removeEmpty(const std::vector< MergeObject<T> >& mo)const{
    std::vector< MergeObject<T> > out;
    for(const auto o: mo){
        if(!o.isEmpty())
            out.push_back(o);
    }
    return out;
}

}//namespace
#endif
