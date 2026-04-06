#include <TNtuple.h>
#include <TString.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

// Example: toggles (replace with your CommandLine options)
struct StoreToggles {
  bool storeDgen = true;
  bool storeDpt = true;
  bool storeDsvpvDistance = true;
  bool storeDsvpvDisErr = true;
  bool storeDsvpvDistance_2D = true;
  bool storeDsvpvDisErr_2D = true;
};

static std::vector<std::string> BuildLeafOrder(const StoreToggles& t)
{
  // Always keep Dmass first for convenience
  std::vector<std::string> v;
  v.push_back("Dmass");

  if (t.storeDgen)             v.push_back("Dgen");
  if (t.storeDpt)              v.push_back("Dpt");
  if (t.storeDsvpvDistance)    v.push_back("DsvpvDistance");
  if (t.storeDsvpvDisErr)      v.push_back("DsvpvDisErr");
  if (t.storeDsvpvDistance_2D) v.push_back("DsvpvDistance_2D");
  if (t.storeDsvpvDisErr_2D)   v.push_back("DsvpvDisErr_2D");

  return v;
}

static TString JoinLeafList(const std::vector<std::string>& order)
{
  TString leaflist;
  for (size_t i = 0; i < order.size(); ++i) {
    if (i) leaflist += ":";
    leaflist += order[i];
  }
  return leaflist;
}
