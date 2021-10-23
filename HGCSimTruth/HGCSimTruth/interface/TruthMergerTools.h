/*
 * TruthMergerTools.h
 *
 *  Created on: 22 Oct 2021
 *      Author: jkiesele
 */

#ifndef HGCSIMTRUTH_HGCSIMTRUTH_INTERFACE_TRUTHMERGERTOOLS_H_
#define HGCSIMTRUTH_HGCSIMTRUTH_INTERFACE_TRUTHMERGERTOOLS_H_

#include <vector>
#include <iostream> //debug

namespace truth{

/*
 * abstract helper classes that implement a 1:1 merging functionality for objects
 * caching not needed because for every evaluation it's either merged or not
 *
 *  class inheriting from MergeWrapper must implement the 'physics of it' in methods:
 *
 *    constructor based on pointer to actual object
 *    e.g. InheritingClass(const T * o)
 *
 *    //calculates a (symmetric) merge score between two instances
 *    float mergeScore(const InheritingClass* rhs)const;
 *
 *    //defines rules to create a new instance out of a set of previously merged instances.
 *    //this is called at the end, not during merging
 *    //should not use any "this" object properties
 *    T mergeAllInList(const std::vector<const InheritingClass* >& tomerge)const;
 *
 *    //OPT: one could add a function that determines how to re-evaluate the merging parameters after a merge
 *    //    but that would require to change the absorb function a bit (and is not needed right now),
 *    //    and then clear the vector v if it's not needed anymore.
 *    //    return true if the vector has been absorbed, false if not used
 *    bool recalcVariables(std::vector<const InheritingClass *>& v)
 *
 * workflow:
 *
 *   - create a wrapper class T inheriting from MergeWrapper around the object
 *     to merge that implements the above two functions
 *     (and whatever is needed for that)
 *   - add each wrapped object to a ObjectMerger<T>
 *   - call the mergeSymmetric function: returns vector of merged Ts
 *   - retrieve original objects from the Ts
 *
 *   - allows to to merge-by-group first and stuff like that
 *
 *
 *
 *   T: target class (e.g. SimCluster)
 *   U: class inheriting from MergeWrapper<T>
 */

template<class T,class U>
class MergeWrapper{
public:

    //virtual bool operator==(const MergeWrapper<T,U>& rhs)const=0;

    virtual ~MergeWrapper(){}

    virtual float mergeScore(const U* rhs)const=0;

    virtual T mergeAllInList(const std::vector<const U *>&)const=0;

    bool recalcVariables(std::vector<const U *>& v){return false;}

};


/*
 *
 * hidden helper class
 *
 * U are here MergeWrapper, T is the target class
 * objects already
 *
 */
template<class U, class T>
class MergeObject {
public:

    //objects should be pointers?
    //they never get lost during merging, so

    MergeObject(const U * obj){
        objects_.push_back(obj);
    }

    bool operator==(const MergeObject<U,T>& rhs)const{
        return objects_==rhs.objects_;
    }

    void absorb(MergeObject& rhs){
        if(&rhs == this)
            return;//can't absorb itself
        objects_.insert(objects_.end(),rhs.objects_.begin(),rhs.objects_.end());
        rhs.objects_.clear();
        if(objects_.size()){
            /* for later
            U &  firstobj = objects_.at(0);

            bool updated = firstobj.recalcVariables(objects_);
            if(updated){
                objects_.clear();
                objects_.push_back(firstobj);//always keep this one
            }
            */
        }
    }

    bool isEmpty()const{
        return objects_.size()==0;
    }

    float maxMergeScore(const MergeObject& rhs)const;

    T mergedObject()const{
        if(isEmpty())
            return T();
        return objects_.at(0)->mergeAllInList(objects_);
    }

private:
    std::vector<const U* > objects_;
};


template<class U,class T>
float MergeObject<U,T>::maxMergeScore(const MergeObject& rhs)const{
    float maxscore=-1e12;
    for(const auto oi: objects_){
        for(const auto oj: rhs.objects_){
            float score = oi->mergeScore(oj);
            if(score>maxscore)
                maxscore=score;
        }
    }
    return maxscore;
}

/*
 * each of these can contain a group of objects that can be merged
 * if there are multiple it is safe to parallelise over them.
 *
 * class T here is MergeWrapper<U>, so type safe with casts within MergeWrapper<U> dervied class
 *
 */
template<class U, class T>
class ObjectMerger{
public:
    //keep track of the original objects here and just pass pointers to the MergeObject one

    void add(const T * o){
        objects.push_back(U(o));
    }

    /*
     * returns merged target objects
     */
    std::vector<T> mergeSymmetric(const float& threshold)const;

private:
    std::vector<U> objects;
    std::vector< MergeObject<U,T> > removeEmpty(const std::vector< MergeObject<U,T> >& mo)const;
};


template<class U,class T>
std::vector<T> ObjectMerger<U,T>::mergeSymmetric(const float& threshold)const{

    std::vector< MergeObject<U,T> > merged;
    for(const auto& o:objects)
        merged.push_back(MergeObject<U,T>(&o));

    while(true){
        for(size_t i=0;i<merged.size();i++){
            for(size_t j=i;j<merged.size();j++){
                if(merged.at(i).maxMergeScore(merged.at(j)) > threshold){
                    merged.at(i).absorb(merged.at(j));
                }
            }
        }

        auto newmerged = removeEmpty(merged);
        if(merged == newmerged){//nothing changed
            break;
        }
        merged=newmerged;
    }
    std::vector<T> out;
    for(const auto& m: merged)
        out.push_back(m.mergedObject());
    return out;
}


template<class U, class T>
std::vector< MergeObject<U,T> > ObjectMerger<U,T>::removeEmpty(const std::vector< MergeObject<U,T> >& mo)const{
    std::vector< MergeObject<U,T> > out;
    for(const auto o: mo){
        if(!o.isEmpty())
            out.push_back(o);
    }
    return out;
}

}//namespace
#endif
