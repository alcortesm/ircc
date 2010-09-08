#include <string>
#include <vector>

class Msg {
 public:
   Msg(std::string& rPrefix, std::string& rCommand, std::vector<std::string>& rParams);
   virtual ~Msg() {};
   virtual void Run() = 0;
 private:
   std::string mPrefix;
   std::string mCommand;
   std::vector<std::string> mParams;
};
