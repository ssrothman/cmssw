#ifndef DataFormats_L1THGCal_AEMap_H
#define DataFormats_L1THGCal_AEMap_H

#include <unordered_map>

namespace l1t {
  class AEMap {
    public:
       AEMap() : map_() {}

       float& operator[](uint32_t key){
         return map_[key];
       }

    private:
        std::unordered_map<uint32_t, float> map_;
  };
}

#endif
