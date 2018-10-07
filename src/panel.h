#include <map>
#include <string>
class Panel {
   public:
    std::string mTitle;
    std::map<std::string, double> *mVariables;
    Panel();
    ~Panel();
    Panel(std::string _title);

    void addVariables(std::map<std::string, double> *_variables);
};
