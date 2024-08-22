#pragma once

#include <ctemplate/template.h>

#include "oj_model.h"

namespace oj_view {
using namespace oj_model;

const std::string template_html_path = "./template_html/";

class view {
 public:
  void expand(const problem& pblm, std::string& output) {
    auto path = template_html_path + "problem.html";
    ctemplate::TemplateDictionary root("problem");
    root.SetValue("id", pblm.id_);
    root.SetValue("name", pblm.name_);
    root.SetValue("level", pblm.level_);
    root.SetValue("description", pblm.description_);
    root.SetValue("solution", pblm.solution_);

    auto templ =
        ctemplate::Template::GetTemplate(path, ctemplate::DO_NOT_STRIP);
    templ->Expand(&output, &root);
  }

  void expand(const std::vector<problem>& plbmst, std::string& output) {
    auto path = template_html_path + "problemset.html";
    ctemplate::TemplateDictionary root("problemset");
    for (const auto& pblm : plbmst) {
      auto t = root.AddSectionDictionary("problem_list");
      t->SetValue("id", pblm.id_);
      t->SetValue("name", pblm.name_);
      t->SetValue("level", pblm.level_);
    }

    auto templ =
        ctemplate::Template::GetTemplate(path, ctemplate::DO_NOT_STRIP);
    templ->Expand(&output, &root);
  }
};
}  // namespace oj_view