
#ifndef UGDK_UTIL_GAMEDATA_H_
#define UGDK_UTIL_GAMEDATA_H_

#include <map>
#include <vector>
#include <string>

namespace ugdk {

class GameData {

  public:

    typedef std::vector<std::string> Entry;

    class Segment {

      public:

        Segment(std::string label) : label_(label) {}

        void InsertEntry(Entry entry) { entries_.push_back(entry); }

      private:

        std::string         label_;
        std::vector<Entry>  entries_;

    };

    GameData(std::string name) : name_(name) {}

    void AddProperty(std::string id, std::string value) { properties_[id] = value; }

    void AddSegment(Segment *segment) { segments_.push_back(segment); }

    ~GameData();

  private:

    std::string                         name_;
    std::map<std::string, std::string>  properties_;
    std::vector<Segment*>               segments_;


};

} /* namespace ugdk */

#endif /* UGDK_UTIL_GAMEDATA_H_ */
